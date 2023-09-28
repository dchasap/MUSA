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
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <map>

#include "core/utils/instrumentation/Instrumentation.h"
#include "core/utils/instrumentation/Paraver.h"

struct conversion_config {
    std::string input_name_;
    std::string output_name_;
    bool binary_;
    conversion_config() : input_name_(), output_name_(), binary_(true) {}
};

void usage(const char* cmd, const std::unordered_map<char, std::string> &tests)
{
    std::cerr << "Usage: " << cmd << " [options]" << std::endl << "    Options:" << std::endl;
    std::unordered_map<char, std::string>::const_iterator it;

    for (it = tests.begin(); it != tests.end(); ++it) {
        std::cerr << "\t-" << it->first << "\t" << it->second << std::endl;
    }
}



void convert(const conversion_config& cfg)
{
    std::cout << "Converting paraver intermediate file:" << std::endl;
    std::cout << " - Input file: " << cfg.input_name_ << std::endl;
    std::cout << " - Output file: " << cfg.output_name_ << std::endl;
    std::cout << " - Binary input?: " << (cfg.binary_ ? "true" : "false") << std::endl;

    sim::utils::instrumentation::Paraver paraver_tracing;
    paraver_tracing.set_text_trace(not cfg.binary_);
    paraver_tracing.load_temporary_trace_restrictions(cfg.input_name_);
    paraver_tracing.generate_final_paraver_file(cfg.output_name_, cfg.input_name_);
    std::cout << "Trace successfully converted." << std::endl;
    // First we find the EventStreamType for the input file:
}

int main(int argc, char* argv[])
{
    // Argument flags:
    std::unordered_map<char, std::string> options;
    options['i'] = "Input trace file";
    options['o'] = "Output trace file";
    options['t'] = "indicates that the input trace is in text format.";

    if (argc < 5) {
        usage(argv[0], options);
        return argc == 1;
    }

    // We parse the input arguments:
    conversion_config cfg;
    char c;
    while ((c = ::getopt(argc, argv, "i:o:t")) != -1) {
        switch (c) {
        case 'i':
            cfg.input_name_ = std::string(optarg);
            break;
        case 'o':
            cfg.output_name_ = std::string(optarg);
            break;
        case 't':
            cfg.binary_ = false;
            break;
        default:
            usage(argv[0], options);
            return 1;
        }
    }
    // Check that both filenames are not the same:
    if (not cfg.input_name_.compare(cfg.output_name_)) {
        std::cerr << "ERROR: Do not use the same name for input and output." << std::endl;
        return 1;
    }
    // Convert!
    convert(cfg);
}
