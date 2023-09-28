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


#ifndef TRACE_FRONTEND_TYPES_H
#define TRACE_FRONTEND_TYPES_H

#include <iostream>
#include <vector>
#include <cstring>
#include <limits>
#include <cstdint>

namespace sim {
namespace trace {
namespace frontend {

/** Struct that holds all infos needed for our frontend runtime */
struct bbl_stat_t {
    uint64_t id_;
    engine::addr_t start_address_;
    size_t size_;
    size_t size_in_bytes_;
    std::vector<size_t> ins_;
};
/** function to read a basic block statistic from an input stream */
std::istream &operator>>(std::istream &, bbl_stat_t &);
/** function to write a basic block statistic to an output stream */
std::ostream &operator<<(std::ostream &, const bbl_stat_t &);

typedef enum {
    BBL = 0, SIMPLE_SYNC, SYNC
} type_t;

typedef enum {
	SIGNAL = 0, WAIT, BARRIER, PARALLEL_START, PARALLEL_END, THREAD_CREATE
} sync_id_t;

struct event_t {
    unsigned type_;
    unsigned id_;
    engine::addr_t address_;
    event_t();
};

std::ostream &operator<<(std::ostream &, const event_t &);
std::istream &operator>>(std::istream &, event_t &);

}
}
}

#include "types-impl.h"
#endif
