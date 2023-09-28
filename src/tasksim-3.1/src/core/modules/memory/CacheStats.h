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


#ifndef CORE_MODULES_MEMORY_CACHESTATS_H_
#define CORE_MODULES_MEMORY_CACHESTATS_H_

#include "core/stats/Average.h"
#include "core/stats/AverageMap.h"
#include "core/stats/Counter.h"
#include "core/stats/CounterMap.h"
#include "core/utils/TimeList.h"

namespace sim {
namespace memory {

class CacheStats {
public:
    struct access_t {
        stats::Counter hit, miss, half_miss;
#ifdef ENABLE_COMPULSORY_MISS
        stats::Counter comp_miss, non_comp_miss;
#endif
        template<typename OwnerType>
        access_t(const OwnerType &owner, const std::string &label);
    };
    struct access_victim_t {
        stats::Counter hit, miss;
        template<typename OwnerType>
        access_victim_t(const OwnerType &owner, const std::string &label);
    };
    access_t read, write, flush, invalidate;
    access_victim_t victim_read, victim_write;
    sim::stats::Counter stalled_cycles;
    stats::Counter read_not_hit;
    stats::Counter write_not_hit;
    stats::Counter victim_swap;
    stats::Counter victim_evict;
    stats::Counter victim_insert_on_read;
    stats::Counter victim_insert_on_write;
    stats::Counter victim_insert_on_evict;
    stats::Average miss_time;
    stats::Counter requests_received;
    stats::Counter acks_received;
    stats::Counter requests_sent;
    stats::Counter acks_sent;
    stats::Counter requests_processed;
    stats::Counter setup_task_io_requests;
    stats::Counter mshr_overcapacity_events;
    stats::CounterMap<sim::runtime::task_dep_t> hit_by_access_type;
    stats::CounterMap<sim::runtime::task_dep_t> miss_by_access_type;
    stats::CounterMap<sim::runtime::task_dep_t> requests_by_access_type;

    stats::AverageMap miss_time_per_task_type;

    stats::CounterMap<unsigned> cache_read_hit_by_task_type;
    stats::CounterMap<unsigned> cache_read_half_miss_by_task_type;
    stats::CounterMap<unsigned> cache_read_miss_by_task_type;
    stats::CounterMap<unsigned> cache_write_hit_by_task_type;
    stats::CounterMap<unsigned> cache_write_half_miss_by_task_type;
    stats::CounterMap<unsigned> cache_write_miss_by_task_type;
    stats::CounterMap<unsigned> victim_read_hit_by_task_type;
    stats::CounterMap<unsigned> victim_read_miss_by_task_type;
    stats::CounterMap<unsigned> victim_write_hit_by_task_type;
    stats::CounterMap<unsigned> victim_write_miss_by_task_type;
    stats::CounterMap<unsigned> victim_swap_by_task_type;
    stats::CounterMap<unsigned> victim_insert_on_read_by_task_type;
    stats::CounterMap<unsigned> victim_insert_on_write_by_task_type;
    stats::CounterMap<unsigned> victim_insert_on_evict_by_task_type;

    typedef sim::stats::CounterMap<sim::runtime::task_dep_t, std::unordered_map<unsigned, unsigned>> matrix_counter_t;
    matrix_counter_t hit_by_access_type_by_task_type_;
    matrix_counter_t miss_by_access_type_by_task_type_;
    matrix_counter_t requests_by_access_type_by_task_type_;

    template<typename OwnerType>
    explicit CacheStats(const OwnerType &);
    void update_access(bool hit, sim::runtime::task_dep_t dependency_type, std::size_t task_type);
};

}  // namespace memory
}  // namespace sim

#include "CacheStats-impl.h"

#endif  // CORE_MODULES_MEMORY_CACHESTATS_H_
