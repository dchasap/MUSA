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


#ifndef CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_UTILS_H_
#define CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_UTILS_H_

#include <set>
#include <core/logic/coherence/single/types.h>

namespace sim {
namespace logic {
namespace memory {
namespace policy {
namespace dynamic {


class RecordProbability
{
public:
    typedef double policy_t;
    typedef uint32_t quality_t;
    
    typedef struct item_t {
        typedef std::vector<quality_t> counter_t;
        policy_t policy_;
        counter_t counters_;

        item_t();
        explicit item_t(const policy_t &item);
        item_t(const policy_t &policy, const counter_t &counter);
        
        quality_t total() const;
        void reset_counters();

        struct smaller {
            bool operator()(const item_t &i1, const item_t &i2)
            {
                return i1.total() < i2.total();
            }
        };
    } item_t;

    class ListRecords {
    public:
        ListRecords(std::size_t max_size);
        void add(quality_t quality);
        quality_t get_quality() const;
        void print_records(std::string separator = " ") const;
    private:
        std::list<quality_t> previous_records_;
        std::size_t max_records_;
        quality_t current_total_;
        quality_t get_sum() const;
        quality_t get_average() const;  // maybe this should return double
//        quality_t get_min() const;
//        quality_t get_max() const;
    };

    RecordProbability(std::size_t history_length);
    void sample(const item_t &item);
//    void sample(policy_t, quality_t);
    policy_t get_best() const;
    std::pair<policy_t, quality_t> get_min() const;
    std::pair<policy_t, quality_t> get_max() const;
    const std::unordered_map<policy_t, ListRecords> *get_records() const;
    void print(std::string line_prefix);
    
private:
//    std::multiset<item_t, item_t::smaller> records_;
    std::size_t history_length_;
    std::unordered_map<policy_t, ListRecords> records_;
    
};

class Counter{
private:
    std::size_t count_;
    std::size_t limit_;
    bool reached_limit_;
public:
    explicit Counter(std::size_t limit, std::size_t init_value = 0);
    void set_limit(std::size_t limit);
    bool test_flag() const;
    void reset_flag();
    void increment(std::size_t inc = 1);
    std::size_t get_value() const;
};

}  // namespace dynamic
}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "PolicyTrainerUtils-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_UTILS_H_

