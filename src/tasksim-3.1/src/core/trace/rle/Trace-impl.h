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


#ifndef CORE_TRACE_RLE_TRACE_IMPL_H_
#define CORE_TRACE_RLE_TRACE_IMPL_H_

#include "core/trace/rle/types.h"
#include "core/trace/mem/types.h"
#include "core/utils/macros.h"

namespace sim {
namespace trace {
namespace rle {

template<template<typename> class StreamType, typename Store>
inline TraceInternal<StreamType, Store>::TraceInternal(const char *name) :
        sim::trace::Trace<StreamType, Store>(store_),
        TraceReader<StreamType, Store>(store_),
        TraceWriter<StreamType, Store>(store_),
        store_(name),
        mem_history_(), bbl_buffer_(0, 0),
        mem_history_map_(), bbl_cache_(),
        last_bbl_(bbl_t())
{
}

template<template<typename> class StreamType, typename Store>
inline TraceInternal<StreamType, Store>::~TraceInternal()
{
}

template<template<typename> class StreamType, typename Store>
inline
void TraceInternal<StreamType, Store>::add_bbl(const bbl_t &bbl, unsigned stream)
{
    bool ret = Writer::template stream<bbl_dict_stream_t>(stream).append(bbl);
    assert(ret == true);
    _unused(ret);
}

template<template<typename> class StreamType, typename Store>
inline
const bbl_t &TraceInternal<StreamType, Store>::get_bbl(const bbl_id_t &bbl_id, unsigned stream) const
{
    if (bbl_id.id_ >= Reader::template stream<bbl_dict_stream_t>(stream).size()) {
        return last_bbl_;
    } else {
        return Reader::template stream<bbl_dict_stream_t>(stream).get(bbl_id.id_);
    }
}

template<template<typename> class StreamType, typename Store>
inline void TraceInternal<StreamType, Store>::add_bbl_trace(const bbl_id_t &bbl_id, unsigned stream)
{
    // Three scenarios are possible:
    if (bbl_id.id_ == 0) {
        // 1 - bbl_id.id_ == 0, so we have to flush the previous, add value bbl 0 and erase the buffer.
        if (bbl_buffer_.value_ != 0) {
            Writer::template stream<Event<rle_bbl_id_t>>(stream).append(bbl_buffer_);
        }
        Writer::template stream<Event<rle_bbl_id_t>>(stream).append(rle_bbl_id_t(0, 1));
        bbl_buffer_.set_value(0, 0);
    } else if ((bbl_id.id_ != bbl_buffer_.value_) || bbl_buffer_.limit()) {
        // 2 - bbl_id.id_ != 0, and is a different value,
        // so we flush the previous value and add the new one to the buffer.
        if (bbl_buffer_.value_ != 0) {
            Writer::template stream<Event<rle_bbl_id_t>>(stream).append(bbl_buffer_);
        }
        bbl_buffer_.set_value(bbl_id.id_);
    } else {
        // 3 - The same value that is in the buffer, so we augment the repetition counter.
        bbl_buffer_.repeat();
    }
}

template<template<typename> class StreamType, typename Store>
inline
const bbl_t &TraceInternal<StreamType, Store>::get_next_bbl(unsigned trace_stream, unsigned dict_stream)
{
    if (bbl_cache_.find(trace_stream) == bbl_cache_.end()) {
        bbl_cache_[trace_stream] = rle_bbl_id_t(0, 0);
    }
    rle_bbl_id_t &bbl = bbl_cache_[trace_stream];
    if (bbl_cache_[trace_stream].repeats_ == 0) {
        bool ret = Reader::template stream<Event<rle_bbl_id_t>>(trace_stream).get_next(bbl);
        if (ret == false) {
            return last_bbl_;
        }
    }
    unsigned id = bbl.value_;
    bbl.consume();
    if (id == 0 && bbl.repeats_ == 0) {
        bbl_cache_.erase(trace_stream);
        return last_bbl_;
    }
    return Reader::template stream<bbl_dict_stream_t>(dict_stream).get(id);
}

template<template<typename> class StreamType, typename Store>
inline void TraceInternal<StreamType, Store>::add_mem_trace(const mem_t &addr, unsigned stream)
{
    channeled_mem_t mem = mem_history_.get_channeled(addr.addr_, stream);
    Writer::template stream<Event<channeled_mem_t>>(stream).append(mem);
}

template<template<typename> class StreamType, typename Store>
inline
addr_t TraceInternal<StreamType, Store>::get_next_mem(unsigned stream) const
{
    channeled_mem_t mem;
    bool ok = Reader::template stream<Event<channeled_mem_t>>(stream).get_next(mem);
    if (ok == false) {
        return addr_t();
    }
    addr_t ret = mem_history_map_[stream].get_value(mem);
    if (ret == 0) {
        // Let's clean the channeled history:
        mem_history_map_.erase(stream);
    }
    return ret;
}

template<template<typename> class StreamType, typename Store>
inline Trace<StreamType, Store>::Trace(const char *name) :
        trace::Trace<StreamType, Store>(this->store_),
        TraceInternal<StreamType, Store>(name)
{
    this->template register_event_stream<bbl_dict_stream_t>(Bbl_Dict_Stream_, "dict");
    this->template register_event_stream<Event<bbl_id_t>>(Bbl_Trace_Stream_, "bbl");
    this->template register_event_stream<Event<mem_t>>(Mem_Trace_Stream_, "mem");
}

template<template<typename> class StreamType, typename Store>
inline Trace<StreamType, Store>::~Trace()
{
    this->flush_event_streams();
}

template<template<typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::add_bbl(const bbl_t &bbl)
{
    Internal::add_bbl(bbl, Bbl_Dict_Stream_);
}

template<template<typename> class StreamType, typename Store>
inline
const bbl_t &Trace<StreamType, Store>::get_bbl(const bbl_id_t &bbl_id) const
{
    assert(0);
    return Internal::get_bbl(bbl_id, Bbl_Dict_Stream_);
}

template<template<typename> class StreamType, typename Store>
inline void Trace<StreamType, Store>::add_bbl_trace(const bbl_id_t &bbl_id)
{
    Internal::add_bbl_trace(bbl_id, Bbl_Trace_Stream_);
}

template<template<typename> class StreamType, typename Store>
inline
const bbl_t &Trace<StreamType, Store>::get_next_bbl() const
{
    assert(0);
    return Internal::get_next_bbl(Bbl_Trace_Stream_, Bbl_Dict_Stream_);
}

template<template<typename> class StreamType, typename Store>
inline void Trace<StreamType, Store>::add_mem_trace(const mem_t &addr)
{
    Internal::add_mem_trace(addr, Mem_Trace_Stream_);
}

template<template<typename> class StreamType, typename Store>
inline
addr_t Trace<StreamType, Store>::get_next_mem() const
{
    return Internal::get_next_mem(Mem_Trace_Stream_);
}

template<template<typename> class StreamType, typename Store>
inline
mitem_t TraceInternal<StreamType, Store>::get_next_mem_item(unsigned stream) const
{
    assert(0);
    return mitem_t();
}

}  // namespace rle
}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_RLE_TRACE_IMPL_H_
