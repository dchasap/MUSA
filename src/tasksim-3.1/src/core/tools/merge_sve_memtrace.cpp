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
#include <dirent.h>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <string>
#include <map>

#include "core/trace/dr/types.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/types.h"
#include "core/utils/CheckTrace.h"
#include "core/utils/Log.h"

#define REG_imm     75
#define REG_mem     76
#define REG_lea     64

using sim::utils::Log;

typedef sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
        sim::trace::selectedtrace::TraceInternal> input_trace_t;
typedef sim::trace::ts::FileTrace<sim::trace::SelectedEventStream,
        sim::trace::selectedtrace::TraceInternal> output_trace_t;

class sve_record_t {
    public:
    sve_record_t();
    uint64_t index;
    int thread_id;
    unsigned bundle;
    bool is_write;
    unsigned size;
    uint64_t addr;
    uint64_t pc;
};

/** function to write a sve_record_t to an output stream */
std::ostream &operator<<(std::ostream &os, const sve_record_t &item);
/** function to read a sve_record_t from an input stream */
std::istream &operator>>(std::istream &is, sve_record_t &item);


sve_record_t::sve_record_t() :
    index(0),
    thread_id(0),
    bundle(0),
    is_write(false),
    size(0),
    addr(0),
    pc(0)
{}

std::ostream &operator<<(std::ostream &os, const sve_record_t &item)
{
    os << item.index << ", "
       << item.thread_id << ", "
       << item.bundle << ", "
       << std::boolalpha << item.is_write << std::noboolalpha << ", "
       << item.size << ", "
       << std::hex << item.addr << ", "
       << item.pc << std::dec;
    return os;
}

std::istream &operator>>(std::istream &is, sve_record_t &item)
{
    char separator;
    is >> item.index >> separator;
    if (not is.good()) {
        std::cout << "is is not good." << std::endl;
        return is;
    }
    assert(separator == ',');
    is >> item.thread_id >> separator;
    assert(separator == ',');
    is >> item.bundle >> separator;
    assert(separator == ',');
    is >> item.is_write >> separator;
    assert(separator == ',');
    is >> item.size >> separator;
    assert(separator == ',');
    std::ios::fmtflags fmt = is.flags();
    // To avoid assert fail on the last line.
    if (item.thread_id == -2) {
        std::string s;
        getline(is, s);
        getline(is, s);
        return is;
    }
    is >> std::hex >> item.addr >> separator;
    assert(separator == ',');
    is >> item.pc;
    is.flags(fmt);
    return is;
}

class footprint {
        std::map<uint64_t, unsigned> memory_map_;
    public:
        void add(uint64_t addr, unsigned size);
        void print_total(unsigned wd = 0);

};

void footprint::add(uint64_t addr, unsigned size) {
    if (memory_map_.empty()) {
        memory_map_[addr] = size;
        return;
    } else if (memory_map_.find(addr) != memory_map_.end()) {
        memory_map_[addr] = std::max((unsigned)size, memory_map_[addr]);
    } else {
        std::map<uint64_t, unsigned>::iterator it = memory_map_.lower_bound(addr);
        if (it != memory_map_.begin()) {
            // There is something smaller.
            it--;
            if (it->first < addr and it->first + it->second > addr) {
                memory_map_[it->first] = std::max((unsigned)(addr - it->first + size), it->second);
            }
        } else {
            // There is nothing smaller
            memory_map_[addr] = size;
        }
    }
    while (memory_map_.upper_bound(addr) != memory_map_.end()) {
        std::map<uint64_t, unsigned>::iterator it = memory_map_.upper_bound(addr);
        if (addr + memory_map_[addr] > it->first) {
            // They overlap
            memory_map_[addr] = std::max((unsigned)(it->first + it->second - addr), memory_map_[addr]);
            memory_map_.erase(it);
        } else {
            break;
        }
    }
}


void footprint::print_total(unsigned wd) {
    uint64_t total_size = 0;
    for (auto &elem : memory_map_) {
        total_size += elem.second;
    }
    if (total_size) {
        std::cout << "FP:" << wd << ":" << total_size << std::endl;
    }
}

