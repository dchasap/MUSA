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


#ifndef CORE_UTILS_INSTRUMENTATION_INSTRUMENTATION_IMPL_H_
#define CORE_UTILS_INSTRUMENTATION_INSTRUMENTATION_IMPL_H_

#include <string>

namespace sim {
namespace utils {
namespace instrumentation {

template <typename TraceType>
void Instrumentation<TraceType>::init(const sim::engine::Config &config)
{
    static_cast<TraceType*>(this)->init(config);
}

template <typename TraceType>
void Instrumentation<TraceType>::finalize()
{
    static_cast<TraceType*>(this)->finalize();
}

template<typename TraceType> template<typename S>
int Instrumentation<TraceType>::add_module(const S &module, int num_components)
{
    return static_cast<TraceType*>(this)->add_module(module, num_components);
}

template <typename TraceType>
void Instrumentation<TraceType>::start_state(const InstrumentationItem &trace_item)
{
    static_cast<TraceType*>(this)->start_state(trace_item);
}

template <typename TraceType>
void Instrumentation<TraceType>::end_state(const InstrumentationItem &trace_item)
{
    static_cast<TraceType*>(this)->end_state(trace_item);
}

template <typename TraceType>
void Instrumentation<TraceType>::start_comm(const InstrumentationItem &trace_item)
{
    static_cast<TraceType*>(this)->start_comm(trace_item);
}

template <typename TraceType>
void Instrumentation<TraceType>::end_comm(const InstrumentationItem &trace_item)
{
    static_cast<TraceType*>(this)->end_comm(trace_item);
}

template <typename TraceType>
void Instrumentation<TraceType>::add_event(const InstrumentationItem &trace_item)
{
    static_cast<TraceType*>(this)->add_event(trace_item);
}

template <typename TraceType>
void Instrumentation<TraceType>::add_event(const InstrumentationItem &trace_item, sim::engine::Module &mod)
{
    static_cast<TraceType*>(this)->add_event(trace_item, mod);
}

template <typename TraceType>
void Instrumentation<TraceType>::add_function_name(const char* name)
{
    static_cast<TraceType*>(this)->add_function_name(name);
}

template <typename TraceType>
sim::engine::cycles_t Instrumentation<TraceType>::get_hardware_sampling_interval()
{
    return static_cast<TraceType*>(this)->get_hardware_sampling_interval();
}

template <typename TraceType>
void Instrumentation<TraceType>::dump_buffer()
{
    return static_cast<TraceType*>(this)->dump_buffer();
}

}  // namespace instrumentation
}  // namespace utils
}  // namespace sim

#endif  // CORE_UTILS_INSTRUMENTATION_INSTRUMENTATION_IMPL_H_
