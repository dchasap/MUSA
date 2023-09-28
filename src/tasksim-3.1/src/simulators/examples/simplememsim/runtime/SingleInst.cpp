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


#include "SingleInst.h"

#include <functional>

namespace sim {
namespace runtime {

SingleInst::cpu_buffer_t::cpu_buffer_t(engine::Simulator &sim, SingleInst &provider,
                                  void (SingleInst::*instr_provider)(instr_buffer_t &),
                                  void (SingleInst::*mem_provider)(mem_buffer_t &),
                                  std::size_t buffer_size, unsigned id) :
    instr_(std::bind(instr_provider, provider, std::placeholders::_1), buffer_size),
    mem_(std::bind(mem_provider, provider, std::placeholders::_1), buffer_size)
{
}

SingleInst::SingleInst(engine::Simulator &sim, const std::string &trace_file) :
        input_trace_(trace_file.c_str()),
        sim_(sim), pending_bbl_(0), halt_(false)
{ }

SingleInst::~SingleInst()
{
    std::vector<cpu_buffer_t *>::const_iterator i;
    for(i = cpu_buffers_.begin(); i != cpu_buffers_.end(); ++i) {
        delete (*i);
    }
}


void SingleInst::fill_instr_buffer(instr_buffer_t &buffer)
{
    assert(buffer.get_id() < cpu_buffers_.size());
    assert(cpu_buffers_[buffer.get_id()] != NULL);

    /* Check if there are instructions left from previous calls */
    if(pending_bbl_.id_ > 0) {
        const trace::bbl_t &bbl = input_trace_.get_bbl(pending_bbl_);
        assert(bbl.ins_.size() < buffer.space_left());
        std::vector<trace::minst_t>::const_iterator i;
        for(i = bbl.ins_.begin(); i != bbl.ins_.end(); ++i) buffer.push_event(*i);
        pending_bbl_.id_ = 0;
    }

    /* Check if there are more BBLs to execute */
    if(halt_ == true) return;

    while(buffer.space_left() > 0) {
        const trace::bbl_t &bbl =  input_trace_.get_next_bbl();
        if(bbl.id_ == 0) { halt_ = true; return; }/* Bbl ID 0 signals end of subtrace */
        assert(bbl.ins_.size() < Event_Buffer_Size_);
        if(bbl.ins_.size() > buffer.space_left()) {
            pending_bbl_ = bbl.id_.id_;
            return;
        }
        std::vector<trace::minst_t>::const_iterator i;
        for(i = bbl.ins_.begin(); i != bbl.ins_.end(); ++i) buffer.push_event(*i);
    }
}


void SingleInst::fill_mem_buffer(mem_buffer_t &buffer)
{
    assert(buffer.get_id() < cpu_buffers_.size());
    assert(cpu_buffers_[buffer.get_id()] != NULL);

    while(buffer.space_left() > 0) {
        const trace::mem_t &mem = input_trace_.get_next_mem();
        if(mem.addr_ == 0) return;
        buffer.push_event(mem);
    }
}

SingleInst::cpu_buffer_t &SingleInst::operator[](unsigned cpu_id)
{
    if(cpu_buffers_.size() <= cpu_id) {
        cpu_buffers_.resize(cpu_id, NULL);
        cpu_buffers_.push_back(new cpu_buffer_t(sim_, *this,
                &SingleInst::fill_instr_buffer, &SingleInst::fill_mem_buffer, Event_Buffer_Size_, cpu_id));
    }
    else if(cpu_buffers_[cpu_id] == NULL) {
        cpu_buffers_[cpu_id] = new cpu_buffer_t(sim_, *this,
                &SingleInst::fill_instr_buffer, &SingleInst::fill_mem_buffer, Event_Buffer_Size_, cpu_id);
    }

    return *cpu_buffers_[cpu_id];
}

}
}
