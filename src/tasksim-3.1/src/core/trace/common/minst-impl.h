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


#ifndef CORE_TRACE_COMMON_MINST_IMPL_H_
#define CORE_TRACE_COMMON_MINST_IMPL_H_

#include <cassert>

namespace sim {
namespace trace {

inline
minst_t::minst_t() :
    type_(op_t::INVALID),
    iword_(0),
    isize_(4),
    pc_(0),
    ld_(0), st_(0),
    sve_(0),
    ld_size_(0), st_size_(0),
    ra_(0), rb_(0), rc_(0), rd_(0)
{}

inline minst_t::minst_t(sim::trace::op_t op, int ld, int st, int ld_size, int st_size) :
    type_(op),
    iword_(0),
    isize_(4),
    pc_(0),
    ld_(ld),
    st_(st),
    sve_(0),
    ld_size_(ld_size),
    st_size_(st_size),
    ra_(0),
    rb_(0),
    rc_(0),
    rd_(0)
{}

// Standard microop creator.
template<typename TransOp>
inline minst_t::minst_t(const uint8_t *iword, op_t op, int size, const TransOp &uop) :
        type_(op),
        iword_(0),
        isize_(0),
        pc_(reinterpret_cast<uint64_t>(iword)),
        ld_((op == op_t::LD || op == op_t::LDL) ? 1 : 0),
        st_((op == op_t::ST || op == op_t::STL) ? 1 : 0),
        sve_(0),
        ld_size_(size), st_size_(size),
        ra_(uop.ra), rb_(uop.rb), rc_(uop.rc), rd_(uop.rd)
{}

inline
minst_t::minst_t(const uint8_t *iword, op_t op, int size, int ra, int rb, int rc, int rd) :
        type_(op),
        iword_(0),
        isize_(0),
        pc_(reinterpret_cast<uint64_t>(iword)),
        ld_((op == op_t::LD || op == op_t::LDL) ? 1 : 0),
        st_((op == op_t::ST || op == op_t::STL) ? 1 : 0),
        sve_(0),
        ld_size_(size), st_size_(size),
        ra_(ra), rb_(rb), rc_(rc), rd_(rd)
{}

inline
minst_t::minst_t(const uint8_t *iword, unsigned size, uint64_t pc) :
        type_(op_t::MACROOP),
        iword_(0),
        isize_(size),
        pc_(pc == 0 ? reinterpret_cast<uint64_t>(iword) : pc),
        ld_(0), st_(0),
        sve_(0),
        ld_size_(0), st_size_(0),
        ra_(0), rb_(0), rc_(0), rd_(0)
{
    int i = 0;
    uint64_t displacement = 0;
    while (pc != 0 && size--) {
        iword_ = iword_ + (static_cast<uint64_t>(iword[i++]) << displacement);
        displacement += 8;
    }
}

inline
bool minst_t::isInvalid() const {
    return type_ == op_t::INVALID;
}

inline
bool minst_t::isValid() const {
    return type_ != op_t::INVALID;
}

inline
bool minst_t::isLoad() const {
    return type_ == op_t::LD or type_ == op_t::LDL;
}

inline
bool minst_t::isStore() const {
    return type_ == op_t::ST or type_ == op_t::STL;
}

inline
bool minst_t::is_vector() const {
  return type_ == op_t::VADD or type_ == op_t::VMASK or type_ == op_t::VMUL or type_ == op_t::VDIV;
}

inline
bool minst_t::is_memory_sve() const {
    if (static_cast<unsigned>(sve_) == 0) return false;
    unsigned first_byte = iword_ & 0xFE000000;
    switch (first_byte) {
        case 0x84000000:
            if ((iword_ & 0x00208000) == 0) {
                // First
                if ((iword_ & 0x01800000) == 0x01800000) {
                    return false;
                }
                // SVE 32-bit gather load (scalar plus 32-bit unscaled offsets)
                return true;
            }
            if ((iword_ & 0x0060E010) == 0x0000C010) {
                // SVE contiguous prefetch (scalar plus scalar)
                // SVE 32-bit gather prefetch (vector plus immediate)
                return true;
            }
            if ((iword_ & 0x00608000) == 0x00208000) {
                // SVE 32-bit gather load (vector plus immediate)
                return true;
            }
            if ((iword_ & 0x00408000) == 0x00408000) {
                // SVE load and broadcast element
                // ArmIE 19.0 seems to fail to store this specific instruction
                return false;
            }
            if ((iword_ & 0x01A08010) == 0x00200000) {
                // SVE 32-bit gather prefetch (scalar plus 32-bit scaled offsets)
                return true;
            }
            if ((iword_ & 0x01A08000) == 0x00A00000) {
                // SVE 32-bit gather load halfwords (scalar plus 32-bit scaled offsets)
                return true;
            }
            if ((iword_ & 0x01A08000) == 0x01200000) {
                //  SVE 32-bit gather load words (scalar plus 32-bit scaled offsets)
                return true;
            }
            if ((iword_ & 0x01C0E010) == 0x01800000) {
                // SVE load predicate register
                // return false;
                return true;
            }
            if ((iword_ & 0x01C0E000) == 0x01804000) {
                // SVE load vector register
                return true;
            }
            if ((iword_ & 0x01C08010) == 0x01C00000) {
                // SVE contiguous prefetch (scalar plus immediate)
                return true;
            }
            return false;
        case 0xA4000000:
            {
                unsigned three_bits = iword_ & 0xE000;
                if (three_bits == 0x0000) {
                    // SVE load and broadcast quadword (scalar plus scalar)
                    std::cout << "SVE load and bcast." << std::endl;
                    return true;
                } else if (three_bits == 0x2000) {
                    if ((iword_ & 0x00100000) == 0x00100000) {
                        return false;
                    } else {
                        // SVE load and broadcast quadword (scalar plus immediate)
                        return true;
                    }
                } else if (three_bits == 0x4000) {
                    // SVE contiguous load (scalar plus scalar)
                    // std::cout << "SVE contiguous load (scalar plus scalar)" << std::endl;
                    // return false;
                    return true;
                } else if (three_bits == 0x6000) {
                    // SVE contiguous first-fault load (scalar plus scalar)
                    return true;
                } else if (three_bits == 0xA000) {
                    // SVE contiguous load (scalar plus immediate)
                    // SVE contiguous non-fault load (scalar plus immediate)
                    return true;
                } else if (three_bits == 0xC000) {
                    // SVE load multiple structures (scalar plus scalar)
                    // SVE contiguous non-temporal load (scalar plus scalar)
                    return true;
                } else if (three_bits == 0xE000) {
                    // SVE load multiple structures (scalar plus immediate)
                    // SVE contiguous non-temporal load (scalar plus immediate)
                    return true;
                }
                return false;
            }
        case 0xC4000000:
            {
                unsigned fifteen = iword_ & 0x8000;
                unsigned highbits = iword_ & 0x00600000;
                if (fifteen == 0x00000) {
                    // SVE 64-bit gather load (scalar plus unpacked 32-bit unscaled offsets)
                    // SVE 64-bit gather load (scalar plus 32-bit unpacked scaled offsets)
                    // SVE 64-bit gather prefetch (scalar plus unpacked 32-bit scaled offsets)
                    return true;
                } else if (highbits == 0x00000000) {
                    if ((iword_ & 0xE010) == 0xE000) {
                        // SVE 64-bit gather prefetch (vector plus immediate)
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    // SVE 64-bit gather load (vector plus immediate)
                    // SVE 64-bit gather load (scalar plus 64-bit unscaled offsets)
                    // SVE 64-bit gather load (scalar plus 64-bit scaled offsets)
                    return true;
                }
            }
            return true;
        case 0xE4000000:
            {
                unsigned third_byte_3b = iword_ & 0xE000;
                if (third_byte_3b == 0x4000) {
                    // SVE contiguous store (scalar plus scalar)
                    // return false;
                    return true;
                }
                if (third_byte_3b == 0x6000) {
                    // SVE store multiple structures (scalar plus scalar)
                    return true;
                }
                if (third_byte_3b == 0xE000) {
                    // SVE contiguous store (scalar plus immediate)
                    // or SVE store multiple structures (scalar plus immediate)
                    return true;
                }
                // SVE contiguous non-temporal store (scalar plus scalar) already included
                // SVE contiguous non-temporal store (scalar plus immediate) already included.
                if (third_byte_3b == 0x8000 or third_byte_3b == 0xC000 ) {
                    // SVE 64-bit scatter store (scalar plus unpacked 32-bit unscaled offsets)
                    // SVE 64-bit scatter store (scalar plus unpacked 32-bit scaled offsets)
                    // SVE 32-bit scatter store (scalar plus 32-bit unscaled offsets)
                    // SVE 32-bit scatter store (scalar plus 32-bit scaled offsets)
                    return true;
                }
                if (third_byte_3b == 0xA000) {
                    // SVE 64-bit scatter store (scalar plus 64-bit unscaled offsets)
                    // SVE 64-bit scatter store (scalar plus 64-bit scaled offsets)
                    // SVE 64-bit scatter store (vector plus immediate)
                    // SVE 32-bit scatter store (vector plus immediate)
                    return true;
                }
                if (third_byte_3b == 0x0000 and ((iword_ & 0x1E000000) == 0x1C000000)) {
                    //  SVE store predicate register
                    return true;
                }
                // SVE store vector register already included
                return false;
            }
        default:
            return false;
    }
    // bool is_mem = (first_byte == 0x84000000) or (first_byte == 0xA4000000) or (first_byte == 0x94000000) or (first_byte == 0xE4000000);
    // return (static_cast<unsigned>(sve_) == 1) and is_mem;
}

inline
bool minst_t::is_ld1w() const
{
    if ((iword_ & 0xFFE0E000) == 0xA480A000) {
        return true;
    } else  if ((iword_ & 0xFFE0E000) == 0xA4808000) {
        return true;
    } else  if ((iword_ & 0xFFA0E000) == 0xC5200000) {
        return true;
    } else if ((iword_ & 0xFFE0E000) == 0xC5608000) {
        return true;
    } else if ((iword_ & 0xFFE0E000) == 0xC5408000) {
        return true;
    } else if ((iword_ & 0xFFA0E000) == 0xC5000000) {
        return true;
    } else if ((iword_ & 0xFFE0E000) == 0xC5208000) {
        return true;
    }
    return false; 
}


inline
bool minst_t::is_sve() const {
    return static_cast<unsigned>(sve_) == 1;
}

inline
complexity_t minst_t::get_sve_complexity() {
    if (not is_sve()) {
        return complexity_t::INVALID;
    }
    unsigned first_byte = iword_ & 0xFF000000;
    switch (first_byte) {
        case 0x04000000:
        {
            unsigned bit21 = iword_ & 0x00200000;
            unsigned lower = iword_ & 0x0000E000;
            if (bit21 == 0x0) {
                switch (lower) {
                    case 0x00000000:
                        // SVE Integer Binary Arithmetic - Predicated
                        // Complex division:
                        if ((iword_ & 0x00180000) == 0x000000) {
                            // ADD/SUB
                            return complexity_t::SIMPLE;
                        } else if ((iword_ & 0x00180000) == 0x00080000) {
                            // MIN/MAX/DIFFERENCE
                            return complexity_t::BITWISE;
                        } else if ((iword_ & 0x001C0000) == 0x00100000) {
                            // MUL
                            return complexity_t::SIMPLE;
                        } else if ((iword_ & 0x001C0000) == 0x00140000) {
                            // DIV
                            return complexity_t::MIDDLE;
                        } else if ((iword_ & 0x00180000) == 0x00180000) {
                            // BITWISE
                            return complexity_t::BITWISE;
                        }
                        break;
                    case 0x00002000:
                        // SVE Integer Reduction
                        return complexity_t::BITWISE;
                    case 0x00008000:
                        // SVE Bitwise Shift - Predicated
                        return complexity_t::BITWISE;
                    case 0x00009000:
                        // SVE Integer Unary Arithmetic - Predicated
                        return complexity_t::BITWISE;
                    default:
                        // SVE Integer Multiply-Add - Predicated
                        return complexity_t::SIMPLE;
                }
            } else {
                switch (lower) {
                    case 0x00000000:
                        // SVE Integer Arithmetic - Unpredicated
                        return complexity_t::BITWISE;
                    case 0x00002000:
                        // SVE Bitwise Logical - Unpredicated
                        return complexity_t::BITWISE;
                    case 0x00004000:
                        // SVE Index Generation & SVE Stack Allocation
                        return complexity_t::BITWISE;
                    case 0x00008000:
                        // SVE Bitwise Shift - Unpredicated
                        return complexity_t::BITWISE;
                    case 0x0000A000:
                        // SVE Address Generation & SVE Integer Misc - Unpredicated
                        return complexity_t::BITWISE;
                    default:
                        // SVE Element Count
                        return complexity_t::BITWISE;
                }
            }
            break;
        }
        case 0x05000000:
        {
            unsigned bit21 = iword_ & 0x00200000;
            if (bit21 == 0x0) {
                unsigned bit22 = iword_ & 0x00100000;
                if (bit22 == 0x0) {
                    // SVE Bitwise Immediate
                    return complexity_t::BITWISE;
                } else {
                    // SVE Integer Wide Immediate - Predicated
                    return complexity_t::BITWISE;
                }
            } else {
                unsigned lower = iword_ & 0x0000E000;
                switch (lower) {
                    case 0x00000000:
                        // SVE Permute Vector - Extract
                        return complexity_t::BITWISE;
                    case 0x00002000:
                        // SVE Permute Vector - Unpredicated
                        return complexity_t::BITWISE;
                    case 0x00004000:
                        // SVE Permute Predicate
                        return complexity_t::BITWISE;
                    case 0x00006000:
                        // SVE Permute Vector - Interleaving
                        return complexity_t::BITWISE;
                    case 0x00008000:
                    case 0x0000A000:
                        // SVE Permute Vector - Predicated
                        return complexity_t::BITWISE;
                    case 0x0000C000:
                    case 0x0000F000:
                        // SVE Vector Select
                        return complexity_t::BITWISE;
                    default:
                        return complexity_t::INVALID;
                }
            }
            break;
        }
        case 0x24000000:
        {
            unsigned bit21 = iword_ & 0x00200000;
            if (bit21 == 0x0) {
                // SVE Integer Compare - Vectors
                return complexity_t::BITWISE;
            } else {
                // SVE Integer Compare - Unsigned Immediate
                return complexity_t::BITWISE;
            }
            break;
        }
        case 0x25000000:
        {
            unsigned bit21 = iword_ & 0x00200000;
            if (bit21 == 0x0) {
                unsigned mask = iword_ & 0x0010C000;
                if (mask == 0x00004000) {
                    // SVE Predicate Logical Operations
                    return complexity_t::BITWISE;
                } else if (mask == 0x0000C000) {
                    // SVE Propagate Break
                    return complexity_t::BITWISE;
                } else if (mask == 0x00104000) {
                    // SVE Partition Break
                    return complexity_t::BITWISE;
                } else if (mask == 0x0010C000) {
                    // SVE Predicate Misc
                    return complexity_t::BITWISE;
                } else {
                    // SVE Integer Compare - Signed Immediate
                    return complexity_t::BITWISE;
                }
            } else {
                if ((iword_ & 0x0018C000) == 0x00008000) {
                    // SVE Predicate Count
                    return complexity_t::BITWISE;
                } else if ((iword_ & 0x0018F000) == 0x00088000) {
                    // SVE Inc/Dec by Predicate Count
                    return complexity_t::BITWISE;
                } else if ((iword_ & 0x0018F000) == 0x00089000) {
                    // SVE Write FFR
                    return complexity_t::BITWISE;
                } else if ((iword_ & 0x0000C000) == 0x00000000) {
                    // SVE Integer Compare - Scalars
                    return complexity_t::BITWISE;
                } else if ((iword_ & 0x0000C000) == 0x0000C000) {
                    // SVE Integer Wide Immediate - Unpredicated
                    return complexity_t::BITWISE;
                }
            }
            break;
        }
        case 0x44000000:
        {
            if ((iword_ & 0x00200000) == 0x00200000) {
                // SVE Multiply - Indexed
                return complexity_t::SIMPLE;
            }
            break;
        }
        case 0x64000000:
        {
            if ((iword_ & 0x003EE000) == 0x00008000) {
                // SVE Floating Point Complex Addition
                return complexity_t::SIMPLE;
            } else if ((iword_ & 0x00208000) == 0x0) {
                // SVE Floating Point Complex Multiply-Add
                return complexity_t::SIMPLE;
            } else if ((iword_ & 0x0020F800) == 0x00200000) {
                // SVE Floating Point Multiply-Add - Indexed
                return complexity_t::SIMPLE;
            } else if ((iword_ & 0x0020F000) == 0x00201000) {
                // SVE Floating Point Complex Multiply-Add - Indexed
                return complexity_t::SIMPLE;
            } else if ((iword_ & 0x0020FC00) == 0x00202000) {
                // SVE Floating Point Multiply - Indexed
                return complexity_t::SIMPLE;
            }
            break;
        }
        case 0x65000000:
        {
            if ((iword_ & 0x0038E000) == 0x00002000) {
                // SVE Floating Point Fast Reduction
                return complexity_t::SIMPLE;
            } else if ((iword_ & 0x0038F000) == 0x00083000) {
                // SVE Floating Point Unary Operations - Unpredicated
                return complexity_t::COMPLEX;
            } else if ((iword_ & 0x0038E000) == 0x00102000) {
                // SVE Floating Point Compare - with Zero
                return complexity_t::BITWISE;
            } else if ((iword_ & 0x0038E000) == 0x00182000) {
                // SVE Floating Point Accumulating Reduction
                return complexity_t::SIMPLE;
            } else if ((iword_ & 0x0020E000) == 0x00000000) {
                // SVE Floating Point Arithmetic - Unpredicated
                unsigned opcode = iword_ & 0x00001C00;
                switch(opcode) {
                    case 0x00000000:  // FADD
                    case 0x00000400:  // FSUB
                    case 0x00000800:  // FMUL
                    case 0x00000C00:  // FTSMUL
                        return complexity_t::SIMPLE;
                    case 0x00001800:  // FRECPS
                    case 0x00001C00:  // FRSQRTS
                        return complexity_t::COMPLEX;
                    default:
                        return complexity_t::INVALID;
                }
            } else if ((iword_ & 0x0020E000) == 0x00008000) {
                // SVE Floating Point Arithmetic - Predicated
                // Here it depends of the operation, it is fairly complex:
                if ((iword_ & 0x00100000) == 0x0) {
                    // SVE floating-point arithmetic (predicated)
                    unsigned opcode = iword_ & 0x000F0000;
                    if ((opcode == 0x000C0000) or (opcode == 0x000D0000)) {
                        // FDIVR or FDIV
                        return complexity_t::MIDDLE;
                    } else {
                        // FADD, DSUB, FMUL, FSUBR, FMAXNM, FMINNM, FMAX, FMIN, FABD, FSCALE, FMUX
                        return complexity_t::SIMPLE;
                    }
                } else if ((iword_ & 0x00181C00) == 0x00100000) {
                    // SVE floating-point trig multiply-add coefficienti
                    return complexity_t::COMPLEX;
                } else if ((iword_ & 0x001803C0) == 0x00180000) {
                    // SVE floating-point arithmetic with immediate (predicated)
                    return complexity_t::SIMPLE;
                }
            } else if ((iword_ & 0x0020E000) == 0x0000A000) {
                // SVE Floating Point Unary Operations - Predicated
                return complexity_t::COMPLEX;
            } else if ((iword_ & 0x00204000) == 0x00004000) {
                // SVE Floating Point Compare - Vectors
                return complexity_t::BITWISE;
            } else if ((iword_ & 0x00200000) == 0x00200000) {
                // SVE Floating Point Multiply-Add
                return complexity_t::SIMPLE;
            }
            break;
        }
    }
    return complexity_t::INVALID;
}

inline
void minst_t::vectorize() {
    // If it is a memory operation or already a vector operation, we cannot vectorize it.
    if (isLoad() or isStore() or is_vector()) {
        return;
    }
      // If none of the registers is a vector register, we cannot force vectorize it:
    if (not(is_vec(ra_) or is_vec(rb_) or is_vec(rc_) or is_vec(rd_))) {
      return;
    }
    // In otder to vectorize all registers must be vector or neutral (ntermediate, immediate or zero register):
    if (is_vec_or_neutral(ra_) and is_vec_or_neutral(rb_) and is_vec_or_neutral(rc_) and is_vec_or_neutral(rd_)) {
        switch (type_) {
        case op_t::IDIV:
        case op_t::FDIV:
            type_ = op_t::VDIV;
            break;
        case op_t::IMUL:
        case op_t::FMUL:
        case op_t::FMA:
            type_ = op_t::VMUL;
            break;
        case op_t::FALU:
        case op_t::IALU:
            type_ = op_t::VADD;
            break;
        default:
            type_ = op_t::VMASK;
        }
    }
}

inline
bool minst_t::is_vec(int reg) {
    // Hardcoded to avoid adding a ptlsim dependency on the simulation binaries.
    // if ((reg >= REG_xmml0 and reg <= REG_xmmh15)
    if (reg >= 16 and reg <= 47) {
        return true;
    } else {
        return false;
    }
}

inline
bool minst_t::is_vec_or_neutral(int reg) {
    // Hardcoded to avoid adding a ptlsim dependency on the simulation binaries.
    // if ((reg >= REG_xmml0 and reg <= REG_xmmh15) or reg == REG_imm or reg == REG_mem or reg == REG_zero){
    if ((reg >= 16 and reg <= 47) or reg == 75 or reg == 76 or reg == 63 or (reg >= 64 and reg <= 71)) {
        return true;
    } else {
        return false;
    }
}

inline
void minst_t::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char *>(this), sizeof(*this));
}

inline
void minst_t::read(std::istream &is)
{
    is.read(reinterpret_cast<char *>(this), sizeof(*this));
}

/** function to write a minst to an output stream */
inline
std::ostream &operator<<(std::ostream &os, const minst_t &item)
{
    os << static_cast<int>(item.type_) << ":"
       << std::hex << item.iword_ << std::dec << ":"
       << static_cast<int>(item.isize_) << ":"
       << std::hex << static_cast<uint64_t>(item.pc_) << std::dec << ":"
       << static_cast<int>(item.ld_) << ":"
       << static_cast<int>(item.st_) << ":"
       << static_cast<int>(item.sve_) << ":"
       << static_cast<int>(item.ld_size_) << ":"
       << static_cast<int>(item.st_size_) << ":"
       << static_cast<int>(item.ra_) << ":"
       << static_cast<int>(item.rb_) << ":"
       << static_cast<int>(item.rc_) << ":"
       << static_cast<int>(item.rd_);
    return os;
}

/** function to read a minst to an output stream */
inline
std::istream &operator>>(std::istream &is, minst_t &minst)
{
    char sep;
    unsigned ld, st, sve, ld_size, st_size, type, isize;
    uint64_t iword, pc;

    is >> type;
    minst.type_ = static_cast<op_t>(type);

    is >> sep;
    assert(sep == ':');
    std::ios::fmtflags fmt = is.flags();
    is >> std::hex >> iword;
    is.flags(fmt);
    minst.iword_ = iword;

    is >> sep;
    assert(sep == ':');
    is >> isize;
    minst.isize_ = isize;

    is >> sep;
    assert(sep == ':');
    fmt = is.flags();
    is >> std::hex >> pc;
    is.flags(fmt);
    minst.pc_ = pc;

    is >> sep;
    assert(sep == ':');
    is >> ld;
    minst.ld_ = ld;

    is >> sep;
    assert(sep == ':');
    is >> st;
    minst.st_ = st;
    assert(ld + st <= 2);

    is >> sep;
    assert(sep == ':');
    is >> sve;
    minst.sve_ = sve;

    is >> sep;
    assert(sep == ':');
    is >> ld_size;
    minst.ld_size_ = ld_size;

    is >> sep;
    assert(sep == ':');
    is >> st_size;
    minst.st_size_ = st_size;

    int reg;

    is >> sep >> reg;
    assert(sep == ':');
    minst.ra_ = reg;

    is >> sep >> reg;
    assert(sep == ':');
    minst.rb_ = reg;

    is >> sep >> reg;
    assert(sep == ':');
    minst.rc_ = reg;

    is >> sep >> reg;
    assert(sep == ':');
    minst.rd_ = reg;

    return is;
}

}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_COMMON_MINST_IMPL_H_
