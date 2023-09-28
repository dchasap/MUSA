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


#ifndef CORE_TRACE_TRACE_IMPL_H_
#define CORE_TRACE_TRACE_IMPL_H_


#include <string>
#include <map>
#include "nextsim/core/utils/Log.h"

namespace sim {
namespace trace {

using sim::utils::Log;


template<template<typename> class StreamType, typename Store>
inline
Trace<StreamType, Store>::Trace(Store &store) :
    store_(store)
{
}


template<template<typename> class StreamType, typename Store>
inline
Trace<StreamType, Store>::~Trace()
{
    assert(event_caches_.empty() == true);
}


template<template<typename> class StreamType, typename Store>
inline
unsigned Trace<StreamType, Store>::get_number_of_streams() const
{
    return store_.get_number_of_streams();
}

template<template<typename> class StreamType, typename Store>
inline
unsigned Trace<StreamType, Store>::get_actual_number_of_streams() const
{
    return store_.get_actual_number_of_streams();
}

template<template<typename> class StreamType, typename Store>
inline
unsigned Trace<StreamType, Store>::get_number_of_files() const
{
    return store_.get_number_of_files();
}

template<template<typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::flush_event_streams()
{
    typename std::map<unsigned, BaseEventCache *>::const_iterator i;
    Log::info() << "Flushing event streams";

    for (i = event_caches_.begin(); i != event_caches_.end(); ++i) {
        if (i->second != NULL)  delete (i->second);
    }
    event_caches_.clear();
}

template<template<typename> class StreamType, typename Store>
template<typename Cache>
inline
void Trace<StreamType, Store>::register_event_stream(unsigned id, const std::string &name)
{
    store_.register_event_stream(id, name);
    event_caches_[id] = new typename Cache::template Type<StreamType, Store>(id, store_);
}

template<template<typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::flush_event_stream(unsigned id)
{
    assert(event_caches_.find(id) != event_caches_.end());
    if ((event_caches_.find(id)->second) != nullptr) {
        delete(event_caches_.find(id)->second);
    }
    event_caches_.erase(id);
}

}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_TRACE_IMPL_H_
