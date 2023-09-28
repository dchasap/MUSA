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

#ifndef CORE_LOGIC_MEMORY_POLICY_SRRIP_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_SRRIP_IMPL_H_

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>

#include "types.h"
#include "core/engine/Config.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {

inline
SRRIP_Parameters::SRRIP_Parameters(std::size_t size, rrpv_width_t rrpv_width, hit_promotion_policy_t hpp,
                                   insert_policies_t dtip, brrip_probabilites_t brrip_probs, bool use_seed, float seed) :
    Parameters(size),
    rrpv_width_(rrpv_width),
    hit_promotion_policy_(hpp),
    insert_policies_(dtip),
    brrip_probabilites_(brrip_probs),
    use_seed_(use_seed),
    seed_(seed)
{
}


inline
insert_policies_t SRRIP_Parameters::init_IPs(const engine::Config &conf)
{
    auto default_ip = string_to_ip.at(conf.get_value<std::string>("insert_policy"));
    insert_policies_t ips({{task_dep_t::IN,         std::vector<insert_policy_t>(1, default_ip)},
                           {task_dep_t::OUT,        std::vector<insert_policy_t>(1, default_ip)},
                           {task_dep_t::INOUT,      std::vector<insert_policy_t>(1, default_ip)},
                           {task_dep_t::NON_IN_OUT, std::vector<insert_policy_t>(1, default_ip)}});

    for (auto dep_type : ips) {
        std::string param_name = runtime::task_dep_to_string.at(dep_type.first) + "_ip";
        if (conf.key_exists(param_name)) {
            std::vector<std::string> dep_ips = conf.get_values<std::string>(param_name);
            for (auto ip_str : dep_ips) {
                ips.at(dep_type.first).push_back(string_to_ip.at(ip_str));
            }
        }
    }
    return ips;
}


inline brrip_probabilites_t SRRIP_Parameters::init_brrip_probs(const engine::Config &conf)
{
    auto default_prob = conf.key_exists("brrip_long_probability") ? conf.get_value<float>("brrip_long_probability") : 1;
    brrip_probabilites_t probs({{task_dep_t::IN,         std::vector<float>(1, default_prob)},
                                {task_dep_t::OUT,        std::vector<float>(1, default_prob)},
                                {task_dep_t::INOUT,      std::vector<float>(1, default_prob)},
                                {task_dep_t::NON_IN_OUT, std::vector<float>(1, default_prob)}});

    for (auto dep_type : probs) {
        std::string param_name = runtime::task_dep_to_string.at(dep_type.first) + "_prob";
        if (conf.key_exists(param_name)) {
            std::vector<float> dep_probs = conf.get_values<float>(param_name);
            probs.at(dep_type.first).insert(probs.at(dep_type.first).end(), dep_probs.begin(), dep_probs.end());
        }
    }
    return probs;
}


template<typename CacheSet>
std::default_random_engine SRRIP<CacheSet>::random_generator_ = std::default_random_engine(std::random_device()());

template<typename CacheSet>
bool SRRIP<CacheSet>::seed_set_ = false;

template<typename CacheSet>
std::uniform_real_distribution<float> SRRIP<CacheSet>::random_distribution_ = std::uniform_real_distribution<float>(0, 1);


template<typename CacheSet>
inline
SRRIP<CacheSet>::SRRIP(CacheSet *set, const SRRIP_Parameters &params) : Policy<CacheSet>(set, params),
    rrpv_width_(params.rrpv_width_),
    kRRI_({static_cast<rrpv_t>(std::pow(2, rrpv_width_) - 1),
           static_cast<rrpv_t>(std::pow(2, rrpv_width_) - 2),
           0}),
    rrpv_vector_(this->params_.size_, kRRI_.distant_),
    hit_promotion_policy_(params.hit_promotion_policy_),
//    random_generator_(std::random_device()()),
//    random_distribution_(0, 1),
    insert_policies_(params.insert_policies_),
    brrip_probabilites_(params.brrip_probabilites_)
{
    if (rrpv_width_ < 1 or rrpv_width_ > 8 * sizeof(rrpv_t)) {
        std::cerr << "[ERROR] SRRIP RRPV width is set to " << static_cast<unsigned int>(rrpv_width_) << " bits. "
                  << "It has to be in the interval [1, " << 8 * sizeof(rrpv_t) << "] bits." << std::endl;
        exit(1);
    }
    if (not seed_set_ and params.use_seed_) {
        random_generator_.seed(params.seed_);
        seed_set_ = true;
    }
    static bool printed = false;
    if (not printed) {
        for (auto dt = brrip_probabilites_.begin(); dt != brrip_probabilites_.end(); ++dt) {
            std::cout << (*dt).first;
            for (auto pit = (*dt).second.begin(); pit != (*dt).second.end(); ++ pit) {
                std::cout << "\t" << *pit;
            }
            std::cout << std::endl;
        }
    }
    printed = true;
}


template<typename CacheSet>
inline
SRRIP<CacheSet>::~SRRIP()
{
}


