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


#ifndef CORE_STATS_STAT_IMPL_H_
#define CORE_STATS_STAT_IMPL_H_

#include <cxxabi.h>
#include <cstring>
#include <typeinfo>
#include <memory>
#include <string>

#include "core/engine/Simulator.h"

namespace sim {
namespace stats {

static inline
std::string demangle(const char *mangled_name)
{
    int status = 0;
    std::unique_ptr<char, void(*)(void*)> demangled {
               abi::__cxa_demangle(mangled_name, NULL, NULL, &status),
               std::free
    };
    if (status != 0) {
        return std::string(mangled_name);
    } else {
        std::string value(demangled.get());
        if (value.find('<') != std::string::npos) {
            value = value.substr(0, value.find('<'));
        }
        if (value.rfind("::") != std::string::npos) {
            value = value.substr(value.rfind("::") + 2);
        }
        return value;
    }
}


template<typename T>
inline
Stat::Stat(const T &obj, const std::string &name) :
    class_(demangle(obj.get_name().c_str())),
    name_(name),
    id_(obj.get_id()),
    print_(true)
{
    // Register stat with Simulator instance
    sim::engine::Simulator *sim = sim::engine::Simulator::get_instance();
    // Exception for TimeStatistics:
    if (sim != nullptr) {
        sim->register_stat(*this);
    }
    if (out_.good()) {
        return;
    }
    const char *stats_file_name = ::getenv("SIM_STATS_OUT");
    if (stats_file_name == NULL) {
        out_.rdbuf(std::cout.rdbuf());
    } else {
        // std::cout << "Using " << std::string(stats_file_name) << " for stats output." << std::endl;
        out_file_.open(stats_file_name);
        out_.rdbuf(out_file_.rdbuf());
    }
}


inline
Stat::~Stat()
{
    if (print_) {
        out_ << ":" << name_ << ":" << class_ << ":" << id_ << std::endl;
    }
}

inline
const std::string &Stat::get_name() const
{
    return name_;
}


}  // namespace stats
}  // namespace sim


#endif  // CORE_STATS_STAT_IMPL_H_
