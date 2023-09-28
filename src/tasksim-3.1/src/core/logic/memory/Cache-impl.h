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


#ifndef CORE_LOGIC_MEMORY_CACHE_IMPL_H_
#define CORE_LOGIC_MEMORY_CACHE_IMPL_H_


#include <cassert>
#include <cstddef>
#include <iostream>
#include <cmath>
#include <set>
#include <utility>

#include "policy/Policy.h"
#include "core/utils/Log.h"

namespace sim {
namespace logic {
namespace memory {

using sim::utils::Log;

template<typename CacheSet>
inline
Cache<CacheSet>::Cache(sim::engine::Simulator &simulator,
                               std::size_t num_lines,
                               std::size_t associativity,
                               std::size_t cache_line_size,
                               bool write_through,
                               const engine::Config &cache_policy_config) :
    num_sets_((num_lines + associativity - 1) / associativity),
    associativity_(associativity),
    tag_shift_(0),
    write_through_(write_through),
    set_dueling_(std::move(policy::SetDueling<CacheSet, Cache>::Create(this, num_sets_, cache_policy_config))),
    policy_trainer_(policy::dynamic::PolicyTrainer<CacheSet, Cache>::Create(this, cache_policy_config, simulator.get_config())),
    simulator_(simulator)
{
    const sim::engine::Config &global_config = simulator.get_config();
    if (cache_line_size == 0  or (cache_line_size & (cache_line_size - 1)) != 0) {
        std::cerr << "[ERROR] Cache line size should be a power of 2." << std::endl;
        exit(-1);
    }
    /* Compute the number of bits to shift the tag to index the set */
    std::size_t cache_line_size_backup = cache_line_size;
    while(cache_line_size >>= 1) {
        ++tag_shift_;
    }
    sets_.reserve(num_sets_);
    for(unsigned n = 0; n < num_sets_; ++n)
    {
        sets_.emplace_back(associativity);

        CacheSet *new_set = &(*(sets_.end() - 1));

        engine::Config set_policy_config;

        if (global_config.is_valid() and cache_policy_config.is_valid()) {
            std::string set_policy_type = set_dueling_.get()->is_enabled() ?
                                          set_dueling_.get()->get_set_policy(n) :
                                          cache_policy_config.get_value<std::string>("default-policy");
            set_policy_config = engine::Config(global_config, set_policy_type);
            policy::Policy<CacheSet>::populate_cache_config_runtime_info(set_policy_config, global_config,
                    num_lines * cache_line_size_backup);
            policy::Policy<CacheSet>::policy_config_register_.add(set_policy_type, set_policy_config);
        } else {
            /* If class is not classical cache and does not specify replacement policy */
            set_policy_config.add_value<std::string>("LRU", "type");
        }

        set_policy_config.add_value<std::size_t>(associativity, "size");

        auto policy = policy::Policy<CacheSet>::Create(new_set, associativity, set_policy_config);
        new_set->set_policy(std::move(policy));

        set_access_count_.emplace_back(0);
    }

    if (policy_trainer_) {
        policy_trainer_->init_policies();
    }

}


template<typename CacheSet>
inline
Cache<CacheSet>::~Cache()
{
    /*unsigned min = set_access_count_[0];
    unsigned max = set_access_count_[0];
    unsigned sum = 0;
    for (unsigned i = 0; i < set_access_count_.size(); i++)
    {
        if (min > set_access_count_[i]) min = set_access_count_[i];
        if (max < set_access_count_[i]) max = set_access_count_[i];
        sum += set_access_count_[i];
    }

    double average = sum / set_access_count_.size();
    double sum2 = 0;
    for (unsigned i = 0; i < set_access_count_.size(); i++)
    {
        sum2 += std::pow(set_access_count_[i] - average, 2);
    }

    double stdev = std::sqrt(sum2 / set_access_count_.size());

    std::cout << "Total   : " << sum                          << std::endl;
    std::cout << "Min     : " << min                          << std::endl;
    std::cout << "Max     : " << max                          << std::endl;
    std::cout << "Average : " << average                      << std::endl;
    std::cout << "StDev   : " << stdev                        << std::endl;
    std::cout << "RelStDev: " << 100 * stdev / average << "%" << std::endl;*/
}


template<typename CacheSet>
inline
unsigned Cache<CacheSet>::get_set_number(const typename CacheSet::tag_t &tag) const
{
    return static_cast<unsigned>(tag >> tag_shift_) % num_sets_;
}


template<typename CacheSet>
inline
std::size_t Cache<CacheSet>::get_num_sets() const
{
    return num_sets_;
}


template<typename CacheSet>
inline
std::size_t Cache<CacheSet>::get_set_size() const
{
    return associativity_;
}


template<typename CacheSet>
template<class T, typename std::enable_if<has_dirty<typename T::line_t>::value, int >::type>
inline
bool Cache<CacheSet>::insert(const typename CacheSet::tag_t &tag, const typename CacheSet::line_t &line)
{
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());
    bool success = sets_[set].insert(tag, line);
    if (!success) {
        return false;
    }

