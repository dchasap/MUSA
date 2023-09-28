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
#include <vector>
#include <unordered_map>
#include <utility>
#include <stack>
#include <string>

#include "core/trace/ts/Trace.h"
#include "core/trace/ts/types.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/MemParser.h"
#include "core/trace/types.h"
#include "core/utils/CheckTrace.h"
#include "core/utils/Log.h"

using sim::utils::Log;

//
// TASK ACCESS TO INPUTS TEST
//
enum AccessType {OTHER = 0, IN = 1, OUT = 2, INOUT = IN | OUT, STACK, TOTAL, CONCURRENT, COMMUTATIVE, MAX};

template <typename streamtype>
class PrettyPrint {
private:
    streamtype &out_;

public:
    explicit PrettyPrint(streamtype &st): out_(st) {}
    void print_number_of_wds(unsigned num_wds)
    {
        out_ << "num_wds = " << num_wds << std::endl;
    }
    void print_wd_summary(unsigned wd_id, const char *wd_name, const std::vector<sim::trace::ts::dep_t> & deps,
            const sim::trace::addr_t &addr, uint64_t count[MAX])
    {
        out_ << "WD [" << wd_id << "] " << wd_name << "\n" << "  num deps: " << deps.size() << std::endl;
        std::vector<sim::trace::ts::dep_t>::const_iterator it;
        for (it = deps.begin(); it != deps.end(); ++it) {
            out_ << "    " << *it << std::endl;
        }
        out_ << "  stack ptr = 0x" << std::hex << addr << std::dec << '\n'
             << "  mem accesses:\n"
             << "    total " << count[TOTAL] << std::endl;

        if (count[TOTAL] > 0) {
            out_ << "    input " << count[IN   ]  << " (" << (count[IN   ] * 100. / count[TOTAL]) << "%)\n"
                 << "    outpt " << count[OUT  ]  << " (" << (count[OUT  ] * 100. / count[TOTAL]) << "%)\n"
                 << "    inout " << count[INOUT]  << " (" << (count[INOUT] * 100. / count[TOTAL]) << "%)\n";
            if (count[COMMUTATIVE] > 0) {
                out_ << "      of which commutative " << count[COMMUTATIVE]
                     << " (" << (count[COMMUTATIVE] * 100. / count[TOTAL]) << "%)\n";
            }
            if (count[CONCURRENT ] > 0) {
                out_ << "      of which concurrent  " << count[CONCURRENT ]
                     << " (" << (count[CONCURRENT ] * 100. / count[TOTAL]) << "%)\n";
            }
            out_ << "    stack " << count[STACK]  << " (" << (count[STACK] * 100. / count[TOTAL]) << "%)\n"
                 << "    other " << count[OTHER]  << " (" << (count[OTHER] * 100. / count[TOTAL]) << "%)" << std::endl;
        }
    }

    void print_totals(uint64_t count[MAX])
    {
        out_ << "Totals:" << std::endl
             << "    total " << count[TOTAL] << std::endl;

        if (count[TOTAL] > 0) {
            out_ << "    input " << count[IN   ]  << " (" << (count[IN   ] * 100. / count[TOTAL]) << "%)\n"
                 << "    outpt " << count[OUT  ]  << " (" << (count[OUT  ] * 100. / count[TOTAL]) << "%)\n"
                 << "    inout " << count[INOUT]  << " (" << (count[INOUT] * 100. / count[TOTAL]) << "%)\n";
            if (count[COMMUTATIVE] > 0) {
                out_ << "      of which commutative " << count[COMMUTATIVE]
                     << " (" << (count[COMMUTATIVE] * 100. / count[TOTAL]) << "%)\n";
            }
            if (count[CONCURRENT ] > 0) {
                out_ << "      of which concurrent  " << count[CONCURRENT ]
                     << " (" << (count[CONCURRENT ] * 100. / count[TOTAL]) << "%)\n";
            }
            out_ << "    stack " << count[STACK]  << " (" << (count[STACK] * 100. / count[TOTAL]) << "%)\n"
                 << "    other " << count[OTHER]  << " (" << (count[OTHER] * 100. / count[TOTAL]) << "%)" << std::endl;
        }
    }
};


