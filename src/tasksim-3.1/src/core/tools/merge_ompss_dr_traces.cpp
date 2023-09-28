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


#include <cassert>
#include <iostream>
#include <string>

#include "core/trace/dr/types.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/types.h"
#include "core/utils/CheckTrace.h"
#include "core/utils/Log.h"
#include "core/tools/ompss_util.h"

using sim::utils::Log;

int main(int argc, char* argv[])
{
    std::string ompss_trace_name, dr_trace_name;
    std::string output_trace_name;
    int res = 0;
    while ((res = ::getopt(argc, argv, "n:d:")) != -1) {
        char c = static_cast<char>(res);
        if (c == 'n') {
            ompss_trace_name = std::string(optarg);
        } else if (c == 'd') {
            dr_trace_name = std::string(optarg);
        } else {
            std::cout << "[ERROR] Unknown symbol " << c << "." << std::endl;
            exit(1);
        }
    }
    sim::tools::merge_traces<sim::trace::BinaryEventStream, sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal>(ompss_trace_name.c_str(), dr_trace_name.c_str());
    sim::tools::cleanup_ompss_trace(ompss_trace_name);
}
