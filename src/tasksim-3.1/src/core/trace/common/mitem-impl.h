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


#ifndef CORE_TRACE_COMMON_MITEM_IMPL_H_
#define CORE_TRACE_COMMON_MITEM_IMPL_H_

#include <cassert>
#include "bbl.h"

namespace sim {
namespace trace {

inline
mitem_t::mitem_t() :
    addr_(0), data_(0), op_(INVALID), flags_(0), size_(0), ins_(0)
{}

inline
mitem_t::mitem_t(addr_t pc, addr_t addr, addr_t data, uint8_t op, uint8_t flags, uint16_t size, uint8_t nins):
    pc_(pc), addr_(addr), data_(data), op_(op), flags_(flags), size_(size), ins_(nins)
{}

inline
std::ostream &operator<<(std::ostream &os, const mitem_t &mitem)
{
    std::ios::fmtflags fmt(os.flags());
    os << static_cast<unsigned>(mitem.op_) << ":"
       << std::hex << mitem.addr_ << std::dec << ":"
       << static_cast<unsigned>(mitem.size_) << ":"
       << static_cast<unsigned>(mitem.flags_) << ":"
       << static_cast<unsigned>(mitem.ins_);
    os.flags(fmt);
    return os;
}

inline
std::istream &operator>>(std::istream &is, mitem_t &mitem)
{
    char sep;
    std::ios::fmtflags fmt = is.flags();
    unsigned op, flags, size, tins;
    addr_t addr;
    is >> op; is >> sep; assert(op < mitem_t::NOPS); mitem.op_ = op;
    is >> std::hex >> addr; is >> sep; mitem.addr_ = addr;
    is >> std::dec >> size; is >> sep; mitem.size_ = size;
    is >> flags; is >> sep; mitem.flags_ = flags;
    is >> tins; mitem.ins_ = tins;
    is.flags(fmt);
    return is;
}
inline
void mitem_t::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char *>(&addr_), sizeof(addr_));
    os.write(reinterpret_cast<const char *>(&data_ + 1), sizeof(*this) - sizeof(addr_) - sizeof(data_));
}

inline
void mitem_t::read(std::istream &is)
{
    is.read(reinterpret_cast<char *>(&addr_), sizeof(addr_));
    is.read(reinterpret_cast<char *>(&data_ + 1), sizeof(*this) - sizeof(addr_) - sizeof(data_));
}

}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_COMMON_MITEM_IMPL_H_
