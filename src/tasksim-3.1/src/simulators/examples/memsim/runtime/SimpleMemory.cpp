/*
 * Copyright (c) 2015, Barcelona Supercomputing Center
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer; redistributions in binary form
 * must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with
 * the distribution; neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "SimpleMemory.h"

#include "core/utils/Log.h"

namespace sim {
namespace runtime {

using sim::utils::Log;

SimpleMemory::cpu_buffer_t::cpu_buffer_t(engine::Simulator &sim, SimpleMemory &provider,
                                  void (SimpleMemory::*mem_provider)(mem_buffer_t &),
                                  std::size_t buffer_size, unsigned id) :
    mem_(std::bind(mem_provider, provider, std::placeholders::_1), buffer_size)
{
}

SimpleMemory::SimpleMemory(engine::Simulator &sim, const std::string &trace_file) :
        input_trace_(trace_file.c_str()),
        sim_(sim), pending_bbl_(0), halt_(false)
{ }

SimpleMemory::~SimpleMemory()
{
    std::vector<cpu_buffer_t *>::const_iterator i;
    for(i = cpu_buffers_.begin(); i != cpu_buffers_.end(); ++i) {
        delete (*i);
    }
}

void SimpleMemory::fill_mem_buffer(mem_buffer_t &buffer)
{
	if (halt_ == true) {
		return;
	}
	Log::debug4() << "fill_mem_buffer()";
    assert(buffer.get_id() < cpu_buffers_.size());
    assert(cpu_buffers_[buffer.get_id()] != NULL);

    while(buffer.space_left() > 0) {
        const trace::mitem_t &mem = input_trace_.get_next_mem_item();
        if(mem.addr_ == 0) {
        	halt_ = true;
        	return;
        }
        buffer.push_event(mem);
    }
}

SimpleMemory::cpu_buffer_t &SimpleMemory::operator[](unsigned cpu_id)
{
    if(cpu_buffers_.size() <= cpu_id) {
        cpu_buffers_.resize(cpu_id, NULL);
        cpu_buffers_.push_back(new cpu_buffer_t(sim_, *this,
        		&SimpleMemory::fill_mem_buffer, Event_Buffer_Size_, cpu_id));
    }
    else if(cpu_buffers_[cpu_id] == NULL) {
        cpu_buffers_[cpu_id] = new cpu_buffer_t(sim_, *this,
        		&SimpleMemory::fill_mem_buffer, Event_Buffer_Size_, cpu_id);
    }

    return *cpu_buffers_[cpu_id];
}

}
}
