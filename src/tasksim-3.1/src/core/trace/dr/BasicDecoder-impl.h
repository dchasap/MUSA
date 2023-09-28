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


#ifndef CORE_TRACE_DR_BASICDECODER_IMPL_H_
#define CORE_TRACE_DR_BASICDECODER_IMPL_H_

#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>

#include "dr_api.h"
#include "drmgr.h"

namespace sim {
namespace trace {
namespace dr {


void
inline
BasicDecoder::handle_invalid(instr_t *current_instr, sim::trace::bbl_t &translated_bbl, TransInfo &info)
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
BasicDecoder::advance_instruction(void *drcontext, instr_t *current_instr, size_t &index, unsigned &instr_l)
{
    index += instr_l;
    if (current_instr != NULL) {
        current_instr = instr_get_next(current_instr);
        if (current_instr != NULL) {
            instr_l = instr_length(drcontext, current_instr);
        } else {
            instr_l = 0;
        }
    }
    return current_instr;
}

sim::trace::phase_data_t
BasicDecoder::translate(void *drcontext, instrlist_t *bb, sim::trace::bbl_t &translated_bbl, bool debug)
{
    // std::cout << "[DEBUG] Beggining of translation." << std::endl;
    TransInfo info;
    size_t bbl_size = 0;
    sim::trace::phase_data_t bbl_data;
    for (instr_t *current_instr = instrlist_first_app(bb); current_instr != NULL;
            current_instr = instr_get_next_app(current_instr)) {
        bbl_size += instr_length(drcontext, current_instr);
    }

    translated_bbl.instr_ptr_ = reinterpret_cast<uint64_t>(instr_get_app_pc(instrlist_first_app(bb)));
    translated_bbl.block_size_ = bbl_size;

    size_t index = 0;
    bool in_emulation = false;
    instr_t *current_instr = instrlist_first(bb);
    while (current_instr != NULL) {
        unsigned instr_l = instr_length(drcontext, current_instr);
        if (drmgr_is_emulation_start(current_instr)) {
            in_emulation = true;
            emulated_instr_t emulated;
            emulated.pc = 0;
            emulated.size = sizeof(emulated_instr_t);
            if (not drmgr_get_emulated_instr_data(current_instr, &emulated)) {
                Log::warning() << "Did not get the SVE emulated instruction.";
                translated_bbl.macroops_.emplace_back(sim::trace::minst_t(emulated.pc, 4, 0));
                current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
                continue;
            } else {
            }
            translated_bbl.macroops_.emplace_back(sim::trace::minst_t(instr_get_raw_bits(emulated.instr), instr_length(drcontext, emulated.instr), reinterpret_cast<uint64_t>(instr_get_raw_bits(emulated.instr))));
            // We mark the instruction as SVE:
            translated_bbl.macroops_.back().sve_ = 1;
            handle_nonmem_sve(drcontext, current_instr, translated_bbl);
            current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
            continue;
        } else if (drmgr_is_emulation_end(current_instr)) {
            in_emulation = false;
            current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
            continue;
        }
        if (in_emulation or not instr_is_app(current_instr) or instr_needs_encoding(current_instr)) {
            current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
            continue;
        }
        translated_bbl.macroops_.emplace_back(sim::trace::minst_t(instr_get_app_pc(current_instr), instr_l));
        if (instr_reads_memory(current_instr) and instr_writes_memory(current_instr)) {
            // This is a ldstex instruction:
            handle_load(drcontext, current_instr, translated_bbl);
            bbl_data.n_lds++;
            handle_store(drcontext, current_instr, translated_bbl);
            bbl_data.n_sts++;
        } else if (instr_reads_memory(current_instr)) {
            handle_load(drcontext, current_instr, translated_bbl);
            bbl_data.n_lds++;
        } else if (instr_writes_memory(current_instr)) {
            handle_store(drcontext, current_instr, translated_bbl);
            bbl_data.n_sts++;
        } else {
            handle_non_memory(drcontext, current_instr, translated_bbl);
        }
        current_instr = advance_instruction(drcontext, current_instr, index, instr_l);
    }
    bbl_data.n_instrs = translated_bbl.ins_.size();
    // std::cout << "[DEBUG] End of translation." << std::endl;
    return bbl_data;
}

void BasicDecoder::handle_load(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl)
{
    std::vector<uint8_t> ld_size;
    for (int i = 0; i < instr_num_srcs(current_instr); i++) {
        opnd_t operand = instr_get_src(current_instr, i);
        if (opnd_is_memory_reference(operand)) {
            ld_size.push_back(opnd_size_in_bytes(opnd_get_size(operand)));
            // We got the register that we use as a base to calculate the memory address.
            if (opnd_is_base_disp(operand)) {
                reg_id_t reg = opnd_get_base(operand);
                // We add a LEA instruction here (to intermediate register...):
                translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::IALU, 0, reg, REG_imm, REG_imm, REG_lea);
            }
            break;
            i++;
            while (i < instr_num_srcs(current_instr)) {
                operand = instr_get_src(current_instr, i);
                if (opnd_is_memory_reference(operand)) {
                    std::cout << "[DEBUG] Adding a second load from instruction." << std::endl;
                    ld_size.push_back(opnd_size_in_bytes(opnd_get_size(operand)));
                }
                i++;
            }
        }
    }

