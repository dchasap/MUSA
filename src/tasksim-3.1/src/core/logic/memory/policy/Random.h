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


#ifndef CORE_LOGIC_MEMORY_POLICY_RANDOM_H_
#define CORE_LOGIC_MEMORY_POLICY_RANDOM_H_

#include <string>
#include <vector>
#include <utility>
#include <random>
#include "Policy.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {

template<typename CacheSet>
class Random : public Policy<CacheSet> {
public:
    using Tag = typename Policy<CacheSet>::Tag;
    using CacheLine = typename Policy<CacheSet>::CacheLine;

    Random(CacheSet *set, const Parameters &params);
    ~Random();

    void insert(const Tag &tag, const CacheLine &line);
    void update(const Tag &tag);
    std::pair<Tag, CacheLine> evict();

    std::string get_name() const;
private:
    std::default_random_engine random_generator_;
    std::vector<std::size_t> sequence_;

    typedef struct {
        bool valid_;
        std::size_t index_;
    } evict_candidate_t;

    evict_candidate_t evict_candidate_;

    std::size_t get_evict_index();
};

}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "Random-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_RANDOM_H_

