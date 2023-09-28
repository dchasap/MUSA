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

#include "core/tools/trace_conversion_config.h"

#include <iostream>
#include <string>

trace_conversion_config::trace_conversion_config() :
        internal_dictionary_({{"mem", MEM_INTERNAL}, {"uop", UOP_INTERNAL}, {"rle", RLE_INTERNAL}}),
        stream_dictionary_({{"bin", BIN_STREAM}, {"txt", TXT_STREAM}}),
        filetype_dictionary_({{"gz", GZ_FILE}, {"nogz", NOGZ_FILE}}),
        input_internal_(ERROR_INTERNAL),
        output_internal_(ERROR_INTERNAL),
        input_stream_(ERROR_STREAM),
        output_stream_(ERROR_STREAM),
        input_filetype_(ERROR_FILE),
        output_filetype_(ERROR_FILE),
        input_name_(""),
        output_name_("")
{}

trace_internal_t trace_conversion_config::internal(const std::string &value)
{
    std::cout << "Format " << value << std::endl;
    if (internal_dictionary_.find(value) != internal_dictionary_.end()) {
        return internal_dictionary_.find(value)->second;
    } else {
        std::cerr << "Do not recognize suplied internal: " << value << std::endl;
        std::cerr << "Options are:" << std::endl;
        for (auto it = internal_dictionary_.begin(); it != internal_dictionary_.end(); it++) {
            std::cerr << it->first << std::endl;
        }
        return ERROR_INTERNAL;
    }
}

trace_stream_t trace_conversion_config::stream(const std::string &value)
{
    std::cout << "StreamType " << value << std::endl;
    if (stream_dictionary_.find(value) != stream_dictionary_.end()) {
        std::cout << "returning " << stream_dictionary_.find(value)->second << std::endl;
        return stream_dictionary_.find(value)->second;
    } else {
        std::cerr << "Do not recognize suplied stream type: " << value << std::endl;
        std::cerr << "Options are:" << std::endl;
        for (auto it = stream_dictionary_.begin(); it != stream_dictionary_.end(); it++) {
            std::cerr << it->first << std::endl;
        }
        return ERROR_STREAM;
    }
}

trace_filetype_t trace_conversion_config::filetype(const std::string &value) {
    std::cout << "Filetype " << value << std::endl;
    if (filetype_dictionary_.find(value) != filetype_dictionary_.end()) {
        return filetype_dictionary_.find(value)->second;
    } else {
        std::cerr << "Do not recognize suplied filetype: " << value << std::endl;
        std::cerr << "Options are:" << std::endl;
        for (auto it = filetype_dictionary_.begin(); it != filetype_dictionary_.end(); it++) {
            std::cerr << it->first << std::endl;
        }
        return ERROR_FILE;
    }
}

void trace_conversion_config::parse_input_config(const std::string &config)
{
    int split = config.find('_');
    int second_split = config.find('_', split+1);
    if (split == -1 || second_split == -1) {
        std::cerr << "Error, invalid configuration option: " << config << std::endl
                  << "Use an option as mem_txt_gz, rle_bin_gz, etc." << std::endl;
        exit(1);
    }
    input_internal_ = internal(config.substr(0, split));
    input_stream_ = stream(config.substr(split+1, second_split - split - 1));
    input_filetype_ = filetype(config.substr(second_split+1));
}

void trace_conversion_config::parse_output_config(const std::string &config)
{
    int split = config.find('_');
    int second_split = config.find('_', split+1);
    if (split == -1 || second_split == -1) {
        std::cerr << "Error, invalid configuration option: " << config << std::endl
                  << "Use an option as mem_txt_gz, rle_bin_gz, etc." << std::endl;
        exit(1);
    }
    output_internal_ = internal(config.substr(0, split));
    output_stream_ = stream(config.substr(split+1, second_split - split - 1));
    output_filetype_ = filetype(config.substr(second_split+1));
}

std::ostream &operator<<(std::ostream &os, const trace_conversion_config& config)
{
    os << "Configuration contents are: " << std::endl;
    os << "Input name " << config.input_name_ << std::endl;
    os << "Output name " << config.output_name_ << std::endl;
    os << "Input internal " << config.input_internal_ << std::endl;
    os << "Output internal " << config.output_internal_ << std::endl;
    os << "Input EventStream " << config.input_stream_ << std::endl;
    os << "Output EventStream " << config.output_stream_ << std::endl;
    os << "Input Filetype " << config.input_filetype_ << std::endl;
    os << "Output Filetype " << config.output_filetype_ << std::endl;
    return os;
}
