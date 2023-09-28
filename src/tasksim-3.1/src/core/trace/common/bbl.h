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


#ifndef CORE_TRACE_COMMON_BBL_H_
#define CORE_TRACE_COMMON_BBL_H_

#include <iostream>
#include <vector>
#include "core/trace/common/minst.h"

namespace sim {
namespace trace {

/** Struct holding a basic block identifier */
struct bbl_id_t {
    unsigned id_;
    explicit bbl_id_t(unsigned id = 0);
    bbl_id_t & operator=(const unsigned &);
    bool operator==(const unsigned &) const;
    bool operator==(const bbl_id_t &) const;
};
/** function to write a basic block identifier to an output stream */
std::ostream &operator<<(std::ostream &, const bbl_id_t &);
/** function to read a basic block identifier from an output stream */
std::istream &operator>>(std::istream &, bbl_id_t &);

/** Struct holding a basic block */
struct bbl_t {
    bbl_id_t id_;
    uint64_t instr_ptr_;
    unsigned block_size_;
    // uint16_t n_ops_;
    std::vector<minst_t> ins_;
    std::vector<minst_t> macroops_;

    void write(std::ostream &) const;
    void read(std::istream &);
    void shallow_copy(const bbl_t & src);
};
/** function to write a basic block to an output stream */
std::ostream &operator<<(std::ostream &, const bbl_t &);
/** function to read a basic block from an input stream */
std::istream &operator>>(std::istream &, bbl_t &);

}  // namespace trace
}  // namespace sim

#include "bbl-impl.h"

#endif  // CORE_TRACE_COMMON_BBL_H_
