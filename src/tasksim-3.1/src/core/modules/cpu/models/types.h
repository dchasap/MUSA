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

#ifndef CORE_MODULES_CPU_MODELS_TYPES_H_
#define CORE_MODULES_CPU_MODELS_TYPES_H_

#include <deque>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "core/engine/Config.h"
#include "core/engine/EventBuffer.h"
#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Message.h"
#include "core/stats/Counter.h"
#include "core/trace/mem/types.h"

namespace sim {
namespace cpu {
namespace timing_models {

/** rob_entry_t holds all the information necessary for a Reoder Buffer entry.
 */
struct rob_entry_t {
public:
    rob_entry_t(sim::trace::mitem_t item, bool served, bool discarded, unsigned n_requests, struct rob_entry_t* dep);
    /** Function to write a ROB::element to an output stream */
    std::string to_string() const;

    /** Associated mitem. */
    sim::trace::mitem_t item_;
    /** Indicates wether the request has been served. */
    bool served_;
    /** Indicates wether the operation is discarded (as in load on load). */
    bool discarded_;
    /** Indicates the number of pending requests. */
    unsigned pending_;
    /**
     * In loads, used to hold the previous store on which depends
     * In stores, used to hold the loads depending on it
     */
    std::vector<struct rob_entry_t*> deps_;
};

/** LDQueue stores a dictionary for the pending Read Requests from instructions
 *  still on the Reorder buffer. Said dictionary is indexed by address and size.
 */

template <typename T>
class LDQueue {
private:
    typedef typename std::pair<unsigned, T> access_list_elem;
    typedef typename std::deque<access_list_elem> access_list;
    typedef typename std::unordered_map<sim::trace::addr_t, access_list> data_map;
    data_map data_;
public:
    bool insert(sim::trace::addr_t addr, unsigned size, T elem);
    T retrieve(sim::trace::addr_t addr, unsigned size);
    void print_status() const;
};

template <typename Output>
class ROB {
public:
    typedef rob_entry_t elem_t;
    /** ReorderBuffer constructor: */
    ROB(unsigned size, unsigned request_size, Output& out);
    /** Inserts item into the ReorderBuffer. */
    elem_t* insert(const sim::trace::mitem_t &item);
    /** Processes Memory ACK from a previous Read Request. */
    void process_ack(const sim::logic::coherence::single::Message::Ack &ack);
    /** Processes Memory ACK from a previous Read Request using a reference to the ReorderBuffer entry. */
    void ack(elem_t &elem);
    /** Retires ins instructions. */
    unsigned retire(unsigned ins);
    /** Checks if item and all the Requests it generates fit into the ReorderBuffer and OutputBuffer. */
    bool check_load_fits_output_buffer(const sim::trace::mitem_t &item);

    /** Returns the maximum number of entries of the ReorderBuffer, */
    std::size_t capacity() const;
    /** Return the free entries remamining in the ReorderBuffer. */
    unsigned free_entries() const;
    /** Returns wether the ReorderBuffer is empty. */
    bool empty() const;
    /** Returns the number of instructions that can be committed (do not depend on a pending Request). */
    unsigned get_ins_to_first_waiting_item() const;
    /** Sets the current Work Descriptor Identifier to wd_id. */
    void set_current_wd_id(uint64_t wd_id);
    /** Prints current status for deadlock detection. */
    void print_status() const;

private:
    typedef typename std::deque<elem_t>::iterator data_iterator;
    typedef typename std::deque<elem_t>::const_iterator data_const_iterator;
    typedef typename std::deque<elem_t>::reverse_iterator data_reverse_iterator;
    typedef typename std::deque<elem_t>::const_reverse_iterator data_const_reverse_iterator;

    /** ReorderBuffer stored data. */
    std::deque<elem_t> data_;
    /** Maximum ReoderBuffer size. */
    const size_t max_size_;
    /** Maximum memory request size and alignment. */
    const size_t max_request_size_;
    /** Current ReoderBuffer size. */
    size_t size_;
    /** OutputBuffer where Read/Write Requests are sent. */
    Output& out_;
    /** Current Work Descriptor identifier. */
    uint64_t current_wd_id_;
    /** PendingLoads queue, which points to ReorderBuffer entries. */
    LDQueue<elem_t*> pending_loads_;

    /**
     * These functions return in first whether the item access is discarded due to aliasing
     * and in second a pointer to the dependency counterpart
     */
    std::pair<bool, elem_t*> check_load_address_match(const sim::trace::mitem_t &item);
    std::pair<bool, elem_t*> check_store_address_match(const sim::trace::mitem_t &item);

    /** Performs all necessary operations to include load item into the reorder buffer. */
    elem_t* process_load_insertion(const sim::trace::mitem_t &item);
    /** Performs all necessary operations to include store item into the reorder buffer. */
    elem_t* process_store_insertion(const sim::trace::mitem_t &item);

    /** Issues all the necessary read Requests to the memory. */
    void issue_read_requests(elem_t &elem);
    /** Issues all the necessary write Requests to the memory. Also sends dependant Read requests. */
    void issue_write_requests(elem_t &elem);

    /** Counts how many memory requests will be generated by Reorder Buffer entry elem. */
    unsigned number_of_requests(const elem_t &elem);
    /** Perform all necesiary operations to commit write ReorderBuffer entry elem. */
    void send_store(elem_t &elem);
    /** Perform all necesiary operations to commit write ReorderBuffer entry elem and its dependants. */
    void send_store_and_deps(elem_t &elem);
};

template <typename T>
class OutputBuffer {
private:
    std::queue<T> out_;
    std::size_t size_;
public:
    explicit OutputBuffer(std::size_t size): size_(size) {}
    ~OutputBuffer();
    typename std::queue<T>::size_type size() const { return out_.size(); }
    typename std::queue<T>::const_reference front() const { return out_.front(); }
    typename std::queue<T>::reference front() { return out_.front(); }
    void push(const T& v) { assert(not full()); out_.push(v); }
    bool empty() const { return out_.empty(); }
    bool full() const { return (out_.size() == size_); }
    void pop() { out_.pop(); }
    typename std::queue<T>::size_type free_entries() const { return size_ - out_.size(); }
};


struct ROAStats {
public:
    // uint64_t loads_recv_;
    // uint64_t stores_recv_;

    // Total is total issued by the ROA.
    // Sent are actually sent to memory
    // Received are the ones for which we have received an answer.

    sim::stats::Counter num_loads_total_;
    sim::stats::Counter num_loads_sent_;
    sim::stats::Counter num_loads_received_;
    sim::stats::Counter num_stores_total_;
    sim::stats::Counter num_stores_sent_;
    sim::stats::Counter num_stores_received_;
    sim::stats::Counter committed_ins_;
    sim::stats::Counter stalled_cycles_;
    sim::stats::Counter out_full_cycles_;
    sim::stats::Counter load_cycles_;
    sim::stats::Counter store_cycles_;

    // For the hardware counters on the instrumentation traces:
    uint64_t prev_issued_instructions_;
    uint64_t prev_committed_instructions_;
    uint64_t prev_cycle_count_;
    uint64_t prev_stalled_cycles_;
    uint64_t prev_load_instructions_;
    uint64_t prev_store_instructions_;

    uint64_t prev_stores_recv_;
    uint64_t prev_loads_recv_;
    uint64_t prev_store_cycles_;
    uint64_t prev_load_cycles_;
    template<typename Ownertype>
    explicit ROAStats(const Ownertype &);
};

}  // namespace timing_models
}  // namespace cpu
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_MODULES_CPU_MODELS_TYPES_H_
