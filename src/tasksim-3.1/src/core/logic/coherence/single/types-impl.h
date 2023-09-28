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

#ifndef CORE_LOGIC_COHERENCE_SINGLE_TYPES_IMPL_H_
#define CORE_LOGIC_COHERENCE_SINGLE_TYPES_IMPL_H_

#include "core/engine/types.h"
#include "core/runtime/TaskDataDep.h"

namespace sim {
namespace logic {
namespace coherence {
namespace single {

inline
CacheLine::CacheLine() :
    state_(INVALID)
{}

inline
CacheLine::CacheLine(state_t state, const sim::runtime::task_dep_t dep_type, std::size_t task_type) :
    state_(state), dep_type_(dep_type), task_type_(task_type)
{}

inline
bool CacheLine::locked() const
{
    return state_ == ALLOCATED;
}

inline
bool CacheLine::dirty() const
{
    return state_ == DIRTY;
}

inline
sim::runtime::task_dep_t CacheLine::get_dep_type() const
{
    return dep_type_;
}

template<typename Message>
inline
ClearRequest<Message>::ClearRequest(unsigned port, unsigned pending_write_acks, unsigned pending_read_acks,
        const typename Message::Request &request) :
    port_(port),
    pending_write_acks_(pending_write_acks),
    pending_read_acks_(pending_read_acks),
    request_(request)
{}


}  // namespace single
}  // namespace coherence
}  // namespace logic
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_LOGIC_COHERENCE_SINGLE_IMPL_TYPES_H_
