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


#ifndef TRACE_FRONTEND_TYPES_IMPL_H
#define TRACE_FRONTEND_TYPES_IMPL_H

namespace sim {
namespace trace {
namespace frontend {

inline
std::istream &operator>>(std::istream &is, bbl_stat_t &bbl)
{
    char sep;
    size_t ins_size;
    bbl.ins_.clear();
    is >> bbl.id_; is >> sep; assert(sep == ':');
    is >> bbl.size_; is >> sep; assert(sep == ':');
    is >> bbl.start_address_;
    bbl.size_in_bytes_ = 0;
    for (unsigned i = 0; i < bbl.size_; i++) {
        is >> sep; assert(sep == ':');
        is >> ins_size; assert(ins_size == 0);
        bbl.ins_.push_back(ins_size);
        bbl.size_in_bytes_ += ins_size;
    }
    return is;
}

inline
std::ostream &operator<<(std::ostream &os, bbl_stat_t &bbl)
{
    os << bbl.id_ << ":" << bbl.size_ << ":" << bbl.start_address_;
    for (unsigned i = 0; i < bbl.size_; i++)
        os << ":" << bbl.ins_[i];
    return os;
}

inline event_t::event_t() : type_(0), id_(0), address_(0)
{
}

inline std::ostream &operator<<(std::ostream &os, const event_t &e)
{
    std::ios::fmtflags fmt(os.flags());
    os << e.type_ << ":" << e.id_ << ":" << std::hex << e.address_;
    os.flags(fmt);
    return os;
}

inline std::istream &operator>>(std::istream &is, event_t &e)
{
    char sep;
    is >> e.type_; is >> sep; assert(sep == ':');
    is >> e.id_; is >> sep; assert(sep == ':');
    is >> std::hex >> e.address_ >> std::dec;
    return is;
}

}
}
}

#endif
