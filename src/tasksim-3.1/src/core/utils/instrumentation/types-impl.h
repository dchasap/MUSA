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


#ifndef CORE_UTILS_INSTRUMENTATION_TYPES_IMPL_H_
#define CORE_UTILS_INSTRUMENTATION_TYPES_IMPL_H_

#include <cassert>
#include <string>

namespace sim {
namespace utils {
namespace instrumentation {

inline
std::ostream &operator<<(std::ostream &os, const InstrumentationItem &item)
{
    switch (item.record_type_) {
        case sim::utils::instrumentation::STATE_RECORD : {
            os << "1:" << item.cpu_id_ << ":1:1:" << item.cpu_id_ + item.thread_id_
                       << ":" << item.time_stamp_ << ":"
                       << item.u.state_.end_time_stamp_ << ":"
                       << item.u.state_.value_;
            break;
        }
        case sim::utils::instrumentation::EVENT_RECORD : {
            os << "2:" << item.cpu_id_ << ":1:1:" << item.cpu_id_ + item.thread_id_
                       << ":" << item.time_stamp_ << ":"
                       << item.u.event_.type_ << ":"
                       << item.u.event_.value_;
            break;
        }
        case sim::utils::instrumentation::COMM_RECORD : {
            os << "3:" << item.cpu_id_ << ":1:1:" << item.cpu_id_ + item.thread_id_
                    << ":" << item.time_stamp_ << ":"
                    << item.u.comm_.send_start_time_stamp_ << ":"
                    << item.u.comm_.recv_cpu_id_ << ":1:1:"
                    << item.u.comm_.recv_thread_id_ << ":"
                    << item.u.comm_.recv_invocation_time_stamp_ << ":"
                    << item.u.comm_.recv_start_time_stamp_ << ":"
                    << item.u.comm_.byte_size_ << ":"
                    << item.u.comm_.tag_;
            break;
        }
        default : {
            std::cerr << "Unknown Instrumentation trace event type: " << item.record_type_ << std::endl;
            std::abort();
        }
    }
    return os;
}

inline
std::istream &operator>>(std::istream &is, InstrumentationItem &item)
{
    int rec_type, thread_id;
    int null;
    char sep;
    is >> rec_type >> sep;
    assert(sep == ':');
    is >> item.cpu_id_ >> sep;
    assert(sep == ':');
    is >> null >> sep;
    assert(sep == ':');
    is >> null >> sep;
    assert(sep == ':');
    is >> thread_id >> sep;
    item.thread_id_ = thread_id - item.cpu_id_;
    assert(sep == ':');
    is >> item.time_stamp_ >> sep;
    assert(sep == ':');

    switch (rec_type) {
        case 1: {
            item.record_type_ = sim::utils::instrumentation::STATE_RECORD;
            is >> item.u.event_.type_ >> sep;
            assert(sep == ':');
            is >> item.u.event_.value_;
            break;
        }
        case 2: {
            item.record_type_ = sim::utils::instrumentation::EVENT_RECORD;
            is >> item.u.state_.end_time_stamp_ >> sep;
            assert(sep == ':');
            is >> item.u.state_.value_;
            break;
        }
        case 3: {
            item.record_type_ = sim::utils::instrumentation::COMM_RECORD;
            is >> item.u.comm_.send_start_time_stamp_ >> sep;
            assert(sep == ':');
            is >> item.u.comm_.recv_cpu_id_ >> sep;
            assert(sep == ':');
            is >> null >> sep;
            assert(sep == ':');
            is >> null >> sep;
            assert(sep == ':');
            is >> item.u.comm_.recv_thread_id_ >> sep;
            assert(sep == ':');
            is >> item.u.comm_.recv_invocation_time_stamp_ >> sep;
            assert(sep == ':');
            is >> item.u.comm_.recv_start_time_stamp_ >> sep;
            assert(sep == ':');
            is >> item.u.comm_.byte_size_ >> sep;
            assert(sep == ':');
            is >> item.u.comm_.tag_;
            break;
        }
        default: {
            std::cerr << "Wrong Record type for instrumentation Event: " << rec_type << std::endl;
            std::abort();
        }
    }
    return is;
}

}  // namespace instrumentation
}  // namespace utils
}  // namespace sim


#endif  // CORE_UTILS_INSTRUMENTATION_TYPES_IMPL_H_
