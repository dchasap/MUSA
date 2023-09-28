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


#ifndef CORE_TRACE_MEM_TRACE_IMPL_H_
#define CORE_TRACE_MEM_TRACE_IMPL_H_

#include <utility>
#include "types.h"

namespace sim {
namespace trace {
namespace mem {

template<template<typename> class StreamType, typename Store>
inline
TraceInternal<StreamType, Store>::TraceInternal(const char *name) :
        sim::trace::Trace<StreamType, Store>(store_),
        TraceReader<StreamType, Store>(store_),
        TraceWriter<StreamType, Store>(store_),
        store_(name), pending_items_(), empty_bbl_()
{
}

template<template<typename> class StreamType, typename Store>
inline
TraceInternal<StreamType, Store>::~TraceInternal()
{
}

template<template<typename> class StreamType, typename Store>
inline
void TraceInternal<StreamType, Store>::add_bbl(const bbl_t &bbl, unsigned stream)
{
    assert(bbl_items_.find(bbl.id_) == bbl_items_.end());

    // unordered_map::empace seems not being available in gcc4.6.3 headers Ubuntu 12.04
    // bbl_items_.emplace(bbl.id_, bbl);
    // replacing it by insert
    bbl_items_.insert(std::pair<bbl_id_t, mitem_queue_t>(bbl.id_, mitem_queue_t(bbl)));
}

template<template<typename> class StreamType, typename Store>
inline
const bbl_t &TraceInternal<StreamType, Store>::get_bbl(const bbl_id_t &bbl_id, unsigned stream) const
{
    assert(0);
    return empty_bbl_;
}

template<template<typename> class StreamType, typename Store>
inline
void TraceInternal<StreamType, Store>::add_bbl_trace(const bbl_id_t &bbl_id, unsigned stream)
{
    if (bbl_id.id_ != 0) {
        assert(pending_items_.empty());

        std::unordered_map<bbl_id_t, mitem_queue_t>::const_iterator it = bbl_items_.find(bbl_id);
        assert(it != bbl_items_.end());

        pending_items_.push(it->second);
    }
}

template<template<typename> class StreamType, typename Store>
inline
const bbl_t &TraceInternal<StreamType, Store>::get_next_bbl(unsigned trace_stream, unsigned dict_stream) const
{
    // this method should never be called
    assert(0);
    return empty_bbl_;  // to avoid compiler error with Werror
}


template<template<typename> class StreamType, typename Store>
inline
void TraceInternal<StreamType, Store>::add_mem_trace(const mem_t &addr, unsigned stream)
{
    if (addr.addr_ != 0) {
        mitem_t& item = pending_items_.front();
        item.addr_ = addr.addr_;

        Writer::template stream<Event<mitem_t>>(stream).append(item);
        pending_items_.pop();
    } else {
        Writer::template stream<Event<mitem_t>>(stream).append(mitem_t());
    }
}

template<template<typename> class StreamType, typename Store>
inline
void TraceInternal<StreamType, Store>::add_mem_item_trace(const mitem_t &item, unsigned stream)
{
    if (item.addr_ != 0) {
        Writer::template stream<Event<mitem_t>>(stream).append(item);
    } else {
        Writer::template stream<Event<mitem_t>>(stream).append(mitem_t());
    }
}

template<template<typename> class StreamType, typename Store>
inline
addr_t TraceInternal<StreamType, Store>::get_next_mem(unsigned stream) const
{
    assert(0);
    return 0;
}

template<template<typename> class StreamType, typename Store>
inline
mitem_t TraceInternal<StreamType, Store>::get_next_mem_item(unsigned stream) const
{
    mitem_t mitem;
    bool ret = Reader::template stream<Event<mitem_t>>(stream).get_next(mitem);
    // assert(ret == true);
    if (ret == false) {
        // CAA debug;  temporary removed.
        //std::cerr << "warning: trying to get_next_mem_item on a finished stream " << stream << std::endl;
    }
    return mitem;
}




template<template<typename> class StreamType, typename Store>
inline
Trace<StreamType, Store>::Trace(const char *name) :
        sim::trace::Trace<StreamType, Store>(this->store_),
        TraceInternal<StreamType, Store>(name)
{
    this->template register_event_stream<Event<mitem_t>>(Mem_Trace_Stream_, "mem");
}

template<template<typename> class StreamType, typename Store>
inline
Trace<StreamType, Store>::~Trace()
{
    this->flush_event_streams();
}

template<template<typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::add_bbl(const bbl_t &bbl)
{
    Internal::add_bbl(bbl, 0); /* mem trace does not have bbl stream */
}

template<template<typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::add_bbl_trace(const bbl_id_t &bbl_id)
{
    Internal::add_bbl_trace(bbl_id, 0); /* mem trace does not have bbl stream */
}


template<template<typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::add_mem_trace(const mem_t &addr)
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
mitem_t Trace<StreamType, Store>::get_next_mem_item() const
{
    return Internal::get_next_mem_item(Mem_Trace_Stream_);
}


}  // namespace mem
}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_MEM_TRACE_IMPL_H_
