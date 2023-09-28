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


#ifndef CORE_TRACE_MEMPARSER_IMPL_H_
#define CORE_TRACE_MEMPARSER_IMPL_H_

#include <algorithm>
#include <vector>
#include "core/trace/ts/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/rle/Trace.h"

namespace sim {
namespace trace {

template<typename trace>
inline
MemParser<trace>::MemParser(trace &t, const sim::engine::Config &config) :
        trace_(t),
        vector_length_((config.key_exists("vector_register_length") ?
                config.get_value<unsigned int>("vector_register_length") : 64) / 8),
        trace_vector_length_(config.key_exists("trace_vector_register_length") ?
                config.get_value<unsigned int>("trace_vector_register_length") / 8 : 0),
        register_file_length_(config.key_exists("register_file_length") ?
                config.get_value<unsigned int>("register_file_length") / 8 : vector_length_),
        zero_(0)
{
}

template<typename trace>
inline
MemParser<trace>::MemParser(trace &t, unsigned vector_length, unsigned trace_vector_length, unsigned register_file_length) :
        trace_(t),
        vector_length_(vector_length),
        trace_vector_length_(trace_vector_length),
        register_file_length_(register_file_length),
        zero_(0)
{
}

template<typename trace>
inline
MemParser<trace>::~MemParser() {
    if (zero_) {
        std::cout << "[WARNING] MemParser found " << zero_ << " accesses to memory address 0." << std::endl;
    }
}

template<typename trace>
inline
sim::trace::mitem_t MemParser<trace>::get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace)
{
    std::cerr << "Error: Memory Parser does not recognize the selected Trace." << std::endl;
    assert(0);
    std::abort();
    return sim::trace::mitem_t();
}

template<typename trace>
inline
sim::trace::mitem_t MemParser<trace>::give_next_mitem(const sim::trace::ts::subtrace_t &subtrace)
{
    sim::trace::mem::mitem_queue_t &instr_queue = pending_items_[subtrace.wd_id];
    if (instr_queue.empty()) {
        // We clean the mem buffer of the 0 address memory access:
        unsigned c = 0;
        while (trace_.get_next_mem(subtrace) != 0) {
            c++;
        }
        if (c) {
            std::cout << "[WARNING] There were " << c << " trailing memory addresses." << std::endl;
        }
        //assert(a == 0);
        //_unused(a);
        pending_items_.erase(subtrace.wd_id);
        return sim::trace::mitem_t();
    } else {
        // If we have instructions in queue, we get the access address.
        sim::trace::mitem_t return_item = instr_queue.front();
        if (pending_addrs_.find(subtrace.wd_id) == pending_addrs_.end()) {
            return_item.addr_ = trace_.get_next_mem(subtrace);
        } else {
            return_item.addr_ = pending_addrs_[subtrace.wd_id].front();
            pending_removed_[subtrace.wd_id]--;
            pending_addrs_[subtrace.wd_id].pop();
            if (pending_removed_[subtrace.wd_id] != pending_addrs_[subtrace.wd_id].size()) {
                std::cout << "[WARNING] Difference in pending addresses: " << pending_removed_[subtrace.wd_id] << " vs " << pending_addrs_[subtrace.wd_id].size() << std::endl;
            }
            if (pending_addrs_[subtrace.wd_id].size() == 0) {
                if (pending_removed_[subtrace.wd_id] != 0) {
                    std::cout << "[WARNING] There pending counter is " << pending_removed_[subtrace.wd_id] << std::endl;
                }
                // assert(pending_removed_[subtrace.wd_id] == 0);
                pending_addrs_.erase(subtrace.wd_id);
                pending_removed_.erase(subtrace.wd_id);
            }
        }
        instr_queue.pop();
        if (return_item.addr_ == 0) {
            zero_++;
        }
        return return_item;
    }
}

template<typename trace>
inline
void MemParser<trace>::fill_not_vectorizing(const sim::trace::ts::subtrace_t &subtrace)
{
    sim::trace::mem::mitem_queue_t &instr_queue = pending_items_[subtrace.wd_id];
    while (instr_queue.empty()) {
        const sim::trace::bbl_t &bbl = trace_.get_next_bbl(subtrace);
        if (bbl.id_.id_ == 0) {
            break;
        } else {
            add_bbl(bbl.id_.id_, 0, instr_queue);
        }
    }
}

template<typename trace>
inline
unsigned MemParser<trace>::get_next_bbl_id(const sim::trace::ts::subtrace_t &subtrace)
{
    if (pending_bbl_id_.find(subtrace.wd_id) == pending_bbl_id_.end()) {
        const sim::trace::bbl_t &next = trace_.get_next_bbl(subtrace);
        if (num_accs_.find(next.id_.id_) == num_accs_.end()){
            init_info(next);
        }
        return next.id_.id_;
    } else {
        unsigned ret = pending_bbl_id_[subtrace.wd_id];
        pending_bbl_id_.erase(subtrace.wd_id);
        return ret;
    }
}

template<typename trace>
inline
void MemParser<trace>::fill_vectorizing(const sim::trace::ts::subtrace_t &subtrace)
{
    // If we do not have a basic block per WD, we pre-assign the next basic block.
    unsigned next_bbl_id = get_next_bbl_id(subtrace);

    sim::trace::mem::mitem_queue_t &instr_queue = pending_items_[subtrace.wd_id];
    while (instr_queue.empty()) {
        unsigned current_bbl = next_bbl_id;
        if (current_bbl == 0) {
            return;
        }
        unsigned eliminated_bbl_count = 0;
        if (vector_length_ > max_accs_[current_bbl]) {
            // Here we eliminate entire basic blocks due to vectorization:
            // We have a vector_length_ and this basic_bloc max_access
            // So: we can merge up to vector_length/max_access.
            // So we can eliminate up to vector_length/max_access - 1
            // Minimum max_access_ is warrantied to be 128.
            next_bbl_id = get_next_bbl_id(subtrace);
            for (unsigned i = 0; (i < vector_length_ / max_accs_[current_bbl] - 1) and (next_bbl_id == current_bbl); i++) {
                // In a normal case we should eliminare extra memory accesses here.
                // We store the original first iteration addresses.
                if (i == 0) {
                    for (unsigned j = 0; j < num_accs_[current_bbl]; j++) {
                        pending_addrs_[subtrace.wd_id].push(trace_.get_next_mem(subtrace));
                    }
                    pending_removed_[subtrace.wd_id] += num_accs_[current_bbl];
                }
                // And then remove 2nd-3rd etc memory addresses.
                for (unsigned j = 0; j < num_accs_[current_bbl]; j++) {
                    trace_.get_next_mem(subtrace);
                }
                next_bbl_id = get_next_bbl_id(subtrace);
            }
        } else {
            next_bbl_id = get_next_bbl_id(subtrace);
        }
        add_bbl(current_bbl, eliminated_bbl_count, instr_queue);
    }
    pending_bbl_id_[subtrace.wd_id] = next_bbl_id;
}

template<typename trace>
inline
sim::trace::mitem_t MemParser<trace>::get_next_mem_uop(const sim::trace::ts::subtrace_t &subtrace)
{
    sim::trace::mem::mitem_queue_t &instr_queue = pending_items_[subtrace.wd_id];
    if (instr_queue.empty()) {
        if (trace_vector_length_ == vector_length_ or vector_length_ == 8) {
            fill_not_vectorizing(subtrace);
        } else {
            fill_vectorizing(subtrace);
        }
    }
    return give_next_mitem(subtrace);
}

template<>
inline
sim::trace::mitem_t MemParser<trace::ts::FileTraceReader<trace::SelectedEventStream, trace::rle::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace)
{
    return get_next_mem_uop(subtrace);
}