    reg_id_t destination;
    for (int i = 0; i < instr_num_dsts(current_instr); i++) {
        opnd_t operand = instr_get_dst(current_instr, i);
        if (opnd_is_reg(operand)) {
            destination = opnd_get_reg(operand);
            break;  // We break in case is a load pair, atm we only load/store one.
        }
    }

    // We add a Load with the destination register here:
    for (uint8_t ld : ld_size) {
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::LD, ld, REG_mem, REG_lea, REG_imm, destination);
    }
    if (ld_size.size() != 1) {
        std::cout << "[DEBUG] Instruction with " << ld_size.size() << " loads." << std::endl;
    }
    // return ld_size.size();
}

void BasicDecoder::handle_store(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl)
{
    std::vector<uint8_t> st_size;
    for (int i = 0; i < instr_num_dsts(current_instr); i++) {
        opnd_t operand = instr_get_dst(current_instr, i);
        if (opnd_is_memory_reference(operand)) {
            st_size.push_back(opnd_size_in_bytes(opnd_get_size(operand)));
            // We got the register that we use as a base to calculate the memory address.
            if (opnd_is_base_disp(operand)) {
                reg_id_t reg = opnd_get_base(operand);
                // We add a LEA instruction here (to intermediate register...):
                translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::IALU, 0, reg, REG_imm, REG_imm, REG_lea);
            }
            break;
            i++;
            while (i < instr_num_dsts(current_instr)) {
                 operand = instr_get_dst(current_instr, i);
                 if (opnd_is_memory_reference(operand)) {
                    std::cout << "[DEBUG] Adding a second store from instruction." << std::endl;
                    st_size.push_back(opnd_size_in_bytes(opnd_get_size(operand)));
                 }
                 i++;
            }
        }
    }

    reg_id_t source;
    for (int i = 0; i < instr_num_srcs(current_instr); i++) {
        opnd_t operand = instr_get_src(current_instr, i);
        if (opnd_is_reg(operand)) {
            source = opnd_get_reg(operand);
            break;  // We break in case is a load pair, atm we only load/store one.
        }
    }

    // We add a Store with the source register here:
    for (uint8_t st : st_size) {
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::ST, st, REG_lea, source, REG_imm, REG_mem);
    }
    if (st_size.size() != 1) {
        std::cout << "[DEBUG] Instruction with " << st_size.size() << " stores." << std::endl;
    }
    // return st_size.size();
}

