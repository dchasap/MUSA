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

#ifndef CORE_LOGIC_MEMORY_POLICY_RANDOM_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_RANDOM_IMPL_H_

#include <string>
#include <utility>
#include <type_traits>
#include <algorithm>

#include "types.h"
#include "Random.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {


template<typename CacheSet>
inline
Random<CacheSet>::Random(CacheSet *set, const Parameters &params) : Policy<CacheSet>(set, params),
    random_generator_(std::random_device()()),
    sequence_(this->params_.size_),
    evict_candidate_{false, 0}
{
    std::iota(sequence_.begin(), sequence_.end(), 0);
}


template<typename CacheSet>
inline
Random<CacheSet>::~Random()
{
}


template<typename CacheSet>
inline
void Random<CacheSet>::insert(const Tag &tag, const CacheLine &line)
{
    const auto success_and_iterator = this->set_->insert_and_get_iterator(tag, line);
    assert(success_and_iterator.first == true);

    this->storage_.insert(tag, success_and_iterator.second);
}


template<typename CacheSet>
inline
void Random<CacheSet>::update(const Tag &tag)
{
}


template<typename CacheSet>
inline
std::pair<typename Random<CacheSet>::Tag, typename Random<CacheSet>::CacheLine> Random<CacheSet>::evict()
{
    const auto retval = Policy<CacheSet>::evict();
    evict_candidate_.valid_ = false;
    return retval;
}


template<typename CacheSet>
inline
std::string Random<CacheSet>::get_name() const
{
    return "Random";
}


template<typename CacheSet>
inline
std::size_t Random<CacheSet>::get_evict_index()
{
    if (evict_candidate_.valid_) {
        return evict_candidate_.index_;
    }
    std::shuffle(sequence_.begin(), sequence_.end(), random_generator_);

    std::size_t index = (size_t)-1;  // invalid value, to assert if not over-written

    for (auto i : sequence_) {
        if (this->storage_.valid_[i] and not this->storage_.get_cache_line(i).locked()) {
            index = i;
            break;
        }
    }

    assert(index != (size_t)-1);
    evict_candidate_ = {true, index};
    return index;
}


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_POLICY_RANDOM_IMPL_H_
