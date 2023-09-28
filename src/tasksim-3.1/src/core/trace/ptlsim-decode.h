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


#ifndef CORE_TRACE_PTLSIM_DECODE_H_
#define CORE_TRACE_PTLSIM_DECODE_H_

/**
 * IntelDecoder is a interface class to use PTLSim's x86 to uops
 * decoder. The translate function translates a basic block at
 * address 'addr' with size 'size', and returns the list of
 * translated uops in the 'uops' vector.
 */

#include <unordered_map>

#define OP_nop ptlsim_OP_nop
#define OP_and ptlsim_OP_and
#define OP_xor ptlsim_OP_xor
#define OP_or ptlsim_OP_or
#define OP_add ptlsim_OP_add
#define OP_sub ptlsim_OP_sub
#define OP_bswap ptlsim_OP_bswap
#define OP_bt ptlsim_OP_bt
#define OP_bts ptlsim_OP_bts
#define OP_btr ptlsim_OP_btr
#define OP_btc ptlsim_OP_btc
#define OP_jmp ptlsim_OP_jmp
#define OP_shl ptlsim_OP_shl
#define OP_shr ptlsim_OP_shr
#define OP_sar ptlsim_OP_sar
#define OP_div ptlsim_OP_div
#define OP_fadd ptlsim_OP_fadd
#define OP_fsub ptlsim_OP_fsub
#define OP_fmul ptlsim_OP_fmul
#define OP_fdiv ptlsim_OP_fdiv
#define OP_fsqrt ptlsim_OP_fsqrt


#include "ptlsim/decode.h"
#include "ptlsim/ptlhwdef.h"

#undef OP_nop
#undef OP_and
#undef OP_xor
#undef OP_or
#undef OP_add
#undef OP_sub
#undef OP_bswap
#undef OP_bt
#undef OP_bts
#undef OP_btr
#undef OP_btc
#undef OP_jmp
#undef OP_shl
#undef OP_shr
#undef OP_sar
#undef OP_div
#undef OP_fadd
#undef OP_fsub
#undef OP_fmul
#undef OP_fdiv
#undef OP_fsqrt

#include "core/trace/common/minst.h"

