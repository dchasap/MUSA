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


#ifndef CORE_LOGIC_MEMORY_POLICY_POLICY_H_
#define CORE_LOGIC_MEMORY_POLICY_POLICY_H_

#include <string>
#include <memory>
#include <utility>
#include "core/utils/NonCopyable.h"
#include "core/engine/Config.h"
#include "types.h"

namespace sim {
namespace logic {
namespace memory {
namespace policy {

class PolicyConfigRegister {
private:
    std::unordered_map<std::string, engine::Config> register_;
public:
    bool add(const std::string, const engine::Config &);
    engine::Config get(const std::string) const;
};


template<typename CacheSet>
class Policy : sim::utils::NonCopyable {
public:
    typedef std::default_delete<Policy> deleter;
    typedef std::unique_ptr<Policy, deleter> policy_ptr_t;

    typedef typename CacheSet::tag_t Tag;
    typedef typename CacheSet::line_t CacheLine;

    Policy(CacheSet *set, const Parameters &params);
    Policy(Policy &&policy) = default;
    virtual ~Policy();

    /* Perform changes on the policy's internal state */
    virtual void insert(const Tag &tag, const CacheLine &line) = 0;
    virtual void update(const Tag &tag) = 0;
    virtual std::pair<Tag, CacheLine> evict();

    // In policies where randomness affects which line is selected for eviction:
    // - calling this function several times, between two calls to evict(), must result in the same line.
    // - calling evict after this function must evict the same line as this function suggested.
    virtual const std::pair<Tag, CacheLine> *inspect_evict();

    // Each derived policy that overrides this function must call the function of the base class
    virtual void remove(const Tag &tag);

    virtual std::string get_name() const = 0;

    static policy_ptr_t Create(CacheSet *set, unsigned associativity, const engine::Config &conf);

    static PolicyConfigRegister policy_config_register_;

    virtual void initialize_from_other_policy(const Policy *policy);

    static void populate_cache_config_runtime_info(engine::Config &cfg, const engine::Config &global_conf, std::size_t size);

    CacheSet *set_;

protected:
    Parameters params_;
    Storage<Tag, CacheSet> storage_;

    // Each derived policy has to implement the function to select the evict candidate and return it's position (index)
    virtual std::size_t get_evict_index() = 0;
};


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "Policy-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_POLICY_H_
