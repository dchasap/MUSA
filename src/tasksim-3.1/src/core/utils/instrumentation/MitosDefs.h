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

#ifndef CORE_UTILS_INSTRUMENTATION_MITOSDEFS_H_
#define CORE_UTILS_INSTRUMENTATION_MITOSDEFS_H_

namespace sim {
namespace utils {
namespace instrumentation {

enum class op_t : uint8_t { INVALID = 0,
               MACROOP = 1,
               // Integer operations 2-15
               IALU = 2,
               IMUL = 3,
               IDIV = 4,
               // Memory operations 16-31
               LD = 16,
               ST = 17,
               REMOVED_LD = 18,
               REMOVED_ST = 19,
               // Branch-jumps 32-47
               JMP = 32,
               BR = 33,
               IBR = 34,
               // Memory locks 48-63
               LDL = 48,  // Load + Lock
               STL = 49,  // Store + Lock
               LL = 50,   // Load link
               SC = 51,   // Store conditional
               // Floating point 64-79
               FALU = 64,
               FMUL = 65,
               FDIV = 66,
               FMA = 67,  // Floating point  multiply add
               // Vector operations 80-95
               VADD = 80,
               VMUL = 81,
               VDIV = 82,
               VMASK = 83,
               // Cache management operations ??? 96-111
               OTHER = 255 
						};

// TODO(Francesc): Until we adopt C++14 we cannot use an enum as  a key on an unordered_map
static const std::map<sim::utils::instrumentation::op_t, std::string> mitos_op_name({
        {sim::utils::instrumentation::op_t::INVALID, "INVALID"},
        {sim::utils::instrumentation::op_t::MACROOP, "MACROOP"},
        {sim::utils::instrumentation::op_t::IALU, "IALU"},
        {sim::utils::instrumentation::op_t::IMUL, "IMUL"},
        {sim::utils::instrumentation::op_t::IDIV, "IDIV"},
        {sim::utils::instrumentation::op_t::LD, "LD"},
        {sim::utils::instrumentation::op_t::ST, "ST"},
        {sim::utils::instrumentation::op_t::REMOVED_LD, "REM_LD"},
        {sim::utils::instrumentation::op_t::REMOVED_ST, "REM_ST"},
        {sim::utils::instrumentation::op_t::JMP, "JMP"},
        {sim::utils::instrumentation::op_t::BR, "BR"},
        {sim::utils::instrumentation::op_t::IBR, "IBR"},
        {sim::utils::instrumentation::op_t::LDL, "LDL"},
        {sim::utils::instrumentation::op_t::STL, "STL"},
        {sim::utils::instrumentation::op_t::LL, "LL"},
        {sim::utils::instrumentation::op_t::SC, "SC"},
        {sim::utils::instrumentation::op_t::FALU, "FALU"},
        {sim::utils::instrumentation::op_t::FMUL, "FMUL"},
        {sim::utils::instrumentation::op_t::FDIV, "FDIV"},
        {sim::utils::instrumentation::op_t::FMA, "FMA"},
        {sim::utils::instrumentation::op_t::VADD, "VADD"},
        {sim::utils::instrumentation::op_t::VMUL, "VMUL"},
        {sim::utils::instrumentation::op_t::VDIV, "VDIV"},
        {sim::utils::instrumentation::op_t::VMASK, "VMASK"},
        {sim::utils::instrumentation::op_t::OTHER, "OTHER"}
        });

enum MEMORY_LEVEL {	UNKNOWN = 0,	
										L1 = 1,
										L2 = 2,
										L3 = 3,
										LFB = 4,
										LOCAL_RAM = 5
									};
/*
enum OP_TYPE {	UNKNOWN = 0,	
								READ = 1,
								WRITE = 2
             };
*/

enum OP_TYPE	{	READ = 0, 
								WRITE = 1, 
								FLUSH = 2, 
								INVALIDATE = 3, 
								PREFETCH = 4, 
								CLEAR = 5, 
								LAST = 6 
							};

}  // namespace instrumentation
}  // namespace utils
}  // namespace sim

#endif // CORE_UTILS_INSTRUMENTATION_MITOSDEFS_H_

