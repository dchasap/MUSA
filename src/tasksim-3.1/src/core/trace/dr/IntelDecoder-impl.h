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


#ifndef CORE_TRACE_DR_INTELDECODER_IMPL_H_
#define CORE_TRACE_DR_INTELDECODER_IMPL_H_

#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>


namespace sim {
namespace trace {
namespace dr {

void
inline
IntelDecoder::process_uop(instr_t *current_instr, const TransOp &uop, sim::trace::bbl_t &translated_bbl, minfo_t &minfo)
{
    unsigned size = 1 << static_cast<unsigned>(uop.size);
    // First loads and stores:
    if (isload(uop.opcode) and not uop.internal and not uop.locked) {
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::LD, size, uop);
        minfo.ld_++;
    } else if (isload(uop.opcode) and not uop.internal) {
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::LDL, size, uop);
        minfo.ld_++;
    } else if (isstore(uop.opcode) and not uop.internal and not (uop.opcode == OP_mf) and not uop.locked) {
        // OP_mf is a memory fence instruction.
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::ST, size, uop);
        minfo.st_++;
    } else if (isstore(uop.opcode) and not uop.internal and not (uop.opcode == OP_mf)) {
        // OP_mf is a memory fence instruction.
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::STL, size, uop);
        minfo.st_++;
    } else {
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), type.at(uop.opcode), size, uop);
        translated_bbl.ins_.rbegin()->vectorize();
        if (translated_bbl.ins_.rbegin()->isLoad()){
            translated_bbl.ins_.rbegin()->type_ = op_t::REMOVED_LD;
            translated_bbl.ins_.rbegin()->ld_ = 0;
        } else if (translated_bbl.ins_.rbegin()->isStore()) {
            translated_bbl.ins_.rbegin()->type_ = op_t::REMOVED_ST;
            translated_bbl.ins_.rbegin()->st_ = 0;
        }
    }
}

bool
inline
IntelDecoder::memory_accesses_mismatch(const minfo_t &ins_minfo, instr_t *current_instr, minfo_t &x86_minfo)
{
    x86_minfo.ld_ = 0;
    if (instr_reads_memory(current_instr)) {
        for (int i = 0; i < instr_num_srcs(current_instr); i++) {
            if (opnd_is_memory_reference(instr_get_src(current_instr, i))) {
                x86_minfo.ld_++;
            }
        }
    }
    x86_minfo.st_ = 0;
    if (instr_writes_memory(current_instr)) {
        for (int i = 0; i < instr_num_dsts(current_instr); i++) {
            if (opnd_is_memory_reference(instr_get_dst(current_instr, i))) {
                x86_minfo.st_++;
            }
        }
    }
    return (ins_minfo.ld_ != x86_minfo.ld_) or (ins_minfo.st_ != x86_minfo.st_);
}

void
inline
IntelDecoder::fix_memory_access_mismatch(instr_t *current_instr, sim::trace::bbl_t &translated_bbl, unsigned start,
                                         const minfo_t &ins_minfo, const minfo_t &x86_minfo)
{
    // fix lack of lds
    if (ins_minfo.ld_ < x86_minfo.ld_) {
        int n_loads = 0;
        unsigned lds_lack = x86_minfo.ld_ - ins_minfo.ld_;
        for (int i = 0; i < instr_num_srcs(current_instr); i++) {
            if (opnd_is_memory_reference(instr_get_src(current_instr, i))) {
                if (n_loads >= ins_minfo.ld_) {
                    int8_t size = opnd_size_in_bytes(opnd_get_size(instr_get_src(current_instr, i)));
                    translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::LD, size, REG_mem, REG_imm, REG_imm, REG_imm);
                    lds_lack--;
                }
                n_loads++;
            }
        }
        assert(lds_lack == 0);
    }

    // fix lack of sts
    if (ins_minfo.st_ < x86_minfo.st_) {
        int n_stores = 0;
        unsigned sts_lack = x86_minfo.st_ - ins_minfo.st_;
        for (int i = 0; i < instr_num_dsts(current_instr); i++) {
            if (opnd_is_memory_reference(instr_get_dst(current_instr, i))) {
                if (n_stores >= ins_minfo.st_) {
                    int8_t size = opnd_size_in_bytes(opnd_get_size(instr_get_dst(current_instr, i)));
                    translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::ST, size, REG_imm, REG_imm, REG_imm, REG_mem);
                    sts_lack--;
                }
                n_stores++;
            }
        }
        assert(sts_lack == 0);
    }

    // fix excess of lds
    if (ins_minfo.ld_ > x86_minfo.ld_) {
        unsigned lds_excess = ins_minfo.ld_ - x86_minfo.ld_;
        for (unsigned i = start; i < translated_bbl.ins_.size() and lds_excess > 0; ++i) {
            if (translated_bbl.ins_[i].isLoad()) {
                if (translated_bbl.ins_[i].ld_<= lds_excess) {
                    lds_excess -= translated_bbl.ins_[i].ld_;
                    translated_bbl.ins_[i].type_ = op_t::REMOVED_LD;
                    translated_bbl.ins_[i].ld_ = 0;
                } else {
                    translated_bbl.ins_[i].ld_ -= lds_excess;
                    lds_excess = 0;
                }
            }
        }
        assert(lds_excess == 0);
    }
    // fix excess of sts
    if (ins_minfo.st_ > x86_minfo.st_) {
        unsigned sts_excess = ins_minfo.st_ - x86_minfo.st_;
        for (unsigned i = start; i < translated_bbl.ins_.size() and sts_excess > 0; ++i) {
            if (translated_bbl.ins_[i].isStore()) {
                translated_bbl.ins_[i].type_ = op_t::REMOVED_ST;
                translated_bbl.ins_[i].st_ = 0;
                sts_excess--;
            }
        }
        assert(sts_excess == 0);
    }
}

