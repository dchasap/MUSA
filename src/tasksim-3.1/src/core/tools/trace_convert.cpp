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
#include <dirent.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <map>

#include "core/tools/memory_transfer.h"
#include "core/tools/trace_conversion_config.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/ts/nanos.h"
#include "core/trace/ts/types.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/types.h"
#include "core/utils/CheckTrace.h"

#include "core/utils/Log.h"

using sim::trace::minst_t;
using sim::utils::Log;

void usage(const char* cmd, const std::unordered_map<char, std::string> &tests)
{
    std::cerr << "Usage: " << cmd << " [options]" << std::endl << "    Options:" << std::endl;
    std::unordered_map<char, std::string>::const_iterator it;

    for (it = tests.begin(); it != tests.end(); ++it) {
        std::cerr << "\t-" << it->first << "\t" << it->second << std::endl;
    }
}


template<typename InputFormat, typename OutputFormat,
         template<typename, typename> class MemoryParser>
void transfer_file(const std::string &input_name, const std::string &output_name)
{
    InputFormat input(input_name.c_str());
    // First we get the number of tasks and work descriptors:
    uint32_t n_tasks = input.get_number_of_task_names();
    uint32_t n_wds = input.get_total_number_of_tasks();
    if (n_wds == 0) {
        std::cerr << "Number of tasks equals 0?" << std::endl;
        exit(1);
    }
    // We frame this block so the file is closed uppon having the base structure:
    // This is done to preserve .default.trace information order.
    // It is not strictly required, but the final order is nicer.
    {
        OutputFormat output(output_name.c_str());
        for (uint32_t i = 0; i < n_tasks; i++) {
            output.add_task_name(i, input.get_task_name(i));
        }
        for (unsigned w = 0; w < n_wds; ++w) {
            sim::trace::ts::subtrace_t tsubt = input.get_task_trace_by_id(w);
            output.add_task_trace(tsubt.wd_id, tsubt.name_id, tsubt.stack);
            std::vector<sim::trace::ts::dep_t> deps = input.get_task_dependencies({w+1});
            // To avoid dependencies on the main task.
            if (w > 0) {
                output.add_wd_info(w+1, deps);
            }
            sim::trace::ts::subtrace_t isubt = input.get_task_trace_by_id(w);
            sim::trace::ts::subtrace_t osubt = output.get_task_trace_by_id(w);
            input.get_wait_on_dependencies(isubt, deps);
            // To avoid adding void dependencies
            if (deps.size() > 0) {
                output.add_wait_on_dependencies(osubt, deps);
            }
        }
    }
    sim::trace::ts::event_t event;
    OutputFormat output(output_name.c_str());
    for (unsigned w = 1; w <= n_wds; ++w) {
        output.add_task(w);
        // add events
        while (input.get_task_event(w, event)) {
            switch (event.type) {
            case sim::trace::ts::PHASE:
                Log::debug2() << "add task event: " << event;
                output.add_task_event(w, sim::trace::ts::PHASE,
                        sim::trace::ts::phase_t({event.u.phase.burst_id,
                                                 event.u.phase.subtrace_id,
                                                 event.u.phase.duration}));
                // If the event has a memory subtrace, we add the memory accesses (or memory + basic blocks)
                if (event.u.phase.subtrace_id != sim::trace::ts::NO_SUBTRACE) {
                    MemoryParser<InputFormat, OutputFormat>::convert_m(input, output, event.u.phase.subtrace_id);
                }
                break;
            case sim::trace::ts::CREATE_TASK:
                Log::debug2() << "add create task: " << event;
                output.add_task_event(w, sim::trace::ts::CREATE_TASK,
                        sim::trace::ts::create_task_t({ event.u.create_task.wd_id }));
                break;
            case sim::trace::ts::TASK_WAIT:
                Log::debug2() << "add task wait: " << event;
                output.add_task_event(w, sim::trace::ts::TASK_WAIT,
                        sim::trace::ts::task_wait_t({}));
                break;
            case sim::trace::ts::WAIT_ON:
                Log::debug2() << "add wait on: " << event;
                output.add_task_event(w, sim::trace::ts::WAIT_ON,
                        sim::trace::ts::wait_on_t({event.u.wait_on.subtrace_id}));
                break;
            default:
                std::cerr << "Unrecognized event type " <<  event.type << std::endl;
                exit(1);
            }
        }
    }
}

