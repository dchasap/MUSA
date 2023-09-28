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

#ifndef CORE_LOGIC_MEMORY_POLICY_LRU_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_LRU_IMPL_H_

#include <cassert>
#include <utility>
#include <string>
#include <type_traits>


namespace sim {
namespace logic {
namespace memory {
namespace policy {

template<typename CacheSet>
inline
LRU<CacheSet>::LRU(CacheSet *set, const Parameters &params) : Policy<CacheSet>(set, params),
    lru_stack_(), map_()
{
}


template<typename CacheSet>
inline
LRU<CacheSet>::~LRU()
{
}


template<typename CacheSet>
inline
void LRU<CacheSet>::insert(const Tag &tag, const CacheLine &line)
{
    // VD: Assuming that insert is not called on full set
    const auto success_and_iterator = this->set_->insert_and_get_iterator(tag, line);
    assert(success_and_iterator.first == true);

    std::size_t free_index = this->storage_.insert(tag, success_and_iterator.second);
    lru_stack_.insert(lru_stack_.begin(), free_index);
    map_.emplace(tag, lru_stack_.begin());
}


template<typename CacheSet>
inline
void LRU<CacheSet>::update(const Tag &tag)
{
    const auto it = map_.find(tag);
    assert(it != map_.end());
    lru_stack_.splice(lru_stack_.begin(), lru_stack_, (*it).second);
}


template<typename CacheSet>
inline
void LRU<CacheSet>::remove(const Tag &tag)
{
    Policy<CacheSet>::remove(tag);
    auto it = map_.at(tag);
    lru_stack_.erase(it);
    map_.erase(tag);
}


template<typename CacheSet>
inline
std::string LRU<CacheSet>::get_name() const
{
    return "LRU";
}


template<typename CacheSet>
inline
void LRU<CacheSet>::initialize_from_other_policy(const Policy<CacheSet> *other_policy)
{
    Policy<CacheSet>::initialize_from_other_policy(other_policy);
    for (std::size_t i = 0; i < this->params_.size_; ++i) {
        if (this->storage_.valid_[i]) {
            lru_stack_.insert(lru_stack_.begin(), i);
            map_.emplace(this->storage_.get_tag(i), lru_stack_.begin());
        }
    }
}


template<typename CacheSet>
inline
std::size_t LRU<CacheSet>::get_evict_index()
{
    for (auto it = lru_stack_.rbegin(); it != lru_stack_.rend(); ++it) {
        if (not this->storage_.get_cache_line(*it).locked()) {
            return (*it);
        }
    }
    assert(0);
    return 0;
}


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_POLICY_LRU_IMPL_H_

