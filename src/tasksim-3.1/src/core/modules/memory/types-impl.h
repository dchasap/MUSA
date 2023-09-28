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


#ifndef CORE_MODULES_MEMORY_TYPES_IMPL_H_
#define CORE_MODULES_MEMORY_TYPES_IMPL_H_


namespace sim {
namespace memory {

template<typename OwnerType>
inline
RAMStats::RAMStats(const OwnerType &owner) :
            latency(owner, "Total requests latency"),
            count(owner, "Request count"),
            blocked_cycles(owner, "Cycles spent blocking new requests"),
            occupancy(owner, "Average occupancy"),
            read_occupancy(owner, "Average read occupancy"),
            write_occupancy(owner, "Average write occupancy"),
            read_requests(owner, "Number of read requests"),
            write_requests(owner, "Number of write requests"),
            access_time(owner, "Average access time"),
            read_time(owner, "Average read access time"),
            write_time(owner, "Average write access time")
{}

}  // namespace memory
}  // namespace sim

#endif  // CORE_MODULES_MEMORY_TYPES_IMPL_H_
