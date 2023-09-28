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

#ifndef SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_POLICYINTERFACE_H_
#define SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_POLICYINTERFACE_H_

#include <simulators/tasksim/runtime/types.h>

namespace sim {
namespace runtime {

class PolicyInterface
{
public:
    virtual ~PolicyInterface() { }
    virtual SimulationMode get_mode(PhaseSignature signature) = 0;
    virtual void start_detailed_phase(PhaseSignature signature) = 0;
    virtual void end_detailed_phase(PhaseSignature signature) = 0;
    virtual void start_burst_phase(PhaseSignature signature) = 0;
    virtual void end_burst_phase(PhaseSignature signature) = 0;
    virtual engine::cycles_t predict_cycle_count(PhaseSignature signature) = 0;
    virtual void task_start(unsigned task_id, unsigned thread_id, unsigned type_id) = 0;
    virtual void task_end(unsigned task_id, unsigned thread_id, unsigned type_id) = 0;
};

}  // namespace runtime
}  // namespace sim

#endif  // SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_POLICYINTERFACE_H_
