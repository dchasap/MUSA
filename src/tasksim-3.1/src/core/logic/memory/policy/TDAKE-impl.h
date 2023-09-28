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


#ifndef CORE_LOGIC_MEMORY_POLICY_TDAKE_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_TDAKE_IMPL_H_

#include <string>
#include <utility>
#include <vector>
#include "Policy.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {

inline
TDAKE_Parameters::TDAKE_Parameters(std::size_t size,
                                   task_dep_t keep_type, task_dep_t evict_type, task_dep_t insert_on_LRU) :
        Parameters(size), keep_type_(keep_type), evict_type_(evict_type), insert_on_LRU_(insert_on_LRU)
{
}


template<typename CacheSet>
inline
TDAKE<CacheSet>::TDAKE(CacheSet *set, const TDAKE_Parameters &params) : LRU<CacheSet>(set, params),
        keep_type_(params.keep_type_), evict_type_(params.evict_type_), insert_on_LRU_(params.insert_on_LRU_)
{
}


template<typename CacheSet>
inline
TDAKE<CacheSet>::~TDAKE()
{
}


template<typename CacheSet>
inline
void TDAKE<CacheSet>::insert(const Tag& tag, const CacheLine& line)
{
    const auto success_and_iterator = this->set_->insert_and_get_iterator(tag, line);
    assert(success_and_iterator.first == true);

    std::size_t free_index = this->storage_.insert(tag, success_and_iterator.second);
    if (runtime::TaskDeps::equal(line.get_dep_type(), insert_on_LRU_)) {
        this->lru_stack_.insert(this->lru_stack_.end(), free_index);
        this->map_.emplace(tag, this->lru_stack_.end());
    } else {
        this->lru_stack_.insert(this->lru_stack_.begin(), free_index);
        this->map_.emplace(tag, this->lru_stack_.begin());
    }
}


template<typename CacheSet>
inline
std::string TDAKE<CacheSet>::get_name() const
{
    return "TDAKE";
}


template<typename CacheSet>
inline
std::size_t TDAKE<CacheSet>::get_evict_index()
{
    assert(this->lru_stack_.size() <= this->params_.size_);
    assert(this->map_.size() <= this->params_.size_);

    struct candidate_t { bool valid_; std::size_t index_; };
    candidate_t first_keep({false, 0});
    candidate_t first_evict({false, 0});
    candidate_t first_normal({false, 0});

    /* find first occurrence of each of three groups */
    for (auto index_it = this->lru_stack_.rbegin(); index_it != this->lru_stack_.rend(); ++index_it) {
        auto line = this->storage_.get_cache_line(*index_it);
        if (not line.locked()) {
            if (not first_keep.valid_ and runtime::TaskDeps::equal(line.get_dep_type(), keep_type_)) {
                first_keep = {true, *index_it};
            }
            if (not first_evict.valid_ and runtime::TaskDeps::equal(line.get_dep_type(), evict_type_)) {
                first_evict = {true, *index_it};
            }
            if (not first_normal.valid_ and not runtime::TaskDeps::equal(line.get_dep_type(), keep_type_)
                                        and not runtime::TaskDeps::equal(line.get_dep_type(), evict_type_)) {
                first_normal = {true, *index_it};
            }
        }
    }

    /* select candidate by preference "evict" > "normal" > "keep" */
    candidate_t candidate({false, 0});
    if (first_evict.valid_) {
        candidate = first_evict;  // std::cout << "have evict" << std::endl;
    } else if (first_normal.valid_) {
        candidate = first_normal;  // std::cout << "have normal" << std::endl;
    } else if (first_keep.valid_) {
        candidate = first_keep;  // std::cout << "have keep" << std::endl;
    }
    assert(candidate.valid_);
    return candidate.index_;
}

}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_POLICY_TDAKE_IMPL_H_
