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


#ifndef CORE_TRACE_DR_BASICDECODER_H_
#define CORE_TRACE_DR_BASICDECODER_H_

/**
 * BasicDecoder is a interface class to use PTLSim's x86 to uops
 * decoder. The translate function translates a basic block at
 * address 'addr' with size 'size', and returns the list of
 * translated uops in the 'uops' vector.
 */

#include <vector>

#include "dr_api.h"

#include "core/trace/dr/types.h"

struct TransOp;

namespace sim {
namespace trace {
namespace dr {

/** BasicDecoder defines a set of functions for translating complex instructions to micro-operations */
class BasicDecoder {
private:
    /** handles how to convert a complex BBL INS to a set of micro-operations
     *  when the decoder was not able to do a proper translation */
    static void handle_invalid(instr_t *current_instr, sim::trace::bbl_t &translated_bbl, TransInfo &info);
    /** queries whether the translation of a complex instruction to micro-ops
     * generated a different number of memory accesses */
    static bool memory_accesses_mismatch(const minfo_t &uops_minfo, instr_t *current_instr, minfo_t &x86_minfo);
    /** fixes a different number of memory accesses of the complex instruction
     * and the translated micro-ops */
    static void fix_memory_access_mismatch(instr_t *current_instr, sim::trace::bbl_t &translated_bbl, unsigned start,
            const minfo_t &uops_minfo, const minfo_t &x86_minfo);
    static instr_t* advance_instruction(void *drcontext, instr_t *current_instr, size_t &index, unsigned &instr_l);
    /** Number of unidentified instructions. */
    static size_t unid_;

    // static unsigned handle_load(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl);
    // static unsigned handle_store(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl);
    static void handle_load(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl);
    static void handle_store(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl);
    static void handle_load_store(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl);
    static void handle_non_memory(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl);
    static void handle_nonmem_sve(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl);
    static bool is_vector(opnd_t &operand);
    static void print_detailled_information(void *drcontext, instr_t *instr);

public:
    /** translates the instructions in a DynamoRIOP BBL basic block to a set of micro-operations
     * and returns statistics to 'info' */
    static sim::trace::phase_data_t translate(void *drcontext, instrlist_t *bb, sim::trace::bbl_t &translated_bbl,
            bool debug = false);
    static sim::trace::op_t transform_opcode(int dr_opcode);
    static sim::trace::op_t transform_opcode_x86(int dr_opcode);
    static sim::trace::op_t transform_opcode_aarch64(int dr_opcode);
};

}  // namespace dr
}  // namespace trace
}  // namespace sim

#include "BasicDecoder-impl.h"

#endif  // CORE_TRACE_DR_BASICDECODER_H_