class armie_reader {
    public:
    armie_reader(const std::string &filename);
    ~armie_reader();
    bool get_next(uint64_t pc, uint64_t &mem_addr);
    bool get_next(uint64_t pc, sve_record_t &record);
    void parse_max_accesses(std::unordered_map<uint64_t, unsigned> &max_accesses);
    private:
    std::ifstream in_file_;
    bool last_access_finished_;
    uint64_t last_pc_;
    uint64_t skipped_;
    std::map<uint64_t, unsigned> skipped_times_, skipped_size_;
    uint64_t memory_footprint_;
};

armie_reader::armie_reader(const std::string &filename) :
    in_file_(filename.c_str()),
    last_access_finished_(false),
    last_pc_(0),
    skipped_(0),
    memory_footprint_(0)
{
    // We must consume the first record that aways has null values.
    std::string header;
    std::getline(in_file_, header);
}

armie_reader::~armie_reader() {
    sve_record_t record;
    if (in_file_.good() and in_file_ >> record) {
        Log::error() << "Record remaining at SVE memory access file " << record;
        Log::error() << "And more...";
        memory_footprint_ += record.size;
    }
    while (in_file_.good() and in_file_ >> record) {
        memory_footprint_ += record.size;
        skipped_++;
        skipped_times_[record.pc]++;
        skipped_size_[record.pc] += record.size;
    }
    if (skipped_) {
        std::cout << "[DEBUG] Skipped " << skipped_ << " records." << std::endl;
        std::cout << "SKIPPED TIMES:" << std::endl;
        for (auto &elem: skipped_times_) {
            std::cout << std::hex << elem.first << std::dec << " " << elem.second << std::endl;
        }
        std::cout << "SKIPPED SIZE:" << std::endl;
        for (auto &elem: skipped_size_) {
            std::cout << std::hex << elem.first << std::dec << " " << elem.second << std::endl;
        }
    }
    // std::cout << "Total memory footprint: " << memory_footprint_ << std::endl;
}

bool armie_reader::get_next(uint64_t pc, sve_record_t &record)
{
    // To get all the memory accesses for a given address.
    if (last_access_finished_ == true) {
        // The last access was end of sequence
        assert(pc == last_pc_);
        last_access_finished_ = false;
        return false;
    }
    bool found = false;
    while (in_file_ >> record) {
        if (record.thread_id == -2) {
            Log::error() << "Reached end of memory file while looking for memory access for PC "
                         << std::hex << pc << std::dec << ".";
            break;
        }
        memory_footprint_ += record.size;
        if (record.pc == pc) {
            found = true;
            break;
        } else {
            Log::info() << "Skipping record " << record << " as " << std::hex << record.pc << " != " << pc << std::dec;
            skipped_++;
            skipped_times_[record.pc]++;
            skipped_size_[record.pc] += record.size;
            // Skip the record as it was not in a USER CODE region
        }
    }
    if (not found) {
        Log::error() << "Finished the SVE memtrace without finding memory access for pc "
                     << std::hex << pc << std::dec;
            std::cout << "Had skipped " << skipped_ << " records." << std::endl;
        in_file_.close();
        exit(-1);
        return false;
    } else {
        // We mark the accesses for this address as finished if it was a contiguos access (0)
        // or it was the end of a gather/scatter list (4 or 6).
        last_access_finished_ = (record.bundle == 0) or (record.bundle > 3);
        last_pc_ = pc;
        return true;
    }
}

void armie_reader::parse_max_accesses(std::unordered_map<uint64_t, unsigned> &max_accesses)
{
    sve_record_t record;
    uint64_t last_pc = 0;
    unsigned counter = 0;
    uint64_t records = 0;
    // footprint fp;
    in_file_ >> record;
    while (in_file_ >> record) {
        records++;
        // std::cout << record << std::endl;
        if (record.thread_id == -2) {
            max_accesses[last_pc] = std::max(counter, max_accesses[last_pc]);
            return;
        } else if (record.pc == last_pc) {
            counter++;
        } else {
            max_accesses[last_pc] = std::max(counter, max_accesses[last_pc]);
            last_pc = record.pc;
            counter = 1;
        }
        if (record.bundle == 0 or record.bundle >3) {
            max_accesses[last_pc] = std::max(counter, max_accesses[last_pc]);
            last_pc = 0;
            counter = 0;
        }
        // fp.add(record.addr, record.size);
    }
    std::cout << "[MERGE] The log file has " << records << " records, corresponding to " << max_accesses.size() << " loged pcs." << std::endl;
}

