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

class wd_stats {
public:
    uint64_t loads, stores, items, macroops, microops;
    wd_stats() : loads(0), stores(0), items(0), macroops(0), microops(0) {}
    void add_data(const sim::trace::bbl_t &bbl) {
        macroops += bbl.macroops_.size();
        microops += bbl.ins_.size();
        for (size_t i = 0; i < bbl.ins_.size(); i++) {
            if (bbl.ins_[i].isLoad()) {
                items++;
                loads++;
            } else if (bbl.ins_[i].isStore()) {
                items++;
                stores++;
            }
        }
    }
    wd_stats& operator+=(const wd_stats& b) {
        loads += b.loads;
        stores += b.stores;
        items += b.items;
        macroops += b.macroops;
        microops += b.microops;
        return *this;
    }
};


void usage(const char* cmd)
{
    std::cerr << "Usage: " << cmd << " trace_a_filename trace_b_filename" << std::endl;
    std::cerr << "It will read both traces, and output for each Work Descriptor:" << std::endl
              << " - Memory accesses" << std::endl
              << " - Loads" << std::endl
              << " - Stores" << std::endl
              << " - Instructions" << std::endl
              << " - MicroInstructions" << std::endl;
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
    wd_stats a_total, b_total;
    if (num_wds_a != num_wds_b) {
        std::cout << "[ERROR] Traces have a different number of tasks"
                  << num_wds_a << " vs " << num_wds_b << "." << std::endl;
        std::abort();
    }

    for (unsigned wd_index = 0; wd_index < num_wds_a; wd_index++) {
        wd_stats a_stats, b_stats;
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

        sim::trace::ts::event_t event_a, event_b;
        while (reader_a.get_task_event(subt_a.wd_id, event_a)) {
            reader_b.get_task_event(subt_b.wd_id, event_b);
            if (event_a.type == sim::trace::ts::type_t::CREATE_TASK and
                    event_b.type == sim::trace::ts::type_t::CREATE_TASK and
                    event_a.u.create_task.wd_id  != event_b.u.create_task.wd_id) {
                std::cout << "[ERROR] Tasks have different Work Descriptor identifiers." << std::endl;
                std::cout << "        A creates WD " << event_a.u.create_task.wd_id << " and B creates "
                          << event_b.u.create_task.wd_id << std::endl;
            }
            if (not(event_a.type == sim::trace::ts::type_t::PHASE and event_a.u.phase.subtrace_id != sim::trace::ts::NO_SUBTRACE)) {
                std::cout << "Not analyzing non phase event " << event_a << " " << event_b << std::endl;
                continue;
            }
            std::cout << "Comparing event " << event_a << "(" << reader_a.get_task_name(subt_a.name_id)
                      << ") with " << event_b << "(" << reader_b.get_task_name(subt_b.name_id) << ")." << std::endl;

            // We purge one bbl from the B trace:
            reader_b.get_next_bbl(subt_b);
            while (true) {
                sim::trace::bbl_t bbl_b = reader_b.get_next_bbl(subt_b);
                if (bbl_b.id_.id_ != 0) {
                    b_stats.add_data(bbl_b);
                } else {
                    break;
                }
            }

            sim::trace::bbl_t prev = reader_a.get_next_bbl(subt_a);
            while (true) {
                sim::trace::bbl_t bbl_a = reader_a.get_next_bbl(subt_a);
                if (bbl_a.id_.id_ != 0) {
                    a_stats.add_data(prev);
                    prev = bbl_a;
                } else {
                    break;
                }
            }
        }
        std::cout << "Data for WD " << subt_a.wd_id << std::endl;
        std::cout << "Items:" << a_stats.items << ":" << b_stats.items << std::endl;
        std::cout << "Loads:" << a_stats.loads << ":" << b_stats.loads << std::endl;
        std::cout << "Stores:" << a_stats.stores << ":" << b_stats.stores << std::endl;
        std::cout << "Macros:" << a_stats.macroops << ":" << b_stats.macroops << std::endl;
        std::cout << "Micros:" << a_stats.microops << ":" << b_stats.microops << std::endl;
        a_total += a_stats;
        b_total += b_stats;
    }
    std::cout << "==========================" << std::endl;
    std::cout << "Trace comparison finished." << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Total data" << std::endl;
    std::cout << "Items:" << a_total.items << ":" << b_total.items << std::endl;
    std::cout << "Loads:" << a_total.loads << ":" << b_total.loads << std::endl;
    std::cout << "Stores:" << a_total.stores << ":" << b_total.stores << std::endl;
    std::cout << "Macros:" << a_total.macroops << ":" << b_total.macroops << std::endl;
    std::cout << "Micros:" << a_total.microops << ":" << b_total.microops << std::endl;
    return 0;
}
