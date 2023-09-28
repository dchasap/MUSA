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

#ifndef SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_SAMPLEHISTORY_H_
#define SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_SAMPLEHISTORY_H_

#include <iostream>
#include <vector>

#include "simulators/tasksim/runtime/types.h"

namespace sim {
namespace runtime {

class SampleHistory
{
private:
    // Signatures of task instances in sample history
    std::vector<std::vector<PhaseSignature>> signatures_history_;
    // Number of samples in history
    uint64_t num_samples_history_;
    // Last task id per task type, used for stats extrapolation
    std::vector<uint64_t> last_id_per_type_;

public:
    // Constructor
    explicit SampleHistory(uint64_t num_samples_history);
    // Destructor
    ~SampleHistory() { }
    // Add sample to history
    void add_sample(PhaseSignature signature);
    // Clear sample history (usually when resampling the simulation)
    void clear();
    // Get mean IPC of the samples of a given task type
    float get_mean_ipc(unsigned task_type);
    // Check if there is at least one sample for a task type
    bool has_sample(unsigned task_type);
    // Check if all task types are fully sampled
    bool all_task_types_fully_sampled();
    // Get task id of last sampled task instance of a type
    uint64_t get_last_id(unsigned task_type);

    // Operator for serializing sample data
    friend std::ostream &operator<<(std::ostream &output, SampleHistory &hist);
    // Operator for unserializing sample data
    friend std::istream &operator>>(std::istream &input, SampleHistory &hist);
};

}  // namespace runtime
}  // namespace sim

#endif  // SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_SAMPLEHISTORY_H_
