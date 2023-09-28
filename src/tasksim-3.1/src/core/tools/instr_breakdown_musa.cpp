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
#include <iomanip>
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
#include "core/tools/instr_breakdown_utils.h"

typedef std::map<sim::trace::op_t, size_t> counter_t;

class mpiphase_info_t {
public:
    uint64_t phase_id;
    uint64_t wd_id;
    uint64_t is_nanos;
    uint64_t is_mem;
    uint64_t mpi_id;
    std::string mpi_name;
};

inline
std::ostream &operator<<(std::ostream &os, const mpiphase_info_t &phase_info)
{
    os << phase_info.phase_id << ":"
       << phase_info.wd_id << ":"
       << phase_info.is_nanos << ":"
       << phase_info.is_mem << ":"
       << phase_info.mpi_id << ":"
       << phase_info.mpi_name;
    return os;
}

inline
std::istream &operator>>(std::istream &is, mpiphase_info_t &phase_info)
{
    char sep;
    is >> phase_info.phase_id >> sep;
    assert(sep == ':');
    is >> phase_info.wd_id >> sep;
    assert(sep == ':');
    is >> phase_info.is_nanos >> sep;
    assert(sep == ':');
    is >> phase_info.is_mem >> sep;
    assert(sep == ':');
    is >> phase_info.mpi_id >> sep;
    assert(sep == ':');
    is >> phase_info.mpi_name;

    return is;
}


void populate_memory_wds(std::ifstream &phase_file,
                         std::queue<int> &wds)
{
    mpiphase_info_t info;
    while (phase_file >> info) {
        if (info.is_mem) {
            wds.push(info.wd_id);
        }
    }
}

void process_file(std::ifstream &phase_file,
                  const std::string &ts_filename,
                  counter_t &counter,
                  int vector_register_length)
{
    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> reader(ts_filename.c_str());
    std::cout << "[DEBUG] Processing file " << ts_filename << std::endl;
    std::queue<int> wds;
    populate_memory_wds(phase_file, wds);

    while (not wds.empty()) {
        int wd = wds.front();
        wds.pop();

        const sim::trace::ts::subtrace_t subt = reader.get_task_trace_by_wd_id(wd);

        sim::trace::ts::event_t event;

        while (reader.get_task_event(wd, event)) {
            if (event.type == sim::trace::ts::CREATE_TASK) {
                wds.push(event.u.create_task.wd_id);
            } else if (event.type == sim::trace::ts::PHASE and reader.has_burst_instr_trace(event.u.phase)) {
                process_memory_phase(reader, subt, counter, counter, vector_register_length);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    std::unordered_map<char, std::string> options;

    options['v'] = "Vector register length";
    options['i'] = "MUSA trace prefix before _XXXXXX.ts";

    int vector_register_length = 64;
    std::string input_file;
    int res;
    while ((res = ::getopt(argc, argv, "v:i:")) != -1) {
        char c = static_cast<char>(res);
        switch (c) {
            case 'i':
                input_file = optarg;
                break;
            case 'v':
                vector_register_length = std::atoi(optarg);
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


    // We check all files untill we find one that does not exist:
    int index = 1;
    counter_t counter;
    while (true) {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(6) << index;
        std::string padded_num = oss.str();
        std::string ts_filename = input_file + "_" + padded_num + ".ts";

        std::ifstream phase_file(ts_filename + ".mpiphases");

        if (not phase_file.is_open()) {
            // We reachde the first file that does not exist.
            break;
        } else {
            process_file(phase_file, ts_filename, counter, vector_register_length);
        }
        index++;
    }
    print_stats(-1, counter);
    return 0;
}
