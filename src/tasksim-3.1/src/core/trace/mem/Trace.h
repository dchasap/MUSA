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


#ifndef CORE_TRACE_MEM_TRACE_H_
#define CORE_TRACE_MEM_TRACE_H_

#include <unordered_map>

#include "nextsim/core/trace/FileTrace.h"
#include "nextsim/core/trace/TraceReader.h"
#include "nextsim/core/trace/TraceWriter.h"
#include "nextsim/core/trace/VectorEvent.h"
#include "types.h"


namespace sim {
namespace trace {
namespace mem {

/**
 * TraceInternal implements the functions to read/write traces in 'mem' format (memory access traces)
 * It does not manage stream IDs. A higher level class must do it, that is why it is pure virtual
 * The read/write methods receive the stream identifier from where they have to read/write
 */
template<template <typename> class StreamType, typename Store>
class TraceInternal : public TraceReader<StreamType, Store>, public TraceWriter<StreamType, Store> {
protected:
    Store store_;

    typedef TraceReader<StreamType, Store> Reader;
    typedef TraceWriter<StreamType, Store> Writer;

    std::unordered_map<bbl_id_t, mitem_queue_t> bbl_items_;
    mitem_queue_t pending_items_;

    bbl_t empty_bbl_;

public:
    explicit TraceInternal(const char *file_name);
    virtual ~TraceInternal() = 0;

    /** method to record the instructions of a basic block */
    void add_bbl(const bbl_t &, unsigned);
    /** this method must be empty as it is not used for 'mem' format traces (FIXME: remove? use SFINAE?) */
    const bbl_t &get_bbl(const bbl_id_t &, unsigned) const;
    /** method to record the execution of a basic block */
    void add_bbl_trace(const bbl_id_t &, unsigned);
    /** this method must be empty as it is not used for 'mem' format traces (FIXME: remove? use SFINAE?) */
    const bbl_t &get_next_bbl(unsigned, unsigned) const;

    /** method to record a memory access address */
    void add_mem_trace(const mem_t &, unsigned);
    /** method to record a memory access item */
    void add_mem_item_trace(const mitem_t &, unsigned);
    /** this method must be empty as it is not used for 'intel' format traces (FIXME: remove? use SFINAE?) */
    addr_t get_next_mem(unsigned) const;
    /** method to retrieve a memory access item */
    mitem_t get_next_mem_item(unsigned) const;
};

/**
 * Trace defines a set of streams to manage a memory access trace in 'mem' format
 */
template<template <typename> class StreamType, typename Store>
class Trace : private TraceInternal<StreamType, Store> {
protected:
    static unsigned const Mem_Trace_Stream_ = 0; /**< stream holding the memory access trace items */

    using TraceInternal<StreamType, Store>::store_;

    typedef TraceInternal<StreamType, Store> Internal;

public:
    explicit Trace(const char *file_name);
    ~Trace();

    /** method to record the instructions of a basic block */
    void add_bbl(const bbl_t &);
    /** method to record the execution of a basic block */
    void add_bbl_trace(const bbl_id_t &);

    /** method to record a memory access address */
    void add_mem_trace(const mem_t &);
    /** method to record a memory access item */
    void add_mem_item_trace(const mitem_t &);
    /** this method calls to the internal method that must be empty as it is
     *   not used for 'mem' format traces (FIXME: remove? use SFINAE?) */
    addr_t get_next_mem() const;
    /** method to retrieve memory access items sequentially */
    mitem_t get_next_mem_item() const;
};

/**
 * Subclass of Trace that uses a file stream as storage
 */
template<template <typename> class StreamType, typename Storage = std::fstream>
class FileTrace : public Trace<StreamType, trace::FileTrace<StreamType, Storage> > {
public:
    explicit FileTrace(const char *file_name) :
        trace::Trace<StreamType, trace::FileTrace<StreamType, Storage> >(this->store_),
        Trace<StreamType, trace::FileTrace<StreamType, Storage> >(file_name)

    {};
};

/**
 * Subclass of Trace that uses an input file stream as storage (read-only)
 */
template<template <typename> class StreamType, typename Storage = std::ifstream>
class FileTraceReader : public Trace<StreamType, trace::FileTrace<StreamType, Storage> > {
public:
    explicit FileTraceReader(const char *file_name) :
        trace::Trace<StreamType, trace::FileTrace<StreamType, Storage> >(this->store_),
        Trace<StreamType, trace::FileTrace<StreamType, Storage> >(file_name)

    {};
};


}  // namespace mem
}  // namespace trace
}  // namespace sim

#include "Trace-impl.h"


#endif  // CORE_TRACE_MEM_TRACE_H_