template<typename InputFile,
         template<typename> class OutputEventStream,
         template<template<typename> class, typename> class OutputInternal,
         template<typename, typename> class MemParser>
inline void convert_5(const trace_conversion_config& cfg)
{
    // Check for output file compression:
    if (cfg.output_filetype_ == GZ_FILE) {
        typedef sim::trace::ts::FileTrace<OutputEventStream, OutputInternal,
                        sim::trace::CompressedFileTrace<OutputEventStream, std::fstream>> OutputFile;
        transfer_file<InputFile, OutputFile, MemParser>(cfg.input_name_, cfg.output_name_);
    } else if (cfg.output_filetype_ == NOGZ_FILE) {
        typedef sim::trace::ts::FileTrace<OutputEventStream, OutputInternal,
                        sim::trace::FileTrace<OutputEventStream, std::fstream>> OutputFile;
        transfer_file<InputFile, OutputFile, MemParser>(cfg.input_name_, cfg.output_name_);
    } else {
        std::cerr << "Output FileType not recognized." << std::endl;
        std::cerr << cfg;
        std::abort();
    }
}

template<template<typename> class InputEventStream,
         template<template<typename> class, typename> class InputInternal,
         template<typename> class OutputEventStream,
         template<template<typename> class, typename> class OutputInternal,
         template<typename, typename> class MemParser>
inline void convert_4(const trace_conversion_config& cfg)
{
    // Check for Input file compression:
    if (cfg.input_filetype_ == GZ_FILE) {
        typedef sim::trace::ts::FileTraceReader<InputEventStream, InputInternal,
                        sim::trace::CompressedFileTrace<InputEventStream, std::ifstream>> InputFormat;
        convert_5<InputFormat, OutputEventStream, OutputInternal, MemParser>(cfg);
    } else if (cfg.input_filetype_ == NOGZ_FILE) {
        typedef sim::trace::ts::FileTraceReader<InputEventStream, InputInternal,
                        sim::trace::FileTrace<InputEventStream, std::ifstream>> InputFormat;
        convert_5<InputFormat, OutputEventStream, OutputInternal, MemParser>(cfg);
    } else {
        std::cerr << "Input FileType not recognized." << std::endl;
        std::cerr << cfg;
        std::abort();
    }
}

template<template<typename> class InputEventStream,
         template<typename> class OutputEventStream>
inline void convert_3(const trace_conversion_config& cfg)
{
    // Here we choose both internals.
    // Realize that we do both at the same time to also select the memory access converter class.
    if (cfg.input_internal_ == MEM_INTERNAL && cfg.output_internal_ == MEM_INTERNAL) {
        convert_4<InputEventStream, sim::trace::mem::TraceInternal,
                  OutputEventStream, sim::trace::mem::TraceInternal, mem_to_mem>(cfg);
    } else if (cfg.input_internal_ == MEM_INTERNAL && cfg.output_internal_ == UOP_INTERNAL) {
        convert_4<InputEventStream, sim::trace::mem::TraceInternal,
                  OutputEventStream, sim::trace::uop::TraceInternal, mem_to_uop>(cfg);
    }  else if (cfg.input_internal_ == MEM_INTERNAL && cfg.output_internal_ == RLE_INTERNAL) {
        convert_4<InputEventStream, sim::trace::mem::TraceInternal,
                  OutputEventStream, sim::trace::rle::TraceInternal, mem_to_uop>(cfg);
    } else if (cfg.input_internal_ == UOP_INTERNAL && cfg.output_internal_ == MEM_INTERNAL) {
        convert_4<InputEventStream, sim::trace::uop::TraceInternal,
                  OutputEventStream, sim::trace::mem::TraceInternal, uop_to_mem>(cfg);
    } else if (cfg.input_internal_ == UOP_INTERNAL && cfg.output_internal_ == UOP_INTERNAL) {
        convert_4<InputEventStream, sim::trace::uop::TraceInternal,
                  OutputEventStream, sim::trace::uop::TraceInternal, uop_to_uop>(cfg);
    } else if (cfg.input_internal_ == UOP_INTERNAL && cfg.output_internal_ == RLE_INTERNAL) {
        convert_4<InputEventStream, sim::trace::uop::TraceInternal,
                  OutputEventStream, sim::trace::rle::TraceInternal, uop_to_uop>(cfg);
    } else if (cfg.input_internal_ == RLE_INTERNAL && cfg.output_internal_ == MEM_INTERNAL) {
        convert_4<InputEventStream, sim::trace::rle::TraceInternal,
                  OutputEventStream, sim::trace::mem::TraceInternal, uop_to_mem>(cfg);
    } else if (cfg.input_internal_ == RLE_INTERNAL && cfg.output_internal_ == UOP_INTERNAL) {
        convert_4<InputEventStream, sim::trace::rle::TraceInternal,
                  OutputEventStream, sim::trace::uop::TraceInternal, uop_to_uop>(cfg);
    } else if (cfg.input_internal_ == RLE_INTERNAL && cfg.output_internal_ == RLE_INTERNAL) {
        convert_4<InputEventStream, sim::trace::rle::TraceInternal,
                  OutputEventStream, sim::trace::rle::TraceInternal, uop_to_uop>(cfg);
    } else {
        std::cerr << "Input-Output Internal nor recognized." << std::endl;
        std::cerr << cfg;
        std::abort();
    }
}