    /* If we are inserting a dirty line in a write-through cache, we add it to the list. */
    if (line.dirty() and write_through_ == true) {
        pending_writes_.push_back( { tag, line } );
    }
    return true;
}


template<typename CacheSet>
template<class T, typename std::enable_if<!has_dirty<typename T::line_t>::value, int >::type>
inline
bool Cache<CacheSet>::insert(const typename CacheSet::tag_t &tag, const typename CacheSet::line_t &line)
{
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());
    return sets_[set].insert(tag, line);
    // As the CacheLines does not have a dirty() function, we cannot mantain write-through coherence.
}

template<typename CacheSet>
inline
bool Cache<CacheSet>::remove(const typename CacheSet::tag_t &tag)
{
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());
    remove_pending_write(tag);
    return sets_[set].remove(tag);
}


template<typename CacheSet>
inline
typename CacheSet::line_t *Cache<CacheSet>::access(const typename CacheSet::tag_t &tag, bool update)
{
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());
    typename CacheSet::iterator i = sets_[set].find(tag);
    if (i == sets_[set].end()) {
        return NULL;
    }

    set_access_count_[set]++;
    if (update) {
        sets_[set].update(tag);
    }

    return &(i->second);
}


template<typename CacheSet>
inline
std::pair<typename CacheSet::tag_t, typename CacheSet::line_t>
Cache<CacheSet>::evict(const typename CacheSet::tag_t &tag)
{
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());
    assert(sets_[set].is_full() == true);
    auto ret = sets_[set].evict();

    typename CacheSet::tag_t val = ret.first;
    // We need to check if the line we evict was dirty and pending a write-through:
    remove_pending_write(val);
    return ret;
}


template<typename CacheSet>
inline
bool Cache<CacheSet>::is_full(const typename CacheSet::tag_t &tag) const
{
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());
    return sets_[set].is_full();
}

template<typename CacheSet>
inline
bool Cache<CacheSet>::can_evict(const typename CacheSet::tag_t &tag) const
{
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());

    if (sets_[set].is_full() == false) {
        return true;
    }

    const typename CacheSet::Container &contents = get_set_contents(set);
    for (auto entry = contents.begin(); entry != contents.end(); entry++) {
        if ((*entry).second.locked() == false) {
            return true;
        }
    }
    return false;
}


template<typename CacheSet>
inline
std::size_t Cache<CacheSet>::get_set(const typename CacheSet::tag_t &tag) const
{
    return get_set_number(tag);
}


template<typename CacheSet>
inline
const typename CacheSet::Container &Cache<CacheSet>::get_set_contents(std::size_t index) const
{
    assert(index >= 0 && index < get_num_sets());
    return sets_[index].get_contents();
}


template<typename CacheSet>
inline
void Cache<CacheSet>::signal_event(const coherence::single::Request &req)
{
    if (req.get_tag() == sim::engine::special_address_t::TASK_END) {
        if (policy_trainer_) {
            policy_trainer_->signal_task_end(req.get_cpu_id(), req.get_task_type());
        }
    } else if (req.get_tag() == sim::engine::special_address_t::TASK_ID) {
        if (policy_trainer_) {
            policy_trainer_->signal_task_start(req.get_cpu_id(), req.get_task_type());
        }
    }
}


