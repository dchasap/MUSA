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

#ifndef CORE_TOOLS_OMPSS_UTIL_H_
#define CORE_TOOLS_OMPSS_UTIL_H_

#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

#include "core/trace/ts/nanos.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/ompss/types.h"

namespace sim {
namespace tools {

typedef std::unordered_map<unsigned, std::queue<sim::trace::ts::dep_t>> wait_on_map_t;
typedef std::unordered_map<unsigned, int16_t> wd_name_map_t;

void cleanup_ompss_trace(const std::string &temp_file_name);
bool file_exists(const std::string &file_name);

void alloc_and_copy(char * argv[], unsigned i, const char *src);

template<template<typename> class TSType ,
         template <template <typename> class, typename> class InternalFormat>
inline void process_event_for_wd_id(sim::trace::ompss::event_t &event,
        sim::trace::ts::FileTrace<TSType, InternalFormat> &ts_trace,
        wait_on_map_t &wait_on_deps,
        std::unordered_map<unsigned, int> &subtrace_phase_count,
        const wd_name_map_t &wd_name_map,
        const sim::trace::ompss::nanos_phase_map_t &nanos_phase_map,
        bool nopin);

template<template<typename> class TSType ,
        template <template <typename> class, typename> class InternalFormat>
void check_phases(sim::trace::ts::FileTrace<TSType, InternalFormat> &ts_trace,
        std::vector<sim::trace::ompss::event_t> &ompss_events,
        const sim::trace::ompss::nanos_phase_map_t &nanos_phase_map);

template<template <typename> class OmpSsFormat,
        template <typename> class TSFormat,
        template <template <typename> class, typename> class InternalFormat>
void merge_traces(const char* ompss_file_name, const char* ts_file_name);

void rename_trace_files(const std::string &out_name, const std::string &final_name, const std::string path_prefix);

}  // namespace tools
}  // namespace sim

#include "core/tools/ompss_util-impl.h"

#endif  // CORE_TOOLS_OMPSS_UTIL_H_
