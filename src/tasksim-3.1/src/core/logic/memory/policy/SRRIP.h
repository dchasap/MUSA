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


#ifndef CORE_LOGIC_MEMORY_POLICY_SRRIP_H_
#define CORE_LOGIC_MEMORY_POLICY_SRRIP_H_

#include <string>
#include <utility>
#include <vector>
#include "Policy.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {

typedef uint8_t rrpv_width_t;
typedef uint32_t rrpv_t;


struct ReReferenceInterval {
    rrpv_t distant_;
    rrpv_t long_;
    rrpv_t immediate_;
};


typedef enum class hit_promotion_policy_t : uint8_t {
    HIT_PRIORITY, FREQUENCY_PRIORITY
} hit_promotion_policy_t;


static std::unordered_map<std::string, hit_promotion_policy_t> string_to_hpp({
    {"HIT_PRIORITY",       hit_promotion_policy_t::HIT_PRIORITY},
    {"FREQUENCY_PRIORITY", hit_promotion_policy_t::FREQUENCY_PRIORITY}
});


typedef enum class insert_policy_t : uint8_t {
    DISTANT, LONG, IMMEDIATE, BIMODAL
} insert_policy_t;


static std::unordered_map<std::string, insert_policy_t> string_to_ip({
    {"DISTANT",   insert_policy_t::DISTANT},
    {"LONG",      insert_policy_t::LONG},
    {"IMMEDIATE", insert_policy_t::IMMEDIATE},
    {"BIMODAL",   insert_policy_t::BIMODAL},
    {"D",         insert_policy_t::DISTANT},
    {"L",         insert_policy_t::LONG},
    {"I",         insert_policy_t::IMMEDIATE},
    {"B",         insert_policy_t::BIMODAL}
});

typedef sim::runtime::task_dep_t task_dep_t;

typedef std::unordered_map<task_dep_t, std::vector<insert_policy_t>, std::hash<unsigned> > insert_policies_t;
typedef std::unordered_map<task_dep_t, std::vector<float>, std::hash<unsigned> > brrip_probabilites_t;

class SRRIP_Parameters : public sim::logic::memory::policy::Parameters
{
public:
    rrpv_width_t rrpv_width_;
    hit_promotion_policy_t hit_promotion_policy_;
//    insert_policy_t insert_policy_;
//    float brrip_long_probability_;
    insert_policies_t insert_policies_;
    brrip_probabilites_t brrip_probabilites_;
    bool use_seed_;
    float seed_;

    SRRIP_Parameters(std::size_t size, rrpv_width_t rrpv_width, hit_promotion_policy_t hpp, insert_policies_t dtip,
            brrip_probabilites_t brrip_probs, bool use_seed, float seed);
    insert_policies_t static init_IPs(const engine::Config &conf);
    brrip_probabilites_t static init_brrip_probs(const engine::Config &conf);
};


template<typename CacheSet>
class SRRIP : public Policy<CacheSet> {
public:
    using Tag = typename Policy<CacheSet>::Tag;
    using CacheLine = typename Policy<CacheSet>::CacheLine;

    SRRIP(CacheSet *set, const SRRIP_Parameters &params);
    ~SRRIP();

    void insert(const Tag &tag, const CacheLine &line);
    void update(const Tag &tag);

    std::string get_name() const;
    void set_brrip_probability(uint16_t task_type, float prob);
private:
    /* Number of bits for Re-Reference Prediction Values (RRPV) */
    const rrpv_width_t rrpv_width_;

    /* constants defining positions */
    const struct ReReferenceInterval kRRI_;

    std::vector<rrpv_t> rrpv_vector_;

    const hit_promotion_policy_t hit_promotion_policy_;
//    const insert_policy_t insert_policy_;
    static std::default_random_engine random_generator_;
    static bool seed_set_;
    static std::uniform_real_distribution<float> random_distribution_;
//    const float brrip_long_probability_;

    /* Runtime awareness */
    insert_policies_t insert_policies_;
    brrip_probabilites_t brrip_probabilites_;

    std::size_t get_evict_index();
};

}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "SRRIP-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_SRRIP_H_

