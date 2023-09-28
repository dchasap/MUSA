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


#ifndef SIMULATORS_TASKSIM_RUNTIME_TYPES_H_
#define SIMULATORS_TASKSIM_RUNTIME_TYPES_H_

#include <cassert>
#include <string>
#include <unordered_map>

#include "core/engine/types.h"

namespace sim {
namespace runtime {

enum SimulationMode { BURST = 0, MEMORY = 1};

struct PhaseSignature
{
    engine::cycles_t duration_;
    int64_t task_type_;
    uint64_t thread_id_;
    int subtrace_id_;
    uint64_t inst_count_;
    float ipc_;
    uint64_t task_id_;

    PhaseSignature(sim::engine::cycles_t duration, int64_t task_type,
                   uint64_t thread_id, int subtrace_id, uint64_t inst_count,
                   float ipc, uint64_t task_id) :
        duration_(duration),
        task_type_(task_type),
        thread_id_(thread_id),
        subtrace_id_(subtrace_id),
        inst_count_(inst_count),
        ipc_(ipc),
        task_id_(task_id)
    {}

    PhaseSignature() = default;

    // Operator for serializing a PhaseSignature
    friend std::ostream &operator<<(std::ostream &output, PhaseSignature &signature) {
        output << signature.duration_;
        output << ":" << signature.task_type_;
        output << ":" << signature.thread_id_;
        output << ":" << signature.subtrace_id_;
        output << ":" << signature.inst_count_;
        output << ":" << signature.ipc_;
        output << ":" << signature.task_id_;
        output << std::endl;

        return output;
    }
    // Operator for unserializing a PhaseSignature
    friend std::istream &operator>>(std::istream &input, PhaseSignature &signature) {
        char sep;
        input >> signature.duration_; input >> sep; assert(sep == ':');
        input >> signature.task_type_; input >> sep; assert(sep == ':');
        input >> signature.thread_id_; input >> sep; assert(sep == ':');
        input >> signature.subtrace_id_; input >> sep; assert(sep == ':');
        input >> signature.inst_count_; input >> sep; assert(sep == ':');
        input >> signature.ipc_; input >> sep; assert(sep == ':');
        input >> signature.task_id_;

        return input;
    }
};

enum class Policy : unsigned {
            BURST_ONLY = 0,
            MEMORY = 1,
            TASK_POINT = 2
};

enum class Measure : unsigned {
            FULL_APPLICATION = 0,
            PARALLEL_SECTIONS = 1 };

static const std::unordered_map<std::string, Policy> policy_map({
        {"BURST_ONLY", Policy::BURST_ONLY},
        {"MEMORY", Policy::MEMORY},
        {"TASK_POINT", Policy::TASK_POINT}
});

static const std::unordered_map<std::string, Measure> measures_map({
        {"FULL_APPLICATION", Measure::FULL_APPLICATION},
        {"PARALLEL_SECTIONS", Measure::PARALLEL_SECTIONS},
});

inline
static Policy get_policy(const std::string &policy_name)
{
    if (policy_map.find(policy_name) != policy_map.end()) {
        return policy_map.find(policy_name)->second;
    } else {
        std::cerr << "Unknown ModeSelector policy requested: "
                  << policy_name << std::endl;
        std::cerr << "Policy names available are:" << std::endl;
        for (auto it = policy_map.begin(); it != policy_map.end(); ++it) {
            std::cerr << it->first << std::endl;
        }
        std::abort();
    }
}

inline static Measure get_measure(const std::string &timing_name)
{
    if (measures_map.find(timing_name) != measures_map.end()) {
        return measures_map.find(timing_name)->second;
    } else {
        std::cerr << "Unknown Sections requested: "
                  << timing_name << std::endl;
        std::cerr << "Sections options available are:" << std::endl;
        for (auto it = measures_map.begin(); it != measures_map.end(); ++it) {
            std::cerr << it->first << std::endl;
        }
        std::abort();
    }
}

}  // namespace runtime
}  // namespace sim

#endif   // SIMULATORS_TASKSIM_RUNTIME_TYPES_H_
