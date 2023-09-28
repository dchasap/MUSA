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


#ifndef CORE_TRACE_INTEL_PIN_INTERFACE_H_
#define CORE_TRACE_INTEL_PIN_INTERFACE_H_

#include <cassert>
#include "pin.H"

namespace sim {
namespace trace {
namespace intel {

/**
 * This function is used by the trace generation tool to convert a PIN BBL to a bbl_t
 */
static
unsigned process_bbl(BBL pin_bbl, UINT32 id, bbl_t &bbl)
{
    unsigned mem_ref = 0;
    bbl.id_ = id;
    for (INS ins = BBL_InsHead(pin_bbl); INS_Valid(ins); ins = INS_Next(ins)) {
        minst_t minst;

        if (INS_IsMemoryRead(ins)) {
            minst.ld_++;
            minst.ld_size_ = INS_MemoryReadSize(ins);
            mem_ref++;
        }

        if (INS_HasMemoryRead2(ins)) {
            minst.ld_++;
            assert(minst.ld_size_ == INS_MemoryReadSize(ins));
            assert(static_cast<int>(minst.ld_size_) != 0);
            mem_ref++;
        }

        if (INS_IsMemoryWrite(ins)) {
            minst.st_++;
            minst.st_size_ = INS_MemoryWriteSize(ins);
            assert(static_cast<int>(minst.st_size_) != 0);
            mem_ref++;
        }
        assert(minst.ld_ + minst.st_ <= 2);
        minst.type_ = sim::trace::op_t::MACROOP;
        bbl.ins_.push_back(minst);
    }

    return mem_ref;
}

}  // namespace intel
}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_INTEL_PIN_INTERFACE_H_
