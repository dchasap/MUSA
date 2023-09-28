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

#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/trace/ts/Trace.h"
#include "core/trace/ts/types.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/types.h"
#include "core/trace/common/types.h"
#include "core/utils/CheckTrace.h"


class bbl_mapper {
private:
    std::unordered_map<unsigned, std::vector<sim::trace::bbl_t>> internal_repo;
    int current_size;
    unsigned add_new(const sim::trace::bbl_t &bbl) {
        sim::trace::bbl_t new_bbl;
        new_bbl.id_ = current_size++;
        new_bbl.macroops_ = bbl.macroops_;
        int size = new_bbl.macroops_.size();
        internal_repo[size].emplace_back(new_bbl);
        return current_size - 1;
    }

    bool equal(const std::vector<sim::trace::minst_t> & macro_a, const std::vector<sim::trace::minst_t> &macro_b) {
        if (macro_a.size() != macro_b.size()) {
            return false;
        }
        for (size_t i = 0; i < macro_a.size(); i++) {
            if (macro_a[i].iword_ != macro_b[i].iword_) {
                return false;
            }
        }
        return true;
    }

public:
    unsigned get_true_id(const sim::trace::bbl_t &bbl) {
        int size = bbl.macroops_.size();
        if (internal_repo.find(size) == internal_repo.end()) {
            return add_new(bbl);
        }
        for (size_t i = 0; i < internal_repo[size].size(); i++) {
            if (equal(bbl.macroops_, internal_repo[size][i].macroops_)) {
                return internal_repo[size][i].id_.id_;
            }
        }
        return add_new(bbl);
    }

    bbl_mapper() : current_size(1) {
        sim::trace::bbl_t null_bbl;
        null_bbl.id_ = 0;
        internal_repo[0] = std::vector<sim::trace::bbl_t>(1, null_bbl);
    }
};


void usage(const char* cmd)
{
    std::cerr << "Usage: " << cmd << " pin_trace_file_prefix dr_trace_prefix" << std::endl;
    std::cerr << "Tool used to compare all instructions from two different traces." << std::endl;
}

void print_relevant(const sim::trace::bbl_t &bbl) {
    std::cout << bbl.id_.id_ << ":" << bbl.ins_.size() << "m:" << bbl.macroops_.size() << "M:";
    for (size_t i = 0; i < bbl.macroops_.size(); i++) {
        uint64_t s = static_cast<uint64_t>(bbl.macroops_[i].isize_);
        uint64_t word = bbl.macroops_[i].iword_;
        if (i) { std::cout << ":";}
        std::cout << "0x";
        std::cout << std::hex << std::setw(2*s) << std::setfill('0') << word << std::dec;
    }
    std::cout << "---";
    for (unsigned i = 0; i < bbl.ins_.size(); i++) {
      std::cout << "-";
      if (bbl.ins_[i].ld_ == 1) {
         std::cout << "L" << static_cast<unsigned>(bbl.ins_[i].ld_size_);
      } else if (bbl.ins_[i].st_ == 1) {
         std::cout << "S" << static_cast<unsigned>(bbl.ins_[i].st_size_);
      } else {
        std::cout << "N0";
      }
    }
    std::cout << std::endl;;
}

bool compare_macros(const sim::trace::bbl_t &a, const sim::trace::bbl_t &b) {
    unsigned int n = std::min(a.macroops_.size(), b.macroops_.size());
    for (unsigned int i = 0; i < n; i++) {
        if (a.macroops_[i].iword_ != b.macroops_[i].iword_) {
            return false;
        }
    }
    return true;
}

void print_error(const sim::trace::bbl_t &a, const sim::trace::bbl_t &b,
                 const sim::trace::bbl_t &a2, const sim::trace::bbl_t &b2,
                 bbl_mapper &mapper, unsigned bbl_counter, const sim::trace::bbl_t &prev)
{
    std::cout << "[ERROR] Basics blocs are intrinsically different at basic_bloc " << bbl_counter << std::endl;
    std::cout << "Previous: ";
    print_relevant(prev);
    std::cout << "A1: ";
    print_relevant(a);
    std::cout << "B1: ";
    print_relevant(b);
    std::cout << "Next WDs:" << std::endl;
    std::cout << "A2: ";
    print_relevant(a2);
    std::cout << "B2: ";
    print_relevant(b2);
    unsigned true_id_a = mapper.get_true_id(a);
    unsigned true_id_b = mapper.get_true_id(b);
    unsigned next_true_id_a = mapper.get_true_id(a2);
    unsigned next_true_id_b = mapper.get_true_id(b2);
    std::cout << "Trace A ids: " << true_id_a << " " << next_true_id_a << std::endl;
    std::cout << "Trace B ids: " << true_id_b << " " << next_true_id_b << std::endl;
}