void BasicDecoder::handle_load_store(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl)
{
    uint8_t ld_size = 0, st_size = 0;
    for (int i = 0; i < instr_num_srcs(current_instr); i++) {
        opnd_t operand = instr_get_src(current_instr, i);
        if (opnd_is_memory_reference(operand)) {
            ld_size = opnd_size_in_bytes(opnd_get_size(operand));
            // We got the register that we use as a base to calculate the memory address.
            if (opnd_is_base_disp(operand)) {
                reg_id_t reg = opnd_get_base(operand);
                // We add a LEA instruction here (to intermediate register...):
                translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::IALU, 0, reg, REG_imm, REG_imm, REG_lea);
            }
            break;
        }
    }

    reg_id_t destination;
    for (int i = 0; i < instr_num_dsts(current_instr); i++) {
        opnd_t operand = instr_get_dst(current_instr, i);
        if (opnd_is_reg(operand)) {
            destination = opnd_get_reg(operand);
            break;  // We break in case is a load pair, atm we only load/store one.
        }
    }

    // We add a Load with the destination register here:
    translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::LD, ld_size, REG_mem, REG_lea, REG_imm, destination);
    // We modify the value of the register:
    translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::IALU, 0, destination, REG_imm, REG_imm, destination);
    // We add a Store with the source register here:
    translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::ST, st_size, REG_lea, destination, REG_imm, REG_mem);
}

void BasicDecoder::handle_non_memory(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl)
{
    // We need:
    // 1 - Map from DynamoRIO op_t to TaskSim's op_t.
    // 2 - Input register list.
    // 3 - Output register list.
    sim::trace::op_t op = transform_opcode(instr_get_opcode(current_instr));
    std::vector<reg_id_t> in_regs, out_regs;
    bool uses_vector_registers = false;
    for (int i = 0; i < instr_num_srcs(current_instr); i++) {
        opnd_t operand = instr_get_src(current_instr, i);
        if (opnd_is_reg(operand)) {
            uses_vector_registers = uses_vector_registers or is_vector(operand);
            in_regs.emplace_back(opnd_get_reg(operand));
        }
    }
    for (int i = 0; i < instr_num_dsts(current_instr); i++) {
        opnd_t operand = instr_get_dst(current_instr, i);
        if (opnd_is_reg(operand)) {
            uses_vector_registers = uses_vector_registers or is_vector(operand);
            out_regs.emplace_back(opnd_get_reg(operand));
        }
    }
    uint8_t ra = in_regs.size() > 0 ? in_regs[0] : REG_imm;
    uint8_t rb = in_regs.size() > 1 ? in_regs[1] : REG_imm;
    uint8_t rc = in_regs.size() > 2 ? in_regs[2] : REG_imm;
    uint8_t rd = out_regs.size() > 0 ? out_regs[0] : REG_imm;
    if (uses_vector_registers) {
        switch (op) {
            case sim::trace::op_t::IALU:
            case sim::trace::op_t::FALU:
                op = sim::trace::op_t::VADD;
                break;
            case sim::trace::op_t::IMUL:
            case sim::trace::op_t::FMUL:
            case sim::trace::op_t::FMA:
                op = sim::trace::op_t::VMUL;
                break;
            case sim::trace::op_t::IDIV:
            case sim::trace::op_t::FDIV:
                op = sim::trace::op_t::VDIV;
                break;
            case sim::trace::op_t::OTHER:
                op = sim::trace::op_t::VMASK;
                break;
            default:
                break;
        }
    }
    translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op, 0, ra, rb, rc, rd);
}

