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


#ifndef CORE_TRACE_TYPES_IMPL_H_
#define CORE_TRACE_TYPES_IMPL_H_

#include <cassert>
#include <iostream>

namespace sim {
namespace trace {


inline
phase_data_t::phase_data_t() :
        n_lds(0),
        n_sts(0),
        n_instrs(0),
        n_unid(0)
{}

inline
phase_data_t::phase_data_t(uint64_t lds, uint64_t sts, uint64_t instrs, uint64_t unid) :
        n_lds(lds),
        n_sts(sts),
        n_instrs(instrs),
        n_unid(unid)
{}

inline
void phase_data_t::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char*>(this), sizeof(*this));
}

inline
void phase_data_t::read(std::istream &is)
{
    is.read(reinterpret_cast<char*>(this), sizeof(*this));
}

inline
void phase_data_t::clear()
{
    n_lds = n_sts = n_instrs = n_unid = 0;
}

inline
void phase_data_t::add_info(const phase_data_t &data)
{
    n_lds += data.n_lds;
    n_sts += data.n_sts;
    n_instrs += data.n_instrs;
    n_unid += data.n_unid;
}

inline
std::ostream &operator<<(std::ostream &os, const phase_data_t &phase_data)
{
    os << static_cast<unsigned>(phase_data.n_lds) << ":"
       << static_cast<unsigned>(phase_data.n_sts) << ":"
       << static_cast<unsigned>(phase_data.n_instrs) << ":"
       << static_cast<unsigned>(phase_data.n_unid);
    return os;
}

inline
std::istream &operator>>(std::istream &is, phase_data_t &phase_data)
{
    char sep;
    uint64_t lds, sts, instrs, unid;
    is >> lds;
    phase_data.n_lds = lds;
    is >> sep;
    assert(sep == ':');
    is >> sts;
    phase_data.n_sts = sts;
    is >> sep;
    assert(sep == ':');
    is >> instrs;
    phase_data.n_instrs = instrs;
    is >> sep;
    assert(sep == ':');
    is >> unid;
    phase_data.n_unid = unid;

    return is;
}


inline
user_code_data_t::user_code_data_t() :
        wd_id_(-1)
{}

inline
user_code_data_t::user_code_data_t(unsigned wd_id, const sim::trace::phase_data_t &phase_data) :
        wd_id_(wd_id),
        data(1, phase_data)
{}

inline
void user_code_data_t::write(std::ostream &os) const
{
    size_t size = data.size();
    os.write(reinterpret_cast<const char*>(&wd_id_), sizeof(unsigned));
    os.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    for (const phase_data_t &phase_data : data) {
        phase_data.write(os);
    }
}

inline
void user_code_data_t::read(std::istream &is)
{
    size_t size;
    unsigned wd_id;
    is.read(reinterpret_cast<char*>(&wd_id), sizeof(unsigned));
    wd_id_ = wd_id;
    is.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    data.resize(size);
    for (size_t i = 0; i < size; i++) {
        data[i].read(is);
    }
}

inline
std::ostream &operator<<(std::ostream &os, const user_code_data_t &user_code_data)
{
    size_t size = user_code_data.data.size();
    os << user_code_data.wd_id_ << ":" << size;
    for (const phase_data_t phase_data : user_code_data.data) {
        os << ":" << phase_data;
    }
    return os;
}

inline
std::istream &operator>>(std::istream &is, user_code_data_t &user_code_data)
{
    char sep;
    size_t size;
    unsigned wd_id;
    is >> wd_id;
    user_code_data.wd_id_ = wd_id;
    is >> sep;
    assert(sep == ':');
    is >> size;
    user_code_data.data.resize(size);
    for (size_t i = 0; i < size; i++) {
      is >> sep;
      assert(sep == ':');
      is >> user_code_data.data[i];
    }
    return is;
}

}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_TYPES_IMPL_H_
