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


#ifndef CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_IMPL_H_

#include <core/utils/instrumentation/Instrumentation.h>
#include <core/utils/macros.h>

#include "PolicyTrainer.h"
namespace sim {
namespace logic {
namespace memory {
namespace policy {
namespace dynamic {


template<typename CacheSet, template<typename> class Cache>
inline
void PolicyTrainer<CacheSet, Cache>::instrument_event(unsigned int cpu, unsigned int type, unsigned int value)
{
    std::size_t cpu_id;
    if (paraver_sampling_policy_ == "DISABLED") {
        cpu_id = -1;
    } else if (paraver_sampling_policy_ == "PER_TASK") {
        cpu_id = cpu;
    } else if (paraver_sampling_policy_ == "PERIODIC") {
        cpu_id = 2 * cpu + 1;
    }
    cache_->get_simulator().get_instrumentation().add_event(utils::instrumentation::InstrumentationItem {
        static_cast<unsigned int>(cpu_id),
        0,
        cache_->get_simulator().get_clock(),
        utils::instrumentation::EVENT_RECORD,
        utils::instrumentation::Event {type, value}});
}


template<typename CacheSet, template<typename> class Cache>
inline
void PolicyTrainer<CacheSet, Cache>::filter_policies(std::size_t task_type)
{
    if (initialized_[task_type]) {
        std::size_t min_quality = records_.at(task_type).get_min().second;
        _unused(min_quality);
        const std::unordered_map<RecordProbability::policy_t, RecordProbability::ListRecords> *records =
            records_.at(task_type).get_records();
        _unused(records);
        std::cout << "Before filter, there are " << used_policies_.at(task_type).size() << " policies for tt=" << task_type;
        std::cout << " Min quality is " << min_quality << std::endl;
        for (auto rit = records->begin(); rit != records->end(); ++rit) {
            std::size_t quality = rit->second.get_quality();
            if (quality > 2 * (min_quality + 1)) {
                // discard this policy
                auto it = std::find(used_policies_.at(task_type).begin(), used_policies_.at(task_type).end(), rit->first);
                if (it != used_policies_.at(task_type).end()) {
                    used_policies_.at(task_type).erase(it);
                    std::cout << "Removing policy " << rit->first << " (" << quality << ") for task_type=" << task_type << std::endl;
                }
            }
        }
        assert(used_policies_.at(task_type).size() != 0);
        current_policy_counter_.at(task_type).set_limit(used_policies_.at(task_type).size());
    }
}


template<typename CacheSet, template<typename> class Cache>
inline
PolicyTrainer<CacheSet, Cache>::PolicyTrainer(Cache<CacheSet> * cache, uint16_t core_count,
                                                uint32_t test_length,
                                                uint32_t train_length_per_policy,
                                                uint32_t history_length,
                                                bool filter_policies,
                                                const std::string &paraver_sampling_policy) :
    policies_in_cache_initialized_(false),
    initialized_(MAX_TASK_TYPES, false),
    cache_(cache),
    test_length_(test_length),
    train_length_per_policy_(train_length_per_policy),
    history_length_(history_length),
    filter_policies_(filter_policies),
    states_(core_count, state_t::WAITING_FOR_TASK_TO_START),
    current_task_types_(core_count),
    current_record_(core_count),
    records_(),
    available_policies_({0, 0.02, 0.06, 0.1, 0.5, 0.7, 0.9, 0.96, 0.98, 1}),
    used_policies_(),
    current_policy_(),
    current_policy_counter_(),
    retrain_counter_(),
    in_training_(),
    test_periods_(*this, "test_periods"),
    training_periods_(*this, "training_periods"),
    task_instances_per_task_type_(*this, "task_instances_per_task_type"),
    best_policy_per_task_type_(*this, "best_policy_per_task_type"),
    last_task_type_per_core_(core_count, 0),
    same_task_instances_per_core_(core_count, 0),
    paraver_sampling_policy_(paraver_sampling_policy)
{
}


template<typename CacheSet, template<typename> class Cache>
inline
PolicyTrainer<CacheSet, Cache>::~PolicyTrainer()
{
    // std::cout << "SIZE of records_ = " << records_.size() << std::endl;
    for (auto record : records_) {
        record.second.print("");
        std::cout << ":" << record.first << "_sampled_records:" << get_name() << ":" << get_id() << std::endl;
        best_policy_per_task_type_[record.first] += 100.0f * record.second.get_best();
    }
}


template<typename CacheSet, template<typename> class Cache>
inline
void PolicyTrainer<CacheSet, Cache>::init_policies()
{
    assert (not policies_in_cache_initialized_);
    std::cout << "PolicyTrainer: Initializing policies" << std::endl;
    for (auto set_it = cache_->sets_.begin(); set_it != cache_->sets_.end(); ++set_it){
        policy::Policy<CacheSet> *curr_policy = (*set_it).get_policy();
        SRRIP<CacheSet> *dyn_policy = dynamic_cast<SRRIP<CacheSet>*>(curr_policy);
        if (dyn_policy) {
            for (std::size_t tt = 0; tt < MAX_TASK_TYPES; ++tt) {
//                std::cout << std::setw(4) << std::left << tt;
                dyn_policy->set_brrip_probability(tt, available_policies_[0]);
            }
//            std::cout << std::endl;
        } else {
//            std::cout << " dyn_cast FAIL ";
        }
    }
    policies_in_cache_initialized_ = true;
}


template<typename CacheSet, template<typename> class Cache>
inline
void PolicyTrainer<CacheSet, Cache>::init(uint16_t task_type)
{
    assert(policies_in_cache_initialized_);
    assert(not initialized_[task_type]);
    used_policies_[task_type] = available_policies_;
    current_policy_[task_type] = 0;
    current_policy_counter_.emplace(std::make_pair(task_type, Counter(train_length_per_policy_)));
    retrain_counter_.emplace(std::make_pair(task_type, Counter(test_length_)));
    in_training_[task_type] = true;
    initialized_[task_type] = true;
    set_policy(task_type, available_policies_[current_policy_[task_type]]);
}


template<typename CacheSet, template<typename> class Cache>
inline
void PolicyTrainer<CacheSet, Cache>::sample(uint16_t cpu_id, uint32_t task_type,
                                             runtime::task_dep_t dep_type,
                                             coherence::single::cache_access_outcome_t outcome)
{
    assert(policies_in_cache_initialized_);
    assert(initialized_[current_task_types_[cpu_id]]);
//    assert(states_[cpu_id] == state_t::TRAINING or states_[cpu_id] == state_t::TESTING);
    if (states_[cpu_id] != state_t::TRAINING and states_[cpu_id] != state_t::TESTING) {
        return;
    }

    if (task_type != last_task_type_per_core_[cpu_id]) {
//        std::cout << std::setw(6) << same_task_instances_per_core_[cpu_id] << " samples\n---------------"
//                << std::endl << "Starting task " << std::setw(4) << task_type << " on cpu "
//                << std::setw(4) << cpu_id << std::endl;
        last_task_type_per_core_[cpu_id] = task_type;
        same_task_instances_per_core_[cpu_id] = 1;
        instrument_event(cpu_id, utils::instrumentation::SIMULATOR_EVENTS::POLICY_TRAINER_SAMPLED_TASK_TYPE,
            task_type);
    } else {
        same_task_instances_per_core_[cpu_id]++;
    }
    if (outcome == coherence::single::cache_access_outcome_t::MISS) {
        current_record_[cpu_id].counters_[runtime::task_dep_to_index.at(dep_type)]++;
    }
}


template<typename CacheSet, template<typename> class Cache>
inline
void PolicyTrainer<CacheSet, Cache>::signal_task_start(uint16_t cpu_id, uint32_t task_type)
{
    assert(policies_in_cache_initialized_);
    if (not initialized_[task_type]) {
        init(task_type);
    }
    assert(initialized_[task_type]);
    if (in_training_[task_type]) {
        states_[cpu_id] = state_t::TRAINING;
    } else {
        states_[cpu_id] = state_t::TESTING;
    }
    current_task_types_[cpu_id] = task_type;
}


template<typename CacheSet, template<typename> class Cache>
inline
void PolicyTrainer<CacheSet, Cache>::signal_task_end(uint16_t cpu_id, uint32_t task_type)
{
    assert(policies_in_cache_initialized_);
    assert(current_task_types_[cpu_id] == task_type);
    assert(initialized_[task_type]);
    task_instances_per_task_type_[task_type]++;
    instrument_event(cpu_id, utils::instrumentation::SIMULATOR_EVENTS::POLICY_TRAINER_TASK_END, task_type);
    instrument_event(cpu_id, utils::instrumentation::SIMULATOR_EVENTS::POLICY_TRAINER_CURRENT_POLICY + task_type,
            current_policy_[task_type]);
    instrument_event(cpu_id, utils::instrumentation::SIMULATOR_EVENTS::POLICY_TRAINER_MISSES,
            RecordProbability::item_t(available_policies_[current_policy_[task_type]],
                                                             current_record_[cpu_id].counters_).total());

    if (records_.find(task_type) == records_.end()) {
        records_.insert(std::make_pair(task_type, RecordProbability(history_length_)));
    }
    assert(records_.find(task_type) != records_.end());
    records_.at(task_type).sample(RecordProbability::item_t(available_policies_[current_policy_[task_type]],
                                                             current_record_[cpu_id].counters_));
    current_record_[cpu_id].reset_counters();
    current_task_types_[cpu_id] = task_type;
    current_policy_counter_.at(task_type).increment();
    states_[cpu_id] = state_t::WAITING_FOR_TASK_TO_START;
    // policy switching

    if (in_training_[task_type]) {
        if (current_policy_counter_.at(task_type).test_flag()) {
            // we finished training for this policy
            current_policy_counter_.at(task_type).reset_flag();
            // we select the next policy
            current_policy_[task_type]++;

            if (current_policy_[task_type] == available_policies_.size()) {
                RecordProbability::policy_t prob;
                if (test_length_ > 0) {
                    // all policies have been exhausted, so training period is over
                    in_training_[task_type] = false;
                    prob = records_.at(task_type).get_best();
                    std::cout << "TEST tt=" << task_type << " prob=" << prob << std::endl;
                    current_policy_[task_type] = std::find(available_policies_.begin(), available_policies_.end(), prob)
                            - available_policies_.begin();
                } else {
                    // we do not do test runs, so we switch back to the first policy
                    current_policy_[task_type] = 0;
                    prob = available_policies_[0];
                }
                training_periods_[task_type]++;
                // so we switch to the new policy
                set_policy(task_type, prob);
                if (filter_policies_) {
                    filter_policies(task_type);
                }
            } else {
                // we set the next policy for training
                set_policy(task_type, available_policies_[current_policy_[task_type]]);
            }
        }
    } else {
        // we were in test period, so we increase the test_period counter
        instrument_event(cpu_id + 1,
                utils::instrumentation::SIMULATOR_EVENTS::POLICY_TRAINER_RETRAIN_COUNTER + task_type,
                retrain_counter_.at(task_type).get_value());
        retrain_counter_.at(task_type).increment();
        if (retrain_counter_.at(task_type).test_flag()) {
            // if the test period is over, we switch to training from the beginning of list of available policies
            retrain_counter_.at(task_type).reset_flag();
            current_policy_[task_type] = 0;
            in_training_[task_type] = true;
            test_periods_[task_type]++;
            // set new policy here
            set_policy(task_type, available_policies_[current_policy_[task_type]]);
        }
    }
    // std::cout << " NEW ";
//        test_periods_[task_type]++;
//        if (not in_training_[task_type]) {
//            cache_->get_simulator().get_instrumentation().add_event(utils::instrumentation::InstrumentationItem {
//                cpu_id,
//                0,
//                cache_->get_simulator()->get_clock(),
//                utils::instrumentation::EVENT_RECORD,
//                utils::instrumentation::Event {4300001, task_type}});
//
//            RecordProbability::policy_t prob = records_[task_type].get_best();
//            set_policy(task_type, prob);
//
//        }

    instrument_event(cpu_id + 1, 43000071, task_type);
}


template<typename CacheSet, template<typename> class Cache>
inline
PolicyTrainer<CacheSet, Cache> *PolicyTrainer<CacheSet, Cache>::Create(Cache<CacheSet> *cache,
                                                                        const engine::Config &cache_policy_config,
                                                                        const engine::Config &global_config)
{
//    std::cout << "Creating PolicyTrainer" << std::endl;
//    cache_policy_config.print();
    bool enabled = cache_policy_config.is_valid() && cache_policy_config.key_exists("trainer");

    if (enabled) {
        /* check if the simulated system is configured properly */
        const bool fwd_task_start = global_config.get_value_with_default<bool>("forward_task_start_to_cpu", false);
        const bool fwd_task_end = global_config.get_value_with_default<bool>("forward_task_end_to_cpu", false);
        if (not (fwd_task_start and fwd_task_end)) {
            std::cerr << "Error: Policy Trainer requires parameters forward_task_start_to_cpu and "
                    "forward_task_end_to_cpu in the global section to be set to 'true'." << std::endl;
            exit(-1);
        }
        std::cout << "PT CREATED" << std::endl;
        const std::string paraver_sampling_policy = engine::Config(global_config, "Paraver")
                .get_value<std::string>("sampling_policy");
        const engine::Config trainer_conf(engine::Config(global_config,
                                                         cache_policy_config.get_value<std::string>("trainer")));

        std::size_t ncpus = global_config.get_value<std::size_t>("ncpus");
        std::size_t test_length = trainer_conf.get_value<std::size_t>("test_period_length");
        std::size_t train_length_per_policy = trainer_conf.get_value<std::size_t>("training_period_length_per_policy");
        std::size_t history_length = trainer_conf.get_value<std::size_t>("history_length");
        bool filter_policies = (trainer_conf.get_value<std::string>("discard_bad_policies") == "true") ? true : false;
        return new PolicyTrainer<CacheSet, Cache>(cache, ncpus, test_length, train_length_per_policy, history_length,
                filter_policies, paraver_sampling_policy);
    } else {
//        std::cout << "PT NOT CREATED" << std::endl;
        return NULL;
    }
}


template<typename CacheSet, template<typename> class Cache>
unsigned PolicyTrainer<CacheSet, Cache>::get_id() const
{
    return 0;
}

template<typename CacheSet, template<typename> class Cache>
const std::string PolicyTrainer<CacheSet, Cache>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}


template<typename CacheSet, template<typename> class Cache>
void PolicyTrainer<CacheSet, Cache>::set_policy(uint16_t task_type, const RecordProbability::policy_t &policy)
{
    for (auto set_it = cache_->sets_.begin(); set_it != cache_->sets_.end(); ++set_it){
        policy::Policy<CacheSet> *curr_policy = (*set_it).get_policy();
        SRRIP<CacheSet> *dyn_policy = dynamic_cast<SRRIP<CacheSet>*>(curr_policy);
        if (dyn_policy) {
            dyn_policy->set_brrip_probability(task_type, policy);
        } else {
//            std::cout << " dyn_cast FAIL ";
        }
    }
}


}  // namespace dynamic
}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_POLICY_POLICY_TRAINER_IMPL_H_

