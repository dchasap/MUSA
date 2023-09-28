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


#ifndef CORE_MODULES_MEMORY_CACHESTATS_IMPL_H_
#define CORE_MODULES_MEMORY_CACHESTATS_IMPL_H_

#include "core/stats/Counter.h"
#include "core/stats/CounterMap.h"
#include "core/utils/TimeList.h"

namespace sim {
namespace memory {

template<typename OwnerType>
inline
CacheStats::access_t::access_t(const OwnerType &owner, const std::string &label) :
    hit(owner, label + "_hit"),
    miss(owner, label + "_miss"),
    half_miss(owner, label + "_half_miss")
#ifdef ENABLE_COMPULSORY_MISS
    ,
    comp_miss(owner, label + "_compulsory_miss"),
    non_comp_miss(owner, label + "_noncompulsory_miss")
#endif
{}

template<typename OwnerType>
inline
CacheStats::access_victim_t::access_victim_t(const OwnerType &owner, const std::string &label) :
    hit(owner, label + "_hit"),
    miss(owner, label + "_miss")
{}

template<typename OwnerType>
inline
CacheStats::CacheStats(const OwnerType &owner) :
        read(owner, "read"),
        write(owner, "write"),
        flush(owner, "flush"),
        invalidate(owner, "invalidate"),
        victim_read(owner, "victim-read"),
        victim_write(owner, "victim-write"),
        stalled_cycles(owner, "stalled cycles"),
        read_not_hit(owner, "read-not-hit"),
        write_not_hit(owner, "write-not-hit"),
        victim_swap(owner, "victim-swap"),
        victim_evict(owner, "victim-evict"),
        victim_insert_on_read(owner, "victim-insert_on_read"),
        victim_insert_on_write(owner, "victim-insert_on_write"),
        victim_insert_on_evict(owner, "victim-insert_on_evict"),
        miss_time(owner, "miss-time"),
        requests_received(owner, "requests-received"),
        acks_received(owner, "acks-received"),
        requests_sent(owner, "requests-sent"),
        acks_sent(owner, "acks-sent"),
        requests_processed(owner, "requests-processed"),
        setup_task_io_requests(owner, "setup-task-io-requests"),
        mshr_overcapacity_events(owner, "mshr-overcapacity-events"),
        hit_by_access_type(owner, "hit_by_access_type", sim::runtime::task_dep_to_string),
        miss_by_access_type(owner, "miss_by_access_type", sim::runtime::task_dep_to_string),
        requests_by_access_type(owner, "requests_by_access_type", sim::runtime::task_dep_to_string),
        miss_time_per_task_type(owner, "miss_time_per_task_type"),
        cache_read_hit_by_task_type(owner, "cache_read_hit_by_task_type"),
        cache_read_half_miss_by_task_type(owner, "cache_read_half_miss_by_task_type"),
        cache_read_miss_by_task_type(owner, "cache_read_miss_by_task_type"),
        cache_write_hit_by_task_type(owner, "cache_write_hit_by_task_type"),
        cache_write_half_miss_by_task_type(owner, "cache_write_half_miss_by_task_type"),
        cache_write_miss_by_task_type(owner, "cache_write_miss_by_task_type"),
        victim_read_hit_by_task_type(owner, "victim_read_hit_by_task_type"),
        victim_read_miss_by_task_type(owner, "victim_read_miss_by_task_type"),
        victim_write_hit_by_task_type(owner, "victim_write_hit_by_task_type"),
        victim_write_miss_by_task_type(owner, "victim_write_miss_by_task_type"),
        victim_swap_by_task_type(owner, "victim_swap_by_task_type"),
        victim_insert_on_read_by_task_type(owner, "victim_insert_on_read_by_task_type"),
        victim_insert_on_write_by_task_type(owner, "victim_insert_on_write_by_task_type"),
        victim_insert_on_evict_by_task_type(owner, "victim_insert_on_evict_by_task_type"),
        hit_by_access_type_by_task_type_(owner, "hits_by_task_type_for_access_type", sim::runtime::task_dep_to_string),
        miss_by_access_type_by_task_type_(owner, "misses_by_task_type_for_access_type", sim::runtime::task_dep_to_string),
        requests_by_access_type_by_task_type_(owner, "requests_by_task_type_for_access_type", sim::runtime::task_dep_to_string)
{}

inline
void CacheStats::update_access(bool hit, sim::runtime::task_dep_t dependency_type, std::size_t task_type)
{
    requests_by_access_type[dependency_type]++;
    requests_by_access_type_by_task_type_[dependency_type][task_type]++;
    if (hit) {
        hit_by_access_type[dependency_type]++;
        hit_by_access_type_by_task_type_[dependency_type][task_type]++;
    } else {
        miss_by_access_type[dependency_type]++;
        miss_by_access_type_by_task_type_[dependency_type][task_type]++;
    }
}

}  // namespace memory
}  // namespace sim

#endif  // CORE_MODULES_MEMORY_CACHESTATS_IMPL_H_
