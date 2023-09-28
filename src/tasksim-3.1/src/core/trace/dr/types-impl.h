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


#ifndef CORE_TRACE_DR_TYPES_IMPL_H_
#define CORE_TRACE_DR_TYPES_IMPL_H_

#include <cassert>
#include <string>
#include <iostream>

namespace sim {
namespace trace {
namespace dr {

inline std::ostream &operator<<(std::ostream &os, const mpi_info_t &mpi_info)
{
    os << mpi_info.index << ":"
        << mpi_info.wd_id << ":"
        << (mpi_info.is_nanos ? "1" : "0") << ":"
        << (mpi_info.is_mem ? "1" : "0") << ":"
        << mpi_info.mpi_id << ":"
        << mpi_info.mpi_name;
    return os;
}

inline std::istream &operator>>(std::istream &is, mpi_info_t &mpi_info)
{
    char sep;
    unsigned val;
    is >> mpi_info.index >> sep;
    assert(sep == ':');
    is >> mpi_info.wd_id >> sep;
    assert(sep == ':');
    is >> val >> sep;
    assert(sep == ':');
    mpi_info.is_nanos = (val==1);
    is >> val >> sep;
    assert(sep == ':');
    mpi_info.is_mem = (val==1);
    is >> mpi_info.mpi_id >> sep;
    assert(sep == ':');
    is >> mpi_info.mpi_name;
    return is;
}

}  // namespace dr
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_DR_TYPES_IMPL_H_