template<template<typename> class InputEventStream>
inline void convert_2(const trace_conversion_config& cfg)
{
    // Second we find the EventStreamType for the output file:
    if (cfg.output_stream_ == TXT_STREAM) {
        convert_3<InputEventStream, sim::trace::TextEventStream>(cfg);
    } else if (cfg.output_stream_ == BIN_STREAM) {
        convert_3<InputEventStream, sim::trace::BinaryEventStream>(cfg);
    } else {
        std::cerr << "Unknown Output file Event Stream " << std::endl;
        std::cerr << cfg;
        std::abort();
    }
}

void convert(const trace_conversion_config& cfg)
{
    // First we find the EventStreamType for the input file:
    if (cfg.input_stream_ == TXT_STREAM) {
        convert_2<sim::trace::TextEventStream>(cfg);
    } else if (cfg.input_stream_ == BIN_STREAM) {
        convert_2<sim::trace::BinaryEventStream>(cfg);
    } else {
        std::cerr << "Unknown Input file Event Stream." << std::endl;
        std::cerr << cfg;
        std::abort();
    }
}

int main(int argc, char* argv[])
{
    // Argument flags:
    std::unordered_map<char, std::string> options;
    options['i'] = "Input trace file prefix";
    options['o'] = "Output trace file prefix";
    options['f'] = "Input trace format";
    options['g'] = "Output trace format";

    if (argc < 9) {
        usage(argv[0], options);
        return argc == 1;
    }

    // We parse the input arguments:
    trace_conversion_config cfg;
    char c;
    while ((c = ::getopt(argc, argv, "i:o:f:g:")) != -1) {
        switch (c) {
        case 'i':
            cfg.input_name_ = std::string(optarg);
            break;
        case 'o':
            cfg.output_name_ = std::string(optarg);
            break;
        case 'f':
            cfg.parse_input_config(std::string(optarg));
            break;
        case 'g':
            cfg.parse_output_config(std::string(optarg));
            break;

        default:
            usage(argv[0], options);
            exit(1);
        }
    }

    if (cfg.input_internal_ == ERROR_INTERNAL || cfg.output_internal_ == ERROR_INTERNAL ||
        cfg.input_stream_ == ERROR_STREAM || cfg.output_stream_ == ERROR_STREAM ||
        cfg.input_filetype_ == ERROR_FILE || cfg.output_filetype_ == ERROR_FILE) {
        return 1;
    }

    // Check that both filenames are not the same:
    if (not cfg.input_name_.compare(cfg.output_name_)) {
        std::cerr << "Do not use the same name for input and output." << std::endl;
        return 1;
    }

    // Check that input trace exists.
    sim::utils::check_trace_exists(cfg.input_name_);
    sim::utils::check_trace_exists(cfg.output_name_);

    // Convert!
    convert(cfg);
}
