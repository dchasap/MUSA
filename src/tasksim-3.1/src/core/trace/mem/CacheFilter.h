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


#ifndef CORE_TRACE_MEM_CACHEFILTER_H_
#define CORE_TRACE_MEM_CACHEFILTER_H_

#include <memory>

#include <nextsim/core/logic/memory/Cache.h>
#include <nextsim/core/logic/memory/CacheSet.h>
#include "types.h"

namespace sim {
namespace trace {
namespace mem {

struct CacheLineBool : sim::logic::memory::CacheLineBase {
    bool value_;
    CacheLineBool(bool value = true);
};

/**
 * Filter that uses a cache to filter 'mem' format traces
 */
class CacheFilter {
private:
    /** using sim::logic::memory::Cache with LRU replacement policy for filtering */
    typedef sim::logic::memory::Cache<sim::logic::memory::LRU,sim::logic::memory::CacheSet<addr_t,CacheLineBool>> Cache;
    std::size_t size_;                          /**< cache size */
    std::size_t sets_;                          /**< cache number of sets */
    std::size_t cache_line_size_;               /**< cache line size */
    std::size_t tag_mask_;                      /**< cache bit mask for the tag */
    std::unique_ptr<CacheFilter::Cache> cache_; /**< actual pointer to the filter cache */
public:
    CacheFilter(std::size_t size, std::size_t sets, std::size_t cache_line_size);
    virtual ~CacheFilter();

    /** method that returns whether an address must be filtered or not */
    bool filter(const mem_t&);
    /** method to reset (clear) the filter cache */
    void reset();
};

}  // namespace mem
}  // namespace trace
}  // namespace sim

#include "CacheFilter-impl.h"

#endif  // CORE_TRACE_MEM_CACHEFILTER_H_