template<>
inline
sim::trace::mitem_t MemParser<trace::ts::FileTrace<trace::SelectedEventStream, trace::rle::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace)
{
    return get_next_mem_uop(subtrace);
}

template<>
inline
sim::trace::mitem_t MemParser<trace::ts::FileTraceReader<trace::SelectedEventStream, trace::uop::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace)
{
    return get_next_mem_uop(subtrace);
}

template<>
inline
sim::trace::mitem_t MemParser<trace::ts::FileTrace<trace::SelectedEventStream, trace::uop::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace)
{
    return get_next_mem_uop(subtrace);
}

template<>
inline
sim::trace::mitem_t MemParser<trace::ts::FileTraceReader<trace::SelectedEventStream, trace::mem::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace)
{
    // In case of a mem trace we do not need to do anything special
    return trace_.get_next_mem_item(subtrace);
}

template<>
inline
sim::trace::mitem_t MemParser<trace::ts::FileTrace<trace::SelectedEventStream, trace::mem::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace)
{
    // In case of a mem trace we do not need to do anything special
    return trace_.get_next_mem_item(subtrace);
}

template<typename trace>
inline
void MemParser<trace>::init_info(const sim::trace::bbl_t &origin)
{
    const std::vector<sim::trace::minst_t> &old_ins = origin.ins_;
    unsigned max_access_size = 8; // 8 bytes 64 bits
    unsigned num_accesses = 0;
    for (size_t i = 0; i < old_ins.size(); i++) {
         // if (old_ins[i].type_ == sim::trace::op_t::LD) {
         if (old_ins[i].isLoad()) {
             max_access_size = std::max(max_access_size, static_cast<unsigned>(old_ins[i].ld_size_));
             num_accesses++;
         // } else if (old_ins[i].type_ == sim::trace::op_t::ST) {
         } else if (old_ins[i].isStore()) {
             max_access_size = std::max(max_access_size, static_cast<unsigned>(old_ins[i].st_size_));
             num_accesses++;
         }
    }
    num_accs_[origin.id_.id_] = num_accesses;
    if (trace_vector_length_ == 0) {
        max_accs_[origin.id_.id_] = max_access_size;
    } else {
        max_accs_[origin.id_.id_] = trace_vector_length_;
    }
}

