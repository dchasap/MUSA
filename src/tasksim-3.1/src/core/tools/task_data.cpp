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


void usage(const char *command, const std::unordered_map<char, std::string> &options)
{
    std::cerr << "Usage: " << command << " [options] -i trace_file_prefix" << std::endl
              << "    Options:" << std::endl;
    std::unordered_map<char, std::string>::const_iterator it;

    for (it = options.begin(); it != options.end(); ++it) {
        std::cerr << "\t-" << it->first << "\t" << it->second << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::unordered_map<char, std::string> options;

    std::string input_file;
    int res;
    while ((res = ::getopt(argc, argv, "i:")) != -1) {
        char c = static_cast<char>(res);
        switch (c) {
            case 'i':
                input_file = optarg;
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

    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> reader(input_file.c_str());

    unsigned n_task_types = reader.get_number_of_task_names();
    std::cout << n_task_types << std::endl;
    for (unsigned i = 0; i < n_task_types; i++) {
        std::cout << i << " " << reader.get_task_name(i) << std::endl;
    }
    unsigned n_tasks = reader.get_total_number_of_tasks();
    std::cout << n_tasks << std::endl;
    for (unsigned i = 0; i < n_tasks; i++) {
        const sim::trace::ts::subtrace_t &subt = reader.get_task_trace_by_id(i);
        std::cout << subt.wd_id << " " << subt.name_id << std::endl;
    }

    return 0;
}
