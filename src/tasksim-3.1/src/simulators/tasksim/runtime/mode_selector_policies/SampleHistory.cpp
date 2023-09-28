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

#include "SampleHistory.h"

#include <cassert>
#include <iostream>
#include <vector>

#include "core/utils/mean.h"


namespace sim {
namespace runtime {

SampleHistory::SampleHistory(uint64_t num_samples_history) :
        num_samples_history_(num_samples_history) { }

void
SampleHistory::add_sample(PhaseSignature signature)
{
    unsigned task_type = signature.task_type_;
    if (task_type + 1 > signatures_history_.size()) {
        signatures_history_.resize(task_type + 1, std::vector<PhaseSignature>(num_samples_history_));
        last_id_per_type_.resize(task_type + 1);
    }

    signatures_history_[task_type].push_back(signature);
    signatures_history_[task_type].erase(signatures_history_[task_type].begin());

    last_id_per_type_[task_type] = signature.task_id_;
}

void
SampleHistory::clear()
{
    signatures_history_.clear();
}

float
SampleHistory::get_mean_ipc(unsigned task_type)
{
    if (signatures_history_.size() > task_type) {
        std::vector<float> ipc_samples;
        for (auto it = signatures_history_[task_type].begin();
             it != signatures_history_[task_type].end(); ++it) {
           if (it->inst_count_ > 0) {
               ipc_samples.push_back(it->ipc_);
           }
        }
        return core::utils::get_mean(ipc_samples);
    }
    return 0;
}

bool
SampleHistory::has_sample(unsigned task_type)
{
    if (signatures_history_.size() > task_type) {
        for (auto it = signatures_history_[task_type].begin();
                it != signatures_history_[task_type].end(); ++it) {
            if (it->inst_count_ > 0) {
                return true;
            }
        }
    }
    return false;
}

bool
SampleHistory::all_task_types_fully_sampled()
{
    // Check if sample history fully populated
    if (signatures_history_.size() == 0) {
        return false;
    }
    for (unsigned i = 1; i < signatures_history_.size(); ++i) {
        for (unsigned j = 0; j < signatures_history_[i].size(); ++j) {
            if (signatures_history_[i][j].inst_count_ == 0) {
                return false;
            }
        }
    }

    return true;
}

uint64_t
SampleHistory::get_last_id(unsigned task_type)
{
    return last_id_per_type_[task_type];
}

std::ostream&
operator<<(std::ostream &output, SampleHistory &hist)
{
    unsigned task_types = hist.signatures_history_.size();
    unsigned samples_per_task_type = 0;
    if (task_types > 0) {
        samples_per_task_type = hist.signatures_history_[0].size();
    }

    output << task_types;
    output << ":" << samples_per_task_type;
    output << std::endl;

    // Serialize last id per task type
    output << hist.last_id_per_type_[0];
    for (unsigned task_type = 1; task_type < task_types; ++task_type) {
        output << ":" << hist.last_id_per_type_[task_type];
    }
    output << std::endl;

    // Serialize all PhaseSignatures in the sample history
    for (unsigned task_type = 0; task_type < task_types; ++task_type) {
        output << hist.signatures_history_[task_type][0];
        for (unsigned sample = 1; sample < samples_per_task_type; ++sample) {
            output << hist.signatures_history_[task_type][sample];
        }
    }
    return output;
}

std::istream&
operator>>(std::istream &input, SampleHistory &hist)
{
    unsigned task_types, samples_per_task_type;
    char sep;

    input >> task_types; input >> sep; assert(sep == ':');
    input >> samples_per_task_type;

    // Unserialize last id per task type
    hist.last_id_per_type_.resize(task_types);
    input >> hist.last_id_per_type_[0];
    for (unsigned task_type = 1; task_type < task_types; ++task_type) {
        input >> sep; assert(sep == ':'); input >> hist.last_id_per_type_[task_type];
    }

    hist.signatures_history_.resize(task_types);
    for (unsigned task_type = 0; task_type < task_types; ++task_type) {
        hist.signatures_history_[task_type].resize(samples_per_task_type);
        input >> hist.signatures_history_[task_type][0];
        for (unsigned sample = 1; sample < samples_per_task_type; ++sample) {
            input >> hist.signatures_history_[task_type][sample];
        }
    }

    return input;
}

}  // namespace runtime
}  // namespace sim
