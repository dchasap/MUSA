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


#ifndef CORE_LOGIC_MEMORY_POLICY_POLICY_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_POLICY_IMPL_H_

#include <string>
#include "LRU.h"
#include "NRU.h"
#include "TDAKE.h"
#include "TreePLRU.h"
#include "Random.h"
#include "SRRIP.h"
#include "core/runtime/TaskDataDep.h"

namespace sim {
namespace logic {
namespace memory {
namespace policy {


template<typename CacheSet>
PolicyConfigRegister Policy<CacheSet>::policy_config_register_ = PolicyConfigRegister();


template<typename CacheSet>
inline
Policy<CacheSet>::Policy(CacheSet *set, const Parameters &params) : set_(set), params_(params),
    storage_(this->params_.size_)
{
    assert(set != nullptr);
}


template<typename CacheSet>
inline
Policy<CacheSet>::~Policy()
{
}

/* VD: When implementing a new policy, add code for its creation to this function */
template<typename CacheSet>
inline
typename Policy<CacheSet>::policy_ptr_t Policy<CacheSet>::Create(CacheSet * set, unsigned associativity, const engine::Config &config)
{
    const std::string type = config.get_value<std::string>("type");

    if (type == "LRU") {
        return policy_ptr_t(new LRU<CacheSet>(set, Parameters(associativity)));
    } else if (type == "NRU") {
        return policy_ptr_t(new NRU<CacheSet>(set, Parameters(associativity)));
    } else if (type == "TreePLRU") {
        return policy_ptr_t(new TreePLRU<CacheSet>(set, Parameters(associativity)));
    } else if (type == "Random") {
        return policy_ptr_t(new Random<CacheSet>(set, Parameters(associativity)));
    } else if (type == "SRRIP") {
        return policy_ptr_t(new SRRIP<CacheSet>(set,
            SRRIP_Parameters(associativity,
                             config.get_value<unsigned int>("rrpv_width"),
                             string_to_hpp.at(config.get_value<std::string>("hit_promotion_policy")),
                             SRRIP_Parameters::init_IPs(config),
                             SRRIP_Parameters::init_brrip_probs(config),
                             config.key_exists("rnd_seed"),
                             config.key_exists("rnd_seed") ? config.get_value<float>("rnd_seed") : 0)));
    } else if (type == "TDAKE") {
        return policy_ptr_t(new TDAKE<CacheSet>(set,
                                       TDAKE_Parameters(associativity,
                                                        runtime::string_to_task_dep.at(
                                                            config.get_value<std::string>("keep_type")),
                                                        runtime::string_to_task_dep.at(
                                                            config.get_value<std::string>("evict_type")),
                                                        runtime::string_to_task_dep.at(
                                                            config.get_value<std::string>("insert_on_LRU")))));
    } else {
        std::cerr << "Unknown cache replacement policy type " << type << std::endl;
        exit(1);
        return policy_ptr_t(nullptr);
    }
}


template<typename CacheSet>
inline
std::pair<typename Policy<CacheSet>::Tag, typename Policy<CacheSet>::CacheLine> Policy<CacheSet>::evict()
{
    std::size_t evict_index = get_evict_index();
    const Tag evicted_tag = this->storage_.get_tag(evict_index);
    const auto retval = this->set_->find(evicted_tag);
    assert(retval != this->set_->get_contents().end());
    assert(retval->first == evicted_tag);
    remove(evicted_tag);
    return *retval;
}


template<typename CacheSet>
inline
void Policy<CacheSet>::remove(const Tag &tag)
{
    storage_.remove(storage_.find(tag));
}


template<typename CacheSet>
inline
const typename std::pair<typename Policy<CacheSet>::Tag, typename Policy<CacheSet>::CacheLine> *
Policy<CacheSet>::inspect_evict()
{
    std::size_t evict_index = get_evict_index();
    return &(*set_->find(storage_.container_[evict_index].first));
}


template<typename CacheSet>
inline
void Policy<CacheSet>::initialize_from_other_policy(const Policy *other_policy)
{
    storage_ = other_policy->storage_;
    storage_.refresh_map();
}


template<typename CacheSet>
inline
void Policy<CacheSet>::populate_cache_config_runtime_info(engine::Config &cfg,
                                                        const engine::Config &global_conf,
                                                        std::size_t size)
{
    if (cfg.key_exists("inject-per-benchmark-config") and
        (cfg.get_value<std::string>("inject-per-benchmark-config") == "true")) {
        if (not global_conf.is_valid()) {
            std::cerr << "Global configuration is not valid" << std::endl;
            return;
        }
        if (cfg.key_exists("type") and cfg.get_value<std::string>("type") != "SRRIP") {
            return;
        }
        if (not global_conf.key_exists("benchmark")) {
            std::cerr << "Benchmark is not specified in the configuration file." << std::endl;
            return;
        }
        std::string benchmark = global_conf.get_value<std::string>("benchmark");
        const engine::Config conf(engine::Config(global_conf, "SRRIP_" + benchmark));
        if (not conf.is_valid()) {
            return;
        }

        static const std::vector<std::string> keys({"IN_prob", "OUT_prob", "INOUT_prob", "NON_IN_OUT_prob"});
        for (auto key : keys) {
            std::vector<float> probs = conf.get_values<float>(std::to_string(size) + "_" + key);
            for (auto prob : probs) {
                cfg.add_value<float>(prob, key);
            }
        }
    }
}


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_MEMORY_POLICY_POLICY_IMPL_H_