template<typename CacheSet>
inline
void Cache<CacheSet>::signal_event(const coherence::single::Request &req,
                                           coherence::single::cache_access_outcome_t outcome)
{
    set_dueling_.get()->signal_event(get_set(req.get_tag()), req.get_op(), outcome);
    if (policy_trainer_) {
        policy_trainer_->sample(req.get_cpu_id(), req.get_task_type(), req.get_dep_type(), outcome);
    }
}


template<typename CacheSet>
inline
engine::cycles_t Cache<CacheSet>::get_clock() const
{
    return simulator_.get_clock();
}


template<typename CacheSet>
inline
std::size_t Cache<CacheSet>::get_set_size(const std::size_t &set) const
{
    assert(set < sets_.size());
    return sets_[set].get_size();
}


template<typename CacheSet>
inline
std::size_t Cache<CacheSet>::get_set_free_size(const std::size_t &set) const
{
    assert(set < sets_.size());
    return sets_[set].get_free_size();
}

template<typename CacheSet>
inline
bool Cache<CacheSet>::mark_line(const typename CacheSet::tag_t &tag, typename CacheSet::line_t status)
{
    // We do not call access to not update the LRU counter.
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());
    assert(sets_[set].find(tag) != sets_[set].end());

    if (sets_[set].find(tag)->second.state_ == status.state_) {
        return false;
    }
    sets_[set].find(tag)->second.state_ = status.state_;
    return true;
}

template<typename CacheSet>
inline
void Cache<CacheSet>::remove_pending_write(const typename CacheSet::tag_t &tag)
{
    if (write_through_) {
        for (auto it = pending_writes_.begin(); it != pending_writes_.end(); it++) {
            if (it->first == tag) {
                pending_writes_.erase(it);
                break;
            }
        }
    }
    return;
}

template<typename CacheSet>
inline
bool Cache<CacheSet>::mark_as_dirty(const typename CacheSet::tag_t &tag)
{
    bool ret = mark_line(tag, CacheSet::line_t::DIRTY);
    if (ret == true and write_through_ == true) {
        unsigned set = get_set_number(tag);
        assert(set < sets_.size());
        assert(sets_[set].find(tag) != sets_[set].end());
        pending_writes_.push_back( { tag, sets_[set].find(tag)->second } );
    }
    return ret;
}

template<typename CacheSet>
inline
bool Cache<CacheSet>::mark_as_clean(const typename CacheSet::tag_t &tag)
{
    return mark_line(tag, CacheSet::line_t::VALID);
}

template<typename CacheSet>
inline
bool Cache<CacheSet>::has_pending_writes()
{
    return not pending_writes_.empty();
}

template<typename CacheSet>
inline
std::pair<typename CacheSet::tag_t, typename CacheSet::line_t> Cache<CacheSet>::get_pending_write()
{
    assert(has_pending_writes());
    auto ret = pending_writes_.front();
    // This should be done in the controller: mark_as_clean(ret);
    pending_writes_.pop_front();
    return ret;
}

template<typename CacheSet>
inline
const std::pair<typename CacheSet::tag_t, typename CacheSet::line_t>*
Cache<CacheSet>::inspect_evict(const typename CacheSet::tag_t &tag)
{
    Log::debug4() << "Inspecting evict.";
    unsigned set = get_set_number(tag);
    assert(set < sets_.size());
    return sets_[set].get_policy()->inspect_evict();
}

template<typename CacheSet>
inline
std::size_t
Cache<CacheSet>::get_free_lines()
{
    size_t free_lines = 0;
    for (size_t i = 0; i < sets_.size(); ++i) {
        free_lines += sets_[i].get_free_size();
    }
    return free_lines;
}

template<typename CacheSet>
inline
sim::engine::Simulator& Cache<CacheSet>::get_simulator()
{
    return simulator_;
}
}  // namespace memory
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_MEMORY_CACHE_IMPL_H_
