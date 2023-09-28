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

#include <algorithm>
#include <iostream>
#include <iomanip>
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

class Histo {
private:
    double low, upp;
    unsigned buckets;
    std::vector<unsigned> values;
    std::vector<double> underflows, overflows;

public:
    Histo(double lower, double upper, unsigned count) :
        low(lower),
        upp(upper),
        buckets(count),
        values(count, 0) {}
    void sample(double val) {
        if (val < low) {
            underflows.push_back(val);
        } else if (val > upp) {
            overflows.push_back(val);
        } else {
            values[static_cast<unsigned>((val - low) * static_cast<double>(buckets) / (upp - low))]++;
        }
    }
    void print() {
        for (unsigned b = 0; b < buckets; b++) {
            std::cout.setf(std::ios::fixed, std::ios::floatfield);
            std::cout << "[" << std::setprecision(1) << (low+b*(upp-low)/buckets)
                      << ":" << std::setprecision(1) << (low+(b+1)*(upp-low)/buckets);

            std::cout.unsetf(std::ios::floatfield);
            std::cout << "]: " << values[b] << std::endl;
        }
        std::cout << "Underflows: " << underflows.size() << std::endl;
        if (underflows.size() > 0) {
            std::sort(underflows.begin(), underflows.end());
            for (auto &u : underflows) {
                std::cout << " " << std::setprecision(5) << u;
            }
            std::cout << std::endl;
        }
        std::cout << "Overflows: " << overflows.size() << std::endl;
        if (overflows.size() > 0) {
            std::sort(overflows.begin(), overflows.end());
            for (auto &o : overflows) {
                std::cout << " " << std::setprecision(5) << o;
            }
            std::cout << std::endl;
        }
    }
};

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

    options['d'] = "Prit additiona debug information";
    options['i'] = "Input trace";
    options['f'] = "Traced frequency (In Mhz)";
    options['c'] = "Phase time cutoff for the histogram";
    bool debug = false;
    std::string input_file;
    uint64_t cutoff = 1000;
    double  freq = 2000.0;
    int res;
    while ((res = ::getopt(argc, argv, "dc:i:f:")) != -1) {
        char c = static_cast<char>(res);
        switch (c) {
            case 'd':
                debug = true;
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'f':
                freq = std::stoll(optarg);
                break;
            case 'c':
                cutoff = std::stod(optarg);
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

    double underflow = 0.5;
    double overflow = 5.0;
    Histo hist(underflow, overflow, 45);
    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> reader(input_file.c_str());

    const unsigned num_wds = reader.get_total_number_of_tasks();
    if (num_wds == 0) {
        std::cerr << "[ERROR] Trace " << input_file
                  << " not found or contains no WD data." << std::endl;
        exit(1);
    }
    std::unordered_map<unsigned, unsigned> phase_count_per_wd;
    sim::trace::ts::event_t event;
    for (unsigned w = 0; w < num_wds; ++w) {
        const sim::trace::ts::subtrace_t subt = reader.get_task_trace_by_id(w);
        const std::string task_name(reader.get_task_name(subt.name_id));
        unsigned task_id = subt.wd_id;
        uint64_t burst_duration = 0, total_instructions = 0;
        while (reader.get_task_event(subt.wd_id, event)) {
            if (event.type == sim::trace::ts::PHASE and
                    event.u.phase.subtrace_id != sim::trace::ts::NO_SUBTRACE) {
                double duration = static_cast<double>(event.u.phase.duration);
                const sim::trace::user_code_data_t &user_data = reader.get_user_code_data(task_id);
                if (user_data.data.size() <= phase_count_per_wd[task_id]) {
                    std::cout << user_data.data.size() << " not > " << phase_count_per_wd[task_id] << std::endl;
                }
                assert(user_data.data.size() > phase_count_per_wd[task_id]);
                const uint64_t phase_instr = user_data.data[phase_count_per_wd[task_id]++].n_instrs;
                double instructions = static_cast<double>(phase_instr);
                double ipc = instructions / (duration * freq / 1000.0);
                if (phase_instr > cutoff) {
                    hist.sample(ipc);
                    if ((ipc > overflow or ipc < underflow) and debug) {
                        std::cout << "[WARNING] Phase with duration " << duration
                                  << " ns and " << instructions
                                  << " instructions, IPC = " << ipc << std::endl;
                        std::cout << "At WD " << task_id << " phase #"
                                  << phase_count_per_wd[task_id] << std::endl;
                    }
                }
                burst_duration += static_cast<double>(event.u.phase.duration);
                total_instructions += phase_instr;
            }
        }
        if (debug) {
            std::cout << "WD " << task_id  << " " << task_name << " has "
                      << total_instructions << "instructions in "
                      << burst_duration << " nanoseconds, so IPC = "
                      << (static_cast<double>(total_instructions) /
                          (static_cast<double>(burst_duration) *
                          static_cast<double>(freq)/1000.0)) << std::endl;
        }
    }
    hist.print();
    return 0;
}