template <typename streamtype>
class ParserPrint {
private:
    streamtype &out_;

public:
    explicit ParserPrint(streamtype &st): out_(st) {}
    void print_number_of_wds(unsigned num_wds) { out_ << num_wds << std::endl; }
    void print_wd_summary(unsigned wd_id, const char *wd_name, const std::vector<sim::trace::ts::dep_t> & deps,
            const sim::trace::addr_t &addr, uint64_t count[MAX])
    {
        char sep = ':';
        out_ << wd_id
             << sep << count[TOTAL]
             << sep << count[IN]
             << sep << count[OUT]
             << sep << count[INOUT]
             << sep << count[STACK]
             << sep << count[OTHER]
             << std::endl;
    }
};

template <template <typename> class StreamType, template <template <typename> class,
        typename> class TraceInternal>
const sim::trace::ts::subtrace_t &get_task_subtrace(
        sim::trace::ts::FileTraceReader<StreamType, TraceInternal> &reader, unsigned wd_id)
{
    bool has_event;
    sim::trace::ts::event_t event;
    while ((has_event = reader.get_task_event(wd_id, event)) == true and
           (event.type != sim::trace::ts::PHASE or event.u.phase.subtrace_id == sim::trace::ts::NO_SUBTRACE)) { }

    assert(has_event == true);
    Log::debug2() << "wd " << wd_id << " -> subtrace id " << event.u.phase.subtrace_id;
    return reader.get_task_trace_by_id(event.u.phase.subtrace_id);
}


bool check_dimension_range(int ndim, const std::vector<nanos_region_dimension_internal_t> &dims,
        sim::trace::addr_t start_addr, sim::trace::addr_t addr)
{
    /* FIXME: The checking of dimension ranges requires testing */
    /* FIXME: It must be extended to use the region lower bound */
    assert(0);

    /* The first dimension can be checked directly */
    if (ndim == 0) {
        return (addr >= start_addr and addr < (start_addr + dims[0].accessed_length));
    }

    /* Calculate how many bytes to advance for each element */
    unsigned stride = 1;
    for (int16_t s = 0; s < ndim; ++s) {
        stride *= dims[s].size;
    }

    /* Check if the address is within any of the elements of this dimension */
    for (size_t s = 0; s < dims[ndim].accessed_length; ++s) {
        if (check_dimension_range(ndim - 1, dims, start_addr, addr) == true) {
            return true;
        }
        start_addr += stride;
    }

    return false;
}