void BasicDecoder::handle_nonmem_sve(void *drcontext, instr_t *current_instr, sim::trace::bbl_t &translated_bbl)
{
    Log::debug() << "[DEBUG] Instruction is " << std::hex <<  translated_bbl.macroops_.back().iword_ << std::dec;
    sim::trace::complexity_t complexity = translated_bbl.macroops_.back().get_sve_complexity();
    Log::debug() << "[DEBUG] Non memory SVE instruction has complexity " << static_cast<unsigned>(complexity);
    for (unsigned i = 0; i < static_cast<unsigned>(complexity); i++) {
        translated_bbl.ins_.emplace_back(instr_get_app_pc(current_instr), op_t::IALU, 0,
                REG_imm, REG_imm, REG_imm, REG_imm);
    }
    // The rest of the code would work in a narive SVE machine only.
    return;
    emulated_instr_t emulated;
    emulated.pc = 0;
    emulated.size = sizeof(emulated_instr_t);
    if (not drmgr_get_emulated_instr_data(current_instr, &emulated)) {
        std::cout << "Could not get SVE instruction pc." << std::endl;
    }
    instr_t sve_instruction;
    instr_init(drcontext, &sve_instruction);
    app_pc sve_pc = decode(drcontext, emulated.pc, &sve_instruction);
    if (sve_pc == NULL) {
        std::cout << "Could not encode SVE instruction." << std::endl;
    } else {
        std::cout << "SVE instruction encoded to a instr_t instruction." << std::endl;
        sim::trace::op_t op = transform_opcode(instr_get_opcode(current_instr));
        std::vector<reg_id_t> in_regs, out_regs;
        bool uses_vector_registers = false;
        std::cout << "Sources " << instr_num_srcs(current_instr) << std::endl;
        std::cout << "Destinations " << instr_num_dsts(current_instr) << std::endl;

        for (int i = 0; i < instr_num_srcs(current_instr); i++) {
            opnd_t operand = instr_get_src(current_instr, i);
            if (opnd_is_reg(operand)) {
                uses_vector_registers = uses_vector_registers or is_vector(operand);
                in_regs.emplace_back(opnd_get_reg(operand));
            }
        }
        for (int i = 0; i < instr_num_dsts(current_instr); i++) {
            opnd_t operand = instr_get_dst(current_instr, i);
            if (opnd_is_reg(operand)) {
                uses_vector_registers = uses_vector_registers or is_vector(operand);
                out_regs.emplace_back(opnd_get_reg(operand));
            }
        }
        uint8_t ra = in_regs.size() > 0 ? in_regs[0] : REG_imm;
        uint8_t rb = in_regs.size() > 1 ? in_regs[1] : REG_imm;
        uint8_t rc = in_regs.size() > 2 ? in_regs[2] : REG_imm;
        uint8_t rd = out_regs.size() > 0 ? out_regs[0] : REG_imm;
        if (uses_vector_registers) {
            switch (op) {
                case sim::trace::op_t::IALU:
                case sim::trace::op_t::FALU:
                    op = sim::trace::op_t::VADD;
                    break;
                case sim::trace::op_t::IMUL:
                case sim::trace::op_t::FMUL:
                case sim::trace::op_t::FMA:
                    op = sim::trace::op_t::VMUL;
                    break;
                case sim::trace::op_t::IDIV:
                case sim::trace::op_t::FDIV:
                    op = sim::trace::op_t::VDIV;
                    break;
                case sim::trace::op_t::OTHER:
                    op = sim::trace::op_t::VMASK;
                    break;
                default:
                    break;
            }
        }
        minst_t ins(instr_get_app_pc(current_instr), op, 0, ra, rb, rc, rd);
        std::cout << ins << std::endl;
    }
    instr_free(drcontext, &sve_instruction);
}

sim::trace::op_t BasicDecoder::transform_opcode(int dr_opcode)
{
#ifdef ARM_64
    return transform_opcode_aarch64(dr_opcode);
#else
    return transform_opcode_x86(dr_opcode);
#endif
}

