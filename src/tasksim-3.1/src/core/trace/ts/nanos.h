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


#ifndef CORE_TRACE_TS_NANOS_H_
#define CORE_TRACE_TS_NANOS_H_

#include <vector>
#include "nanos-int.h"

namespace sim {
namespace trace {
namespace ts {

struct dep_t {
    void *address;
    nanos_access_type_internal_t flags;
    std::vector<nanos_region_dimension_internal_t> dimensions;
    ptrdiff_t offset;

    dep_t();
    dep_t(const nanos_data_access_internal_t &);

    void write(std::ostream &) const;
    void read(std::istream &);

    size_t accessed_length() const;
    bool contiguous() const;
};

std::ostream &operator<<(std::ostream &, const dep_t &);
std::istream &operator>>(std::istream &, dep_t &);

struct wd_info_t {
    std::vector<dep_t> deps_;

    void write(std::ostream &) const;
    void read(std::istream &);
};

struct wd_info_event_t {
    unsigned wd_id_;
    wd_info_t info_;

    void write(std::ostream &) const;
    void read(std::istream &);
};

typedef wd_info_t wait_info_t;

std::ostream &operator<<(std::ostream &, const wd_info_t &);
std::istream &operator>>(std::istream &, wd_info_t &);


}  // namespace ts
}  // namespace trace
}  // namespace sim


#include "nanos-impl.h"

#endif  // CORE_TRACE_TS_NANOS_H_