template<typename trace>
inline
sim::trace::bbl_t MemParser<trace>::parse_bbl(const sim::trace::bbl_t &origin)
{
    sim::trace::bbl_t parsed;
    parsed.shallow_copy(origin);
    int multiplication_factor = 1;
    const std::vector<sim::trace::minst_t> &old_ins = origin.ins_;
    unsigned max_access_ = 0;
    for (size_t i = 0; i < old_ins.size(); i++) {
         if (old_ins[i].type_ == sim::trace::op_t::LD) {
             max_access_ = std::max(max_access_, static_cast<unsigned>(old_ins[i].ld_size_));
         } else if (old_ins[i].type_ == sim::trace::op_t::ST) {
             max_access_ = std::max(max_access_, static_cast<unsigned>(old_ins[i].st_size_));
         }
    }
    if (max_access_ == 8) {
        multiplication_factor = 2;
    } else if (max_access_ == 16) {
        multiplication_factor = 1;
    }
    // We just need to use the id and the microinstructions vector, no need to use anything elese.
    if (vector_length_ > 64) {
        // Here we first eliminarte duplicated vector ops and extra memory loads/stores.
        std::vector<sim::trace::minst_t> &new_ins = parsed.ins_;
        // const std::vector<sim::trace::minst_t> &old_ins = origin.ins_;
        for (size_t i = 0; i < old_ins.size(); i++) {
            if (old_ins[i].is_vector()) {
                if (old_ins.size() > (size_t)(i+1) and old_ins[i+1].is_vector() and
                       ((old_ins[i].ra_ == old_ins[i+1].ra_) or (old_ins[i].ra_ + 1 == old_ins[i+1].ra_)) and
                       ((old_ins[i].rb_ == old_ins[i+1].rb_) or (old_ins[i].rb_ + 1 == old_ins[i+1].rb_)) and
                       ((old_ins[i].rc_ == old_ins[i+1].rc_) or (old_ins[i].rc_ + 1 == old_ins[i+1].rc_)) and
                       ((old_ins[i].rd_ == old_ins[i+1].rd_) or (old_ins[i].rd_ + 1 == old_ins[i+1].rd_)) )  {
                    new_ins.push_back(old_ins[i]);
                    i++;
                    continue;
                }
            } else if (old_ins[i].type_ == sim::trace::op_t::IALU) {
                if (old_ins.size() > i+1 and
                        (old_ins[i+1].type_ == sim::trace::op_t::REMOVED_LD or
                        old_ins[i+1].type_ == sim::trace::op_t::REMOVED_ST) and
                        old_ins[i].rd_ == old_ins[i+1].ra_) {
                    continue;
                }
            } else if (old_ins[i].type_ == sim::trace::op_t::REMOVED_LD or
                        old_ins[i].type_ == sim::trace::op_t::REMOVED_ST) {
                continue;
            } else if (old_ins[i].type_ == sim::trace::op_t::LD) {
                new_ins.push_back(old_ins[i]);
                new_ins.rbegin()->ld_size_ *= multiplication_factor;
                continue;
            } else if (old_ins[i].type_ == sim::trace::op_t::ST) {
                new_ins.push_back(old_ins[i]);
                new_ins.rbegin()->st_size_ *= multiplication_factor;
                continue;
            }
            new_ins.push_back(old_ins[i]);
        }
    } else {
        parsed.ins_ = origin.ins_;
    }
    return parsed;
}