/**
 * Function that converts the translated TransOps (from a complex BBL INS), to a set of micro-operations
 *  and returns some statistics in 'info'
 * If the translation to micro-ops generated a different number of memory accesses compared to the complex
 *  instruction, it fixes that mismatch
 */
void
inline
IntelDecoder::process_x86_translation(instr_t *current_instr, const TransOp *transops,
        unsigned &current,  unsigned count,
        sim::trace::bbl_t &translated_bbl, TransInfo &info)
{
    minfo_t ins_minfo;
    unsigned ins_start = translated_bbl.ins_.size();
    for (unsigned i = current; i < count; ++i) {
        process_uop(current_instr, transops[i], translated_bbl, ins_minfo);
    }
    minfo_t x86_minfo;
    if (memory_accesses_mismatch(ins_minfo, current_instr, x86_minfo)) {
        fix_memory_access_mismatch(current_instr, translated_bbl, ins_start, ins_minfo, x86_minfo);
        info.num_fixed_++;
    }
    info.num_x86_trans_++;
    info.minfo_.push_back(ins_minfo);
    current = count;
}

void
inline
IntelDecoder::handle_invalid(instr_t *current_instr, sim::trace::bbl_t &translated_bbl, TransInfo &info)
{
    /**
     * Assume a ld-op-st instruction
     * Add 'ld' if it's memory read, and 'st' if it's memory write
     * Always add one 'other' in any case
     */
    minfo_t minfo;
    if (current_instr == NULL) {
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::OTHER, 0, REG_imm, REG_imm, REG_imm, REG_imm);
    } else {
        if (instr_reads_memory(current_instr)) {
            for (int i = 0; i < instr_num_srcs(current_instr); i++) {
                if (opnd_is_memory_reference(instr_get_src(current_instr, i))) {
                    int8_t size = opnd_size_in_bytes(opnd_get_size(instr_get_src(current_instr, i)));
                    translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::LD, size, REG_mem, REG_imm, REG_imm, REG_imm);
                    minfo.ld_++;
                }
            }
        }
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::OTHER, 0, REG_imm, REG_imm, REG_imm, REG_imm);
        if (instr_writes_memory(current_instr)) {
            for (int i = 0; i < instr_num_dsts(current_instr); i++) {
                if (opnd_is_memory_reference(instr_get_dst(current_instr, i))) {
                    int8_t size = opnd_size_in_bytes(opnd_get_size(instr_get_dst(current_instr, i)));
                    translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::ST, size, REG_imm, REG_imm, REG_imm, REG_mem);
                    minfo.st_++;
                }
            }
        }
    }
    info.num_x86_trans_++;
    info.num_invalid_++;
    info.minfo_.push_back(minfo);
}

