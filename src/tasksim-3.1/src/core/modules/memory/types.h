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


#ifndef CORE_MODULES_MEMORY_TYPES_H_
#define CORE_MODULES_MEMORY_TYPES_H_

#include "core/stats/Counter.h"
#include "core/stats/Average.h"
#include "core/stats/TimeAverage.h"
#include "core/utils/TimeList.h"
#include "core/stats/VulnerabilityStat.h"

namespace sim {
namespace memory {

struct RAMStats {
    /** Generic DRAM stats constructor. */
    template<typename OwnerType>
    explicit RAMStats(const OwnerType &parent);
    /** Total requests latency. */
    sim::stats::Counter latency;
    /** Total request count. */
    sim::stats::Counter count;
    /** Number of cycles spent with new requests blocked. */
    sim::stats::Counter blocked_cycles;
    /** Average number of pending requets. */
    sim::stats::TimeAverage occupancy;
    /** Average number of pending read requests. */
    sim::stats::TimeAverage read_occupancy;
    /** Average number of pending write requests. */
    sim::stats::TimeAverage write_occupancy;
    /** Total read Requests. */
    sim::stats::Counter read_requests;
    /** Total write requests. */
    sim::stats::Counter write_requests;
    /** Average access time. */
    sim::stats::Average access_time;
    /** Average read request time. */
    sim::stats::Average read_time;
    /** Average write request time. */
    sim::stats::Average write_time;
    /** Vulnerability statistics */
    std::unique_ptr<sim::stats::VulnerabilityStat> vulnerability_;
};

}  // namespace memory
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_MODULES_MEMORY_TYPES_H_
