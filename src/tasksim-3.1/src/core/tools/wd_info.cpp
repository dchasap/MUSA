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

typedef sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> trace_t;

void usage(const char *command, const std::unordered_map<char, std::string> &options)
{
    std::cerr << "Usage: " << command << " [options] -i trace_file_prefix" << std::endl
              << "    Options:" << std::endl;
    std::unordered_map<char, std::string>::const_iterator it;

    for (it = options.begin(); it != options.end(); ++it) {
        std::cerr << "\t-" << it->first << "\t" << it->second << std::endl;
    }
}

void print_info_wd(trace_t &trace_reader, unsigned wd)
{
    const sim::trace::ts::subtrace_t subtrace = trace_reader.get_task_trace_by_wd_id(wd);
    const std::string task_name(trace_reader.get_task_name(subtrace.name_id));
    std::cout << "WD " << wd << ": " << task_name << std::endl;
    int c = 1;
    sim::trace::ts::event_t event;
    while (trace_reader.get_task_event(wd, event)) {
        std::cout << "Event " << c++ << " " << event << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::unordered_map<char, std::string> options;

    options['w'] = "Print all phases belonging to the  selected WD)";

    bool w_flag = false;
    std::string input_file;
    unsigned wd = 1;
    int res;
    while ((res = ::getopt(argc, argv, "w:i:")) != -1) {
        char c = static_cast<char>(res);
        switch (c) {
            case 'i':
                input_file = optarg;
                break;
            case 'w':
                w_flag = true;
                wd = std::atoi(optarg);
                break;
            default:
                usage(argv[0], options);
                return 1;
                break;
        }
    }

    if (input_file.empty()) {
        usage(argv[0], options);
        return argc == 1;
    }

    // Check that input trace exists.
    sim::utils::check_trace_exists(input_file);

    trace_t trace_reader(input_file.c_str());
    if (w_flag) {
        print_info_wd(trace_reader, wd);
    } else {
        for (unsigned i = 0; i < trace_reader.get_total_number_of_tasks(); i++) {
            const sim::trace::ts::subtrace_t subtrace = trace_reader.get_task_trace_by_id(i);
            print_info_wd(trace_reader, subtrace.wd_id);
        }
    }

    return 0;
}
