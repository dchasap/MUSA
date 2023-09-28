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


#ifndef CORE_STATS_AVERAGEMAP_IMPL_H_
#define CORE_STATS_AVERAGEMAP_IMPL_H_

#include <string>

namespace sim {
namespace stats {

inline
AverageMap::Instance::Instance() :
    accum_(0.0f),
    samples_(0)
{}


template<typename T>
inline
void AverageMap::Instance::sample(const T &value)
{
    accum_ += value;
    samples_++;
}

template<typename T>
inline
AverageMap::AverageMap(const T &obj, const std::string &name) :
    Stat(obj, name)
{}

inline
AverageMap::~AverageMap()
{
    bool first = true;
    for( auto &elem : data_) {
        if (not first) {
            Stat::out_ << ":";
        }
        Stat::out_ << elem.first << ":"<< (elem.second.accum_ / elem.second.samples_);
        first = false;
    }
}

template<typename T>
inline
void AverageMap::sample(const T &value)
{
    data_[0].accum_ += value;
    data_[0].samples_++;
}

inline
AverageMap::Instance& AverageMap::operator[](uint64_t index)
{
    return data_[index];
}


inline
double AverageMap::get_value()
{
    uint64_t total = 0.0f;
    uint64_t count = 0;
    for( auto &elem : data_) {
        total += elem.second.accum_;
        count += elem.second.samples_;
    }
    if (count > 0) {
        return total / count;
    } else {
        return 0;
    }
}

inline
void AverageMap::extrapolate(uint64_t from, uint64_t to)
{

}

}  // namespace stats
}  // namespace sim


#endif  // CORE_STATS_AVERAGEMAP_IMPL_H_
