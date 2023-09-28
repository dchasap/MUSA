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

#ifndef CORE_TOOLS_INSTR_BREAKDOWN_UTILS_H_
#define CORE_TOOLS_INSTR_BREAKDOWN_UTILS_H_

#include <unistd.h>
#include <map>
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
#include "core/trace/rle/Trace.h"
#include "core/trace/types.h"

#include "core/utils/Log.h"

using sim::utils::Log;

typedef sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
        sim::trace::selectedtrace::TraceInternal> trace_t;

bool equal_or_next(uint8_t first, uint8_t second);

void process_memory_phase(trace_t &reader,
        const sim::trace::ts::subtrace_t &subt,
        std::map<sim::trace::op_t, size_t> &counter,
        std::map<sim::trace::op_t, size_t> &counter_by_wd,
        int vector_register_length);

void print_stats(int wd, const std::map<sim::trace::op_t, size_t> &stats);

void usage(const char *command, const std::unordered_map<char, std::string> &options);

#endif  // CORE_TOOLS_INSTR_BREAKDOWN_UTILS_H_
