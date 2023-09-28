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


#ifndef CORE_LOGIC_MEMORY_POLICY_SETDUELING_H_
#define CORE_LOGIC_MEMORY_POLICY_SETDUELING_H_

#include <string>
#include <vector>
#include "core/logic/coherence/single/types.h"
#include "core/logic/coherence/single/Message.h"
#include "core/utils/NonCopyable.h"
// #include <core/stats/Counter.h>
// #include <core/stats/List.h>

namespace sim {
namespace logic {
namespace memory {
namespace policy {


template<typename CacheSet, template<typename> class Cache>
class SetDueling : sim::utils::NonCopyable {
private:
    typedef sim::logic::coherence::single::Request::op_t op_t;
    typedef sim::logic::coherence::single::cache_access_outcome_t outcome_t;

    const bool enabled_;

    Cache<CacheSet> *cache_;

    const engine::cycles_t update_interval_;
    engine::cycles_t next_update_;

    typedef int64_t sdm_val_t;  // Set Dueling Monitor - a counter that quantifies policy performance
    typedef std::unordered_map<std::string, sdm_val_t> set_dueling_monitors_t;
    set_dueling_monitors_t sdms_;

    set_dueling_monitors_t::iterator current_best_;  // policy to be used by follower sets on next update
    set_dueling_monitors_t::iterator current_active_;  // policy currently used by follower sets

    /* Mapping training sets to their policies */
    const std::unordered_map<std::size_t, std::string> set_num_to_policy_;
    /* List of sets that use the currently best policy */
    const std::vector<std::size_t> follower_sets_;

    struct EventRewardCalculator {
        typedef std::function<sdm_val_t(op_t, outcome_t)> function_t;
        typedef std::unordered_map<std::string, function_t> function_map_t;
        static sdm_val_t dummy(op_t op, outcome_t outcome);
        static sdm_val_t less_misses(op_t op, outcome_t outcome);
    };

    static const typename EventRewardCalculator::function_map_t event_reward_calculators_;
    typename EventRewardCalculator::function_t calculate_reward_;

    /* Used later for constructing new policies */
    const engine::Config global_config_;

    SetDueling(const bool enabled,
               Cache<CacheSet> *cache,
               const std::size_t set_count,
               const uint64_t update_interval,
               const set_dueling_monitors_t sdms,
               const std::string initial_policy,
               const std::unordered_map<std::size_t, std::string> set_num_to_policy,
               const std::vector<std::size_t> follower_sets,
               const typename EventRewardCalculator::function_t calculate_reward);

    /* temporary stats */
//    SetDuelingStats stats_;

    engine::cycles_t get_clock() const;

public:
    ~SetDueling();
    void signal_event(std::size_t set_num, op_t op, outcome_t outcome);

    bool is_enabled() const;

    std::string get_set_policy(const std::size_t set_num) const;

    typedef std::unique_ptr<SetDueling> ptr_t;
    static ptr_t Create(Cache<CacheSet> *cache, const std::size_t set_count, const engine::Config &conf);
};


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim


#include "SetDueling-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_SETDUELING_H_
