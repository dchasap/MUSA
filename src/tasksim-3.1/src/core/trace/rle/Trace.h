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


#ifndef CORE_TRACE_RLE_TRACE_H_
#define CORE_TRACE_RLE_TRACE_H_

#include <unordered_map>
#include <vector>
#include "core/trace/mem/types.h"
#include "core/trace/FileTrace.h"
#include "core/trace/TraceReader.h"
#include "core/trace/TraceWriter.h"
#include "core/trace/VectorEvent.h"
#include "core/trace/rle/types.h"


namespace sim {
namespace trace {

template <typename> class TextEventStream;
template <typename> class BinaryEventStream;

namespace rle {

/**
 * TraceInternal implements the functions to manage instruction traces in 'intel' format
 * It does not manage stream IDs. A higher level class must do it, that is why it is pure virtual
 * The read/write methods receive the stream identifier from where they have to read/write
 */
template<template <typename> class StreamType, typename Store>
class TraceInternal : public TraceReader<StreamType, Store>, public TraceWriter<StreamType, Store> {
protected:
    Store store_;
    // static const unsigned Bbl_Mem_Dict_Stream_ = 5; /**< stream holding basic block dictionary */
    // static const unsigned Bbl_Red_Stream_ = 6; /**< stream holding basic block dictionary */
    typedef TraceReader<StreamType, Store> Reader;
    typedef TraceWriter<StreamType, Store> Writer;
    typedef VectorEvent<bbl_t> bbl_dict_stream_t;

    // For writting purposses:
    mem_history mem_history_;
    rle_bbl_id_t bbl_buffer_;
    // For reading purposes, they have their own isntance for each stream
    mutable mem_history_map mem_history_map_;
    bbl_id_by_stream_t bbl_cache_;
    const bbl_t last_bbl_;

public:
    explicit TraceInternal(const char *);
    virtual ~TraceInternal() = 0;

    /** method to record the instructions of a basic block */
    void add_bbl(const bbl_t &bbl, unsigned stream);
    /** method to retrive a basic block by its identifier */
    const bbl_t &get_bbl(const bbl_id_t &bbl_id, unsigned stream) const;

    /** method to record the execution of a basic block */
    void add_bbl_trace(const bbl_id_t &bbl_id, unsigned stream);
    /** retrieves basic blocks sequentially from a given task using its subtrace */
    const bbl_t &get_next_bbl(unsigned trace_stream, unsigned dict_stream);

    /** method to record a memory access address */
    void add_mem_trace(const mem_t &addr, unsigned stream);
    /** method to retrieve memory access addresses sequentially */
    addr_t get_next_mem(unsigned stream) const;
    /** this method must be empty as it is not used for 'intel' format traces (FIXME: remove? use SFINAE?) */
    mitem_t get_next_mem_item(unsigned stream) const;
};

/**
 * Trace defines a set of streams to manage a single instruction trace in 'intel' format
 */
template<template <typename> class StreamType, typename Store>
class Trace : private TraceInternal<StreamType, Store> {
protected:
    static unsigned const Bbl_Dict_Stream_ = 0;  /**< stream holding the instructions for all basic blocks */
    static unsigned const Bbl_Trace_Stream_ = 1; /**< stream holding the sequence of executed basic blocks (just IDs) */
    static unsigned const Mem_Trace_Stream_ = 2; /**< stream holding all memory addresses accessed during execution */

    using TraceInternal<StreamType, Store>::store_;

    typedef TraceInternal<StreamType, Store> Internal;
    typedef VectorEvent<bbl_t> bbl_dict_stream_t;

public:
    explicit Trace(const char *);
    ~Trace();

    /** method to record the instructions of a basic block */
    void add_bbl(const bbl_t &bbl);
    /** method to retrive a basic block by its identifier */
    const bbl_t &get_bbl(const bbl_id_t &bbl_id) const;
    /** method to record the execution of a basic block */
    void add_bbl_trace(const bbl_id_t &bbl_id);
    /** method to retrive basic blocks sequentially */
    const bbl_t &get_next_bbl() const;

    /** method to record a memory access address */
    void add_mem_trace(const mem_t &addr);
    /** method to retrieve memory access addresses sequentially */
    addr_t get_next_mem() const;
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


}  // namespace rle
}  // namespace trace
}  // namespace sim

#include "Trace-impl.h"


#endif  // CORE_TRACE_RLE_TRACE_H_