template <template <typename> class otype, typename streamtype>
void task_access_to_inputs(const std::string &filename, streamtype &st)
{
    otype<streamtype> out(st);
    uint64_t num_phases, total_phases, wd_count[MAX], total_count[MAX];

    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> reader(filename.c_str());
    sim::trace::MemParser<sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal>> mem_parser(reader);


    const unsigned num_wds = reader.get_total_number_of_tasks();
    out.print_number_of_wds(num_wds);

    memset(total_count, 0, sizeof(wd_count));
    total_phases = 0;
    for (unsigned w = 0; w < num_wds; ++w) {
        const sim::trace::ts::subtrace_t subtrace = reader.get_task_trace_by_id(w);
        std::vector<sim::trace::ts::dep_t> deps;

        if (subtrace.wd_id != 1)
            deps = reader.get_task_dependencies({subtrace.wd_id});

        const sim::trace::addr_t stack =
                (reinterpret_cast<sim::trace::addr_t>(subtrace.stack) & ~(0x1FFFLL)) + 8192;  // align to 8K

        memset(wd_count, 0, sizeof(wd_count));
        num_phases = 0;

        sim::trace::ts::event_t event;
        while (reader.get_task_event(subtrace.wd_id, event)) {
            if (event.type == sim::trace::ts::PHASE and reader.has_burst_instr_trace(event.u.phase)) {
                ++num_phases;
                assert(event.u.phase.subtrace_id >= 0 && static_cast<unsigned>(event.u.phase.subtrace_id) == w);

                sim::trace::mitem_t mitem = mem_parser.get_next_mem_item(subtrace);
                while (mitem.op_ != sim::trace::mitem_t::INVALID) {
                    wd_count[TOTAL]++;
                    sim::trace::addr_t addr = mitem.addr_;
                    if (addr == 0) {
                        std::cerr << "WARNING: NULL-address mitem " << mitem << ", in WD " << subtrace.wd_id
                                  << " during phase " << event << std::endl;
                    }
                    bool found = false;
                    for (auto it = deps.cbegin(); it != deps.cend(); ++it) {
                        sim::trace::addr_t dep_addr = reinterpret_cast<sim::trace::addr_t>(it->address);

                        /* Move the pointer to the first accessed element in the region */
                        dep_addr += it->offset;

                        /* get the size of the dependency */
                        std::size_t dep_size = 1;
                        std::size_t holes = 0;
                        for (uint16_t s = 0; s < it->dimensions.size(); s++) {
                            dep_size *= it->dimensions[s].size;
                            holes += (it->dimensions[s].size - it->dimensions[s].accessed_length);
                        }
                        sim::trace::addr_t end_addr = dep_addr + dep_size;

                        /* Check if the address is outside the potential range */
                        if (addr < dep_addr or addr >= end_addr) {
                            continue;
                        }

                        /* If the region is contiguous, the simple comparison above suffices */
                        /* Otherwise, check recursively all the dimensions within the range */
                        found = holes == 0 || check_dimension_range(it->dimensions.size() - 1, it->dimensions, dep_addr, addr);

                        assert(it->flags.input || it->flags.output);  // it must be either input or output or both
                        wd_count[it->flags.input * IN | it->flags.output * OUT]++;

                        // Breaking down special cases of inout
                        if (it->flags.concurrent) {
                            assert(it->flags.input && it->flags.output);
                            wd_count[CONCURRENT]++;
                        } else if (it->flags.commutative) {
                            assert(it->flags.input && it->flags.output);
                            wd_count[COMMUTATIVE]++;
                        }
                        break;
                    }
                    if (not found) {
                        if (stack >= addr and addr > (stack - 8192)) {
                            wd_count[STACK]++;
                        } else {
                            wd_count[OTHER]++;
                        }
                    }
                    mitem = mem_parser.get_next_mem_item(subtrace);
                }
            } else if (event.type == sim::trace::ts::PHASE and event.u.phase.subtrace_id == sim::trace::ts::EMPTY_SUBTRACE) {
                sim::trace::mitem_t mitem = mem_parser.get_next_mem_item(subtrace);
                assert(mitem.addr_ == 0 and mitem.op_ == sim::trace::mitem_t::op_t::INVALID);
                _unused(mitem);
            }
        }
        total_phases += num_phases;
        for (int i = 0; i < MAX; i++) {
            total_count[i] += wd_count[i];
        }
        const char *name = reader.get_task_name(subtrace.name_id);
        out.print_wd_summary(subtrace.wd_id, name, deps, stack, wd_count);
    }
    out.print_totals(total_count);
}


//
// TRACE INFO / DEBUGGING OUTPUT / FULL TRACE PARSE TESTS
//

template <typename streamtype>
class DebugOutput {
private:
    streamtype &out_;

public:
    explicit DebugOutput(streamtype &st): out_(st) {}
    void start_wd(unsigned wd_id) {
        out_ << "Start WD id = " << wd_id << std::endl;
    }
    void on_event(unsigned wd_id, const sim::trace::ts::event_t &event) {
        switch (event.type) {
            case sim::trace::ts::PHASE:
                out_ << "Event: type " << event.type << " phase, " << event.u.phase.burst_id
                          << ", " << event.u.phase.duration << ", " << event.u.phase.subtrace_id << std::endl;
                break;
            case sim::trace::ts::CREATE_TASK:
                out_ << "Event: type " << event.type << " create task, wd " << event.u.create_task.wd_id << std::endl;
                break;
            case sim::trace::ts::TASK_WAIT:
                out_ << "Event: type " << event.type << " taskwait" << std::endl;
                break;
            case sim::trace::ts::WAIT_ON:
                out_ << "Event: type " << event.type << " taskwait on, subtrace " << event.u.wait_on.subtrace_id << std::endl;
                break;
            case sim::trace::ts::SET_LOCK:
                out_ << "Event: type " << event.type << " set lock " << std::hex << event.u.lock.address << std::dec << std::endl;
                break;
            case sim::trace::ts::UNSET_LOCK:
                out_ << "Event: type " << event.type << " unset lock " << std::hex << event.u.lock.address << std::dec << std::endl;
                break;
            default:
                out_ << "Event: type " << event.type << std::endl;
        }
    }
    void on_phase(unsigned wd_id, const sim::trace::ts::phase_t &phase) {
        out_ << "Phase: " << phase.burst_id << std::endl;
    }
    void on_phase_subtrace(unsigned wd_id, const sim::trace::ts::subtrace_t &subt) {
        out_ << "Subtrace: wd" << subt.wd_id << ", dict " << subt.bbl_dict_id
                   << ", trace " << subt.bbl_trace_id << ", mem " << subt.mem_trace_id << std::endl;
    }
    void on_bbl(unsigned wd_id, const sim::trace::bbl_t &bbl) {
        out_ << "bbl: " << bbl.id_ << " (" << bbl.ins_.size() << " instrs)"
             << " pc " << std::hex << bbl.instr_ptr_ << std::dec << std::endl;
    }
    void on_inst(unsigned wd_id, unsigned bbl_index, const sim::trace::minst_t &inst,
            const std::vector<sim::trace::addr_t> &mems) {
        out_ << bbl_index << ":" << static_cast<int16_t>(inst.ld_)
                          << ":" << static_cast<int16_t>(inst.st_)
                          << ":" << static_cast<int16_t>(inst.ld_size_)
                          << ":" << static_cast<int16_t>(inst.st_size_);
        for (int i = 0; i < inst.ld_ + inst.st_; ++i) {
            out_ << " " << std::hex << mems[i] << std::dec;
        }
        out_ << std::endl;
    }
    void finished() {
        out_ << "Finished" << std::endl;
    }
};

