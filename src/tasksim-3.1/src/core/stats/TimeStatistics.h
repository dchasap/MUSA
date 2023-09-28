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


#ifndef CORE_STATS_TIMESTATISTICS_H_
#define CORE_STATS_TIMESTATISTICS_H_

#include <string>
#include <stack>

#include "core/engine/types.h"
#include "core/stats/cycle.h"
#include "core/stats/CounterMap.h"
#include "core/stats/FalseStat.h"

namespace sim {
namespace stats {

template<typename Keeper>
class TimeStatistics {
protected:
    TimeStatistics();
public:
    void phaseIn(sim::engine::CodeRegions region);
    void phaseOut(sim::engine::CodeRegions region);
};

class NoKeeper : public TimeStatistics<NoKeeper> {
public:
    NoKeeper() {}
    void phaseIn(sim::engine::CodeRegions region) { }
    void phaseOut(sim::engine::CodeRegions region) { }
};

class TimeKeeper : public TimeStatistics<TimeKeeper> {
protected:
    /** Stored statistics*/
    CounterMap<unsigned> region_cycles_;
    CounterMap<unsigned> region_count_;
    /** Struct that defines a currently running CodeRegion, has its id,
     *  start time and duration of all nested CodeRegions.
     * */
    typedef struct {
        sim::engine::CodeRegions region_id_;
        ticks region_start_;
        ticks nested_duration_;
    } PhaseElement;
    /** Stack that will store the regions that we are currently monitoring.*/
    std::stack<PhaseElement> region_stack_;

public:
    TimeKeeper();
    void phaseIn(sim::engine::CodeRegions region);
    void phaseOut(sim::engine::CodeRegions region);
    /** Necessary for CounterMap */
    unsigned get_id() const;
    const std::string get_name() const;
};

}  // namespace stats
}  // namespace sim

#include "TimeStatistics-impl.h"

namespace sim {
namespace stats {

#ifdef ENABLE_TIME_STATS
typedef TimeKeeper TimeStats;
#else  // ENABLE_TIME_STATS
typedef NoKeeper TimeStats;
#endif  // ENABLE_TIME_STATS
extern TimeStats Time_Stats;

}  // namespace stats
}  // namespace sim

#endif  // CORE_STATS_TIMESTATISTICS_H_
