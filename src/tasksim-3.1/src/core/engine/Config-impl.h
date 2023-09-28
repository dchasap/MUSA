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


#ifndef CORE_ENGINE_CONFIG_IMPL_H_
#define CORE_ENGINE_CONFIG_IMPL_H_

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>

#include "core/utils/Log.h"

namespace sim {
namespace engine {

using sim::utils::Log;

inline
Config::Config() :
//    valid_(true)
    valid_(false),
    section_("Global")
{
}

template<typename T>
inline
std::vector<T> Config::get_values(const std::string &label, const std::string &key) const
{
    label_map_t::const_iterator keys = labels_.find(label);
    if (keys == labels_.end()) {
        Log::error() << "[ERROR] Searching for label: " << label << " val: " << key;
        Log::error() << "[ERROR] At configuration from section " << section_;
        Log::error() << "[ERROR] Cannot find label " << label;
        exit(-1);
    }
    key_map_t::const_iterator i = keys->second.find(key);
    if (i == keys->second.end()) {
        Log::error() << "[ERROR] Searching for label: " << label << " val: " << key;
        Log::error() << "[ERROR] At configuration from section " << section_;
        Log::error() << "[ERROR] Found label " << label << ", but cannot find key " << key;
        exit(-1);
    }
    std::vector<T> ret;
    value_t::const_iterator v;
    for (v = i->second.begin(); v != i->second.end(); v++) {
        T tmp;
        std::stringstream ss(*v);
        ss >> tmp;
        ret.push_back(tmp);
    }
    return ret;
}

template<typename T>
inline
std::vector<T> Config::get_values(const std::string &key) const
{
    return get_values<T>("Global", key);
}

template<typename T>
inline
std::vector<T> Config::get_values_with_default(const std::string &label, const std::string &key, const std::vector<T> default_values) const
{
    if (key_exists(label, key)) {
        return get_values<T>(label, key);
    } else {
        Log::debug() << "[Config] Cannot find key " << key << " at label " << label
                       << " using default values";
        Log::debug() << "[Config] At configuration from section " << section_;
        return default_values;
    }
}

template<typename T>
inline
std::vector<T> Config::get_values_with_default(const std::string &key, const std::vector<T> default_values) const
{
    return get_values_with_default<T>("Global", key, default_values);
}

template<typename T>
inline
T Config::get_value(const std::string &label, const std::string &key, unsigned index) const
{
    T ret;
    label_map_t::const_iterator keys = labels_.find(label);
    if (keys == labels_.end()) {
        Log::error() << "[ERROR] Searching for label: " << label << " val: " << key;
        Log::error() << "[ERROR] At configuration from section " << section_;
        Log::error() << "[ERROR] Cannot find label " << label;
        exit(-1);
    }
    key_map_t::const_iterator i = keys->second.find(key);
    if (i == keys->second.end()) {
        Log::error() << "[ERROR] Searching for label: " << label << " val: " << key;
        Log::error() << "[ERROR] At configuration from section " << section_;
        Log::error() << "[ERROR] Found label " << label << ", but cannot find key " << key;
        exit(-1);
    }
    std::stringstream ss(i->second[0]);
    ss >> ret;
    return ret;
}

template<>
inline
bool Config::get_value<bool>(const std::string &label, const std::string &key, unsigned index) const
{
    std::string val = get_value<std::string>(label, key, index);
    std::transform(val.begin(), val.end(), val.begin(), [](unsigned char c){ return std::tolower(c); });
    if (val.compare("true") == 0 or val.compare("yes") == 0 or val.compare("1") == 0 or val.compare("on") == 0) {
        return true;
    } else if (val.compare("false") == 0 or val.compare("no") == 0 or val.compare("0") == 0 or val.compare("off") == 0) {
        return false;
    } else {
        Log::error() << "[ERROR] Label: " << label << " val: " << key;
        Log::error() << val << " cannot be cast to bool";
        exit(-1);
    }
}


template<typename T>
inline
T Config::get_value(const std::string &key, unsigned index) const
{
    return get_value<T>("Global", key, index);
}

template<typename T>
inline
T Config::get_value_with_default(const std::string &label, const std::string &key, T default_value, unsigned index) const
{
    if (key_exists(label, key)) {
        return get_value<T>("Global", key, index);
    } else {
        Log::debug() << "[Config] Cannot find key " << key << " at label " << label
                       << " using default value " << default_value;
        Log::debug() << "[Config] At configuration from section " << section_;
        return default_value;
    }
}

template<typename T>
inline
T Config::get_value_with_default(const std::string &key, T default_value,  unsigned index) const
{
    return get_value_with_default<T>("Global", key, default_value, index);
}


template<typename T>
inline
bool Config::add_value(const T &value, const std::string &key, const std::string &label)
{
    std::stringstream ss;
    ss << value;
    key_map_t &target = this->labels_[label];
    key_map_t::iterator i = target.find(key);
    if (i != target.end()) {
        i->second.push_back(ss.str());
    } else {
        value_t v;
        v.push_back(ss.str());
        target.insert(key_map_t::value_type(key, v));
    }
    return true;
}


inline
void Config::set_values(const std::string &value, const std::string &key, const std::string &label)
{
    std::istringstream iss(value);
    value_t v{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
    key_map_t &target = this->labels_[label];
    target[key] = std::move(v);
}


inline
bool Config::key_exists(const std::string &key) const
{
    return key_exists("Global", key);
}

inline
bool Config::key_exists(const std::string &label, const std::string &key) const
{
    label_map_t::const_iterator keys = labels_.find(label);
    if (keys == labels_.end()) {
        Log::error() << "[ERROR] Searching for label: " << label << " val: " << key;
        Log::error() << "[ERROR] At configuration from section " << section_;
        Log::error() << "[ERROR] Cannot find label " << label;
        exit(-1);
    }
    key_map_t::const_iterator i = keys->second.find(key);

    return (i != keys->second.end());
}

}  // namespace engine
}  // namespace sim



#endif  // CORE_ENGINE_CONFIG_IMPL_H_