template <typename streamtype>
class TraceInfo {
private:
    streamtype &out_;
    struct WDInfo {
        unsigned wd_id;
        std::unordered_map<unsigned, unsigned> ev_type_hist_;
        std::unordered_map<unsigned, unsigned> ph_burst_hist_;
        std::unordered_map<unsigned, unsigned> bbl_hist_;
        unsigned num_instructions_;
        unsigned num_loads_;
        unsigned num_stores_;
        explicit WDInfo(unsigned id) : wd_id(id), ev_type_hist_(), ph_burst_hist_(),
                num_instructions_(0), num_loads_(0), num_stores_(0) {}
    };
    std::unordered_map<unsigned, WDInfo> _wds;

public:
    explicit TraceInfo(streamtype &st) : out_(st), _wds() {}
    void start_wd(unsigned wd_id) {
        _wds.insert(std::pair<unsigned, WDInfo>(wd_id, WDInfo(wd_id)));
    }
    WDInfo &get_wd_info(unsigned wd_id) {
        typename std::unordered_map<unsigned, WDInfo>::iterator wd_it =
                _wds.find(wd_id);
        if (wd_it == _wds.end()) {
            std::cerr << "on_event of non-existing wd_id " << wd_id << std::endl;
            exit(1);
        }
        return wd_it->second;
    }
    void update_hist(std::unordered_map<unsigned, unsigned> &map, unsigned key, unsigned val) {
        std::unordered_map<unsigned, unsigned>::iterator it = map.find(key);
        if (it == map.end())
            map[key] = val;
        else
            map[key] += val;
    }
    void on_event(unsigned wd_id, const sim::trace::ts::event_t &event) {
        WDInfo &wd_info = get_wd_info(wd_id);
        update_hist(wd_info.ev_type_hist_, event.type, 1);
    }
    void on_phase(unsigned wd_id, const sim::trace::ts::phase_t &phase) {
        WDInfo &wd_info = get_wd_info(wd_id);
        update_hist(wd_info.ph_burst_hist_, phase.burst_id, 1);
    }
    void on_phase_subtrace(unsigned wd_id, const sim::trace::ts::subtrace_t &subt) {}
    void on_bbl(unsigned wd_id, const sim::trace::bbl_t &bbl) {
        WDInfo &wd_info = get_wd_info(wd_id);
        update_hist(wd_info.bbl_hist_, bbl.id_.id_, 1);
    }
    void on_inst(unsigned wd_id, unsigned bbl_index, const sim::trace::minst_t &inst,
                const std::vector<sim::trace::addr_t> &mems) {
        WDInfo &wd_info = get_wd_info(wd_id);
        ++wd_info.num_instructions_;
        wd_info.num_loads_ += inst.ld_;
        wd_info.num_stores_ += inst.st_;
    }
    void print_hist(const std::string &type, const std::unordered_map<unsigned, unsigned> &hist,
            unsigned max) {
        out_ << "  " << type << " (" << hist.size() << "):" << std::endl;
        unsigned count = 0;
        std::unordered_map<unsigned, unsigned>::const_iterator it;
        for (it = hist.begin(); it != hist.end() and count < max; ++it, ++count)
            out_ << "    " << it->first << ':' << it->second << std::endl;
    }
    void finished() {
        unsigned num_loads = 0, num_stores = 0, num_instrs = 0;
        for (unsigned i = 0; i < _wds.size(); ++i) {
            out_ << "WD [" << i << ']' << std::endl;
            const WDInfo &wd_info = get_wd_info(i);
            print_hist("Phase", wd_info.ev_type_hist_, 10);
            print_hist("Burst", wd_info.ph_burst_hist_, 10);
            // TODO(???): BBLs hist should be shown from most frequent to less
            print_hist("BBL", wd_info.bbl_hist_, 10);
            out_ << "  Num. instr: " << wd_info.num_instructions_ << std::endl
                 << "  Num. loads: " << wd_info.num_loads_ << std::endl
                 << "  Num. store: " << wd_info.num_stores_ << std::endl
                 << "  Mem. total: " << (wd_info.num_loads_ + wd_info.num_stores_) << std::endl;
            num_instrs += wd_info.num_instructions_;
            num_loads += wd_info.num_loads_;
            num_stores += wd_info.num_stores_;
        }
        out_ << "Totals [" << _wds.size() << "]" << std::endl
             << "  Num. instr: " << num_instrs << std::endl
             << "  Num. loads: " << num_loads << std::endl
             << "  Num. store: " << num_stores << std::endl;
        // Show totals
    }
};

