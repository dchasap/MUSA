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


#include "Config.h"

#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include "core/utils/Log.h"

namespace sim {
namespace engine {

using sim::utils::Log;

const std::set<std::string> Config::retired_keys({"cpu_freq_mhz"});

Config::Config(const std::string &file_name) :
        valid_(false),
        section_("Global")
{
    std::ifstream conf_file(file_name.c_str());
    if (conf_file.good() == false) {
        std::cerr << "[ERROR] Cannot open configuration file " << file_name << std::endl;
        std::abort();
        return;
    }

    std::string line, label, key, value;
    unsigned line_count = 0;
    label_map_t::iterator current = labels_.find("Global");
    if (current == labels_.end()) {
        std::pair<label_map_t::iterator, bool> ret =
                labels_.insert(label_map_t::value_type("Global", key_map_t()));
        if (ret.second == false) {
            return;
        }
        current = ret.first;
    }

    while (std::getline(conf_file, line)) {
        line_count++;
        std::string::size_type begin = line.find_first_not_of(" \t\f\v\n\r");

        /* Blank line */
        if (begin == std::string::npos) {
            continue;
        }
        /* Comment */
        if (std::string("#;").find(line[begin]) != std::string::npos) {
            continue;
        }

        /* Check for new label */
        if (std::string("[").find(line[begin]) != std::string::npos) {
            std::string::size_type end = line.find(']', begin);
            label = line.substr(begin + 1, end - begin - 1);
            std::pair<label_map_t::iterator, bool> ret =
                labels_.insert(label_map_t::value_type(label, key_map_t()));
            current = ret.first;
        } else {
            /* Read the key */
            std::string::size_type end = line.find('=', begin);
            key = line.substr(begin, end - begin);
            key.erase(key.find_last_not_of(" \t\f\v") + 1);
            if (key.empty()) {
                std::cerr << "[WARNING] Invalid key at line " << line_count << std::endl;
                continue;
            } else if (retired_keys.find(key) != retired_keys.end()) {
                std::cerr << "[WARNING] Key \"" << key << "\" at line " << line_count
                          << " is deprecated, please update your configuration file." << std::endl;
                exit(-1);
            }

            while (begin != line.npos and end != line.npos and line[begin] != '#' and line[end] != '#') {
                /* Read the value */
                begin = line.find_first_not_of(" \t\f\v\n\r", end + 1);
                if (begin != line.npos and line[begin] != '#') {
                    end = line.find_first_of(" \t\f\v\r\n#;", begin + 1);
                    value = line.substr(begin, end - begin);

                    /* Insert the key */
                    current->second[key].push_back(value);
                }
            }
        }
    }
    valid_ = true;
}

Config::Config(const Config &parent, const std::string &label) :
    valid_(false),
    section_(label)
{
    label_map_t::const_iterator subtree =  parent.labels_.find(label);
    if (subtree == parent.labels_.end()) {
        Log::error() << "TaskSim configuration lacks a [" << label << "] section";
        exit(-1);
        return;
    }

    key_map_t &target = this->labels_["Global"];
    key_map_t::const_iterator k;
    for (k = subtree->second.begin(); k != subtree->second.end(); k++) {
        target.insert(key_map_t::value_type(k->first, k->second));
    }
    valid_ = true;
}

Config::Config(const Config &parent, const std::string &label, unsigned index) :
    valid_(false),
    section_(label)
{
    label_map_t::const_iterator subtree =  parent.labels_.find(label);
    if (subtree == parent.labels_.end()) {
        Log::error() << "TaskSim configuration lacks a [" << label << "] section";
        exit(-1);
        return;
    }

    key_map_t &target = this->labels_["Global"];
    key_map_t::const_iterator k;
    for (k = subtree->second.begin(); k != subtree->second.end(); k++) {
        value_t t;
        t.push_back(k->second[index]);
        target.insert(key_map_t::value_type(k->first, t));
    }
    valid_ = true;
}


Config::~Config()
{
}


void Config::print() const
{
    for (auto label : labels_) {
        std::cout << label.first << std::endl;
        for (auto param : label.second) {
            std::cout << "\t" << param.first << " =";
            for (auto val : param.second) {
                std::cout << " " << val;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}


bool Config::is_valid() const
{
    return valid_;
}

}  // namespace engine
}  // namespace sim