#ifdef ARM_64
sim::trace::op_t BasicDecoder::transform_opcode_aarch64(int dr_opcode)
{
    if (dr_opcode >= OP_ld1 and dr_opcode <= OP_ldxrh) {
        return sim::trace::op_t::LD;
    } else if (dr_opcode >= OP_st1 and dr_opcode <= OP_stxrh) {
        return sim::trace::op_t::ST;
    } else if (dr_opcode == OP_mul or dr_opcode == OP_pmul or dr_opcode == OP_smulh or
               dr_opcode == OP_umulh or dr_opcode == OP_madd or dr_opcode == OP_smaddl or
               dr_opcode == OP_umaddl) {
        return sim::trace::op_t::IMUL;
    } else if (dr_opcode == OP_fmul or dr_opcode == OP_fmulx or dr_opcode == OP_fnmul or
               dr_opcode == OP_sqdmulh or dr_opcode == OP_sqrdmulh or dr_opcode == OP_fmadd or
               dr_opcode == OP_fnmadd) {
        return sim::trace::op_t::FMUL;
    } else if (dr_opcode == OP_udiv or dr_opcode == OP_sdiv) {
        return sim::trace::op_t::IDIV;
    } else if (dr_opcode == OP_fdiv) {
        return sim::trace::op_t::FDIV;
    } else if ((dr_opcode >= OP_b and dr_opcode <= OP_br) or dr_opcode == OP_cbnz or dr_opcode == OP_cbz) {
        return sim::trace::op_t::BR;
    } else if (dr_opcode >= OP_fmla and dr_opcode <= OP_fmlsl2) {
        return sim::trace::op_t::FMUL;
    } else if (dr_opcode >= OP_fabd and dr_opcode <= OP_fsub) {
        return sim::trace::op_t::FALU;
    } else {
        return sim::trace::op_t::IALU;
    }
}

sim::trace::op_t BasicDecoder::transform_opcode_x86(int dr_opcode)
{
    return sim::trace::op_t::INVALID;
}

bool BasicDecoder::is_vector(opnd_t &operand) {
    if (opnd_size_in_bytes(opnd_get_size(operand)) > 8) {
        return true;
    } else {
        return false;
    }
}

#else

sim::trace::op_t BasicDecoder::transform_opcode_aarch64(int dr_opcode)
{
    return sim::trace::op_t::INVALID;
}

sim::trace::op_t BasicDecoder::transform_opcode_x86(int dr_opcode)
{
    if (dr_opcode == OP_lds or dr_opcode == OP_ldmxcsr or dr_opcode == OP_lddqu) {
        return sim::trace::op_t::LD;
    } else if (dr_opcode == OP_stc or dr_opcode == OP_sti or dr_opcode == OP_std or
               dr_opcode == OP_str or dr_opcode == OP_stmxcsr or dr_opcode == OP_stos or
               dr_opcode == OP_stgi) {
        return sim::trace::op_t::ST;
    } else if (dr_opcode >=  OP_add and dr_opcode <= OP_arpl) {
        return sim::trace::op_t::IALU;
    } else if (dr_opcode == OP_mul or dr_opcode == OP_imul) {
        return sim::trace::op_t::IMUL;
    } else if (dr_opcode == OP_div or dr_opcode == OP_idiv) {
        return sim::trace::op_t::IDIV;
    } else if (dr_opcode >= OP_jo_short and dr_opcode <= OP_jnle_short) {
        return sim::trace::op_t::JMP;
    } else if (dr_opcode >= OP_call and dr_opcode <= OP_call_far_ind) {
        return sim::trace::op_t::JMP;  // We do not have CALL type...
    } else if (dr_opcode >= OP_jmp and dr_opcode <= OP_jecxz) {
        return sim::trace::op_t::JMP;
    } else if (dr_opcode >= OP_divps and dr_opcode <= OP_divsd) {
        return sim::trace::op_t::FDIV;
    } else if (dr_opcode == OP_fdiv or dr_opcode <= OP_fdivr or
               dr_opcode == OP_fidiv or dr_opcode <= OP_fidivr or
               dr_opcode == OP_fdivp or dr_opcode <= OP_fdivrp) {
        return sim::trace::op_t::FDIV;
    } else if (dr_opcode >= OP_vmulps and dr_opcode <= OP_vmulsd) {
        return sim::trace::op_t::VMUL;
    } else if ((dr_opcode >= OP_vdivps and dr_opcode <= OP_vdivsd) or
               (dr_opcode >= OP_divps and dr_opcode >= OP_divsd)) {
        return sim::trace::op_t::VDIV;
    } else if (dr_opcode >= OP_vmovss and dr_opcode <= OP_vextractf128) {
        return sim::trace::op_t::VADD;  // AVX instructions
    } else if (dr_opcode == OP_pmulhw or dr_opcode == OP_pmulhuw or
               dr_opcode == OP_pmuldq or dr_opcode == OP_pmuludq or
               dr_opcode == OP_pmuldq or dr_opcode == OP_pmullw or
               dr_opcode == OP_pmulhrw or dr_opcode == OP_pmulhrsw) {
        return sim::trace::op_t::VMUL;
    } else if ((dr_opcode >= OP_vfmadd132ps and dr_opcode <= OP_vfnmsub231sd) or
               dr_opcode == OP_pmaddwd or dr_opcode == OP_pmaddubsw) {
        return sim::trace::op_t::VMUL;  // VFMA
    } else if (dr_opcode >= OP_vfmaddsubps and dr_opcode <= OP_vfnmsubsd) {
        return sim::trace::op_t::VMUL;  // VFMA4
    } else if (dr_opcode >= OP_vfrczps and dr_opcode <= OP_vpshaq) {
        return sim::trace::op_t::VMASK;  // AMD XOP
    } else if (dr_opcode >= OP_vpgatherdd and dr_opcode <= OP_vpbroadcastq) {
        return sim::trace::op_t::VADD;  // AVX2 instructions
    } else {
        return sim::trace::op_t::OTHER;
    }
}

