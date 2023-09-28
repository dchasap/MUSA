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


#ifndef CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_UTILS_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_UTILS_IMPL_H_

#include <queue>
#include <core/logic/coherence/single/types.h>

#include "PolicyTrainerUtils.h"

namespace sim {
namespace logic {
namespace memory {
namespace policy {
namespace dynamic {

inline
RecordProbability::ListRecords::ListRecords(std::size_t max_size) : max_records_(max_size), current_total_(0)
{
}

inline
void RecordProbability::ListRecords::add(quality_t q)
{
    if (previous_records_.size() == max_records_) {
        current_total_ -= previous_records_.front();
        assert(current_total_ >= 0);
        previous_records_.pop_front();
    }
    previous_records_.push_back(q);
    current_total_ += q;
}


inline
RecordProbability::quality_t RecordProbability::ListRecords::get_quality() const
{
    return get_average();
}


inline
void RecordProbability::ListRecords::print_records(std::string separator) const
{
    auto it = previous_records_.begin();
    std::cout << (*it);
    ++it;
    for (; it != previous_records_.end(); ++it) {
        std::cout << separator << (*it);
    }
}


inline
RecordProbability::quality_t RecordProbability::ListRecords::get_sum() const
{
    return current_total_;
}


inline
RecordProbability::quality_t RecordProbability::ListRecords::get_average() const
{
    if (previous_records_.size() == 0) {
        return 0;
    } else {
        return get_sum() / previous_records_.size();
    }
}


inline
RecordProbability::RecordProbability(std::size_t history_length) : history_length_(history_length)
{
}

inline
void RecordProbability::sample(const item_t& item)
{
//    records_.insert(item);
    if (records_.find(item.policy_) == records_.end()) {
        records_.insert(std::pair<policy_t, ListRecords>(item.policy_, ListRecords(history_length_)));
    }
    records_.at(item.policy_).add(item.total());
}


inline
RecordProbability::policy_t RecordProbability::get_best() const
{
//    return records_.begin()->policy_;
    return get_min().first;
}


inline
std::pair<RecordProbability::policy_t, RecordProbability::quality_t> RecordProbability::get_min() const
{
    std::pair<policy_t, quality_t> min(records_.begin()->first, records_.begin()->second.get_quality());
    for (auto record : records_) {
        if (record.second.get_quality() < min.second) {
//            min = record;
            min.first = record.first;
            min.second = record.second.get_quality();
        }
    }
    return min;
}


inline
std::pair<RecordProbability::policy_t, RecordProbability::quality_t> RecordProbability::get_max() const
{
    std::pair<policy_t, quality_t> max(records_.begin()->first, records_.begin()->second.get_quality());
    for (auto record : records_) {
        if (record.second.get_quality() > max.second) {
            max.first = record.first;
            max.second = record.second.get_quality();
        }
    }
    return max;
}


inline
const std::unordered_map<RecordProbability::policy_t, RecordProbability::ListRecords>
    *RecordProbability::get_records() const
{
    return &records_;
}


inline
void RecordProbability::print(std::string line_prefix)
{
    auto it = records_.begin();
    std::cout << (*it).first << ":";
    (*it).second.print_records();
    ++it;
    for (; it != records_.end(); ++it) {
        std::cout << ":" << (*it).first << ":";
        (*it).second.print_records();
    }
}

inline
RecordProbability::item_t::item_t() :
    policy_(0), counters_(4, 0)
{
}

inline
RecordProbability::item_t::item_t(const policy_t &policy) :
    policy_(policy), counters_(4, 0)
{
}


inline
RecordProbability::item_t::item_t(const policy_t &policy, const counter_t &counters) :
    policy_(policy), counters_(counters)
{
}


inline
RecordProbability::quality_t RecordProbability::item_t::total() const
{
    quality_t sum = 0;
    for (auto it = counters_.begin(); it != counters_.end(); ++it) {
        sum += *it;
    }
    return sum;
}


inline
void RecordProbability::item_t::reset_counters()
{
    for (auto it = counters_.begin(); it != counters_.end(); ++it) {
        *it = 0;
    }
}


inline
Counter::Counter(std::size_t limit, std::size_t initial_value) :
    count_(initial_value), limit_(limit), reached_limit_(false)
{
}


inline
void Counter::set_limit(std::size_t limit)
{
    assert(count_ == 0);
    assert(reached_limit_ == false);
    limit_ = limit;
}


inline
bool Counter::test_flag() const
{
    return reached_limit_;
}


inline
void Counter::reset_flag()
{
    reached_limit_ = false;
}


inline
void Counter::increment(std::size_t inc)
{
    count_ += inc;
    if (count_ >= limit_) {
        assert(limit_ > 0);
        count_ = count_ % limit_;
        reached_limit_ = true;
    }
}


inline
std::size_t Counter::get_value() const
{
    return count_;
}


}  // namespace dynamic
}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_POLICY_DYNAMIC_POLICY_TRAINER_UTILS_IMPL_H_

