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


#ifndef CORE_UTILS_INSTRUMENTATION_INSTRUMENTATION_H_
#define CORE_UTILS_INSTRUMENTATION_INSTRUMENTATION_H_

#include <unordered_map>
#include <map>
#include <fstream>
#include <utility>
#include <string>
#include <vector>

#include "core/engine/types.h"
#include "core/engine/Config.h"
#include "types.h"

namespace sim {
namespace engine {
class Module;
}  // namespace engine
}  // namespace sim

namespace sim {
namespace utils {
namespace instrumentation {

template <typename TraceType>
class Instrumentation
{
protected:
    bool init_;

    Instrumentation() :
        init_(false)
    { }

public:
    void init(const sim::engine::Config &config);
    void finalize();

    template<typename S>
    int add_module(const S &module, int num_components = 1);
    void start_state(const InstrumentationItem&);
    void end_state(const InstrumentationItem&);
    void start_comm(const InstrumentationItem&);
    void end_comm(const InstrumentationItem&);
    void add_event(const InstrumentationItem&);
    void add_event(const InstrumentationItem&, sim::engine::Module&);
    void add_function_name(const char* name);
    sim::engine::cycles_t get_hardware_sampling_interval();
    void dump_buffer();
};

class NoTrace : public Instrumentation<NoTrace>
{
public:
    void init(const sim::engine::Config &config) { }
    void finalize() { }

    template<typename S>
    int add_module(const S &module, int num_components = 1) { return -1;}
    void start_state(const InstrumentationItem&) { }
    void end_state(const InstrumentationItem&) { }
    void start_comm(const InstrumentationItem&) { }
    void end_comm(const InstrumentationItem&) { }
    void add_event(const InstrumentationItem&) { }
    void add_event(const InstrumentationItem&, sim::engine::Module&) { }
    void add_function_name(const char* name) { }
    sim::engine::cycles_t get_hardware_sampling_interval() { return sim::engine::NEVER; }
    void dump_buffer() {}
};

}  // namespace instrumentation
}  // namespace utils
}  // namespace sim

#include "Instrumentation-impl.h"

#ifdef ENABLE_PARAVER
#include "Paraver.h"
namespace sim {
namespace utils {
namespace instrumentation {
typedef Paraver SelectedInstrumentation;
}  // namespace instrumentation
}  // namespace utils
}  // namespace sim
#else
namespace sim {
namespace utils {
namespace instrumentation {
typedef NoTrace SelectedInstrumentation;
}  // namespace instrumentation
}  // namespace utils
}  // namespace sim
#endif


#endif  // CORE_UTILS_INSTRUMENTATION_INSTRUMENTATION_H_
