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

#ifndef CORE_LOGIC_MEMORY_POLICY_NRU_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_NRU_IMPL_H_

#include <string>
#include <utility>
#include <type_traits>
#include "types.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {

template<typename CacheSet>
inline
NRU<CacheSet>::NRU(CacheSet *set, const Parameters &params) : Policy<CacheSet>(set, params),
    nru_(this->params_.size_, NRUState::DISTANT)
{
}


template<typename CacheSet>
inline
NRU<CacheSet>::~NRU()
{
}


template<typename CacheSet>
inline
void NRU<CacheSet>::insert(const Tag &tag, const CacheLine &line)
{
    const auto success_and_iterator = this->set_->insert_and_get_iterator(tag, line);
    assert(success_and_iterator.first == true);

    std::size_t free_index = this->storage_.insert(tag, success_and_iterator.second);

    nru_[free_index] = NRUState::NEAR_IMMEDIATE;
}


template<typename CacheSet>
inline
void NRU<CacheSet>::update(const Tag &tag)
{
    const size_t index = this->storage_.find(tag);
    nru_[index] = NRUState::NEAR_IMMEDIATE;
}


template<typename CacheSet>
inline
std::size_t NRU<CacheSet>::get_evict_index()
{
    const size_t start_index = 0;  // VD: change this once per-set iterator is introduced

    size_t candidate_index = this->params_.size_;  // VD: this is invalid value
    std::pair<bool, size_t> evict = std::make_pair(false, candidate_index);

    size_t index = start_index;
    do {
        if (!this->storage_.valid_[index] or this->storage_.get_cache_line(index).locked()) {
            continue;
        }

        if (candidate_index == this->params_.size_) {
            candidate_index = index;
        }

        if (nru_[index] == NRUState::NEAR_IMMEDIATE) {
            continue;
        }

        evict = std::make_pair(true, index);
        break;
    } while ((index = (index + 1) % this->params_.size_) != start_index);

    assert(candidate_index != this->params_.size_);

    // If nothing found for eviction
    if (!evict.first) {
        std::fill(nru_.begin(), nru_.end(), NRUState::DISTANT);  // set all NRU bits to stale
        evict = std::make_pair(true, candidate_index);
    }

    assert(evict.first);

    return evict.second;
}


template<typename CacheSet>
inline
std::string NRU<CacheSet>::get_name() const
{
    return "NRU";
}


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_POLICY_NRU_IMPL_H_

