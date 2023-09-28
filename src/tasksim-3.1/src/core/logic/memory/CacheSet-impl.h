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


#ifndef CORE_LOGIC_MEMORY_CACHESET_IMPL_H_
#define CORE_LOGIC_MEMORY_CACHESET_IMPL_H_

#include <utility>
#include <memory>
#include "policy/Policy.h"

#include "core/utils/Log.h"

namespace sim {
namespace logic {
namespace memory {

using sim::utils::Log;

template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
CacheSet<Tag, CacheLine, Array>::CacheSet(std::size_t size) :
    size_(size),
    free_entries_(size_)
{
}

//
//template<typename Tag, typename CacheLine, template<typename...> class Array>
//inline
//CacheSet<Tag, CacheLine, Array>::CacheSet(CacheSet<Tag, CacheLine> && cache_set) :
//    size_(cache_set.size_),
//    contents_(std::move(cache_set.contents_)),
//    free_entries_(std::move(cache_set.free_entries_)),
//    array_(std::move(cache_set.array_)),
//    policy_(std::move(cache_set.policy_))
//{
//}
template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
CacheSet<Tag, CacheLine, Array>::CacheSet(CacheSet<Tag, CacheLine> && cache_set) :
    size_(cache_set.size_),
//    contents_(cache_set.contents_),
    free_entries_(cache_set.free_entries_.size()),
//    array_(cache_set.array_),
    policy_(std::move(cache_set.policy_))
{
    policy_.get()->set_ = this;
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
CacheSet<Tag, CacheLine, Array>::~CacheSet()
{
//    delete policy_;
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
std::size_t CacheSet<Tag, CacheLine, Array>::get_size() const
{
    return size_;
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
std::size_t CacheSet<Tag, CacheLine, Array>::get_free_size() const
{
    return free_entries_.size();
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
bool CacheSet<Tag, CacheLine, Array>::is_full() const
{
    return free_entries_.empty();
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
bool CacheSet<Tag, CacheLine, Array>::insert(const Tag &tag, const CacheLine &cache_line)
{
    if (is_full()) {
        return false;
    } else {
        get_policy()->insert(tag, cache_line);
        return true;
    }
//    return insert(contents_.begin(), tag, cache_line);
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
std::pair<bool, typename CacheSet<Tag, CacheLine, Array>::iterator>
CacheSet<Tag, CacheLine, Array>::insert_and_get_iterator(const Tag &tag, const CacheLine &cache_line)
{
    return insert_and_get_iterator(contents_.begin(), tag, cache_line);
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
std::pair<bool, typename CacheSet<Tag, CacheLine, Array>::iterator>
CacheSet<Tag, CacheLine, Array>::insert_and_get_iterator(iterator pos, const Tag &tag, const CacheLine &cache_line)
{
    if (is_full()) {
        return std::make_pair(false, CacheSet::iterator());
    }
    std::pair<typename Array<Tag, iterator>::iterator, bool> ret =
//            array_.insert(typename Array<Tag, iterator>::value_type(tag, free_entries_.begin()));
            array_.insert(std::make_pair(tag, free_entries_.begin()));
    if (ret.second == false) {
        return std::make_pair(false, CacheSet::iterator());
    }
    free_entries_.front() = std::make_pair(tag, cache_line);
    auto cache_line_iter = free_entries_.begin();
    contents_.splice(pos, free_entries_, free_entries_.begin());

    return std::make_pair(ret.second, cache_line_iter);
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
void CacheSet<Tag, CacheLine, Array>::update(const Tag& tag)
{
    get_policy()->update(tag);
}


template<typename Tag, typename CacheLine, template<typename ...> class Array>
inline
std::pair<Tag, CacheLine> CacheSet<Tag, CacheLine, Array>::evict() {
    auto retval = get_policy()->evict();
    remove(retval.first, false);
    return retval;
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
bool CacheSet<Tag, CacheLine, Array>::remove(const Tag &tag, bool call_policy_remove)
{
    typename Array<Tag, iterator>::iterator i = array_.find(tag);
    if (i == array_.end()) {
        return true;
    }
    if (call_policy_remove) {
        get_policy()->remove(tag);
    }
    free_entries_.splice(free_entries_.begin(), contents_, i->second);
    array_.erase(i);
    return true;
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
typename CacheSet<Tag, CacheLine, Array>::iterator CacheSet<Tag, CacheLine, Array>::find(const Tag &tag)
{
    typename Array<Tag, iterator>::iterator line = array_.find(tag);
    if (line == array_.end()) {
        return contents_.end();
    }
    return line->second;
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
typename CacheSet<Tag, CacheLine, Array>::const_iterator CacheSet<Tag, CacheLine, Array>::find(const Tag &tag) const
{
    typename Array<Tag, iterator>::const_iterator line = array_.find(tag);
    if (line == array_.end()) {
        return contents_.end();
    }
    return line->second;
}



template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
typename CacheSet<Tag, CacheLine, Array>::iterator CacheSet<Tag, CacheLine, Array>::end()
{
    return contents_.end();
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
typename CacheSet<Tag, CacheLine, Array>::const_iterator CacheSet<Tag, CacheLine, Array>::end() const
{
    return contents_.end();
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
typename CacheSet<Tag, CacheLine, Array>::Policy *CacheSet<Tag, CacheLine, Array>::get_policy() const
{
    return policy_.get();
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
void CacheSet<Tag, CacheLine, Array>::set_policy(typename Policy::policy_ptr_t policy)
{
    policy_ = std::move(policy);
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
typename CacheSet<Tag, CacheLine, Array>::Container &CacheSet<Tag, CacheLine, Array>::get_contents()
{
    return contents_;
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
const typename CacheSet<Tag, CacheLine, Array>::Container &CacheSet<Tag, CacheLine, Array>::get_contents() const
{
    return contents_;
}


template<typename Tag, typename CacheLine, template<typename...> class Array>
inline
void CacheSet<Tag, CacheLine, Array>::print_status() const
{
    Log::warning() << " CacheSet contents:";
    for (const auto& item : contents_) {
        Log::warning() << "   - " << item.first;
    }
}

}  // namespace memory
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_MEMORY_CACHESET_IMPL_H_
