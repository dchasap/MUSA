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



#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>

#include <core/trace/TextEventStream.h>
#include <core/trace/ts/Trace.h>
#include <core/trace/mem/Trace.h>

typedef struct {
    std::size_t cache_size;
} user_config;

void filter_trace(const std::string &ifilename, const std::string &ofilename, const user_config &cfg)
{
    sim::trace::ts::FileTraceReader<sim::trace::TextEventStream,sim::trace::mem::TraceInternal> input(ifilename.c_str());
    sim::trace::ts::FileTrace<sim::trace::TextEventStream,sim::trace::mem::TraceInternal> output(ofilename.c_str());

    const unsigned num_names = input.get_number_of_task_names();
    if (num_names == 0) {
        std::cerr << "Input trace format not recognized or not valid" << std::endl;
        exit(1);
    }

    const unsigned num_wds = input.get_total_number_of_tasks();
    if (num_wds == 0) {
        std::cerr << "Input trace format not recognized or not valid" << std::endl;
        exit(1);
    }
    std::cout << "number of tasks = " << num_wds << std::endl;

    for (unsigned n = 0; n < num_names; ++n) {
        const char* name = input.get_task_name(n);
        output.add_task_name(n, name);
        std::cout << "adding name " << name << ", n: " << n << std::endl;
    }

    sim::trace::ts::event_t event;
    for (unsigned w = 0; w < num_wds; ++w) {
        std::cout << "wd: " << w << std::endl;
        sim::trace::ts::subtrace_t tsubt = input.get_task_trace_by_id(w);
        unsigned subt_id = input.get_task_trace_id_by_name(tsubt.name_id_, tsubt.call_id_);
        output.add_task_trace(tsubt.name_id_, tsubt.call_id_, tsubt.stack_);
        std::cout << "subt: " << tsubt << std::endl;

        //add events
        while (input.get_task_event(w, event)) {
            output.add_task(w);
            switch (event.type) {
            case sim::trace::ts::PHASE:
                std::cout << "add task event: " << event << std::endl;
                output.add_task_event(w, sim::trace::ts::PHASE,
                        sim::trace::ts::phase_t( { event.u.phase.burst_id,
                                                   event.u.phase.subtrace_id,
                                                   event.u.phase.duration } ));
                if (event.u.phase.subtrace_id != sim::trace::ts::NO_SUBTRACE) {
                    sim::trace::ts::subtrace_t esubt = input.get_task_trace_by_id(event.u.phase.subtrace_id);
                    std::cout << "esubt: " << esubt << std::endl;
                    //assert(subt.mem_trace_id_ == subt2.mem_trace_id_);
                    sim::trace::mitem_t item = input.get_next_mem_item(esubt);
                    while(item.op_ != sim::trace::mitem_t::INVALID) {
                        std::cout << "item: " << item << std::endl;
                        output.add_mem_item_trace(esubt, item);
                        item = input.get_next_mem_item(esubt);
                    }
                    std::cout << "item: " << item << std::endl;
                    output.add_mem_item_trace(esubt, item);
                }
                break;
            case sim::trace::ts::CREATE_TASK:
                output.add_task_event(w, sim::trace::ts::CREATE_TASK,
                        sim::trace::ts::create_task_t( { event.u.create_task.wd_id } ));
                break;
            case sim::trace::ts::TASK_WAIT:
                output.add_task_event(w, sim::trace::ts::TASK_WAIT,
                        sim::trace::ts::task_wait_t( {} ));
                break;
            case sim::trace::ts::WAIT_ON:
                output.add_task_event(w, sim::trace::ts::WAIT_ON,
                        sim::trace::ts::wait_on_t( { subt_id } ));
                break;
            default:
                std::cerr << "Unrecognized event type " <<  event.type << std::endl;
                exit(1);
            }
        }
    }
}

//
// MAIN
//
void usage(const char* cmd, const std::unordered_map<char,std::string> &tests)
{
    std::cerr << "Usage: " << cmd << " [options]" << std::endl << "    Options:" << std::endl;
    std::unordered_map<char,std::string>::const_iterator it;

    for (it = tests.begin(); it != tests.end(); ++it) {
        std::cerr << "\t-" << it->first << "\t" << it->second << std::endl;
    }
}



int main(int argc, char* argv[])
{
    std::unordered_map<char,std::string> options;
    options['s'] = "Cache size";
    options['i'] = "Input trace file prefix";
    options['o'] = "Output trace file prefix";

    if (argc < 2) { usage(argv[0], options); exit(1); }

    user_config cfg;
    std::string input_file, output_file;
    char c;
    while((c = ::getopt(argc, argv,"s:i:o:")) != -1) {
        switch (c) {
        case 's':
            cfg.cache_size = std::atoi(optarg);
            break;
        case 'i':
            input_file = optarg;
            break;
        case 'o':
            output_file = optarg;
            break;
        default: {
            usage(argv[0], options);
            exit(1);
        }
        }
    }

    if (input_file.empty() or output_file.empty()) {
        usage(argv[0], options);
        return argc == 1;
    }

    std::cout << "Filter trace " << input_file
              << " with cache size " << cfg.cache_size
              << " and writing to " << output_file << std::endl;

    filter_trace(input_file, output_file, cfg);

    return 0;
}
