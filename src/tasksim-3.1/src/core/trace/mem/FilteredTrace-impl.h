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


#ifndef CORE_TRACE_MEM_FILTEREDTRACE_IMPL_H_
#define CORE_TRACE_MEM_FILTEREDTRACE_IMPL_H_

#include "Trace.h"
#include "types.h"

namespace sim {
namespace trace {
namespace mem {

template<template<typename> class StreamType, typename Store>
inline
FilteredTraceInternal<StreamType, Store>::FilteredTraceInternal(const char *name) :
        sim::trace::Trace<StreamType, Store>(store_),
        TraceInternal<StreamType, Store>(name),
        filter_(64, 64, 128),  // 64, 64, 128 -> 8KB direct 128B line
        prev_ins_(0),
        // stats
        total_(0),
        filtered_(0)
{
}

template<template<typename> class StreamType, typename Store>
inline
FilteredTraceInternal<StreamType, Store>::~FilteredTraceInternal()
{
    std::cout << "Total accesses: " << total_ << std::endl;
    std::cout << "Filtered: " << filtered_ << std::endl;
    std::cout << "% filtered: " << (static_cast<double>(filtered_)/static_cast<double>(total_)*100) << std::endl;
}

template<template<typename> class StreamType, typename Store>
inline
void FilteredTraceInternal<StreamType, Store>::add_bbl_trace(const bbl_id_t &bbl_id, unsigned stream)
{
    static const bool enable_reset = false;
    // reset the filter when a CPU burst is finished (bbl id == 0)
    if (bbl_id.id_ == 0 and enable_reset) {
        filter_.reset();
        prev_ins_ = 0;
    }
    Internal::add_bbl_trace(bbl_id, stream);
}

template<template<typename> class StreamType, typename Store>
inline
void FilteredTraceInternal<StreamType, Store>::add_mem_trace(const mem_t &addr, unsigned stream)
{
    ++total_;
    if (addr.addr_ != 0) {
        mitem_t& item = Internal::pending_items_.front();
        Internal::pending_items_.pop();
        item.addr_ = addr.addr_;

        if (filter_.filter(addr)) {  // do not record item
            prev_ins_ += item.ins_;
            ++filtered_;
        } else {                     // accumulate previous instrs and record item
            item.ins_ += prev_ins_;
            Internal::Writer::template stream<Event<mitem_t>>(stream).append(item);
            prev_ins_ = 0;
        }
    } else {
        Internal::Writer::template stream<Event<mitem_t>>(stream).append(mitem_t());
    }
}

}  // namespace mem
}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_MEM_FILTEREDTRACE_IMPL_H_
