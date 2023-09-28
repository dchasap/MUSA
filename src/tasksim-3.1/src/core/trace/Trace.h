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


#ifndef CORE_TRACE_TRACE_H_
#define CORE_TRACE_TRACE_H_

#include <string>
#include <map>

namespace sim {
namespace trace {

class BaseEventCache;

template<typename EventType> class Event;

/**
 * Trace is a collection of event streams
 */
template<template<typename> class StreamType, typename Store>
class Trace {
protected:
    Store &store_;
    std::map<unsigned, BaseEventCache*> event_caches_;

    unsigned get_number_of_streams() const;
    unsigned get_actual_number_of_streams() const;
    unsigned get_number_of_files() const;
    void flush_event_streams();
public:
    explicit Trace(Store &store);
    virtual ~Trace();

    template<typename Cache>
    void register_event_stream(unsigned id, const std::string & = "default");
    void flush_event_stream(unsigned id);
};

}  // namespace trace
}  // namespace sim

#include "Trace-impl.h"

#endif  // CORE_TRACE_TRACE_H_
