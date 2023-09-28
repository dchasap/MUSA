/*
 * Copyright (c) 2015, Barcelona Supercomputing Center
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer; redistributions in binary form
 * must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with
 * the distribution; neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef CORE_STATS_TIMESTATISTICS_IMPL_H_
#define CORE_STATS_TIMESTATISTICS_IMPL_H_

#include <cassert>

#include "core/engine/types.h"
#include "core/utils/Log.h"

using sim::utils::Log;

namespace sim {
namespace stats {

template<typename Keeper>
inline
TimeStatistics<Keeper>::TimeStatistics()
{
}

template<typename Keeper>
inline
void TimeStatistics<Keeper>::phaseIn(sim::engine::CodeRegions region)
{
    return static_cast<Keeper*>(this)->phaseIn(region);
}

template<typename Keeper>
inline
void TimeStatistics<Keeper>::phaseOut(sim::engine::CodeRegions region)
{
    return static_cast<Keeper*>(this)->phaseOut(region);
}

inline
unsigned TimeKeeper::get_id() const
{
    return 0;
}

inline
const std::string TimeKeeper::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

inline
TimeKeeper::TimeKeeper() :
        region_cycles_(*this, "CyclesSpentInRegion", sim::engine::region_names),
        region_count_(*this, "NumberOfPhasesInRegion", sim::engine::region_names),
        region_stack_()
{
}

inline
void TimeKeeper::phaseIn(sim::engine::CodeRegions region)
{
    ticks current_time = getticks();
    region_stack_.push({region, current_time, 0});
}

inline
void TimeKeeper::phaseOut(sim::engine::CodeRegions region)
{
    assert(region_stack_.empty() == false);
    if (region != region_stack_.top().region_id_) {
        Log::error() << "Error: exiting region " << region << " (" << sim::engine::region_names.at(region)
                     << ")  while inside region " << region_stack_.top().region_id_ << " ("
                     << sim::engine::region_names.at(region_stack_.top().region_id_) << ").";
        std::abort();
    }

    ticks current_time = getticks();
    ticks initial_time = region_stack_.top().region_start_;
    ticks duration = current_time - initial_time - region_stack_.top().nested_duration_;
    region_cycles_[region] += duration;
    region_count_[region]++;
    region_stack_.pop();
    if (region_stack_.empty() == false) {
        region_stack_.top().nested_duration_ += current_time - initial_time;
    }
}

}  // namespace stats
}  // namespace sim

#endif  // CORE_STATS_TIMESTATISTICS_IMPL_H_
