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


#ifndef CORE_TRACE_COMMON_MEM_H_
#define CORE_TRACE_COMMON_MEM_H_

#include <iostream>
#include "core/engine/types.h"

namespace sim {
namespace trace {

/** alias of an address in tracing for the one used in the engine */
typedef engine::addr_t addr_t;

/** struct holding a memory address
  * The NOLINT Comment helps the coding style script ignore the non-explicit constructor
  * It is to be ignored as mem_t is a wrapper class around a addr_t type.
  */

struct mem_t {
    addr_t addr_;
    mem_t(const addr_t& = 0);  // NOLINT(runtime/explicit)
    mem_t &operator=(const addr_t&);
};

/** operator to write a memory address to an output stream */
std::ostream &operator<<(std::ostream &, const mem_t &);
/** operator to read a memory address from an input stream */
std::istream &operator>>(std::istream &, mem_t &);

}  // namespace trace
}  // namespace sim

#include "mem-impl.h"

#endif  // CORE_TRACE_COMMON_MEM_H_
