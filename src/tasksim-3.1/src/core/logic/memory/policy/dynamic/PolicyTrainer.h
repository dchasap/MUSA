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


#ifndef CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_H_
#define CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_H_

#include <unordered_map>
#include <vector>
#include <core/logic/memory/policy/Policy.h>
#include <core/logic/memory/policy/SRRIP.h>
#include <core/logic/memory/policy/dynamic/PolicyTrainerUtils.h>
#include <core/logic/coherence/single/types.h>
#include <core/stats/CounterMap.h>

namespace sim {
namespace logic {
namespace memory {
namespace policy {
namespace dynamic {


template<typename CacheSet, template<typename> class Cache>
class PolicyTrainer
{
private:
    typedef enum {WAITING_FOR_TASK_TO_START, TRAINING, TESTING} state_t;
    const static uint16_t MAX_TASK_TYPES = 100;

    // whether default policies were set in the cache
    bool policies_in_cache_initialized_;
    // whether the structures for i-th task are initialized
    std::vector<bool> initialized_;
    // pointer to the cache, so the policies can be changed from this class
    Cache<CacheSet> *cache_;
    // parameters
    std::size_t test_length_;
    std::size_t train_length_per_policy_;
    std::size_t history_length_;
    bool filter_policies_;

    // state of the trainer per cpu
    std::vector<state_t> states_;
    // maps cpu_id to current task type
    std::vector<uint16_t> current_task_types_;
    // maps cpu_id to the number of misses
    std::vector<RecordProbability::item_t> current_record_;
    // tracks performance of different policies per task_type
    std::unordered_map<uint16_t, RecordProbability> records_;
    // available policies (TODO: move this to better place)
    const std::vector<RecordProbability::policy_t> available_policies_;
    // policies used for training, per task (subset of avalable_policies_)
    std::unordered_map<uint16_t, std::vector<RecordProbability::policy_t>> used_policies_;
    // indicates the position of active policy in available_policies_ (per task type)
    std::unordered_map<uint16_t, std::size_t> current_policy_;
    // Signals when next policy from available_policies_ should be used (per task type)
    std::unordered_map<uint16_t, Counter> current_policy_counter_;
    // Signals when new retrain process should be done (per task type)
    std::unordered_map<uint16_t, Counter> retrain_counter_;
    // Whether the training process is active (per task type)
    std::unordered_map<uint16_t, bool> in_training_;

    stats::CounterMap<uint16_t> test_periods_;
    stats::CounterMap<uint16_t> training_periods_;
    stats::CounterMap<uint16_t> task_instances_per_task_type_;
    stats::CounterMap<uint16_t> best_policy_per_task_type_;

    std::vector<uint16_t> last_task_type_per_core_;
    std::vector<uint16_t> same_task_instances_per_core_;

    // Functions and members below are used for debugging purposes
    std::string paraver_sampling_policy_;
    void instrument_event(unsigned int cpu, unsigned int type, unsigned int value);
    void filter_policies(std::size_t task_type);

public:
    PolicyTrainer(Cache<CacheSet> *, uint16_t, uint32_t, uint32_t, uint32_t, bool filter_policies, const std::string &);
    ~PolicyTrainer();
    void init_policies();
    void init(uint16_t task_type);
    void sample(uint16_t cpu_id, uint32_t task_type, runtime::task_dep_t dep_type,
            coherence::single::cache_access_outcome_t outcome);
    void signal_task_start(uint16_t cpu_id, uint32_t task_type);
    void signal_task_end(uint16_t cpu_id, uint32_t task_type);
//    int get_best(uint32_t task_type, coherence::single::task_dep_t dep_type);
    static PolicyTrainer *Create(Cache<CacheSet> *cache, const engine::Config &cache_policy, const engine::Config &global_config);

    unsigned get_id() const;
    const std::string get_name() const;
private:
    void set_policy(uint16_t task_type, const RecordProbability::policy_t & policy);
};


}  // namespace dynamic
}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim



#include "PolicyTrainer-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_H_

