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


#ifndef CORE_TRACE_TRACEWRITER_H_
#define CORE_TRACE_TRACEWRITER_H_

#include "Trace.h"

#include <vector>
#include <iostream>

namespace sim {
namespace trace {

/**
 * TraceWriter adds the functionality to retrieve a stream using its ID
 * As the return reference is non-constant, it is read/write
 */
template<template<typename> class StreamType , typename Store>
class TraceWriter: public virtual Trace<StreamType, Store> {
public:
    explicit TraceWriter(Store &store);
    virtual ~TraceWriter();

    template<typename Cache>
    inline
    typename Cache::template Type<StreamType, Store> & stream(unsigned id) {
        assert(this->event_caches_.find(id) != this->event_caches_.end());
        return *(static_cast<typename Cache::template Type<StreamType, Store> *>(this->event_caches_[id]));
    }
};

}  // namespace trace
}  // namespace sim

#include "TraceWriter-impl.h"

#endif  // CORE_TRACE_TRACEWRITER_H_
