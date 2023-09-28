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


#include "OmpSs_test.h"

#include <functional>

#include <core/engine/Simulator.h>

namespace sim {
namespace runtime {

OmpSs::cpu_buffer_t::cpu_buffer_t(engine::Simulator &sim, OmpSs &provider,
                                  void (OmpSs::*burst_provider)(burst_buffer_t &),
                                  void (OmpSs::*instr_provider)(instr_buffer_t &),
                                  void (OmpSs::*mem_provider)(mem_buffer_t &),
                                  std::size_t buffer_size, unsigned id) :
    burst_(std::bind(burst_provider, provider, std::placeholders::_1), buffer_size, id),
    instr_(std::bind(instr_provider, provider, std::placeholders::_1), buffer_size, id),
    mem_(std::bind(mem_provider, provider, std::placeholders::_1), buffer_size, id)
{
}



OmpSs::OmpSs(engine::Simulator &sim, const std::string &trace_file) :
        input_trace_(trace_file.c_str()),
        sim_(sim)
{ }


OmpSs::~OmpSs()
{
    for (std::vector<cpu_buffer_t*>::iterator it = cpu_buffers_.begin(); it != cpu_buffers_.end(); it++) {
        delete *it;
    }
}


void OmpSs::fill_burst_buffer(burst_buffer_t &buffer)
{
    /* Check if the intruction and memory ports of the buffer are connected */
    unsigned buffer_id = buffer.get_id();
    assert(buffer_id < cpu_buffers_.size());
    assert(cpu_buffers_[buffer_id] != NULL);
    cpu_buffer_t &cpu_buffer = *cpu_buffers_[buffer_id];
    // Commented as this is phased as an example now and it does no longer work:
    // bool stop_on_phase = cpu_buffer.instr_.out_.is_connected() || cpu_buffer.mem_.out_.is_connected();
    bool stop_on_phase = false;

    while(buffer.space_left() > 0) {
        trace::ts::event_t event;
        bool ret = input_trace_.get_task_event(0, event);
        if(ret == false) return;
        if(event.type == trace::ts::PHASE) {
            buffer.push_event(event.u.phase);
            /* There are no instruction associated to the burst, so continue fetching bursts */
            if(stop_on_phase  == false || event.u.phase.subtrace_id < 0) continue;

            assert(buffer_id < cpu_buffers_.size());
            cpu_buffer.current_phase_ = event.u.phase;
            return;
        }
    }

}


void OmpSs::fill_instr_buffer(instr_buffer_t &buffer)
{
    unsigned buffer_id = buffer.get_id();
    assert(buffer_id < cpu_buffers_.size());
    assert(cpu_buffers_[buffer_id] != NULL);
    cpu_buffer_t &cpu_buffer = *cpu_buffers_[buffer_id];
    // assert(cpu_buffer.instr_.out_.is_connected() == true);
    unsigned subtrace_id = cpu_buffer.current_phase_.subtrace_id;
    if(subtrace_id < 0) return;
    const trace::ts::subtrace_t &subtrace = input_trace_.get_task_trace_by_id(subtrace_id);
    while(buffer.space_left() > 0) {
        const trace::bbl_t &bbl = input_trace_.get_next_bbl(subtrace);
        if(bbl.id_ == 0) return; /* Bbl ID 0 signals end of subtrace */
        if(bbl.ins_.size() > buffer.space_left()) return;
        std::vector<trace::minst_t>::const_iterator i;
        for(i = bbl.ins_.begin(); i != bbl.ins_.end(); ++i) buffer.push_event(*i);
    }
}


void OmpSs::fill_mem_buffer(mem_buffer_t &buffer)
{
    unsigned buffer_id = buffer.get_id();
    assert(buffer_id < cpu_buffers_.size());
    assert(cpu_buffers_[buffer_id] != NULL);
    cpu_buffer_t &cpu_buffer = *cpu_buffers_[buffer_id];
    // assert(cpu_buffer.instr_.out_.is_connected() == true);
    unsigned subtrace_id = cpu_buffer.current_phase_.subtrace_id;
    if(subtrace_id < 0) return;
    const trace::ts::subtrace_t &subtrace = input_trace_.get_task_trace_by_id(subtrace_id);
    while(buffer.space_left() > 0) {
        const trace::mem_t &mem = input_trace_.get_next_mem(subtrace);
        if(mem.addr_ == 0) return;
        buffer.push_event(mem);
    }
}

OmpSs::cpu_buffer_t &OmpSs::operator[](unsigned cpu_id)
{
    if(cpu_buffers_.size() <= cpu_id) {
        cpu_buffers_.resize(cpu_id, NULL);
        cpu_buffers_.push_back(new cpu_buffer_t(sim_, *this,
                &OmpSs::fill_burst_buffer, &OmpSs::fill_instr_buffer, &OmpSs::fill_mem_buffer, Event_Buffer_Size_, cpu_id));
    }
    else if(cpu_buffers_[cpu_id] == NULL) {
        cpu_buffers_[cpu_id] = new cpu_buffer_t(sim_, *this,
                &OmpSs::fill_burst_buffer, &OmpSs::fill_instr_buffer, &OmpSs::fill_mem_buffer, Event_Buffer_Size_, cpu_id);
    }

    return *cpu_buffers_[cpu_id];
}

}
}