inline bool BasicDecoder::is_vector(opnd_t &operand) {
    return false;
}

inline void BasicDecoder::print_detailled_information(void *drcontext, instr_t *instr) {
    char *buffer = reinterpret_cast<char*>(malloc(512));
    bool reads = false, writes = false;
    instr_disassemble_to_buffer(drcontext, instr, buffer, 512);
    std::cout << "Emulated instruction " <<  buffer << std::endl;
    for (int i = 0; i < instr_num_srcs(instr); i++) {
        opnd_t operand = instr_get_src(instr, i);
        if (opnd_is_memory_reference(operand)) {
            reads = true;
        }
        if (opnd_is_memory_reference(operand)) {
            std::cout << "Source operand " << i << " is a memory reference of size "
                      << opnd_size_in_bytes(opnd_get_size(operand)) << " bytes." << std::endl;
        } else if (opnd_is_reg(operand)) {
            std::cout << "Source operand " << i << " is a register of size "
                      << opnd_size_in_bytes(opnd_get_size(operand)) << " bytes." << std::endl;
        } else if(opnd_is_immed(operand)) {
            std::cout << "Source operand " << i << " is an immmediate value." << std::endl;
        } else {
            std::cout << "Source operand " << i << " is of a yet uncatalogged type." << std::endl;
        }
    }
    for (int i = 0; i < instr_num_dsts(instr); i++) {
        opnd_t operand = instr_get_dst(instr, i);
        if (opnd_is_memory_reference(operand)) {
            writes = true;
        }
        if (opnd_is_memory_reference(operand)) {
            std::cout << "Destination operand " << i << " is a memory reference of size "
                      << opnd_size_in_bytes(opnd_get_size(operand)) << " bytes." << std::endl;
        } else if (opnd_is_reg(operand)) {
            std::cout << "Destination operand " << i << " is a register of size "
                      << opnd_size_in_bytes(opnd_get_size(operand)) << " bytes." << std::endl;
        } else if(opnd_is_immed(operand)) {
            std::cout << "Destination operand " << i << " is an immmediate value." << std::endl;
        } else {
            std::cout << "Destination operand " << i << " is of a yet uncatalogged type." << std::endl;
        }
    }
    std::cout << "[INFO] Instruction operands " << instr_num_srcs(instr) << " " << instr_num_dsts(instr) << std::endl;
    std::cout << "[INFO] Reads = " << (reads?"true":"false") << " writes = " << (writes?"true":"false") << std::endl;
    if (instr_reads_memory(instr)) {
        std::cout << "[INFO] Instruction reads memory." << std::endl;
    } else if (instr_writes_memory(instr)) {
        std::cout << "[INFO] Instruction writes memory." << std::endl;
    } else {
        std::cout << "[INFO] Instruction does not read nor write memory." << std::endl;
    }
    free(buffer);
}

#endif

}  // namespace dr
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_DR_BASICDECODER_IMPL_H_
