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

#include <unistd.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <stack>
#include <string>

#include "core/trace/ts/Trace.h"
#include "core/trace/ts/types.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/MemParser.h"
#include "core/trace/types.h"
#include "core/utils/CheckTrace.h"
#include "core/utils/Log.h"

using sim::utils::Log;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <trace_prefix>" << std::endl;
        exit(1);
    }

    // Check that input trace exists.
    sim::utils::check_trace_exists(argv[1]);
    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> reader(argv[1]);


    const char *dep_name[] = {"??", "in", "out", "inout"};
    const unsigned num_wds = reader.get_total_number_of_tasks();

    std::cout << "wd:description:duration:dependencies\n";
    for (unsigned w = 0; w < num_wds; ++w) {
        const sim::trace::ts::subtrace_t subtrace = reader.get_task_trace_by_id(w);
        if (subtrace.wd_id == 1) {
            continue;
        }

        uint64_t task_duration = 0;
        sim::trace::ts::event_t event;
        while (reader.get_task_event(subtrace.wd_id, event)) {
            if (event.type == sim::trace::ts::PHASE) {
                task_duration += event.u.phase.duration;
            }
        }

        std::vector<sim::trace::ts::dep_t> deps = reader.get_task_dependencies({subtrace.wd_id});

        std::cout << subtrace.wd_id << ':' << reader.get_task_name(subtrace.name_id) << ':' << task_duration << ':' << deps.size();
        for (const auto &dep: deps) {
            if (dep.flags.can_rename)
                abort();

            size_t size = 1;

            // Only support contiguous data ranges:
            // - if all dimensions have size == accessed_length, ok
            // - if one dimension has size > accessed_length, all deps above must have accessed_length == 1
            bool has_partial_dim = false;

            for (const auto &dim: dep.dimensions)
            {
                size *= dim.size;

                if (has_partial_dim && dim.accessed_length != 1)
                    abort();
                else if (dim.size != dim.accessed_length)
                    has_partial_dim = true;
            }

            std::cout << ':' << std::hex << ((uintptr_t)dep.address + dep.offset) << std::dec << ':' << size << ':';
            if (dep.flags.commutative)
                std::cout << "commutative";
            else if (dep.flags.concurrent)
                std::cout << "concurrent";
            else
                std::cout << dep_name[dep.flags.output * 2 + dep.flags.input];
        }
        std::cout << std::endl;
    }

    return 0;
}
