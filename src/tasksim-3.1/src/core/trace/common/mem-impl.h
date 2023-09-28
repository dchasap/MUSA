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


#ifndef CORE_TRACE_COMMON_MEM_IMPL_H_
#define CORE_TRACE_COMMON_MEM_IMPL_H_

#include <cassert>
#include <iostream>

namespace sim {
namespace trace {

inline
mem_t::mem_t(const addr_t &addr) :
    addr_(addr)
{
}

inline
mem_t &mem_t::operator=(const addr_t &value)
{
    addr_ = value;
    return *this;
}

inline
std::ostream &operator<<(std::ostream &os, const mem_t &mem)
{
    std::ios::fmtflags fmt(os.flags());
    os << std::hex << mem.addr_;
    os.flags(fmt);
    return os;
}

inline
std::istream &operator>>(std::istream &is, mem_t &mem)
{
    addr_t addr;
    std::ios::fmtflags fmt = is.flags();
    is >> std::hex >> addr;
    is.flags(fmt);
    mem.addr_ = addr;
    return is;
}


}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_COMMON_MEM_IMPL_H_