namespace sim {
namespace trace {

static const std::unordered_map<unsigned, sim::trace::op_t> type({
        {ptlsim_OP_nop, op_t::OTHER},
        {OP_mov, op_t::OTHER},
        // Logical
        {ptlsim_OP_and, op_t::IALU},
        {OP_andnot, op_t::IALU},
        {ptlsim_OP_xor, op_t::IALU},
        {ptlsim_OP_or, op_t::IALU},
        {OP_nand, op_t::IALU},
        {OP_ornot, op_t::IALU},
        {OP_eqv, op_t::IALU},
        {OP_nor, op_t::IALU},
        // Mask, insert or extract bytes
        {OP_maskb, op_t::IALU},
        // Add and subtract
        {ptlsim_OP_add, op_t::IALU},
        {ptlsim_OP_sub, op_t::IALU},
        {OP_adda, op_t::IALU},
        {OP_suba, op_t::IALU},
        {OP_addm, op_t::IALU},
        {OP_subm, op_t::IALU},
        // Condition code logical ops
        {OP_andcc, op_t::IALU},
        {OP_orcc, op_t::IALU},
        {OP_xorcc, op_t::IALU},
        {OP_ornotcc, op_t::IALU},
        // Condition code movement and merging
        {OP_movccr, op_t::IALU},
        {OP_movrcc, op_t::IALU},
        {OP_collcc, op_t::IALU},
        // Simple shifting (restricted to small immediate 1..8)
        {OP_shls, op_t::IALU},
        {OP_shrs, op_t::IALU},
        {ptlsim_OP_bswap, op_t::IALU},
        {OP_sars, op_t::IALU},
        // Bit testing
        {ptlsim_OP_bt, op_t::IALU},
        {ptlsim_OP_bts, op_t::IALU},
        {ptlsim_OP_btr, op_t::IALU},
        {ptlsim_OP_btc, op_t::IALU},
        // Set and select
        {OP_set, op_t::IALU},
        {OP_set_sub, op_t::IALU},
        {OP_set_and, op_t::IALU},
        {OP_sel, op_t::IALU},
        {OP_sel_cmp, op_t::IALU},
        // Branches
        {OP_br, op_t::BR},
        {OP_br_sub, op_t::BR},
        {OP_br_and, op_t::BR},
        {ptlsim_OP_jmp, op_t::JMP},
        {OP_bru, op_t::IBR},
        {OP_jmpp, op_t::JMP},
        {OP_brp, op_t::IBR},
        // Checks
        {OP_chk, op_t::IALU},
        {OP_chk_sub, op_t::IALU},
        {OP_chk_and, op_t::IALU},
        // Loads and stores
        {OP_ld, op_t::LD},
        {OP_ldx, op_t::LD},
        {OP_ld_pre, op_t::OTHER},
        {OP_st, op_t::ST},
        {OP_mf, op_t::OTHER},
        // Shifts, rotates and complex masking
        {ptlsim_OP_shl, op_t::IALU},
        {ptlsim_OP_shr, op_t::IALU},
        {OP_mask, op_t::IALU},
        {ptlsim_OP_sar, op_t::IALU},
        {OP_rotl, op_t::IALU},
        {OP_rotr, op_t::IALU},
        {OP_rotcl, op_t::IALU},
        {OP_rotcr, op_t::IALU},
        // Multiplication
        {OP_mull, op_t::IMUL},
        {OP_mulh, op_t::IMUL},
        {OP_mulhu, op_t::IMUL},
        {OP_mulhl, op_t::IMUL},
        // Bit scans
        {OP_ctz, op_t::IALU},
        {OP_clz, op_t::IALU},
        {OP_ctpop, op_t::IALU},
        {OP_permb, op_t::IALU},
        // Integer divide and remainder step
        {ptlsim_OP_div, op_t::IDIV},
        {OP_rem, op_t::IDIV},
        {OP_divs, op_t::IDIV},
        {OP_rems, op_t::IDIV},
        // Minimum and maximum
        {OP_min, op_t::IALU},
        {OP_max, op_t::IALU},
        {OP_min_s, op_t::IALU},
        {OP_max_s, op_t::IALU},
        // Floating point
        {ptlsim_OP_fadd, op_t::FALU},
        {ptlsim_OP_fsub, op_t::FALU},
        {ptlsim_OP_fmul, op_t::FMUL},
        {OP_fmadd, op_t::FMA},
        {OP_fmsub, op_t::FMA},
        {OP_fmsubr, op_t::FMA},
        {ptlsim_OP_fdiv, op_t::FDIV},
        {ptlsim_OP_fsqrt, op_t::FDIV},
        {OP_frcp, op_t::FDIV},
        {OP_frsqrt, op_t::FDIV},
        {OP_fmin, op_t::FALU},
        {OP_fmax, op_t::FALU},
        {OP_fcmp, op_t::FALU},
        {OP_fcmpcc, op_t::FALU},
        {OP_fcvt_i2s_ins, op_t::FALU},
        {OP_fcvt_i2s_p, op_t::FALU},
        {OP_fcvt_i2d_lo, op_t::FALU},
        {OP_fcvt_i2d_hi, op_t::FALU},
        {OP_fcvt_q2s_ins, op_t::FALU},
        {OP_fcvt_q2d, op_t::FALU},
        {OP_fcvt_s2i, op_t::FALU},
        {OP_fcvt_s2q, op_t::FALU},
        {OP_fcvt_s2i_p, op_t::FALU},
        {OP_fcvt_d2i, op_t::FALU},
        {OP_fcvt_d2q, op_t::FALU},
        {OP_fcvt_d2i_p, op_t::FALU},
        {OP_fcvt_d2s_ins, op_t::FALU},
        {OP_fcvt_d2s_p, op_t::FALU},
        {OP_fcvt_s2d_lo, op_t::FALU},
        {OP_fcvt_s2d_hi, op_t::FALU},
        // Vector integer uops
        // size defines element size: 00 = byte, 01 = W16, 10 = W32, 11 = W64 (same as normal ops)
        {OP_vadd, op_t::VADD},
        {OP_vsub, op_t::VADD},
        {OP_vadd_us, op_t::VADD},
        {OP_vsub_us, op_t::VADD},
        {OP_vadd_ss, op_t::VADD},
        {OP_vsub_ss, op_t::VADD},
        {OP_vshl, op_t::VMASK},
        {OP_vshr, op_t::VMASK},
        {OP_vbt, op_t::VMASK},  // bit test vector (e.g. pack bit 7 of 8 bytes into 8-bit output, for pmovmskb)
        {OP_vsar, op_t::VMASK},
        {OP_vavg, op_t::VADD},
        {OP_vcmp, op_t::VADD},
        {OP_vmin, op_t::VADD},
        {OP_vmax, op_t::VADD},
        {OP_vmin_s, op_t::VADD},
        {OP_vmax_s, op_t::VADD},
        {OP_vmull, op_t::VMUL},
        {OP_vmulh, op_t::VMUL},
        {OP_vmulhu, op_t::VMUL},
        {OP_vmaddp, op_t::VMUL},
        {OP_vsad, op_t::VADD},
        {OP_vpack_us, op_t::VADD},
        {OP_vpack_ss, op_t::VADD}
        });

}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_PTLSIM_DECODE_H_
