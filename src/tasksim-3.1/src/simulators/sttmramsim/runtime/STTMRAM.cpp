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


#include "STTMRAM.h"

#include <fstream>
#include <functional>

namespace sim {
namespace runtime {

STTMRAM::cpu_buffer_t::cpu_buffer_t(engine::Simulator &sim, STTMRAM &provider,
                                  void (STTMRAM::*mem_provider)(mem_buffer_t &),
                                  std::size_t buffer_size, unsigned id) :
    mem_(std::bind(mem_provider, provider, std::placeholders::_1), buffer_size, id),
    tr_(NULL)
{
}

STTMRAM::STTMRAM(engine::Simulator &sim, const std::string &master_trace_file) :
        sim_(sim)
{
	std::ifstream infile(master_trace_file);
	std::string line;
	trace_files_.push_back(std::vector<std::string>());
	while (std::getline(infile, line))
	{
		if (line == "")
			trace_files_.push_back(std::vector<std::string>());
		else
			trace_files_.back().push_back(line);
	}
	infile.close();
}

STTMRAM::~STTMRAM()
{
    std::vector<cpu_buffer_t *>::const_iterator i;
    for(i = cpu_buffers_.begin(); i != cpu_buffers_.end(); ++i) {
        delete (*i);
    }
}

void STTMRAM::fill_mem_buffer(mem_buffer_t &buffer)
{
    unsigned buffer_id = buffer.get_id();
    assert(buffer_id < cpu_buffers_.size());
    assert(cpu_buffers_[buffer_id] != NULL);

    while (buffer.space_left() > 0) {
    	if (cpu_buffers_[buffer_id]->tr_ == NULL) {
    		if (trace_files_[buffer_id].size() > 0) {
    			cpu_buffers_[buffer_id]->tr_ = new sim::trace::sttmram::TraceReader(trace_files_[buffer_id][0]);
    			trace_files_[buffer_id].erase(trace_files_[buffer_id].begin());
    		}
    		else
    			break;
    	}

    	trace::sttmram::mem_t mem = cpu_buffers_[buffer_id]->tr_->getNextMem();
    	if (cpu_buffers_[buffer_id]->tr_->eof()) {
    		cpu_buffers_[buffer_id]->tr_ = NULL;
    	}
    	else
    		buffer.push_event(mem);
    }
}

STTMRAM::cpu_buffer_t &STTMRAM::operator[](unsigned cpu_id)
{
    if(cpu_buffers_.size() <= cpu_id)
    {
        cpu_buffers_.resize(cpu_id + 1, NULL);
    }

    if(cpu_buffers_[cpu_id] == NULL)
    {
        cpu_buffers_[cpu_id] = new cpu_buffer_t(sim_, *this,
                &STTMRAM::fill_mem_buffer, Event_Buffer_Size_, cpu_id);
    }

    return *cpu_buffers_[cpu_id];
}

}
}
