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


#ifndef CORE_TRACE_TS_TYPES_IMPL_H_
#define CORE_TRACE_TS_TYPES_IMPL_H_

#include <cassert>
#include <cstring>
#include <iostream>

namespace sim {
namespace trace {
namespace ts {

inline std::ostream &operator<<(std::ostream &os, const subtrace_t &st)
{
    os << st.wd_id << ":" << st.name_id << ":" << st.bbl_dict_id
            << ":" << st.mem_trace_id << ":" << st.bbl_trace_id
            << ":" << st.wait_trace_id << ":" << st.n_instr
            << ":" << st.mem_trace_chunks << ":" << st.stack;
    return os;
}

inline std::istream &operator>>(std::istream &is, subtrace_t &st)
{
    char sep;
    is >> st.wd_id; is >> sep; assert(sep == ':');
    is >> st.name_id; is >> sep; assert(sep == ':');
    is >> st.bbl_dict_id; is >> sep; assert(sep == ':');
    is >> st.mem_trace_id; is >> sep; assert(sep == ':');
    is >> st.bbl_trace_id; is >> sep; assert(sep == ':');
    is >> st.wait_trace_id; is >> sep; assert(sep == ':');
    is >> st.n_instr; is >> sep; assert(sep == ':');
    is >> st.mem_trace_chunks; is >> sep; assert(sep == ':');
    is >> st.stack;
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const task_t &t)
{
    os << t.wd_id << ":" << t.stream_id;
    return os;
}

inline std::istream &operator>>(std::istream &is, task_t &t)
{
    char sep;
    is >> t.wd_id; is >> sep; assert(sep == ':');
    is >> t.stream_id;
    return is;
}

inline event_t::event_t()
{
    memset(this, 0, sizeof(*this));
}

inline std::ostream &operator<<(std::ostream &os, const event_t &e)
{
    os << e.type << ":" << e.u.phase.burst_id << ":" << e.u.phase.subtrace_id << ":" << e.u.phase.duration;
    return os;
}

inline std::istream &operator>>(std::istream &is, event_t &e)
{
    char sep;
    is >> e.type; is >> sep; assert(sep == ':');
    is >> e.u.phase.burst_id; is >> sep; assert(sep == ':');
    is >> e.u.phase.subtrace_id; is >> sep; assert(sep == ':');
    is >> e.u.phase.duration;
    return is;
}

inline replay_event_t::replay_event_t() : wd_id(0)
{}

inline replay_event_t::replay_event_t(unsigned id) : wd_id(id)
{}

inline std::ostream &operator<<(std::ostream &os, const replay_event_t &e)
{
    os << e.wd_id;
    return os;
}

inline std::istream &operator>>(std::istream &is, replay_event_t &e)
{
    is >> e.wd_id;
    return is;
}


}  // namespace ts
}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_TS_TYPES_IMPL_H_
