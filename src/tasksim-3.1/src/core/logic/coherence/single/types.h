/*
 * Copyright - Barcelona Supercomputing Center.
 * All rights reserved.
 *
 * Redistribution in source and binary forms, with or without
 * modification, is not permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef CORE_LOGIC_COHERENCE_SINGLE_TYPES_H_
#define CORE_LOGIC_COHERENCE_SINGLE_TYPES_H_

#include "core/engine/types.h"
#include "core/runtime/TaskDataDep.h"

namespace sim {
namespace logic {
namespace coherence {
namespace single {

typedef enum cache_access_outcome_t { HIT, MISS, HALF_MISS } cache_access_outcome_t;

struct CacheLine {
    /** CacheLine state. */
    typedef enum { INVALID, VALID, DIRTY, ALLOCATED } state_t;
    state_t state_;
    /** Task dependency type of the allocating request. */
    sim::runtime::task_dep_t dep_type_;
    /** Task type of the allocating request. */
    std::size_t task_type_;

    /** Empty constructor. */
    CacheLine();
    /** Full constructor. */
    CacheLine(state_t, const sim::runtime::task_dep_t dep_type = sim::runtime::task_dep_t::NON_IN_OUT,
            std::size_t task_type = 0);
    /** Locked bit getter. */
    bool locked() const;
    /** Dirty bit getter. */
    bool dirty() const;
    /** Returns allocating request task type. */
    sim::runtime::task_dep_t get_dep_type() const;
};
template<typename Message>
class ClearRequest{
public:
    unsigned port_;
    unsigned pending_write_acks_;
    unsigned pending_read_acks_;
    typename Message::Request request_;
    ClearRequest(unsigned port, unsigned pending_write_acks, unsigned pending_read_acks,
            const typename Message::Request &request);
};

}  // namespace single
}  // namespace coherence
}  // namespace logic
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_LOGIC_COHERENCE_SINGLE_TYPES_H_