template <typename streamtype>
class FullTraceParse {
private:
    streamtype &out_;
public:
    explicit FullTraceParse(streamtype &st) : out_(st) {}
    void start_wd(unsigned wd_id) {}
    void on_event(unsigned wd_id, const sim::trace::ts::event_t &event) {}
    void on_phase(unsigned wd_id, const sim::trace::ts::phase_t &phase) {}
    void on_phase_subtrace(unsigned wd_id, const sim::trace::ts::subtrace_t &subt) {}
    void on_bbl(unsigned wd_id, const sim::trace::bbl_t &bbl) {}
    void on_inst(unsigned wd_id, unsigned bbl_index, const sim::trace::minst_t &inst,
            const std::vector<sim::trace::addr_t> &mems) {}
    void finished() {
        out_ << "Finished" << std::endl;
    }
};


template<template <typename> class otype, typename streamtype>
void full_trace_parse(const std::string &filename, streamtype &st)
{
    otype<streamtype> out(st);
    sim::trace::ts::FileTrace<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> reader(filename.c_str());
    sim::trace::ts::event_t event;

    const unsigned TOTAL_WDS = reader.get_total_number_of_tasks();  // remove the -1 when the bug is fixed
    for (unsigned w = 0; w < TOTAL_WDS; ++w) {
        const sim::trace::ts::subtrace_t subt = reader.get_task_trace_by_id(w);
        unsigned wd_id = subt.wd_id;
        out.start_wd(wd_id);
        while (reader.get_task_event(wd_id, event)) {
            out.on_event(subt.wd_id, event);
            if (event.type == sim::trace::ts::PHASE) {
                out.on_phase(wd_id, event.u.phase);
            }
            if (event.type == sim::trace::ts::PHASE and reader.has_burst_instr_trace(event.u.phase)) {
                assert(event.u.phase.subtrace_id >= 0 && w == static_cast<unsigned>(event.u.phase.subtrace_id));
                out.on_phase_subtrace(wd_id, subt);
                while (true)
                {
                    sim::trace::bbl_t bbl = reader.get_next_bbl(subt);
                    out.on_bbl(wd_id, bbl);
                    if (bbl.id_.id_ == 0) {
                        sim::trace::addr_t addr = reader.get_next_mem(subt);
                        assert(addr == 0);
                        _unused(addr);
                        break;
                    }
                    unsigned ins = 0;
                    for (std::vector<sim::trace::minst_t>::iterator it = bbl.ins_.begin(); it != bbl.ins_.end(); it++) {
                        assert(it->ld_ >= 0 and it->ld_ <=2);
                        assert(it->st_ >= 0 and it->st_ <=1);
                        if (it->isValid()) {
                            std::vector<sim::trace::addr_t> mems;
                            for (int i = 0; i < (it->ld_ + it->st_); ++i) {
                                sim::trace::addr_t addr = reader.get_next_mem(subt);
                                mems.push_back(addr);
                                if (addr == 0) {
                                    std::cerr << "Warning: missing or NULL address" << std::endl;
                                }
                            }
                            out.on_inst(wd_id, ins, *it, mems);
                            ++ins;
                        }
                    }
                }
            } else if (event.type == sim::trace::ts::PHASE and event.u.phase.subtrace_id == sim::trace::ts::EMPTY_SUBTRACE) {
                sim::trace::bbl_t bbl = reader.get_next_bbl(subt);
                assert(bbl.id_.id_ == 0);
                sim::trace::addr_t addr = reader.get_next_mem(subt);
                assert(addr == 0);
                _unused(addr);
            }
        }
    }
    out.finished();
}


