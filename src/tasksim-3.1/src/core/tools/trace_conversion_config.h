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

#ifndef CORE_TOOLS_TRACE_CONVERSION_CONFIG_H_
#define CORE_TOOLS_TRACE_CONVERSION_CONFIG_H_

#include <string>
#include <unordered_map>

// Configuration options:
enum trace_internal_t { MEM_INTERNAL = 0, UOP_INTERNAL = 1, RLE_INTERNAL = 2, ERROR_INTERNAL = 3};
enum trace_stream_t { TXT_STREAM = 0, BIN_STREAM = 1, ERROR_STREAM = 2};
enum trace_filetype_t { GZ_FILE = 0, NOGZ_FILE = 1, ERROR_FILE = 2};

class trace_conversion_config {
    const std::unordered_map<std::string, trace_internal_t> internal_dictionary_;
    const std::unordered_map<std::string, trace_stream_t> stream_dictionary_;
    const std::unordered_map<std::string, trace_filetype_t> filetype_dictionary_;

public:
    trace_internal_t input_internal_;
    trace_internal_t output_internal_;
    trace_stream_t input_stream_;
    trace_stream_t output_stream_;
    trace_filetype_t input_filetype_;
    trace_filetype_t output_filetype_;
    std::string input_name_;
    std::string output_name_;
    // Creator:
    trace_conversion_config();
    // Parses the string for the internal type
    trace_internal_t internal(const std::string &value);
    // Parses the string for the stream type
    trace_stream_t stream(const std::string &value);
    // Parses the string for the filetype
    trace_filetype_t filetype(const std::string &value);
    // Parses the input configuration string and updates input_ variables
    void parse_input_config(const std::string &config);
    // Parses the input configuration string and updates output_ variables
    void parse_output_config(const std::string &config);
    // Writes the variables values to an output stream
    friend std::ostream &operator<<(std::ostream &os, const trace_conversion_config& config);
};

#endif  // CORE_TOOLS_TRACE_CONVERSION_CONFIG_H_
