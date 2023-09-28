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


class stats {
public:
    uint64_t bbl_, loads_, stores_, micro_;
    std::vector<sim::trace::bbl_t> v;
    stats() : bbl_(0), loads_(0), stores_(0), micro_(0) {}
    void print(const std::string &name, bool error) {
        std::cout << "Trace " << name << " at basic block " << bbl_
                  << " LD:ST:uOP:" << loads_ << ":" << stores_
                  << ":" << micro_ << std::endl;
        if (error) {
            for (size_t i = 0; i < v.size(); i++) {
                std::cout << "BBL " << i << " MacroOps " << v[i].macroops_.size() << " : " << std::endl;
                for (size_t j = 0; j < v[i].macroops_.size(); j++) {
                    std::cout << j << " " << std::hex << v[i].macroops_[j].iword_ << std::dec << std::endl;
                }
                std::cout << "micro " << v[i].ins_.size() << ": ";
                for (size_t j = 0; j < v[i].ins_.size(); j++) {
                    std::cout << j << " " << std::hex << v[i].ins_[j] << std::dec << std::endl;
                }
            }
        }
    }
    void printdiff(const stats& b) {
      std::cout << "Differential stats A: " << this->bbl_ << ":"
                << this->loads_ << ":" << this->stores_ << ":" << this->micro_
                << " B: " << b.bbl_ << ":" << b.loads_ << ":" << b.stores_
                << ":" << b.micro_ << std::endl;
    }
    void add_stats(sim::trace::bbl_t &bbl) {
      bbl_++;
      micro_ += bbl.ins_.size();
      for (size_t i = 0; i < bbl.ins_.size(); i++) {
        if (bbl.ins_[i].isLoad()) {
          loads_++;
        }
        if (bbl.ins_[i].isStore()) {
          stores_++;
        }
      }
      if (v.size() < 10) {
          v.push_back(bbl);
      }
    }

    stats& operator+=(const stats& other) {
        this->bbl_ += other.bbl_;
        this->loads_ += other.loads_;
        this->stores_ += other.stores_;
        this->micro_ += other.micro_;
        return *this;
    }
};

class bbl_store {
public:
    std::vector<sim::trace::minst_t> v_a, v_b;
    stats a_stats, b_stats;
    size_t processed;
    void add_data(sim::trace::bbl_t &bbl, stats &stats, std::vector<sim::trace::minst_t> &v) {
       stats.add_stats(bbl);
       for (size_t i = 0; i < bbl.macroops_.size(); i++) {
           v.push_back(bbl.macroops_[i]);
       }
    }

    bool clear_data() {
        size_t m = std::min(v_a.size(), v_b.size());
        for (size_t i = 0; i < m; i++) {
            if (v_a[i].iword_ != v_b[i].iword_) {
                std::cout << "[ERROR] MacroOps differ at MacroOp." << (processed + i) << "." << std::endl;
                std::cout << "A" << (v_a.size()-i);
                for (size_t j = i; j < v_a.size(); j++) {
                  std::cout << ":" << std::hex << v_a[j].iword_ << std::dec;
                }
                std::cout << std::endl;
                std::cout << "B" << (v_b.size()-i);
                for (size_t j = i; j < v_b.size(); j++) {
                  std::cout << ":" << std::hex << v_b[j].iword_ << std::dec;
                }
                std::cout << std::endl;
                a_stats.printdiff(b_stats);
                // a_stats.print("A", false);
                // b_stats.print("B", false);
                return false;
            }
        }
        processed += m;
        v_a.erase(v_a.begin(), v_a.begin() + m);
        v_b.erase(v_b.begin(), v_b.begin() + m);
        return true;
    }

    bool add_a(sim::trace::bbl_t &a) {
        add_data(a, a_stats, v_a);
        return clear_data();
    }

    bool add_b(sim::trace::bbl_t &b) {
        add_data(b, b_stats, v_b);
        return clear_data();
    }

    bool load_a() {
      return v_a.size() <= v_b.size();
    }

    bool load_b() {
      return v_a.size() > v_b.size();
    }

