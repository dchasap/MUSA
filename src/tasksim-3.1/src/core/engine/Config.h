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


#ifndef CORE_ENGINE_CONFIG_H_
#define CORE_ENGINE_CONFIG_H_

#include <vector>
#include <set>
#include <string>
#include <unordered_map>

namespace sim {
namespace engine {

class Config {
protected:
    typedef std::vector<std::string> value_t;
    typedef std::unordered_map<std::string, value_t> key_map_t;
    typedef std::unordered_map<std::string, key_map_t> label_map_t;

    label_map_t labels_;
    bool valid_;
    std::string section_;

    static const std::set<std::string> retired_keys;

public:
    Config();
    explicit Config(const std::string &filename);
    Config(const Config &parent, const std::string &label);
    Config(const Config &parent, const std::string &label, unsigned index);
    ~Config();

    bool is_valid() const;

    template<typename T>
    std::vector<T> get_values(const std::string &label, const std::string &key) const;

    template<typename T>
    std::vector<T> get_values(const std::string &key) const;

    template<typename T>
    std::vector<T> get_values_with_default(const std::string &key, const std::vector<T> default_values) const;

    template<typename T>
    std::vector<T> get_values_with_default(const std::string &label, const std::string &key, const std::vector<T> default_values) const;

    template<typename T>
    T get_value(const std::string &key, unsigned index = 0) const;

    template<typename T>
    T get_value(const std::string &label, const std::string &key, unsigned index = 0) const;

    template<typename T>
    T get_value_with_default(const std::string &key, T default_value,  unsigned index = 0) const;

    template<typename T>
    T get_value_with_default(const std::string &label, const std::string &key, T default_value, unsigned index = 0) const;

    template<typename T>
    bool add_value(const T &value, const std::string &key, const std::string &label = "Global");

    void set_values(const std::string &value, const std::string &key, const std::string &label = "Global");

    bool key_exists(const std::string &key) const;
    bool key_exists(const std::string &label, const std::string &key) const;

    void print() const;
};

}  // namespace engine
}  // namespace sim

#include "Config-impl.h"

#endif  // CORE_ENGINE_CONFIG_H_
