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


#ifndef CORE_LOGIC_MEMORY_POLICY_SETDUELING_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_SETDUELING_IMPL_H_

#include <algorithm>
#include <string>
#include <vector>
#include "Policy.h"

namespace sim {
namespace logic {
namespace memory {
namespace policy {

//SetDuelingStats::SetDuelingStats(const std::string & name) :
//        name_(name),
//        number_of_checks(owner, "number-of-checks"),
//        number_of_changes(owner, "number-of-changes"),
//        interval_boundaries(owner, "interval-boundaries"),
//        active_policies(owner, "active-policies")
//{
//}

template<typename CacheSet, template<typename> class Cache>
const typename SetDueling<CacheSet, Cache>::EventRewardCalculator::function_map_t
    SetDueling<CacheSet, Cache>::event_reward_calculators_{
        {"dummy",       SetDueling<CacheSet, Cache>::EventRewardCalculator::dummy},
        {"less-misses", SetDueling<CacheSet, Cache>::EventRewardCalculator::less_misses}
};


template<typename CacheSet, template<typename> class Cache>
typename SetDueling<CacheSet, Cache>::sdm_val_t
    SetDueling<CacheSet, Cache>::EventRewardCalculator::dummy(op_t op, outcome_t outcome)
{
    return 0;
}


template<typename CacheSet, template<typename> class Cache>
typename SetDueling<CacheSet, Cache>::sdm_val_t
    SetDueling<CacheSet, Cache>::EventRewardCalculator::less_misses(op_t op, outcome_t outcome)
{
    return (outcome == outcome_t::MISS) ? -1 : 0;
}


template<typename CacheSet, template<typename> class Cache>
inline
SetDueling<CacheSet, Cache>::SetDueling(const bool enabled,
                                         Cache<CacheSet> *cache,
                                         const std::size_t set_count,
                                         const uint64_t update_interval,
                                         const set_dueling_monitors_t sdms,
                                         const std::string initial_policy,
                                         const std::unordered_map<std::size_t, std::string> set_num_to_policy,
                                         const std::vector<std::size_t> follower_sets,
                                         const typename EventRewardCalculator::function_t calculate_reward) :
        enabled_(enabled),
        cache_(cache),
        update_interval_(static_cast<engine::cycles_t>(update_interval)),
        next_update_(update_interval_),
        sdms_(sdms),
        current_best_(sdms_.find(initial_policy)),
        current_active_(sdms_.find(initial_policy)),
        set_num_to_policy_(set_num_to_policy),
        follower_sets_(follower_sets),
        calculate_reward_(calculate_reward)
{
    assert(cache != nullptr);
    if (enabled) {
        std::cout << "---- Initialization of SetDueling ----" << std::endl;
        std::cout << "enabled " << enabled_ << std::endl;
        std::cout << "cache   " << cache_ << std::endl;
        std::cout << "update_interval " << update_interval_ << std::endl;
        std::cout << "sdms"  << std::endl;
        for (auto shit : sdms_) std::cout << "\t" << shit.first << " " << shit.second << std::endl;
        std::cout << "current best " << (*current_best_).first << " " << (*current_best_).second  << std::endl;
        std::cout << "current_active " << (*current_active_).first << " " << (*current_active_).second  << std::endl;
        std::cout << "set_num_to_policy"  << std::endl;
        for (auto shit : set_num_to_policy_) std::cout << "\t" << shit.first << " " << shit.second  << std::endl;
        std::cout << "follower sets"  << std::endl;
        for (auto shit : follower_sets_) std::cout << "\t" << shit << std::endl;
        std::cout << "=======================================" << std::endl;
    }
}


template<typename CacheSet, template<typename> class Cache>
inline
engine::cycles_t SetDueling<CacheSet, Cache>::get_clock() const
{
    return cache_->get_clock();
}

template<typename CacheSet, template<typename> class Cache>
inline
SetDueling<CacheSet, Cache>::~SetDueling()
{
}


template<typename CacheSet, template<typename> class Cache>
typename SetDueling<CacheSet, Cache>::ptr_t
    SetDueling<CacheSet, Cache>::Create(Cache<CacheSet>* cache,
                                         const std::size_t set_count,
                                         const engine::Config& conf)
{
    /* Declare all SetDueling constructor parameters */
    const bool enabled = conf.is_valid() and conf.get_value_with_default<bool>("set-dueling-enabled", false);
    uint64_t update_interval = 0;
    typename SetDueling<CacheSet, Cache>::set_dueling_monitors_t sdms;
    std::string initial_policy;
    std::unordered_map<std::size_t, std::string> set_num_to_policy;
    std::vector<std::size_t> follower_sets(set_count);
    typename EventRewardCalculator::function_t calculate_reward =
        SetDueling<CacheSet, Cache>::event_reward_calculators_.at("dummy");

    /* Compute all SetDueling constructor parameters */
    if (enabled) {
        update_interval = conf.get_value<uint64_t>("set-dueling-update-interval");

        const std::vector<std::string> policies(conf.get_values<std::string>("set-dueling-policies"));
        const std::vector<std::string> tr_set_lists(conf.get_values<std::string>("set-dueling-training-sets"));

        if (policies.size() < 2) {
            std::cerr << "At least 2 policies should be specified for parameter 'set-dueling-policies' "
                         "when Set Dueling is enabled."
                      << std::endl;
            exit(1);
        }

        if (policies.size() != tr_set_lists.size()) {
            std::cerr << "Number of set dueling policies has to be the same as the number of training set sequences."
                      << std::endl;
            exit(1);
        }
        std::iota(follower_sets.begin(), follower_sets.end(), 0);
        std::cout << "Follower sets" << std::endl;
        for (auto f : follower_sets) std::cout << f << " " << std::endl;
        for (std::size_t i = 0; i < policies.size(); ++i) {
            sdms.emplace(policies[i], 0);
            /* Parse training sets for the current policy */
            std::cout << "training sets " << tr_set_lists[i] << std::endl;
            std::stringstream ss(tr_set_lists[i]);
            std::string tr_set;
            while (std::getline(ss, tr_set, ',')) {
                const std::size_t tr_set_num = std::stoull(tr_set);
                std::cout << "training set " << tr_set_num << std::endl;
                set_num_to_policy.emplace(tr_set_num, policies[i]);
                auto it = std::find(follower_sets.begin(), follower_sets.end(), tr_set_num);
                if (it == follower_sets.end()) {
                    std::cerr << "Set number " << tr_set_num << " is being specified as training set for the policy "
                              << policies[i] << " but has already been specified as training set for another policy."
                              << std::endl;
                    exit(1);
                }
                /* Remove training set from follower list */
                follower_sets.erase(it);
            }
        }
        follower_sets.shrink_to_fit();

        initial_policy = conf.get_value<std::string>("default-policy");
        if (sdms.find(initial_policy) == sdms.end()) {
            std::cerr << "Default policy does not belong to the list of Set Dueling policies." << std::endl;
            exit(1);
        }

        const std::string sdpc(conf.get_value<std::string>("set-dueling-policy-comparator"));
        if (event_reward_calculators_.find(sdpc) == event_reward_calculators_.end()) {
            std::cerr << sdpc << " is not a valid Set Dueling Policy Comparator" << std::endl;
            exit(1);
        }
        calculate_reward = event_reward_calculators_.at(sdpc);
    }

    /* Create the object */
    return ptr_t(new SetDueling(enabled, cache, set_count, update_interval, sdms, initial_policy, set_num_to_policy,
                                follower_sets, calculate_reward));
}


template<typename CacheSet, template<typename> class Cache>
inline
void SetDueling<CacheSet, Cache>::signal_event(std::size_t set_num, op_t op, outcome_t outcome)
{
    if (!enabled_) {
        return;
    }
    /* Check if the event occurs in a training set */
    const auto set_policy = set_num_to_policy_.find(set_num);
    if (set_policy != set_num_to_policy_.end()) {
        const auto sdm = sdms_.find((*set_policy).second);
        assert(sdm != sdms_.end());
        (*sdm).second += calculate_reward_(op, outcome);

        /* Check if the updated policy is better than previous best one */
        if ((*sdm).second > (*current_best_).second) {
            current_best_ = sdm;
        }
    }

    const engine::cycles_t current_clock = get_clock();
    if (current_clock >= next_update_) {
        while (current_clock >= next_update_) {
            next_update_ += update_interval_;
        }
        if (current_active_ != current_best_) {
            /* Change policies of follower sets*/
            current_active_ = current_best_;

            engine::Config set_policy_conf =
                    policy::Policy<CacheSet>::policy_config_register_.get((*current_active_).first);
            unsigned size = cache_->sets_[0].get_size();

            for (auto follower_set : follower_sets_) {
                CacheSet *set = &(cache_->sets_[follower_set]);
                auto new_policy = policy::Policy<CacheSet>::Create(set, size, set_policy_conf);
                new_policy.get()->initialize_from_other_policy(set->get_policy());
                set->set_policy(std::move(new_policy));
            }
        }
    }
}


template<typename CacheSet, template<typename> class Cache>
inline
bool SetDueling<CacheSet, Cache>::is_enabled() const
{
    return enabled_;
}


template<typename CacheSet, template<typename> class Cache>
inline
std::string SetDueling<CacheSet, Cache>::get_set_policy(const std::size_t set_num) const
{
    assert(enabled_);
    if (std::find(follower_sets_.begin(), follower_sets_.end(), set_num) != follower_sets_.end()) {
        return (*current_active_).first;
    } else {
        auto it = sdms_.find(set_num_to_policy_.at(set_num));
        assert(it != sdms_.end());
        return (*it).first;
    }
}

}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_MEMORY_POLICY_SETDUELING_IMPL_H_
