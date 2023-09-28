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


#ifndef CORE_STATS_HISTOGRAM_IMPL_H_
#define CORE_STATS_HISTOGRAM_IMPL_H_

#include <string>

namespace sim {
namespace stats {

template<typename T>
inline
Histogram::Histogram(const T &obj, const std::string &name, double lower_bound, double upper_bound, unsigned num_bins) :
        Stat(obj, name),
        histogram_(num_bins),
        underflows_(0),
        overflows_(0),
        lower_bound_(lower_bound),
        upper_bound_(upper_bound),
        num_bins_(num_bins)
{
}

inline
Histogram::~Histogram()
{
    Stat::out_ << underflows_ << ":underflows" ":" << name_ << ":" << class_ << ":" << id_ << std::endl;

    double bin_size = (upper_bound_ - lower_bound_) / num_bins_;
    for (unsigned i = 0; i < histogram_.size(); ++i) {
        Stat::out_ << histogram_[i] << ":" << lower_bound_ + i * bin_size << " to "
                   << lower_bound_ + (i+1) * bin_size << ":" << name_ << ":" << class_ << ":" << id_ << std::endl;
    }

    Stat::out_ << overflows_ << ":overflows" ":" << name_ << ":" << class_ << ":" << id_ << std::endl;
    this->print_ = false;
}

template<typename T>
inline
void Histogram::sample(const T &value)
{
    if (value < lower_bound_) {
        ++underflows_;
        return;
    }

    if (value >= upper_bound_) {
        ++overflows_;
        return;
    }
    // To calculate the bin the value is assigned we divide it by the range we are measuring and multiply by
    // the number of bins. But we multiply first to avoid rounding errors.
    T range = upper_bound_ - lower_bound_;
    T diff = value - lower_bound_;
    unsigned bin = diff * num_bins_ / range;
    ++histogram_[bin];
}

inline
void Histogram::extrapolate(uint64_t from, uint64_t to)
{

}

}  // namespace stats
}  // namespace sim

#endif  // CORE_STATS_HISTOGRAM_IMPL_H_
