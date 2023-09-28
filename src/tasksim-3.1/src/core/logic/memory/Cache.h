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


#ifndef CORE_LOGIC_MEMORY_CACHE_H_
#define CORE_LOGIC_MEMORY_CACHE_H_

#include <list>
#include <utility>
#include <vector>
#include <core/engine/Simulator.h>
#include <core/logic/coherence/single/types.h>
#include <core/logic/memory/policy/dynamic/PolicyTrainer.h>
#include <core/logic/memory/policy/SetDueling.h>
#include <core/engine/Config.h>

namespace sim {
namespace logic {
namespace memory {

/* This template will identify if the supplied CacheSet:line_t has the dirty() function that returns true if
 * the line cache line state is dirty.
 * This is done to be able to use the module with a CacheSet::line_t that has not a dirty state.
 */

template<typename T>
struct has_dirty {
    template<typename U>
    static auto has_dirty_test(const U *u) -> decltype(u->dirty(), bool()) {}
    static void has_dirty_test(...) {}
    static const bool value = !std::is_void<decltype(has_dirty_test(reinterpret_cast<T*>(0)))>::value;
};


template<typename CacheSet>
class Cache {
protected:
    std::vector<CacheSet> sets_;
    std::vector<unsigned> set_access_count_;
    /* List of dirty lines mantained for the write-through policy. */
    std::list<std::pair<typename CacheSet::tag_t, typename CacheSet::line_t>> pending_writes_;

    unsigned num_sets_;
    unsigned associativity_;
    unsigned tag_shift_;
    bool write_through_;

    typename policy::SetDueling<CacheSet, Cache>::ptr_t set_dueling_;
    std::unique_ptr<policy::dynamic::PolicyTrainer<CacheSet, Cache> > policy_trainer_;

    unsigned get_set_number(const typename CacheSet::tag_t &tag) const;
    /* Function to mark a cache line either as clean or dirty. */
    bool mark_line(const typename CacheSet::tag_t &tag, typename CacheSet::line_t status);

    void remove_pending_write(const typename CacheSet::tag_t &);

    sim::engine::Simulator &simulator_;

    friend class policy::SetDueling<CacheSet, Cache>;
    friend class policy::dynamic::PolicyTrainer<CacheSet, Cache>;
public:
    Cache(sim::engine::Simulator &simulator, std::size_t num_lines, std::size_t associativity,
            std::size_t cache_line_size, bool write_through, const sim::engine::Config &policy_config);
    ~Cache();

    std::size_t get_num_sets() const;
    std::size_t get_set_size() const;
    /* Insert is separated into two functions to enable the write-through policy.
     * This policy is only enforceable if the CacheLine has a dirty() function.
     */
    template<class T = CacheSet, typename std::enable_if<has_dirty<typename T::line_t>::value, int >::type = 0>
    bool insert(const typename CacheSet::tag_t &, const typename CacheSet::line_t &);
    /* This function is used if the CacheLine does not have a dirty() function,
     * It does not mantain write-through coherence.
     */
    template<class T = CacheSet, typename std::enable_if<!has_dirty<typename T::line_t>::value, int >::type = 0>
    bool insert(const typename CacheSet::tag_t &, const typename CacheSet::line_t &);

    bool remove(const typename CacheSet::tag_t &);

    typename CacheSet::line_t *access(const typename CacheSet::tag_t &, bool update = true);

    std::pair<typename CacheSet::tag_t, typename CacheSet::line_t>
    evict(const typename CacheSet::tag_t &);

    /* Function added to snoop the eviction candidate. */
    const std::pair<typename CacheSet::tag_t, typename CacheSet::line_t> *
    inspect_evict(const typename CacheSet::tag_t &);

    bool is_full(const typename CacheSet::tag_t &) const;
    bool can_evict(const typename CacheSet::tag_t &) const;

    std::size_t get_set(const typename CacheSet::tag_t &) const;

    const typename CacheSet::Container &get_set_contents(std::size_t) const;

    void signal_event(const coherence::single::Request&);
    void signal_event(const coherence::single::Request&, coherence::single::cache_access_outcome_t);

    engine::cycles_t get_clock() const;

    std::size_t get_set_size(const std::size_t &set) const;
    std::size_t get_set_free_size(const std::size_t &set) const;

    /* Functions added for marking the cache lines. They help managa the write-through conditions. */
    bool mark_as_dirty(const typename CacheSet::tag_t &tag);
    bool mark_as_clean(const typename CacheSet::tag_t &tag);

    /* Functions to consult if the cache has dirty lines and get their values.
     * Only used on write-through.
     */
    bool has_pending_writes();
    std::pair<typename CacheSet::tag_t, typename CacheSet::line_t> get_pending_write();

    // Get number of free lines
    std::size_t get_free_lines();

    /** Get a Simulator reference: */
    sim::engine::Simulator& get_simulator();
};

}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "Cache-impl.h"

#endif  // CORE_LOGIC_MEMORY_CACHE_H_