void print_remaining(const sim::trace::bbl_t &a, const sim::trace::bbl_t &b,
        sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream, sim::trace::rle::TraceInternal> &reader_a,
        sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream, sim::trace::rle::TraceInternal> &reader_b,
        const sim::trace::ts::subtrace_t subt_a, const sim::trace::ts::subtrace_t &subt_b,
        const int limit = 20)
{
    bool a_left = (a.id_.id_ != 0), b_left = (b.id_.id_ != 0);
    if (a.id_.id_ != 0) {
        std::cout << "Next " << limit << " basic blocs for trace A:" << std::endl;
        int c = 0;
        while (c++ < limit) {
            const sim::trace::bbl_t &aa = reader_a.get_next_bbl(subt_a);
            if (aa.id_.id_ == 0) {
              a_left = false;
              break;
            }
            std:: cout << c << " ";
            print_relevant(aa);
        }
        std::cout << "====================================================" << std::endl;
    }
    if (b.id_.id_ != 0) {
        int c = 0;
        std::cout << "Next " << limit << " basic blocs for trace B:" << std::endl;
        while (c++ < limit) {
            const sim::trace::bbl_t &bb = reader_b.get_next_bbl(subt_b);
            if (bb.id_.id_ == 0) {
                b_left = false;
                break;
            }
            std:: cout << c << " ";
            print_relevant(bb);
        }
        std::cout << "====================================================" << std::endl;
    }
    uint64_t a_c = 0, b_c = 0, a_i = 0, b_i = 0;
    int last_a = 0;
    while (a_left) {
      const sim::trace::bbl_t &aa = reader_a.get_next_bbl(subt_a);
      if (aa.id_.id_ == 0) break;
      a_c++;
      a_i += aa.macroops_.size();
      last_a = a.macroops_.size();
    }

    while (b_left) {
      const sim::trace::bbl_t &bb = reader_b.get_next_bbl(subt_b);
      if (bb.id_.id_ == 0) break;
      b_c++;
      b_i += bb.macroops_.size();
    }
    // In order to remove the last basic block from the DR trace.
    if (a_c != 0) {
      a_c--;
      a_i -= last_a;
    }
    std::cout << "They still had " << a_c << " and " << b_c << " basic blocks left." << std::endl;
    std::cout << "They had " << a_i << " and " << b_i << " macroops respectively." << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        usage(argv[0]);
        return argc == 1;
    }

    // First we load the traces.
    std::string name_a(argv[1]);
    std::string name_b(argv[2]);

    // Check that input trace exists.
    sim::utils::check_trace_exists(name_a);
    sim::utils::check_trace_exists(name_b);

    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::rle::TraceInternal> reader_a(name_a.c_str());

    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::rle::TraceInternal> reader_b(name_b.c_str());

    // We cannot load statically the information about the basic blocs
    std::map<unsigned, unsigned> wd_map;
    wd_map[1] = 1;
    const unsigned num_wds_a = reader_a.get_total_number_of_tasks();
    const unsigned num_wds_b = reader_b.get_total_number_of_tasks();
    if (num_wds_a != num_wds_b) {
        std::cout << "[ERROR] Traces have a different number of tasks" << num_wds_a
                  << " vs " << num_wds_b << "." << std::endl;
        std::abort();
    }
    std::map<unsigned, unsigned> bbl_a_true_id;
    std::map<unsigned, unsigned> bbl_b_true_id;

    bbl_mapper mapper;
    bool error = false;
    for (unsigned wd_index = 0; wd_index < num_wds_a; wd_index++) {
        const sim::trace::ts::subtrace_t subt_a = reader_a.get_task_trace_by_id(wd_index);
        const sim::trace::ts::subtrace_t subt_b = reader_b.get_task_trace_by_wd_id(subt_a.wd_id);
        if (subt_a.wd_id != subt_b.wd_id) {
            std::cout << "[WARNING] Subtrace " << wd_index << " point to different WD identifiers: "
                      << subt_a.wd_id << " vs " << subt_b.wd_id << std::endl;
        }
        if (subt_a.name_id != subt_b.name_id) {
            std::cout << "[WARNING] Subtrace " << wd_index << " point to different task types: "
                      << subt_a.name_id << " vs " << subt_b.name_id << std::endl;
        }
        std::cout << "Analyzing Work Descriptor " << subt_a.wd_id << std::endl;
        std::cout << subt_a << " " << subt_b << std::endl;
        // Trick to compare DR and PIN:

        bool error_in_wd = false;
        sim::trace::ts::event_t event_a, event_b;
        while (reader_a.get_task_event(subt_a.wd_id, event_a)) {
            bool wd_finished = false;
            unsigned bbl_counter = 0;
            reader_b.get_task_event(subt_b.wd_id, event_b);
            if (event_a.type == sim::trace::ts::type_t::CREATE_TASK and
                    event_b.type == sim::trace::ts::type_t::CREATE_TASK and
                    event_a.u.create_task.wd_id  != event_b.u.create_task.wd_id) {
                std::cout << "[WTF] Tasks have different Work Descriptor identifiers." << std::endl;
                std::cout << "      DR creates WD " << event_a.u.create_task.wd_id
                          << " and PIN creates " << event_b.u.create_task.wd_id << std::endl;
            }
            if (not(event_a.type == sim::trace::ts::type_t::PHASE and event_a.u.phase.subtrace_id != sim::trace::ts::NO_SUBTRACE)) {
                std::cout << "Not analyzing non phase event " << event_a << " " << event_b << std::endl;
                continue;
            }
            std::cout << "Comparing event " << event_a << "(" << reader_a.get_task_name(subt_a.name_id)
                      << ") with " << event_b << "(" << reader_b.get_task_name(subt_b.name_id) << ")." << std::endl;

            reader_b.get_next_bbl(subt_b);
            sim::trace::bbl_t prev;
            while (not wd_finished) {
                bbl_counter++;
                sim::trace::bbl_t bbl_a = reader_a.get_next_bbl(subt_a);
                sim::trace::bbl_t bbl_b = reader_b.get_next_bbl(subt_b);

                // To hide REP MOV Errors:
                if (bbl_b.macroops_.size() == 1 and bbl_b.macroops_[0].iword_ == 0xf348ab) {
                    while (bbl_b.macroops_.size() == 1 and bbl_b.macroops_[0].iword_ == 0xf348ab) {
                        bbl_b = reader_b.get_next_bbl(subt_b);
                    }
                    bbl_b = reader_b.get_next_bbl(subt_b);
                }
                if (bbl_a.id_.id_ == 0 or bbl_b.id_.id_ == 0) {
                    wd_finished = true;
                    if (bbl_a.id_.id_ != 0) {
                        int count = 1;
                        while (reader_a.get_next_bbl(subt_a).id_.id_ != 0) {
                            count++;
                        }
                        if (count > 1) {
                            std::cout << "[ERROR] Trace A still had " << count << " basic blocs  of "
                                      << bbl_counter << " on WD " << subt_a.wd_id << std::endl;
                        } else {
                            std::cout << "[OK] Neither trace had more basic blocs than the other("
                                      << bbl_counter << ")." << std::endl;
                        }
                    } else if (bbl_b.id_.id_ != 0) {
                        int count = 1;
                        while (reader_b.get_next_bbl(subt_b).id_.id_ != 0) {
                            count++;
                        }
                        if (count > 0) {
                            std::cout << "[ERROR] Trace B still had " << count << " basic blocs  of "
                                      << bbl_counter << " on WD " << subt_b.wd_id << std::endl;
                        } else {
                            std::cout << "[OK] Neither trace had more basic blocs than the other("
                                      << bbl_counter << ")." << std::endl;
                        }
                    } else {
                        std::cout << "[OK] Neither trace had more basic blocs than the other("
                                  << bbl_counter << ")." << std::endl;
                    }
                } else {
                    unsigned true_id_a = mapper.get_true_id(bbl_a);
                    unsigned true_id_b = mapper.get_true_id(bbl_b);

                    if (true_id_a != true_id_b) {
                        sim::trace::bbl_t comb_a = bbl_a;
                        sim::trace::bbl_t comb_b = bbl_b;
                        // While we have different number of insturctions and the existing ones are the same:
                        while (comb_a.macroops_.size() != comb_b.macroops_.size() and compare_macros(comb_a, comb_b)) {
                            if (comb_a.macroops_.size() < comb_b.macroops_.size()) {
                                sim::trace::bbl_t aux = reader_a.get_next_bbl(subt_a);
                                if (aux.id_.id_ == 0) break;
                                for (unsigned int i = 0; i < aux.macroops_.size(); i++) {
                                    comb_a.macroops_.push_back(aux.macroops_[i]);
                                }
                            } else {
                                sim::trace::bbl_t aux = reader_b.get_next_bbl(subt_b);
                                if (aux.id_.id_ == 0) break;
                                for (unsigned int i  = 0; i < aux.macroops_.size(); i++) {
                                    comb_b.macroops_.push_back(aux.macroops_[i]);
                                }
                            }
                        }
                        true_id_a = mapper.get_true_id(comb_a);
                        true_id_b = mapper.get_true_id(comb_b);

                        if (true_id_a != true_id_b) {
                            std::cout << "[ERROR] Traces have different instructions." << std::endl;
                            const sim::trace::bbl_t &next_bbl_a = reader_a.get_next_bbl(subt_a);
                            const sim::trace::bbl_t &next_bbl_b = reader_b.get_next_bbl(subt_b);
                            print_error(comb_a, comb_b, next_bbl_a, next_bbl_b, mapper, bbl_counter, prev);
                            print_remaining(next_bbl_a, next_bbl_b, reader_a, reader_b, subt_a, subt_b);
                            error_in_wd = true;
                            error = true;
                            wd_finished = true;
                        }
                    }  // END of IF true_id_a != true_id_b
                }  // END of IF id == 0/else
                prev = bbl_a;
            }  // END of WHILE
        }  // END of WHILE read event
        if (not error_in_wd) {
            std::cout << "Traces correlate properly in all basic blocs for Work Descriptor "
                      << subt_a.wd_id << std::endl;
        }
    }  // ENF of FOR WD.
    std::cout << "==========================" << std::endl;
    std::cout << "Trace comparison finished." << std::endl;
    std::cout << "==========================" << std::endl;
    if (not error) {
        std::cout << "Traces correlate in all basic blocs." << std::endl;
    }
    return 0;
}
