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
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/trace/common/minst.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/ts/types.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/MemParser.h"
#include "core/trace/types.h"
#include "core/utils/CheckTrace.h"

#include "core/utils/Log.h"
#include "core/tools/instr_breakdown_utils.h"

using sim::utils::Log;

int main(int argc, char* argv[])
{
    std::unordered_map<char, std::string> options;

    options['x'] = "Break down only the selected WD (requires a WD as a parameter, incompatible with -w).";
    options['v'] = "Vector register length";
    options['p'] = "Print also breakdown by Work Descriptor";
    options['w'] = "Breakdown selected WD and its children (requires a WD as a parameter, incompatible with -x)";

    int vector_register_length = 64;
    bool x_flag = false, w_flag = false;
    bool print_by_wd = false;
    std::string input_file;
    unsigned selected_wd = 1;
    int res;
    while ((res = ::getopt(argc, argv, "pw:x:v:i:")) != -1) {
        char c = static_cast<char>(res);
        switch (c) {
            case 'i':
                input_file = optarg;
                break;
            case 'x':
                x_flag = true;
                selected_wd = std::atoi(optarg);
                break;
            case 'v':
                vector_register_length = std::atoi(optarg);
                break;
            case 'w':
                w_flag = true;
                selected_wd = std::atoi(optarg);
                break;
            case 'p':
                print_by_wd = true;
                break;
            default:
                usage(argv[0], options);
                return 1;
                break;
        }
    }

    if (input_file.empty() or (x_flag and w_flag)) {
        usage(argv[0], options);
        return argc == 1;
    }

    // Check that input trace exists.
    sim::utils::check_trace_exists(input_file);

    trace_t reader(input_file.c_str());

    std::queue<int> wds;
    wds.push(selected_wd);

    std::map<sim::trace::op_t, size_t> counter;

    while (not wds.empty()) {
        int wd = wds.front();
        wds.pop();

        const sim::trace::ts::subtrace_t subt = reader.get_task_trace_by_wd_id(wd);

        sim::trace::ts::event_t event;
        std::map<sim::trace::op_t, size_t> counter_by_wd;

        while (reader.get_task_event(wd, event)) {
            Log::debug() << "Processing event " << event;
            if (event.type == sim::trace::ts::CREATE_TASK and not x_flag) {
                Log::debug() << "Adding task " << event.u.create_task.wd_id << " to examine.";
                int new_wd_id = event.u.create_task.wd_id;
                wds.push(new_wd_id);
            } else if (event.type == sim::trace::ts::PHASE and reader.has_burst_instr_trace(event.u.phase)) {
                Log::debug() << "Memory segment.";
                process_memory_phase(reader, subt, counter, counter_by_wd, vector_register_length);
            }
        }
        if (print_by_wd) {
            print_stats(wd, counter_by_wd);
        }
    }
    print_stats(-1, counter);
    return 0;
}