//
// CHECK MEM ADDRESSES FOR TASKSIM
//

void check_mem_addresses_for_TaskSim(const std::string &filename)
{
    static const uint64_t Modules_Area  = 0xC000000000000001LLU;
    static const uint64_t Vsyscall_Area = 0xFFFFFFFFFF000000LLU;
    static const uint64_t Max_Address   = 0xFFFFFFFFFFFFFFFFLLU;

    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> reader(filename.c_str());
    sim::trace::ts::event_t event;

    unsigned num_wds = reader.get_total_number_of_tasks();
    if (num_wds == 0) {
        std::cerr << filename << " not found" << std::endl;
        exit(1);
    }

    bool success = true;
    for (unsigned w = 0; w < num_wds; ++w) {
        const sim::trace::ts::subtrace_t subtrace = reader.get_task_trace_by_id(w);

        const sim::trace::addr_t stack =
                (reinterpret_cast<sim::trace::addr_t>(subtrace.stack) & ~(0x1FFFLL)) + 8192;  // align to 8K

        sim::trace::addr_t addr;
        while ((addr = reader.get_next_mem(subtrace)) != 0) {
            if ((addr >= 0x0 and addr < Modules_Area) or (addr >= Vsyscall_Area and addr < Max_Address))
                continue;

            std::cout << "FAILED: in WD " << w << " stack " << std::hex << stack << " address " << addr << std::dec
                      << " is out of TaskSim main memory ranges" << std::endl;
            success = false;
        }
    }
    if (success) std::cout << "OK" << std::endl;
}

//
// CHECK MEM TRACE
//
template <typename streamtype>
void check_mem_trace(const std::string &filename, streamtype &st, unsigned vector_length, unsigned trace_vector_length, unsigned register_file_length)
{
    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> reader(filename.c_str());

    const unsigned num_wds = reader.get_total_number_of_tasks();
    if (num_wds == 0) {
        std::cerr << filename << " not found" << std::endl;
        exit(1);
    }
    sim::trace::MemParser<sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal>> mem_parser(reader, vector_length,
             trace_vector_length, register_file_length);
    sim::trace::ts::event_t event;
    uint64_t total_items = 0, total_loads = 0, total_stores = 0, total_instr = 0, total_phases = 0;
    for (unsigned w = 0; w < num_wds; ++w) {
        uint64_t num_items = 0, num_loads = 0, num_stores = 0, num_instr = 0, num_phases = 0;
        const sim::trace::ts::subtrace_t subt = reader.get_task_trace_by_id(w);
        while (reader.get_task_event(subt.wd_id, event)) {
            if (event.type == sim::trace::ts::PHASE and reader.has_burst_instr_trace(event.u.phase)) {
                num_phases++;
                assert(event.u.phase.subtrace_id >= 0 && static_cast<unsigned>(event.u.phase.subtrace_id) == w);
                sim::trace::mitem_t item = mem_parser.get_next_mem_item(subt);

                while (item.op_ != sim::trace::mitem_t::INVALID) {
                    num_instr += item.ins_;
                    if (item.op_ == sim::trace::mitem_t::LOAD) {
                        num_loads++;
                    } else if (item.op_ == sim::trace::mitem_t::STORE) {
                        num_stores++;
                    } else {
                        std::cout << "Error: invalid mem item operation " << item.op_ << std::endl;
                        exit(1);
                    }
                    num_items++;
                    item = mem_parser.get_next_mem_item(subt);
                }
            } else if (event.type == sim::trace::ts::PHASE and event.u.phase.subtrace_id == sim::trace::ts::EMPTY_SUBTRACE) {
                sim::trace::mitem_t mitem = mem_parser.get_next_mem_item(subt);
                assert(mitem.addr_ == 0 and mitem.op_ == sim::trace::mitem_t::op_t::INVALID);
                _unused(mitem);
            }
        }
        std::cout << "WD [" << subt.wd_id << "]" << std::endl
                  << "  Num. items: " << num_items << std::endl
                  << "  Num. loads: " << num_loads << std::endl
                  << "  Num. stores: " << num_stores << std::endl
                  << "  Num. instructions: " << num_instr << std::endl
                  << "  Num. instr. subtr. " << subt.n_instr << std::endl
                  << "  Num. phases: " << num_phases << std::endl;
        total_items += num_items;
        total_loads += num_loads;
        total_stores += num_stores;
        total_instr += num_instr;
        total_phases += num_phases;
    }
    std::cout << "Finished" << std::endl;
    std::cout << "Total statistics" << std::endl
              << "  Num. items: " << total_items << std::endl
              << "  Num. loads: " << total_loads << std::endl
              << "  Num. stores: " << total_stores << std::endl
              << "  Num. instructions: " << total_instr << std::endl
              << "  Num. phases: " << total_phases << std::endl;
}