instr_t*
IntelDecoder::advance_instruction(void *drcontext, instr_t *current_instr, size_t &index, unsigned &instr_l)
{
    index += instr_l;
    if (current_instr != NULL) {
        current_instr = instr_get_next_app(current_instr);
        if (current_instr != NULL) {
            instr_l = instr_length(drcontext, current_instr);
        } else {
            instr_l = 0;
        }
    }
    return current_instr;
}

sim::trace::phase_data_t
IntelDecoder::translate(void *drcontext, instrlist_t *bb, sim::trace::bbl_t &translated_bbl, bool debug)
{
    /**
     * We use PTLSim TraceDecoder for converting x86 PIN instructions to uops.
     *
     * We have found that PTLSim does not support all x86 instructions set, and that
     * for some operations it generates a different number of loads and/or stores.
     * The code gets a bit more complex to handle these corner cases.
     */
    sim::trace::phase_data_t bbl_data;
    TransInfo *info = new TransInfo();
    bool finished = false, invalid = false;
    size_t bbl_size = 0;
    byte *base_bbl = instr_get_app_pc(instrlist_first_app(bb));
    byte *current_pointer = base_bbl;
    for (instr_t *current_instr = instrlist_first_app(bb); current_instr != NULL;
            current_instr = instr_get_next_app(current_instr)) {
        bbl_size += instr_length(drcontext, current_instr);
    }
    translated_bbl.instr_ptr_ = reinterpret_cast<uint64_t>(instr_get_app_pc(instrlist_first_app(bb)));
    translated_bbl.block_size_ = bbl_size;

    unsigned user_insn_count = 0;
    size_t index = 0;
    instr_t *current_instr = instrlist_first_app(bb);
    unsigned instr_l = 0;
    std::vector<int> pos;
    do {
        const unsigned size = bbl_size - index;
        instr_l = instr_length(drcontext, current_instr);
        TraceDecoder dec(0, (sizeof(void*) == 8), false, false);
        unsigned current = 0;
        dec.insnbytes = instr_get_app_pc(current_instr);
        dec.insnbytes_bufsize = size;
        dec.valid_byte_count = size;

        while (dec.translate() and current_instr != NULL) {
            translated_bbl.macroops_.emplace_back(sim::trace::minst_t(instr_get_app_pc(current_instr), instr_l));
            process_x86_translation(current_instr, dec.bb.transops, current, dec.bb.count, translated_bbl, *info);
            pos.push_back(translated_bbl.ins_.size());
            current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
        }
        if (current_instr == NULL) {
            std::cerr << "[WARNING] Basic bloc finished unexpectedly." << std::endl;
            break;
        }
        if (dec.invalid) {
            translated_bbl.macroops_.emplace_back(sim::trace::minst_t(instr_get_app_pc(current_instr), instr_l));
            handle_invalid(current_instr, translated_bbl, *info);
            invalid = true;
            bbl_data.n_unid++;
            current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
        } else {
            translated_bbl.macroops_.emplace_back(sim::trace::minst_t(instr_get_app_pc(current_instr), instr_l));
            process_x86_translation(current_instr, dec.bb.transops, current, dec.bb.count, translated_bbl, *info);
            pos.push_back(translated_bbl.ins_.size());
            instr_t *old = current_instr;
            current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
            if (current_instr != NULL) {
                invalid = true;
                bbl_data.n_unid++;
            }
            finished = true;
        }

        user_insn_count += dec.user_insn_count;
    } while (not finished and current_instr != NULL);

    // To ensure that all blocks are properly represented even if ptlsim cannot decode them
    /* Commented as we do not do it for PIN */
    int c = 0;
    while (current_instr != NULL) {
        invalid = true;
        bbl_data.n_unid++;
        translated_bbl.macroops_.emplace_back(sim::trace::minst_t(instr_get_app_pc(current_instr), instr_l));
        handle_invalid(current_instr, translated_bbl, *info);
        current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
        user_insn_count++;
    }
    if (info->num_x86_trans_ != user_insn_count) {
        std::cerr << "[WARNING] Basic bloc decoding the wrong number of instructions:" << std::endl
                  << "          " << info->num_x86_trans_ << " vs " << user_insn_count << std::endl;

    }
    bbl_data.n_instrs = translated_bbl.ins_.size();
    for (auto &ins_info : info->minfo_) {
        bbl_data.n_lds = ins_info.ld_;
        bbl_data.n_sts = ins_info.st_;
    }
    delete(info);
    return bbl_data;
}

}  // namespace dr
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_DR_INTELDECODER_IMPL_H_