    void end_of_wd() {
      if (v_a.size()) {
        std::cout << "[ERROR] Trace A still got " << v_a.size() << " macroops at the end." << std::endl;
      } else  if (v_b.size()) {
        std::cout << "[ERROR] Trace B still got " << v_b.size() << " macroops at the end." << std::endl;
      } else {
        std::cout << "[OK] Both traces have the same macroOps." << std::endl;
      }
      bool debug = false;
      if ((a_stats.loads_ == b_stats.loads_) and (a_stats.micro_ != b_stats.micro_)) {
         debug = true;
      }
      a_stats.printdiff(b_stats);
      a_stats.print("A", debug);
      b_stats.print("B", debug);
    }

    bbl_store() : a_stats(), b_stats(), processed(0) {}
};


void usage(const char* cmd)
{
    std::cerr << "Usage: " << cmd << " trace_a_filename trace_b_fillename" << std::endl;
    std::cerr << "It will compare both traces and output the number of: " << std::endl
              << " - Basic blocks" << std::endl
              << " - Loads" << std::endl
              << " - Stores" << std::endl
              << " - MicroInstructions" << std::endl;
    std::cerr << "It will also output if the instructions encoded are different." << std::endl;
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
        std::cout << "Traces have a different number of tasks" << num_wds_a << " vs " << num_wds_b << "." << std::endl;
        // std::abort();
    }
    std::map<unsigned, unsigned> bbl_a_true_id;
    std::map<unsigned, unsigned> bbl_b_true_id;
    stats total_a_stats, total_b_stats;
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

            bbl_store store;

            while (not wd_finished) {
              bool ok = true;
                if (store.load_a()) {
                  sim::trace::bbl_t bbl_a = reader_a.get_next_bbl(subt_a);
                  if (bbl_a.id_.id_ == 0) {
                    sim::trace::bbl_t bbl_b = reader_b.get_next_bbl(subt_b);
                    while (bbl_b.id_.id_ != 0) {
                      bbl_b = reader_b.get_next_bbl(subt_b);
                      store.add_b(bbl_b);
                    }
                    wd_finished = true;
                    store.end_of_wd();
                  } else {
                    ok = store.add_a(bbl_a);
                  }
                }
                if ((not wd_finished) and ok and store.load_b()) {
                  sim::trace::bbl_t bbl_b = reader_b.get_next_bbl(subt_b);
                  if (bbl_b.id_.id_ == 0) {
                    sim::trace::bbl_t bbl_a = reader_a.get_next_bbl(subt_a);
                    while (bbl_a.id_.id_ != 0) {
                      bbl_a = reader_a.get_next_bbl(subt_a);
                      store.add_a(bbl_a);
                    }
                    wd_finished = true;
                    store.end_of_wd();
                  } else {
                    ok = store.add_b(bbl_b);
                  }
                }
                if (not ok) {
                    // We clear the A trace:
                    sim::trace::bbl_t bbl_a = reader_a.get_next_bbl(subt_a);
                    while (bbl_a.id_.id_ != 0) {
                      bbl_a = reader_a.get_next_bbl(subt_a);
                    }
                    // We clear the B trace:
                    sim::trace::bbl_t bbl_b = reader_b.get_next_bbl(subt_b);
                    while (bbl_b.id_.id_ != 0) {
                      bbl_b = reader_b.get_next_bbl(subt_b);
                    }
                    wd_finished = true;
                }
            }  // END of WHILE
            total_a_stats += store.a_stats;
            total_b_stats += store.b_stats;
            total_a_stats.printdiff(total_b_stats);
        }  // END of WHILE read event
        if (not error_in_wd) {
            std::cout << "Traces correlate properly in all basic blocs for Work Descriptor "
                      << subt_a.wd_id << std::endl;
        }
    }  // ENF of FOR WD.
    std::cout << "==========================" << std::endl;
    std::cout << "Trace comparison finished." << std::endl;
    std::cout << "==========================" << std::endl;
    total_a_stats.printdiff(total_b_stats);
    if (not error) {
        std::cout << "Traces correlate in all basic blocs." << std::endl;
    }
    return 0;
}
