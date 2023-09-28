/*
 * Copyright (c) 2016, Barcelona Supercomputing Center
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

// Authors: Thomas Grass (thomas.grass@bsc.es)

#include "Memory.h"

#include "core/engine/Simulator.h"
#include "core/engine/types.h"
#include "core/trace/ts/types.h"

namespace sim {
namespace runtime {

Memory::Memory(engine::Simulator &sim, const sim::engine::Config &config) :
    sim_(sim)
{
}

SimulationMode Memory::get_mode(PhaseSignature signature)
{
    // If the event has a subtrace, simulate it.
    if (signature.subtrace_id_ != sim::trace::ts::NO_SUBTRACE) {
        start_detailed_phase(signature);
        return MEMORY;
    } else {
        start_burst_phase(signature);
        return BURST;
    }
}

void Memory::start_detailed_phase(PhaseSignature signature)
{
    uint64_t thread_id = signature.thread_id_;

    if (start_cycle_detailed_.size() < thread_id + 1)
        start_cycle_detailed_.resize(thread_id + 1);

    start_cycle_detailed_[thread_id] = sim_.get_clock();
}

void Memory::end_detailed_phase(PhaseSignature signature)
{
    uint64_t thread_id = signature.thread_id_;
    int64_t task_type = signature.task_type_;

    if (task_type+1 > static_cast<int64_t>(last_cycle_count_.size())) {
        last_cycle_count_.resize(task_type+1);
    }
    last_cycle_count_[task_type] = sim_.get_clock() -
                                 start_cycle_detailed_[thread_id];
    // TG: std::cout << sim_.get_clock() << ": Thread " << thread_id
    //     << ": Finished detailed simulation of task of type "
    //     << task_id << ", " << last_cycle_count_[task_id]
    //     << " cycles" << std::endl;
}

engine::cycles_t Memory::predict_cycle_count(PhaseSignature signature)
{
    return engine::NEVER;
}

void Memory::task_start(unsigned task_id, unsigned thread_id, unsigned type_id)
{
}

void Memory::task_end(unsigned task_id, unsigned thread_id, unsigned type_id)
{
}

}  // namespace runtime
}  // namespace sim
