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

#include "instr_breakdown_utils.h"

#include <unistd.h>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/trace/common/minst.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/ts/types.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/MemParser.h"
#include "core/trace/types.h"

#include "core/utils/Log.h"

using sim::utils::Log;

bool equal_or_next(uint8_t first, uint8_t second)
{
    return (first == second) or (first+1 == second);
}

void process_memory_phase(trace_t &reader,
        const sim::trace::ts::subtrace_t &subt,
        std::map<sim::trace::op_t, size_t> &counter,
        std::map<sim::trace::op_t, size_t> &counter_by_wd,
        int vector_register_length)
{
    sim::trace::bbl_t bbl, next;
    next = reader.get_next_bbl(subt);
    while (true) {
        bbl = next;
        if (bbl.id_.id_ == 0) {
            break;
        }
        if (vector_register_length > 64) {
            // Here we first eliminarte duplicated vector ops and extra memory loads/stores.
            std::vector<sim::trace::minst_t> new_ins;
            std::vector<sim::trace::minst_t> &old_ins = bbl.ins_;
            bool changed = false;
            for (size_t i = 0; i < old_ins.size(); i++) {
                if (i + 1 < old_ins.size() and old_ins[i].is_vector() and old_ins[i+1].is_vector() and
                       equal_or_next(old_ins[i].ra_, old_ins[i+1].ra_) and
                       equal_or_next(old_ins[i].rb_, old_ins[i+1].rb_) and
                       equal_or_next(old_ins[i].rc_, old_ins[i+1].rc_) and
                       equal_or_next(old_ins[i].rd_, old_ins[i+1].rd_)) {
                    changed = true;
                    new_ins.push_back(old_ins[i]);
                    i++;
                    continue;
                } else if (old_ins[i].type_ == sim::trace::op_t::IALU) {
                    if (old_ins.size() > i+1 and
                            (old_ins[i+1].type_ == sim::trace::op_t::REMOVED_LD or
                             old_ins[i+1].type_ == sim::trace::op_t::REMOVED_ST) and
                            old_ins[i].rd_ == old_ins[i+1].ra_) {
                        changed = true;
                        continue;
                    }
                } else if (old_ins[i].type_ == sim::trace::op_t::REMOVED_LD or
                           old_ins[i].type_ == sim::trace::op_t::REMOVED_ST) {
                    changed = true;
                    continue;
                }
                new_ins.push_back(old_ins[i]);
            }
            if (changed) {
                bbl.ins_.swap(new_ins);
            }
            // Here we eliminate entire basic blocks due to vectorization:
            bool got_next = false;
            for (int i = 0; i < (vector_register_length/128)-1; i++) {
                next = reader.get_next_bbl(subt);
                if (next.id_.id_ != bbl.id_.id_) {
                    got_next = true;
                    break;
                }
            }
            if (not got_next) {
                next = reader.get_next_bbl(subt);
            }
        } else {
            next = reader.get_next_bbl(subt);
        }

        // Here we count the instructions:
        for (size_t i = 0; i < bbl.ins_.size(); i++) {
            counter[bbl.ins_[i].type_]++;
            counter_by_wd[bbl.ins_[i].type_]++;
        }
    }
}

void print_stats(int wd, const std::map<sim::trace::op_t, size_t> &stats)
{
    if (wd == -1) {
        std::cout << "==== Dynamic instruction breakdown for the selected trace: ====" << std::endl;
    } else {
        std::cout << "==== Dynamic instruction breadown for WD " << wd << " ====" << std::endl;
    }
    for (auto it = stats.begin(); it != stats.end(); it++) {
        if (sim::trace::opname.find(it->first) != sim::trace::opname.end()) {
            std::cout << "  " << sim::trace::opname.at(it->first) << " " << it->second << std::endl;
        } else {
            std::cout << "  " << static_cast<unsigned>(it->first) << " " << it->second << std::endl;
        }
    }
    std::cout << "===================================================================" << std::endl;
}

void usage(const char *command, const std::unordered_map<char, std::string> &options)
{
    std::cerr << "Usage: " << command << " [options] -i trace_file_prefix" << std::endl
              << "    Options:" << std::endl;
    std::unordered_map<char, std::string>::const_iterator it;

    for (it = options.begin(); it != options.end(); ++it) {
        std::cerr << "\t-" << it->first << "\t" << it->second << std::endl;
    }
}
