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


#ifndef CORE_TRACE_MEM_FILTEREDTRACE_H_
#define CORE_TRACE_MEM_FILTEREDTRACE_H_

#include <unordered_map>

#include "Trace.h"
#include "CacheFilter.h"
#include "types.h"
#include "nextsim/core/trace/FileTrace.h"
#include "nextsim/core/trace/TraceReader.h"
#include "nextsim/core/trace/TraceWriter.h"
#include "nextsim/core/trace/VectorEvent.h"


namespace sim {
namespace trace {
namespace mem {

/**
 * FilteredTraceInternal implements the functions to filter accesses when writing a 'mem' format trace (memory access traces)
 * The trace reading methods are provided in its superclass mem::TraceInternal
 * It does not manage stream IDs. A higher level class must do it, that is why it is pure virtual
 * The read/write methods receive the stream identifier from where they have to read/write
 */
template<template <typename> class StreamType, typename Store>
class FilteredTraceInternal : public mem::TraceInternal<StreamType, Store> {
protected:
    using TraceInternal<StreamType, Store>::store_;
    typedef TraceInternal<StreamType, Store> Internal;

    CacheFilter filter_; /**< filter cache */
    unsigned prev_ins_;  /**< holds the number of instructions after the last recorded item */

    /** statistics */
    unsigned total_;
    unsigned filtered_;
public:
    explicit FilteredTraceInternal(const char *file_name);
    virtual ~FilteredTraceInternal() = 0;

    /** method to record the execution of a basic block (using its ID) */
    void add_bbl_trace(const bbl_id_t &bbl_id, unsigned stream);
    /** method to record a memory access address */
    void add_mem_trace(const mem_t &, unsigned);
};

}  // namespace mem
}  // namespace trace
}  // namespace sim

#include "FilteredTrace-impl.h"

#endif  // CORE_TRACE_MEM_FILTEREDTRACE_H_