void usage(const char* cmd, const std::unordered_map<char, std::string> &tests)
{
    std::cerr << "Usage: " << cmd << " [options]" << std::endl << "    Options:" << std::endl;
    std::unordered_map<char, std::string>::const_iterator it;

    for (it = tests.begin(); it != tests.end(); ++it) {
        std::cerr << "\t-" << it->first << "\t" << it->second << std::endl;
    }
}




void merge_memtrace(input_trace_t &input_trace, const std::string &sve_memtrace_filename, const std::string &output_name, bool no_nanos, bool verbose)
{
    // First we get the number of tasks and work descriptors:
    uint32_t n_tasks = input_trace.get_number_of_task_names();
    uint32_t n_wds = input_trace.get_total_number_of_tasks();
    Log::info() << "Input task has " << n_wds << " Work Descriptors and "
                << n_tasks << " task types.";
    if (n_wds == 0) {
        Log::error()  << "Input trace has no Work Descriptors. Trace corrupted.";
        exit(1);
    }
    std::unordered_map<uint64_t, unsigned>  max_accesses;
    {
        armie_reader(sve_memtrace_filename).parse_max_accesses(max_accesses);
    }
    // We frame this block so the file is closed uppon having the base structure:
    // This is done to preserve .default.trace information order.
    // It is not strictly required, but the final order is nicer.
    std::cout  << "[MERGE[ Copying task information..." << std::endl;
    {
        output_trace_t output(output_name.c_str());

        for (uint32_t i = 0; i < n_tasks; i++) {
            Log::debug() << "[MERGE] Copying task name " << i;
            output.add_task_name(i, input_trace.get_task_name(i));
        }
        for (unsigned w = 0; w < n_wds; ++w) {
            Log::debug2() << "[MERGE] Copying information for task " << w;
            sim::trace::ts::subtrace_t tsubt = input_trace.get_task_trace_by_id(w);
            output.add_task_trace(tsubt.wd_id, tsubt.name_id, tsubt.stack);
            const sim::trace::user_code_data_t& user_data = input_trace.get_user_code_data(tsubt.wd_id);
            sim::trace::ts::subtrace_t osubt = output.get_task_trace_by_id(w);
            for (const auto& phase_data : user_data.data) {
                output.add_phase_data(osubt, phase_data);
            }
        }
    }
    // Now lets copy all the events:
    sim::trace::ts::event_t event;
    output_trace_t output(output_name.c_str());
    std::cout  << "[MERGE] Copying events..." << std::endl;
    if (no_nanos == true) {
        sim::trace::ompss::event_t ompss_event;
        while (input_trace.get_next_phase_event(ompss_event)) {
            unsigned wd_id = ompss_event.wd_id_;
            int subtrace_id = -1;
            if (ompss_event.value_ == sim::trace::ompss::phase_id_t::USER_CODE_PHASE){
                subtrace_id = output.get_task_trace_id_by_wd_id(wd_id);
            }
            output.add_task_event(wd_id, sim::trace::ts::PHASE,
                        sim::trace::ts::phase_t({static_cast<unsigned>(ompss_event.value_),
                        subtrace_id, 1}));
        }
    } else {
        for (unsigned subtrace_id = 0; subtrace_id < n_wds; subtrace_id++) {
            const sim::trace::ts::subtrace_t subt = input_trace.get_task_trace_by_id(subtrace_id);
            output.add_task(subt.wd_id);
            Log::info() << "Added task " << subt.wd_id << " to the output trace.";
            // Add events
            while (input_trace.get_task_event(subt.wd_id, event)) {
                Log::info() << "Processing event " << event;
                switch (event.type) {
                case sim::trace::ts::PHASE:
                    Log::debug3() << "add task event: " << event;
                    output.add_task_event(subt.wd_id, sim::trace::ts::PHASE,
                            sim::trace::ts::phase_t({event.u.phase.burst_id,
                                                     event.u.phase.subtrace_id,
                                                     event.u.phase.duration}));
                    // We have to add the memory acceses in order, we use the replay information later on.
                    break;
                case sim::trace::ts::CREATE_TASK:
                    {
                        Log::debug2() << "add create task: " << event;
                        output.add_task_event(subt.wd_id, sim::trace::ts::CREATE_TASK,
                                sim::trace::ts::create_task_t({ event.u.create_task.wd_id }));
                        std::vector<sim::trace::ts::dep_t> deps = input_trace.get_task_dependencies({event.u.create_task.wd_id});
                        output.add_wd_info(event.u.create_task.wd_id, deps);
                        sim::trace::ts::subtrace_t isubt = input_trace.get_task_trace_by_wd_id(event.u.create_task.wd_id);
                        sim::trace::ts::subtrace_t osubt = output.get_task_trace_by_wd_id(event.u.create_task.wd_id);
                        input_trace.get_wait_on_dependencies(isubt, deps);
                        // To avoid adding void dependencies
                        if (deps.size() > 0) {
                            output.add_wait_on_dependencies(osubt, deps);
                        }
                    }
                    break;
                case sim::trace::ts::TASK_WAIT:
                    Log::debug2() << "add task wait: " << event;
                    output.add_task_event(subt.wd_id, sim::trace::ts::TASK_WAIT,
                            sim::trace::ts::task_wait_t({}));
                    break;
                case sim::trace::ts::WAIT_ON:
                    Log::debug2() << "add wait on: " << event;
                    output.add_task_event(subt.wd_id, sim::trace::ts::WAIT_ON,
                            sim::trace::ts::wait_on_t({event.u.wait_on.subtrace_id}));
                    break;
                case sim::trace::ompss::SET_LOCK:
                    output.add_task_event(subt.wd_id, sim::trace::ts::SET_LOCK, sim::trace::ts::lock_t({event.u.lock.address}));
                    break;
                case sim::trace::ompss::UNSET_LOCK:
                    output.add_task_event(subt.wd_id, sim::trace::ts::UNSET_LOCK, sim::trace::ts::lock_t({event.u.lock.address}));
                    break;
                default:
                    Log::error() << "Unrecognized event type " <<  event.type;
                    exit(1);
                }
            }
        }
    }
    // Now last, and most important, lets copy the basic bloc dictionary, list and memory addresses:
    // We add the 0 id basic block
    sim::trace::bbl_id_t id(1);
    std::unordered_map<unsigned, sim::trace::bbl_t> bbl_dictionary;
    sim::trace::bbl_t bbl = input_trace.get_bbl(id);
    // Just in case:
    bbl_dictionary[0] = sim::trace::bbl_t();
    while (bbl.id_.id_ != 0) {
        bbl_dictionary[bbl.id_.id_] = bbl;
        if (id.id_ != bbl.id_.id_) {
            std::cout << "[WARNING] BBL IDs missmatch " << bbl.id_.id_ << " " << id.id_ << std::endl;
        }
        id.id_++;
        bbl = input_trace.get_bbl(id);
    }

    // We have fully read the bbl dictionary!
    unsigned bbl_dictionary_size = bbl_dictionary.size();
    // Now we should modify the dictionary so all SVE instructions are tagged/decoded properly.
    unsigned total = 0, mem_sve = 0, nonmem_sve = 0;
    std::unordered_map<unsigned, unsigned> bbl_nonmem_sve;
    for (unsigned int i = 0; i < bbl_dictionary_size; i++) {
        // First we look at the macro instructions.
        for (unsigned int j = 0; j < bbl_dictionary[i].macroops_.size(); j++) {
            total++;
            if (bbl_dictionary[i].macroops_[j].is_memory_sve()) {
                mem_sve++;
                Log::info() << "BBL " << i << " instruction " << j << " is an SVE memory operation.";
                Log::info() << bbl_dictionary[i].macroops_[j];
            } else if (bbl_dictionary[i].macroops_[j].is_sve()) {
                nonmem_sve++;
                Log::info() << "BBL " << i << " instruction "
                            << j << " is an SVE non-memory operation.";
                Log::info() << bbl_dictionary[i].macroops_[j];
                bbl_nonmem_sve[i]++;
            }
        }
    }
    Log::info() << "From " << total << " instructions " << mem_sve << " are memory-sve and "
                << nonmem_sve << " are non-memory-sve instructions.";
    // Now we replay the basiuc blocs and memory accesses in the proper order:
    unsigned wd_id = 0;
    armie_reader sve_reader(sve_memtrace_filename);
    std::unordered_map<uint64_t, unsigned> sve_access_counter;
    std::unordered_map<uint64_t, unsigned> sve_used;
    std::unordered_map<uint64_t, unsigned> sve_times;
    // std::unordered_map<uint64_t, unsigned> instr_times;
    std::unordered_map<uint64_t, unsigned> sve_padding;
    std::unordered_map<uint64_t, unsigned> sve_mem_type;
    std::unordered_set<uint64_t> missing;
    std::unordered_map<uint64_t, std::vector<sve_record_t>> sve_accesses;
    uint64_t total_mems = 0, sve_ld = 0, sve_st = 0, sve_mems = 0, sve_mem_instr = 0, sve_nonmem = 0, num_loads = 0, num_stores = 0, zero = 0, total_instructions = 0;
    uint64_t total_padding_footprint = 0;
    // footprint fp;
    std::cout  << "[MERGE] Replaying memory accesses..." << std::endl;
    while (input_trace.get_replay_wd_id(wd_id)) {
        Log::info() << "Replaying user code phase for WD " << wd_id;
        const sim::trace::ts::subtrace_t subtrace = input_trace.get_task_trace_by_wd_id(wd_id);
        const sim::trace::ts::subtrace_t o_subtrace = output.get_task_trace_by_wd_id(wd_id);
        unsigned seg_mem = 0, seg_ld = 0, seg_st = 0, seg_sve_mem = 0, seg_zero = 0;
        // footprint wd_fp;
        while (true) {
            const sim::trace::bbl_t &bbl = input_trace.get_next_bbl(subtrace);
            sve_nonmem += bbl_nonmem_sve[bbl.id_.id_];
            // If id == 0, we have finished the chunk.
            if (bbl.id_ == 0) {
                // End of stream.
                sim::engine::addr_t addr = input_trace.get_next_mem(subtrace);
                // assert(addr == 0);
                unsigned c = 0;
                while (addr != 0) {
                    c++;
                    addr = input_trace.get_next_mem(subtrace);
                }
                if (c) {
                    Log::error() << "Messed up the trace merge, expected 0 address accces, got "
                                 << c << " pending.";
                }
                if (seg_zero or c) {
                    std::cout << "[DEBUG] Finalized transcriing segment belonging to WD " << o_subtrace.wd_id << std::endl;
                    std::cout << "[DEBUG] Segment has " << seg_mem << " memory accesses, from which " << seg_ld << " are loads, " << seg_st << " are stores." << std::endl;
                    std::cout << "        It also had " << seg_sve_mem << " SVE memory accesses and was missing " << seg_zero << " addresses and " << c << "extra accesses." << std::endl;
                }
                output.add_bbl_trace(o_subtrace, bbl.id_);
                output.add_mem_trace(o_subtrace, 0);
                output.add_detailed_trace(o_subtrace.wd_id, o_subtrace.name_id);
                break;
            }
            output.add_bbl_trace(o_subtrace, bbl.id_);
            const sim::trace::bbl_t modified_bbl = bbl_dictionary[bbl.id_.id_];
            uint64_t pc = modified_bbl.instr_ptr_;
            unsigned micro_index = 0;
            total_instructions += modified_bbl.macroops_.size();
            for (unsigned int i = 0; i < modified_bbl.macroops_.size(); i++) {
                // instr_times[pc]++;
                if (modified_bbl.macroops_.at(i).is_memory_sve()) {
                    seg_sve_mem++;
                    if (max_accesses.find(pc) == max_accesses.end()) {
                        missing.insert(modified_bbl.macroops_.at(i).iword_);
                    }
                    Log::debug3() << "Instruction " << i << " is memory sve.";
                    // Now we want all the consecutive memory accesses generated by @addr.
                    sve_record_t record;
                    unsigned c = 0;
                    sve_mem_instr++;
                    while (sve_reader.get_next(pc, record)) {
                        // We add it to the trace.
                        if (record.addr == 0) {
                            std::cout << "[MERGE][ERROR] SVE access to address 0." << std::endl;
                        }
                        output.add_mem_trace(o_subtrace, record.addr);
                        total_mems++;
                        sve_mems++;
                        c++;
                        if (sve_access_counter.find(pc)  == sve_access_counter.end()) {
                            sve_accesses[pc].push_back(record);
                        }
                        sve_times[pc]++;
                    }
                    if (record.is_write) {
                        sve_st++;
                    } else {
                        sve_ld++;
                    }
                    while (c < max_accesses[pc]) {
                        // We add it to the trace.
                        if (record.addr == 0) {
                            std::cout << "[MERGE][ERROR] SVE access to address 0." << std::endl;
                        }
                        output.add_mem_trace(o_subtrace, record.addr);
                        total_mems++;
                        sve_mems++;
                        c++;
                        sve_times[pc]++;
                        sve_padding[pc]++;
                        total_padding_footprint += record.size;
                    }
                    if (sve_access_counter.find(pc) == sve_access_counter.end() or c > sve_access_counter[pc]) {
                        sve_access_counter[pc] = c;
                    }
                } else {
                    // We add this macro-op memory accesses (if any):
                    while (micro_index < modified_bbl.ins_.size() and modified_bbl.ins_[micro_index].pc_ <= pc) {
                        if (modified_bbl.ins_.at(micro_index).isLoad() or
                                modified_bbl.ins_.at(micro_index).isStore()) {
                            uint64_t addr = input_trace.get_next_mem(subtrace);
                            if (addr == 0) {
                                std::cout << "[MERGE][ERROR] Memory access to address 0 fo WD " << o_subtrace.wd_id << "." << std::endl;
                                addr = 1;
                                zero++;
                                seg_zero++;
                            }
                            output.add_mem_trace(o_subtrace, addr);
                            total_mems++;
                            seg_mem++;
                        }
                        if (modified_bbl.ins_.at(micro_index).isLoad()) {
                            num_loads++;
                            seg_ld++;
                        } else if (modified_bbl.ins_.at(micro_index).isStore()) {
                            num_stores++;
                            seg_st++;
                        }
                        micro_index++;
                    }
                }
                pc += modified_bbl.macroops_.at(i).isize_;
            }
            while (micro_index < modified_bbl.ins_.size()) {
                if (modified_bbl.ins_.at(micro_index).isLoad() or
                        modified_bbl.ins_.at(micro_index).isStore()) {
                    uint64_t addr = input_trace.get_next_mem(subtrace);
                    if (addr == 0) {
                        std::cout << "[MERGE][ERROR] Memory access to address 0 fo WD " << o_subtrace.wd_id << "." << std::endl;
                        addr = 1;
                        zero++;
                        seg_zero++;
                    }
                    output.add_mem_trace(o_subtrace, addr);
                    total_mems++;
                    seg_mem++;
                }
                if (modified_bbl.ins_.at(micro_index).isLoad()) {
                    num_loads++;
                    seg_ld++;
                } else if (modified_bbl.ins_.at(micro_index).isStore()) {
                    num_stores++;
                    seg_st++;
                }
                micro_index++;
            }
        }
    }
    for (auto &info : sve_access_counter) {
        std::cout  << "[MERGE] Instruction with pc 0x" << std::hex << info.first << std::dec
                     << " has " << info.second << " associated memory accesses." << std::endl;
    }
    if (missing.size()) {
        std::cout << "[MERGE] [ERROR] SVE log has " << missing.size() << " missing opcodes." << std::endl;
        for (auto &iword : missing) {
            std::cout << std::hex << iword << std::dec << std::endl;
        }
    }
    std::cout << "[MERGE] Processing the basic bloc dictionary." << std::endl;
    uint64_t sve_inserted_lds = 0, sve_inserted_sts = 0;
    uint64_t total_inserted_footprint = 0;
    for (unsigned int i = 0; i < bbl_dictionary.size(); i++) {
        for (unsigned int j = 0; j < bbl_dictionary[i].macroops_.size(); j++) {
            if (bbl_dictionary[i].macroops_[j].is_memory_sve()) {
                uint64_t pc = bbl_dictionary[i].macroops_[j].pc_;
                uint8_t *micro_pc;
                micro_pc = reinterpret_cast<uint8_t*>(pc);
                // We need to add the relevant memory operations:
                unsigned base_microop = 0;
                while (base_microop < bbl_dictionary[i].ins_.size() and
                        bbl_dictionary[i].ins_[base_microop].pc_ < pc) {
                    base_microop++;
                }
                if (sve_access_counter.find(pc) == sve_access_counter.end()) {
                    sve_access_counter[pc] = 0;
                }
                sve_used[pc]++;
                for (unsigned a = 1; a < sve_accesses[pc].size(); a++) {
                    if (sve_accesses[pc][0].size != sve_accesses[pc][a].size) {
                        std::cout << "[ERROR] Not all access counters are the same. " << sve_accesses[pc][0].size << " vs " << sve_accesses[pc][a].size << std::endl;
                    }
                }
                if (sve_accesses[pc].size() == 0) {
                    std::cout << "[MERGE] [ERROR] SVE memory instruction with no SVE accesses 0x" << std::hex << pc << std::dec << std::endl;
                    continue;
                }
                if (bbl_dictionary[i].macroops_[j].is_ld1w()) {
                    sve_accesses[pc][0].size = sve_accesses[pc][0].size/2;
                }
                for (unsigned c = 0; c < sve_access_counter.at(pc); c++) {
                    bbl_dictionary[i].ins_.insert(bbl_dictionary[i].ins_.begin() + base_microop, sim::trace::minst_t(micro_pc, sim::trace::op_t::IALU, 0, REG_imm, REG_imm, REG_imm, REG_lea));
                    base_microop++;
                    assert(sve_accesses.find(pc) != sve_accesses.end());
                    if (sve_accesses[pc].front().is_write) {
                        bbl_dictionary[i].ins_.insert(bbl_dictionary[i].ins_.begin() + base_microop, sim::trace::minst_t(micro_pc, sim::trace::op_t::ST, sve_accesses[pc][0].size, REG_imm, REG_lea, REG_imm, REG_mem));
                        sve_inserted_sts++;
                    } else {
                        bbl_dictionary[i].ins_.insert(bbl_dictionary[i].ins_.begin() + base_microop, sim::trace::minst_t(micro_pc, sim::trace::op_t::LD, sve_accesses[pc][0].size, REG_mem, REG_lea, REG_imm, REG_imm));
                        sve_inserted_lds++;
                    }
                    base_microop++;
                }
                Log::debug() << "Added " << sve_access_counter.at(pc)
                             << " memory accesses to the SVE memory access at 0x"
                             << std::hex << pc << std::dec  << " at basic bloc #" << i;
            } else if (bbl_dictionary[i].macroops_[j].is_sve()) {
                uint64_t pc = bbl_dictionary[i].macroops_[j].pc_;
                unsigned base_microop = 0;
                while (base_microop < bbl_dictionary[i].ins_.size() and
                        bbl_dictionary[i].ins_[base_microop].pc_ < pc) {
                    base_microop++;
                }
                // bbl_dictionary[i].ins_.insert(bbl_dictionary[i].ins_.begin() + base_microop, sim::trace::minst_t(micro_pc, sim::trace::op_t::IALU, 0, REG_imm, REG_imm, REG_imm, REG_imm));
                base_microop++;
            }
        }
        output.add_bbl(bbl_dictionary[i]);
    }
    // std::cout << "SVE Times has size " << sve_times.size() << " SVE_used has size " << sve_used.size() << std::endl;
    uint64_t total_sve_inserted = 0;
    for (auto &it : sve_used) {
        total_sve_inserted += sve_times[it.first];
    }
    /*
    for (auto &it : instr_times) {
        std::cout << std::hex << it.first << std::dec << ": " << it.second << std::endl;
    }*/
    for (auto &it : sve_times) {
        if (it.second != 0) {
            if (sve_accesses.find(it.first) != sve_accesses.end() and sve_accesses[it.first].size() > 0) {
                total_inserted_footprint += it.second *  sve_accesses[it.first][0].size;
                std::cout << "[MERGE] Instruction at 0x" << std::hex << it.first << std::dec << " executed " << it.second << " times." << std::endl;
            } else {
                std::cout << "[MERGE] [WARNING] sve_times has " << it.second << " entries for pc "
                          << std::hex << it.first << std::dec << " but it has no sce_accesses recorded." << std::endl;
            }
        }
    }
    std::cout << "[MERGE] Finished merging the traces. " << sve_mems << " SVE memory operations of " << total_mems << " total memory operations." << std::endl;
    std::cout << "[MERGE] In total the were " << sve_mem_instr << " SVE memory instructions executed." << std::endl;
    std::cout << "[MERGE] " << sve_ld << " SVE loads." << std::endl;
    std::cout << "[MERGE] " << sve_st << " SVE stores." << std::endl;
    std::cout << "[MERGE] " << sve_nonmem << " SVE non-memory instructions executed." << std::endl;
    std::cout << "[MERGE] " << num_loads << " non=-SVE loads." << std::endl;
    std::cout << "[MERGE] " << num_stores << " non-SVE stores." << std::endl;
    std::cout << "[MERGE] " << total_instructions << " total instructions." << std::endl;
    if (zero) {
        std::cout << "[WARNING] There were " << zero << " zero address memory accesses," << std::endl;
    }
    // std::cout << "[DEBUG] The total inserted memory accesses footprint is " << total_inserted_footprint << std::endl;
    // std::cout << "[DEBUG] The total padding memory accesses footprint is " << total_padding_footprint << std::endl;
    std::cout << "[MERGE] Finished." << std::endl;
    return;
}