//
// MAIN
//
void usage(const char* cmd, const std::unordered_map<char, std::string> &tests)
{
    std::cerr << "Usage: " << cmd << " [options] -i trace_file_prefix" << std::endl << "    Options:" << std::endl;
    std::unordered_map<char, std::string>::const_iterator it;

    for (it = tests.begin(); it != tests.end(); ++it) {
        std::cerr << "\t-" << it->first << "\t" << it->second << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::unordered_map<char, std::string> tests;

    tests['d'] = "Debugging Output";
    tests['f'] = "Full Trace Parse";
    tests['t'] = "Trace Info";
    tests['m'] = "Check Memory Addresses for TaskSim";
    tests['a'] = "Task Access to Inputs Analysis";
    tests['c'] = "Full Pass Mem Trace";

    if (argc < 2) {
        usage(argv[0], tests); exit(1);
    }
    unsigned vector_length = 8, trace_vector_length = 8, register_file_length = 8;
    int res;
    std::vector<char> to_be_done;
    std::string input_file;
    while ((res = ::getopt(argc, argv, "dftmaci:v:w:x:")) != -1) {
        char c = static_cast<char>(res);
        if (tests.find(c) != tests.end()) {
            to_be_done.push_back(c);
        } else if (c == 'i') {
            input_file = optarg;
        } else if (c == 'v') {
            vector_length = std::atoi(optarg);
        } else if (c == 'w') {
            trace_vector_length = std::atoi(optarg);
        } else if (c == 'x') {
            register_file_length = std::atoi(optarg);
        } else {
            usage(argv[0], tests);
            exit(1);
        }
    }

    if (input_file.empty() or to_be_done.empty()) {
        usage(argv[0], tests);
        return argc == 1;
    }

    // Check that input trace exists.
    sim::utils::check_trace_exists(input_file);

    for (std::vector<char>::const_iterator it = to_be_done.begin(); it != to_be_done.end(); ++it) {
        std::cout << tests[*it] << std::endl;
        switch (*it) {
        case 'd':
            full_trace_parse<DebugOutput, std::ostream>(input_file, std::cout);
            break;
        case 'f':
            full_trace_parse<FullTraceParse, std::ostream>(input_file, std::cout);
            break;
        case 't':
            full_trace_parse<TraceInfo, std::ostream>(input_file, std::cout);
            break;
        case 'm':
            check_mem_addresses_for_TaskSim(input_file);
            break;
        case 'a':
            task_access_to_inputs<PrettyPrint, std::ostream>(input_file, std::cout);
            break;
        case 'c':
            check_mem_trace(input_file, std::cout, vector_length, trace_vector_length, register_file_length);
            break;
        default:
            usage(argv[0], tests);
            exit(1);
        }
    }

    return 0;
}