template<typename CacheSet>
inline
void SRRIP<CacheSet>::insert(const Tag &tag, const CacheLine &line)
{
    const auto success_and_iterator = this->set_->insert_and_get_iterator(tag, line);
    assert(success_and_iterator.first == true);

    const std::size_t free_index = this->storage_.insert(tag, success_and_iterator.second);

    rrpv_t rrpv_value;
    insert_policy_t insert_policy;
    if (insert_policies_.at(line.get_dep_type()).size() <= 2) {
        insert_policy = insert_policies_.at(line.get_dep_type())[insert_policies_.at(line.get_dep_type()).size() - 1];
    } else {
        if (insert_policies_.at(line.get_dep_type()).size() <= line.task_type_) {
            std::cerr << "[RRIP] Insertion policy not defined for task type " << line.task_type_
                    << " and dependency type "
                    << sim::runtime::task_dep_to_string.at(line.get_dep_type())
                    << "." << std::endl;
            exit(1);
        }
        insert_policy = insert_policies_.at(line.get_dep_type())[line.task_type_];
    }
    switch (insert_policy) {
        case insert_policy_t::DISTANT:
            rrpv_value = kRRI_.distant_;
            break;
        case insert_policy_t::LONG:
            rrpv_value = kRRI_.long_;
            break;
        case insert_policy_t::IMMEDIATE:
            rrpv_value = kRRI_.immediate_;
            break;
        case insert_policy_t::BIMODAL:
        {
            float brrip_long_probability;
            if (brrip_probabilites_.at(line.get_dep_type()).size() <= 2) {
                brrip_long_probability = brrip_probabilites_.at(line.get_dep_type())[
                        brrip_probabilites_.at(line.get_dep_type()).size() - 1];
            } else {
                if (brrip_probabilites_.at(line.get_dep_type()).size() <= line.task_type_) {
                   std::cerr << "[RRIP] BRRIP long probability not defined for task " << line.task_type_
                           << " and dependency type "
                           << sim::runtime::task_dep_to_string.at(line.get_dep_type())
                           << "." << std::endl;
                   exit(1);
                }
                brrip_long_probability = brrip_probabilites_.at(line.get_dep_type())[line.task_type_];
            }
            if (random_distribution_(random_generator_) < brrip_long_probability) {
                rrpv_value = kRRI_.long_;
            } else {
                rrpv_value = kRRI_.distant_;
            }
            break;
        }
        default:
            std::cerr << "Not supported insert_policy_t for " << get_name() << " " << (std::size_t)insert_policy
                    << std::endl;
            exit(1);
    }
    rrpv_vector_[free_index] = rrpv_value;
}


template<typename CacheSet>
inline
void SRRIP<CacheSet>::update(const Tag &tag)
{
    const size_t index = this->storage_.find(tag);
    if (hit_promotion_policy_ == hit_promotion_policy_t::HIT_PRIORITY) {
        rrpv_vector_[index] = kRRI_.immediate_;
    } else if (hit_promotion_policy_ == hit_promotion_policy_t::FREQUENCY_PRIORITY) {
        rrpv_vector_[index] = (rrpv_vector_[index] == kRRI_.immediate_ ?
                               kRRI_.immediate_:
                               rrpv_vector_[index] - 1);
    }
}


template<typename CacheSet>
inline
std::string SRRIP<CacheSet>::get_name() const
{
    return "SRRIP";
}


template<typename CacheSet>
inline
void SRRIP<CacheSet>::set_brrip_probability(uint16_t task_type, float prob)
{
    for (auto it = brrip_probabilites_.begin(); it != brrip_probabilites_.end(); ++it) {
        if ((*it).second.size() <= task_type) {  // resize the vector to accommodate new task
            (*it).second.resize(task_type + 1);
        }
        (*it).second[task_type] = prob;
    }
}


template<typename CacheSet>
inline
std::size_t SRRIP<CacheSet>::get_evict_index()
{
    std::size_t start_index = 0;

    struct candidate_t { bool found_; size_t index_; };
    candidate_t candidate = {false, this->params_.size_};

    size_t index = start_index;
    struct max_t { bool valid_; std::size_t index_; rrpv_t val_; };
    max_t max = {false, 0, 0};

    do {
        if (!this->storage_.valid_[index] or (this->storage_.get_cache_line(index).locked())) {
            continue;
        }

        if (rrpv_vector_[index] == kRRI_.distant_) {
            candidate = {true, index};
            break;
        } else {
            if (rrpv_vector_[index] > max.val_ or !max.valid_) {
                max = {true, index, rrpv_vector_[index]};
            }
        }
    } while ((index = (index + 1) % this->params_.size_) != start_index );

    if (!candidate.found_) {
        assert(max.valid_);
        /* Increase all RRPVs */
        assert(kRRI_.distant_ > max.val_);
        const rrpv_t increment = kRRI_.distant_ - max.val_;

        for (auto it = rrpv_vector_.begin(); it != rrpv_vector_.end(); ++it) {
            (*it) = std::min((*it) + increment, kRRI_.distant_);
        }
        /* Now we can form the candidate for eviction */
        candidate = {true, max.index_};
    }

    assert(candidate.found_);
    assert(candidate.index_ != this->params_.size_);
    return candidate.index_;
}


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_POLICY_SRRIP_IMPL_H_
