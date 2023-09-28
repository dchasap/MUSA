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


#ifndef CORE_TRACE_MEM_CACHEFILTER_IMPL_H_
#define CORE_TRACE_MEM_CACHEFILTER_IMPL_H_

#include <cstddef>
#include "types.h"
#include "nextsim/core/utils/macros.h"

namespace sim {
namespace trace {
namespace mem {

CacheLineBool::CacheLineBool(bool value) :
    value_(value)
{}

inline
CacheFilter::CacheFilter(std::size_t size, std::size_t sets, std::size_t cache_line_size):
    size_(size),
    sets_(sets),
    cache_line_size_(cache_line_size),
    tag_mask_(~(cache_line_size_ - 1)),
    cache_(new Cache(size_, size_/sets_, cache_line_size_, false))
{}

inline
CacheFilter::~CacheFilter() {}

inline
bool
CacheFilter::filter(const mem_t& addr)
{
    addr_t tag = addr.addr_ & tag_mask_;
    if (cache_->access(tag) == NULL) {//miss
        if (not cache_->insert(tag, CacheLineBool(true))){
            cache_->evict(tag);
            bool ret = cache_->insert(tag, CacheLineBool(true));
            assert(ret == true);
            _unused(ret);
        }
        return false;
    } else {  // hit
        return true;
    }
}

inline
void
CacheFilter::reset()
{
    // this may be a performance limitation
    cache_.reset(new Cache(size_, size_/sets_, cache_line_size_, false));
}

}  // namespace mem
}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_MEM_CACHEFILTER_IMPL_H_