template<typename trace>
inline
void MemParser<trace>::add_bbl(unsigned id, unsigned eliminated_bbls, sim::trace::mem::mitem_queue_t& instr_queue)
{
    const sim::trace::bbl_t &origin = trace_.get_bbl(sim::trace::bbl_id_t(id));
    if (vector_length_ == trace_vector_length_) {
        instr_queue.push(origin);
        return;
    }
    unsigned multiplication_factor = eliminated_bbls + 1;
    unsigned memory_ratio = 0, instr_ratio = 0;
    if (trace_vector_length_ == 0) {
        assert(max_accs_.find(id) != max_accs_.end());
        assert(max_accs_[id] != 0);
        // If vector_length > register_file_length and (eliminated_bbls+1)*eliminated_length > register_file_length
        // We should duplicated all non-memory operations.
        memory_ratio = std::max(1u, vector_length_ / max_accs_[id]);
        instr_ratio = std::max(1u, register_file_length_ / max_accs_[id]);
    } else {
        memory_ratio = std::max(1u, vector_length_ / trace_vector_length_);
        instr_ratio = std::max(1u, register_file_length_ / trace_vector_length_);
    }
    bool duplicate_non_mem = (memory_ratio != instr_ratio) and (eliminated_bbls >= instr_ratio);
    unsigned duplicated_times = eliminated_bbls / instr_ratio;
    const std::vector<sim::trace::minst_t> &old_ins = origin.ins_;
    for (size_t i = 0; i < old_ins.size(); i++) {
        if (old_ins[i].is_vector()) {
            if (old_ins.size() > (size_t)(i+1) and old_ins[i+1].is_vector() and
                   ((old_ins[i].ra_ == old_ins[i+1].ra_) or (old_ins[i].ra_ + 1 == old_ins[i+1].ra_)) and
                   ((old_ins[i].rb_ == old_ins[i+1].rb_) or (old_ins[i].rb_ + 1 == old_ins[i+1].rb_)) and
                   ((old_ins[i].rc_ == old_ins[i+1].rc_) or (old_ins[i].rc_ + 1 == old_ins[i+1].rc_)) and
                   ((old_ins[i].rd_ == old_ins[i+1].rd_) or (old_ins[i].rd_ + 1 == old_ins[i+1].rd_)) )  {
                instr_queue.push(old_ins[i]);
                if (duplicate_non_mem) {
                    for (unsigned j = 0; j < duplicated_times; j++) {
                      instr_queue.push(old_ins[i]);
                    }
                }
                i++;
                continue;
            }
        } else if (old_ins[i].type_ == sim::trace::op_t::IALU) {
            if (old_ins.size() > i+1 and
                    (old_ins[i+1].type_ == sim::trace::op_t::REMOVED_LD or
                    old_ins[i+1].type_ == sim::trace::op_t::REMOVED_ST) and
                    old_ins[i].rd_ == old_ins[i+1].ra_) {
                continue;
            }
        } else if (old_ins[i].type_ == sim::trace::op_t::REMOVED_LD or
                    old_ins[i].type_ == sim::trace::op_t::REMOVED_ST) {
            continue;
        } else if (old_ins[i].isLoad()) {
            sim::trace::minst_t load = old_ins[i];
            instr_queue.push(load, multiplication_factor * static_cast<unsigned>(load.ld_size_));
            continue;
        } else if (old_ins[i].isStore()) {
            sim::trace::minst_t store = old_ins[i];
            instr_queue.push(store, multiplication_factor * static_cast<unsigned>(store.st_size_));
            continue;
        }
        instr_queue.push(old_ins[i]);
        if (duplicate_non_mem) {
            for (unsigned j = 0; j < duplicated_times; j++) {
                instr_queue.push(old_ins[i]);
            }
        }
    }
}

}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_MEMPARSER_IMPL_H_
