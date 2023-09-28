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


#ifndef CORE_STATS_TIMEAVERAGE_IMPL_H_
#define CORE_STATS_TIMEAVERAGE_IMPL_H_

#include <string>

namespace sim {
namespace stats {

template<typename T>
inline
TimeAverage::TimeAverage(const T &obj, const std::string &name) :
    Stat(obj, name),
    total_accum_(0),
    local_accum_(0),
    last_polling_cycle_(0),
    last_cycle_(0)
{}

inline
TimeAverage::~TimeAverage()
{
    if (last_cycle_ > 0) {
        Stat::out_ << total_accum_ / last_cycle_;
    } else {
        Stat::out_ << 0;
    }
}

template<typename T>
inline
void TimeAverage::sample(const T &value, sim::engine::cycles_t time)
{
    if (time > last_cycle_) {
        local_accum_ += (value) * (time - last_cycle_);
        total_accum_ += (value) * (time - last_cycle_);
        last_cycle_ = time;
    }
}

inline
double TimeAverage::get_total_average()
{
    if (last_cycle_ > 0) {
        return total_accum_ / last_cycle_;
    } else {
        return 0;
    }
}

inline
double TimeAverage::get_local_average()
{
    if (last_cycle_ > last_polling_cycle_) {
        double ret = local_accum_ / (last_cycle_ - last_polling_cycle_);
        last_polling_cycle_ = last_cycle_;
        local_accum_ = 0;
        return ret;
    } else {
        return 0;
    }
}

inline
void TimeAverage::extrapolate(uint64_t from, uint64_t to)
{

}

}  // namespace stats
}  // namespace sim


#endif  // CORE_STATS_TIMEAVERAGE_IMPL_H_
