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


#ifndef CORE_LOGIC_MEMORY_POLICY_TDAKE_H_
#define CORE_LOGIC_MEMORY_POLICY_TDAKE_H_

#include <string>
#include <utility>
#include <vector>
#include "Policy.h"
#include "core/runtime/TaskDataDep.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {

typedef sim::runtime::task_dep_t task_dep_t;

class TDAKE_Parameters : public Parameters {
public:
    task_dep_t keep_type_;
    task_dep_t evict_type_;
    task_dep_t insert_on_LRU_;

    TDAKE_Parameters(std::size_t size, task_dep_t keep_type, task_dep_t evict_type, task_dep_t insert_on_LRU);
};

template<typename CacheSet>
class TDAKE : public LRU<CacheSet> {
public:
    using Tag = typename Policy<CacheSet>::Tag;
    using CacheLine = typename Policy<CacheSet>::CacheLine;

    TDAKE(CacheSet *set, const TDAKE_Parameters &params);
    ~TDAKE();

    void insert(const Tag &tag, const CacheLine &line);
//    void update(const Tag &);

    std::string get_name() const;

//    void initialize_from_other_policy(const Policy<CacheSet> *);
private:
    /* Which types of dependency are kept or evicted */
    task_dep_t keep_type_;
    task_dep_t evict_type_;
    task_dep_t insert_on_LRU_;

    std::size_t get_evict_index();
};

}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "TDAKE-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_TDAKE_H_
