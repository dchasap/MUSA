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


#ifndef CORE_TRACE_COMMON_MITEM_H_
#define CORE_TRACE_COMMON_MITEM_H_

#include "core/engine/types.h"
#include "core/trace/common/bbl.h"
#include "core/trace/common/mem.h"

namespace sim {
namespace trace {
/**
 * Struct holding a memory access trace item
 * pack(1) is specified to avoid padding so binary traces occupy less space
 * It includes the address, the data access size and the number of instructions
 *  after the previous item and this one
 */
#pragma pack(1)
struct mitem_t {
    /** item operation. NOMEM represents just a set of non-memory instructions */
    typedef enum { INVALID = 0, LOAD = 1, STORE = 2, NOMEM = 3, NOPS = 4 } op_t;
		addr_t pc_;							/**< Address of the instruction initiating the memory access */
    addr_t addr_;           /**< Address of the memory access */
    addr_t data_;           /**< Read/written data (debug/new features) */
    unsigned char op_:2;    /**< item operation, takes values of type mitem_t::op_t */
    unsigned char flags_:2; /**< reserved bits */
    unsigned char :4;       /**< Remaining bits */
    uint16_t size_;         /**< data access size */
    uint8_t ins_;           /**< number of instruction to the previous item */

    mitem_t();
    mitem_t(addr_t ip, addr_t addr, addr_t data, uint8_t op, uint8_t flags, uint16_t size, uint8_t nins);
    void write(std::ostream &) const;
    void read(std::istream &);
};
#pragma pack()

/** operator to write a memory trace item to an output stream */
std::ostream &operator<<(std::ostream &, const mitem_t &);
/** operator to read a memory trace item from an input stream */
std::istream &operator>>(std::istream &, mitem_t &);

}  // namespace trace
}  // namespace sim

#include "mitem-impl.h"

#endif  // CORE_TRACE_COMMON_MITEM_H_
