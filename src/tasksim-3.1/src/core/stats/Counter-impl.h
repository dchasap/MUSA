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


#ifndef CORE_STATS_COUNTER_IMPL_H_
#define CORE_STATS_COUNTER_IMPL_H_

#include <string>

namespace sim {
namespace stats {

template<typename T>
inline
Counter::Counter(const T &obj, const std::string &name) :
    Stat(obj, name)
{
}

inline
Counter::~Counter()
{
    uint64_t count = 0;
    for (std::pair<unsigned,uint64_t> c_id : count_per_id_) {
        count += c_id.second;
    }
    Stat::out_ << count;
}

inline
uint64_t Counter::get_value()
{
    uint64_t count = 0;
    for (std::pair<unsigned,uint64_t> c_id : count_per_id_) {
        count += c_id.second;
    }
    return count;
}

inline
void Counter::extrapolate(uint64_t from, uint64_t to)
{
    count_per_id_[to] = count_per_id_[from];
}

inline
uint64_t Counter::operator++(int)
{
    count_per_id_[0]++;
    return count_per_id_[0];
}

inline
uint64_t Counter::operator+=(uint64_t n)
{
    count_per_id_[0] += n;
    return count_per_id_[0];
}

inline
uint64_t &Counter::operator[](uint64_t user_code_id)
{
    return count_per_id_[user_code_id];
}

}  // namespace stats
}  // namespace sim


#endif  // CORE_STATS_COUNTER_IMPL_H_