void copy_file(std::string input_name, std::string output_name) {
    std::ifstream source(input_name.c_str(), std::ios::binary);
    std::ofstream destination(output_name.c_str(), std::ios::binary);
    destination << source.rdbuf();
    source.close();
    destination.close();
}

int main(int argc, char* argv[])
{
    // Argument flags:
    std::unordered_map<char, std::string> options;
    options['i'] = "Input trace file prefix";
    options['o'] = "Output trace file prefix";
    options['l'] = "ArmIE emulation client sve memtrace file";
    options['v'] = "Verbose execution";
    options['n'] = "Trace without Nanos++ events";

    std::string input_name, output_name, memtrace_fname;

    bool verbose = false, no_nanos = false;;
    char c;
    while ((c = ::getopt(argc, argv, "i:o:l:vn")) != static_cast<char>(-1)) {
        switch (c) {
        case 'i':
            input_name = std::string(optarg);
            break;
        case 'o':
            output_name = std::string(optarg);
            break;
        case 'l':
            memtrace_fname = std::string(optarg);
            break;
        case 'v':
            verbose = true;
            break;
        case 'n':
            no_nanos = true;
            break;
        default:
            usage(argv[0], options);
            exit(1);
        }
    }

    // Check that input trace exists.
    sim::utils::check_trace_exists(input_name);

    Log::info() << "Merging input Tasksim trace " << input_name << " with ArmIE SVE memory trace file "
                << memtrace_fname << " into TaskSim trace " << output_name;

    input_trace_t input_trace(input_name.c_str());
    std::cout << "[MERGE] ArmIE's SVE Log file is " << memtrace_fname << std::endl;
    merge_memtrace(input_trace, memtrace_fname, output_name, no_nanos, verbose);
    copy_file(input_name + ".mpiphases", output_name + ".mpiphases");
}
