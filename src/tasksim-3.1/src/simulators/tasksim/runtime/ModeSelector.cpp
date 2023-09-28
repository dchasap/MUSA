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

// Authors: Thomas Grass (thomas.grass@bsc.es)

#include "ModeSelector.h"

#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
#include "core/engine/types.h"

namespace sim {
namespace runtime {

ModeSelector::ModeSelector(sim::engine::Simulator &sim,
                           const sim::engine::Config &config,
                           const sim::runtime::Policy &selector,
                           unsigned num_threads)
{
    if (selector == sim::runtime::Policy::BURST_ONLY) {
        policy_ = std::unique_ptr<PolicyInterface>(new BurstOnly(sim, config));
    } else if (selector == sim::runtime::Policy::MEMORY) {
        policy_ = std::unique_ptr<PolicyInterface>(new Memory(sim, config));
    } else if (selector == sim::runtime::Policy::TASK_POINT) {
        policy_ = std::unique_ptr<PolicyInterface>(new TaskPoint(sim, config, num_threads));
    } else {
        std::cerr << "Cannot identify configuration option for ModeSelector"
                  << std::endl;
        std::cerr << "Use either BURST_ONLY, MEMORY or TASK_POINT."
                  << std::endl;
        std::abort();
    }
}

SimulationMode ModeSelector::get_mode(PhaseSignature signature)
{
    return policy_->get_mode(signature);
}

void ModeSelector::start_detailed_phase(PhaseSignature signature)
{
    policy_->start_detailed_phase(signature);
}

void ModeSelector::end_detailed_phase(PhaseSignature signature)
{
    policy_->end_detailed_phase(signature);
}

void ModeSelector::start_burst_phase(PhaseSignature signature)
{
    policy_->start_burst_phase(signature);
}

void ModeSelector::end_burst_phase(PhaseSignature signature)
{
    policy_->end_burst_phase(signature);
}

sim::engine::cycles_t ModeSelector::predict_cycle_count(PhaseSignature signature)
{
    return policy_->predict_cycle_count(signature);
}

void ModeSelector::task_start(unsigned task_id, unsigned thread_id, unsigned type_id)
{
    policy_->task_start(task_id, thread_id, type_id);
}

void ModeSelector::task_end(unsigned task_id, unsigned thread_id, unsigned type_id)
{
    policy_->task_end(task_id, thread_id, type_id);
}

}  // namespace runtime
}  // namespace sim
