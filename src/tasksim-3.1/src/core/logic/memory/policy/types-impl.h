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


#ifndef CORE_LOGIC_MEMORY_POLICY_TYPES_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_TYPES_IMPL_H_

#include <cassert>
#include "types.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {


inline
Parameters::Parameters(std::size_t size) : size_(size)
{
}


template<typename Tag, typename CacheSet>
inline
Storage<Tag, CacheSet>::Storage(const std::size_t size) :
    container_(size),
    valid_(size, false),
    free_indices_(size)
{
    std::iota(free_indices_.rbegin(), free_indices_.rend(), 0);
}


template<typename Tag, typename CacheSet>
inline
size_t Storage<Tag, CacheSet>::insert(const Tag &tag, const typename CacheSet::iterator &iter)
{
    assert(free_indices_.empty() == false);

    const size_t free_index = free_indices_.back();
    free_indices_.pop_back();

    assert(valid_[free_index] == false);

    container_[free_index] = std::make_pair(tag, iter);
    valid_[free_index] = true;
    map_.emplace(tag, container_.begin() + free_index);

    return free_index;
}


template<typename Tag, typename CacheSet>
inline
size_t Storage<Tag, CacheSet>::find(const Tag &tag) const
{
    const auto map_iter = map_.find(tag);
    assert(map_iter != map_.end());
    const auto container_iter = (*map_iter).second;
    return container_iter - container_.begin();
}


template<typename Tag, typename CacheSet>
inline
Tag Storage<Tag, CacheSet>::get_tag(const std::size_t index) const
{
    assert(index < container_.size());
    assert(valid_[index] == true);
    return container_[index].first;
}


template<typename Tag, typename CacheSet>
inline
const typename CacheSet::line_t& Storage<Tag, CacheSet>::get_cache_line(const std::size_t index) const
{
    assert(index < container_.size());
    assert(valid_[index] == true);
    return (*container_[index].second).second;
}


template<typename Tag, typename CacheSet>
inline
void Storage<Tag, CacheSet>::remove(const std::size_t index)
{
    assert(index < container_.size());
    assert(valid_[index] == true);

    valid_[index] = false;
    const Tag removed_tag = container_[index].first;
    map_.erase(removed_tag);
    free_indices_.push_back(index);
}


template<typename Tag, typename CacheSet>
inline
void Storage<Tag, CacheSet>::refresh_map()
{
    map_.clear();
    for (auto elem = container_.begin(); elem != container_.end(); ++elem) {
        map_.emplace((*elem).first, elem);
    }
}


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_MEMORY_POLICY_TYPES_IMPL_H_
