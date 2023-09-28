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


#ifndef CORE_TRACE_INTEL_TRACE_IMPL_H_
#define CORE_TRACE_INTEL_TRACE_IMPL_H_

#include <core/utils/macros.h>
#include "types.h"

namespace sim {
namespace trace {
namespace intel {

template<template<typename> class StreamType, typename Store>
inline TraceInternal<StreamType, Store>::TraceInternal(const char *name) :
        sim::trace::Trace<StreamType, Store>(store_),
        TraceReader<StreamType, Store>(store_),
        TraceWriter<StreamType, Store>(store_),
        store_(name), last_bbl_(bbl_t())
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
    return Reader::template stream<bbl_dict_stream_t>(stream).get(bbl_id.id_);
}

template<template<typename> class StreamType, typename Store>
inline void TraceInternal<StreamType, Store>::add_bbl_trace(const bbl_id_t &bbl_id, unsigned stream)
{
    Writer::template stream<Event<bbl_id_t>>(stream).append(bbl_id);
}

template<template<typename> class StreamType, typename Store>
inline
const bbl_t &TraceInternal<StreamType, Store>::get_next_bbl(unsigned trace_stream, unsigned dict_stream) const
{
    bbl_id_t bbl_id;
    bool ret = Reader::template stream<Event<bbl_id_t>>(trace_stream).get_next(bbl_id);
    if (ret == false) {
        return last_bbl_;
    }
    if (bbl_id.id_ == 0) {
        return last_bbl_;  // bbl 0 delimiter found
    }
    assert(bbl_id.id_ > 0);
    return Reader::template stream<bbl_dict_stream_t>(dict_stream).get(bbl_id.id_);
}



template<template<typename> class StreamType, typename Store>
inline void TraceInternal<StreamType, Store>::add_mem_trace(const mem_t &addr, unsigned stream)
{
    if (addr.addr_ != 0) {
        Writer::template stream<Event<mem_t>>(stream).append(addr);
    }
}


template<template<typename> class StreamType, typename Store>
inline
addr_t TraceInternal<StreamType, Store>::get_next_mem(unsigned stream) const
{
    mem_t mem;
    bool ret = Reader::template stream<Event<mem_t>>(stream).get_next(mem);
    if (ret == false) {
        mem.addr_ = 0;
    }
    return mem.addr_;
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

}  // namespace intel
}  // namespace trace
}  // namespace sim

#endif  //  CORE_TRACE_INTEL_TRACE_IMPL_H_
