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

#ifndef CORE_MODULES_CPU_MODELS_TYPES_IMPL_H_
#define CORE_MODULES_CPU_MODELS_TYPES_IMPL_H_

#include <cassert>
#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include "core/logic/coherence/single/Message.h"
#include "core/utils/macros.h"
#include "core/utils/Log.h"


namespace sim {
namespace cpu {
namespace timing_models {

using sim::utils::Log;

static inline bool overlap(sim::trace::addr_t x1, sim::trace::addr_t x2,
        sim::trace::addr_t y1, sim::trace::addr_t y2)
{
    return x1 <= y2 and y1 <= x2;
}

// x range contains y range
static inline bool contains(sim::trace::addr_t x1, sim::trace::addr_t x2,
        sim::trace::addr_t y1, sim::trace::addr_t y2)
{
    return x1 <= y1 and y2 <= x2;
}

inline rob_entry_t::rob_entry_t(sim::trace::mitem_t item, bool served,
        bool discarded, unsigned n_requests, struct rob_entry_t* dep):
    item_(item), served_(served), discarded_(discarded), pending_(n_requests), deps_()
{
    if (dep != NULL) {
        deps_.push_back(dep);
    }
}

inline
std::string rob_entry_t::to_string() const
{
     std::ostringstream oss;
     oss << item_ << ":" << served_ << ":" << discarded_;
     return oss.str();
}

template <typename Output>
inline
ROB<Output>::ROB(unsigned size, unsigned request_size, Output& out) :
    data_(),
    max_size_(size),
    max_request_size_(request_size),
    size_(0),
    out_(out),
    current_wd_id_(0)
{}

template <typename Output>
inline
std::pair<bool, typename ROB<Output>::elem_t*>
ROB<Output>::check_load_address_match(const sim::trace::mitem_t& item)
{
    std::pair<bool, elem_t*> ret(false, NULL);
    for (auto rit = data_.rbegin(); rit != data_.rend(); rit++) {
        if (rit->item_.op_ != sim::trace::mitem_t::NOMEM and not rit->discarded_) {
            if (contains(rit->item_.addr_, rit->item_.addr_ + rit->item_.size_ - 1,
                    item.addr_, item.addr_ + item.size_ - 1)) {
                Log::debug() << ": discard LOAD";
                ret.first = true;  // discard load
                return ret;
            }
        }
    }
    for (auto rit = data_.rbegin(); rit != data_.rend(); rit++) {
        if (rit->item_.op_ != sim::trace::mitem_t::NOMEM and not rit->discarded_) {
            if (rit->item_.op_ == sim::trace::mitem_t::STORE and
                    overlap(rit->item_.addr_, rit->item_.addr_ + rit->item_.size_ - 1,
                    item.addr_, item.addr_ + item.size_ - 1)) {
                Log::debug() << ": do not discard, but hold";
                ret.first = false;
                ret.second = &*rit;  // do not discard, but hold until previous dep store is sent
                return ret;
            }
        }
    }
    return ret;
}


template <typename Output>
inline
std::pair<bool, typename ROB<Output>::elem_t*>
ROB<Output>::check_store_address_match(const sim::trace::mitem_t& item)
{
    std::pair<bool, elem_t*> ret(false, NULL);
    data_reverse_iterator rit = data_.rbegin();
    while (rit != data_.rend() and not rit->discarded_) {
        if (rit->item_.op_ == sim::trace::mitem_t::STORE and
                rit->deps_.empty() and  // has no deps (if has deps cannot be discarded)
                contains(item.addr_, item.addr_ + item.size_,
                         rit->item_.addr_, rit->item_.addr_ + rit->item_.size_)) {
            ret.first = true;
            ret.second = &*rit;  // prev store to be discarded
            return ret;
        }
        ++rit;
    }

    return ret;
}


template <typename Output>
inline
typename ROB<Output>::elem_t*
ROB<Output>::process_load_insertion(const sim::trace::mitem_t& item)
{
    Log::debug() << "ROB::process_load_insertion(): addr = " << item.addr_;
    // check for previous non discarded matching
    std::pair<bool, elem_t*> ret = check_load_address_match(item);
    if (ret.first or item.size_ == 0) {
        // if discarded, do not send, mark as served
        assert(ret.second == NULL);
        data_.insert(data_.end(), elem_t(item, true, true, 0,  ret.second));
        return NULL;
    } else if (ret.second != NULL) {
        // not discarded, but hold
        assert(ret.second->item_.op_ == sim::trace::mitem_t::STORE);
        data_iterator it = data_.insert(data_.end(), elem_t(item, false, false, 1, ret.second));
        ret.second->deps_.push_back(&*it);
        return &*it;
    } else {
        // not discarded, no deps, send
        data_iterator it = data_.insert(data_.end(), elem_t(item, false, false, 0, NULL));
        issue_read_requests(*it);
        return &*it;
    }
}


template <typename Output>
inline
typename ROB<Output>::elem_t*
ROB<Output>::process_store_insertion(const sim::trace::mitem_t& item)
{
    // check for previous non discarded matching
    std::pair<bool, elem_t*> ret = check_store_address_match(item);
    if (ret.first) {
        assert(ret.second != NULL);
        ret.second->discarded_ = true;
    }

    // This marks the store to be pushed at commit:
    data_.push_back(elem_t(item, true, false, 0, NULL));
    return NULL;
}

template <typename Output>
inline
void
ROB<Output>::issue_read_requests(elem_t &elem)
{
    unsigned n_requests = 0;
    sim::engine::addr_t addr = elem.item_.addr_, size = elem.item_.size_;
    while (size) {
        // First we calculate the request size
        unsigned req_size = std::min(size, max_request_size_ - (addr % max_request_size_));
        sim::logic::coherence::single::Request request(sim::logic::coherence::single::Request::op_t::READ,
                addr, req_size, 0, elem.item_.data_);
        Log::debug() << "ROB::issue_read_requests: Adding a read request for address "
                     << std::hex << addr << std::dec << " and size " << req_size;
        out_.push(request);
        pending_loads_.insert(addr, req_size, &elem);
        size -= req_size;
        addr += req_size;
        n_requests++;
    }
    elem.pending_ = n_requests;
}

template <typename Output>
inline
void
ROB<Output>::issue_write_requests(elem_t &elem)
{
    sim::engine::addr_t addr = elem.item_.addr_, size = elem.item_.size_;
    while (size) {
        // First we calculate the request size
        unsigned req_size = std::min(size, max_request_size_ - (addr % max_request_size_));
        logic::coherence::single::Request request(logic::coherence::single::Request::op_t::WRITE,
                addr, 0, req_size, elem.item_.data_);
        Log::debug() << "ROB::issue_write_requests: Adding a write request for address "
                     << std::hex << addr << std::dec << " and size " << size;
        out_.push(request);
        size -= req_size;
        addr += req_size;
    }
}

template <typename Output>
inline
unsigned
ROB<Output>::number_of_requests(const elem_t &elem)
{
    unsigned count = 0;
    sim::engine::addr_t addr = elem.item_.addr_, size = elem.item_.size_;
    while (size) {
        // First we calculate the request size
        unsigned req_size = std::min(size, max_request_size_ - (addr % max_request_size_));
        size -= req_size;
        addr += req_size;
        count++;
    }
    if (elem.item_.op_ == sim::trace::mitem_t::STORE) {
        for (elem_t *read : elem.deps_) {
            count += number_of_requests(*read);
        }
    }
    return count;
}

template <typename Output>
inline
typename ROB<Output>::elem_t*
ROB<Output>::insert(const sim::trace::mitem_t& item)
{
    assert(size_ + item.ins_ <= max_size_);

    size_ += item.ins_;
    if (item.op_ == sim::trace::mitem_t::LOAD) {
        assert(not out_.full());
        return process_load_insertion(item);
    } else if (item.op_ == sim::trace::mitem_t::STORE) {
        return process_store_insertion(item);
    } else if (item.op_ == sim::trace::mitem_t::NOMEM) {
        data_.push_back(elem_t(item, true, false, 0, NULL));
        return NULL;
    } else {
        std::cerr << "Memory item type not supported: " << item.op_ << std::endl;
        exit(1);
    }
}

template <typename Output>
inline
unsigned ROB<Output>::free_entries() const
{
    return (max_size_ - size_);
}

template <typename Output>
inline
void
ROB<Output>::process_ack(const sim::logic::coherence::single::Message::Ack &ack)
{
    elem_t *elem = pending_loads_.retrieve(ack.get_tag(), ack.get_data_size());
    if (elem == nullptr) {
        Log::error() << "ROB::process_ack(): Cannot find pending load for address "
                     << std::hex << ack.get_tag() << std::dec << " and size " << ack.get_size();
        exit(-1);
    }
    this->ack(*elem);
}

template <typename Output>
inline
void
ROB<Output>::ack(elem_t &elem)
{
    elem.pending_--;
    if (elem.pending_ == 0) {
        elem.served_ = true;
    }
}

template <typename Output>
inline
void
ROB<Output>::send_store(elem_t &top)
{
    issue_write_requests(top);
}


template <typename Output>
inline
void
ROB<Output>::send_store_and_deps(elem_t &top)
{
    issue_write_requests(top);
    for (typename std::vector<elem_t*>::iterator it = top.deps_.begin(); it != top.deps_.end(); ++it) {
        elem_t& e = *(*it);
        assert(e.deps_.size() == 1);
        assert(e.deps_[0] == &top);
        issue_read_requests(e);
        e.deps_.clear();
    }
}


template <typename Output>
inline
unsigned
ROB<Output>::retire(unsigned ins)
{
    assert(ins <= max_size_);

    if (size_ == 0) {
        assert(data_.empty());
        return 0;
    }
    assert(not data_.empty() and size_ > 0);

    elem_t* top = &data_.front();

    if (top->item_.ins_ == 1 and not top->served_) {
        return 0;
    }

    unsigned total_removed = 0;
    while (ins >= top->item_.ins_) {
        unsigned n_requests = number_of_requests(*top);
        if (top->served_ and top->deps_.empty() and  // if top element is ready
            (not(top->item_.op_ == sim::trace::mitem_t::STORE) or top->discarded_  // and non discarded st can be sent
             or out_.free_entries() >= n_requests)) {
            // remove top elem
            if (top->item_.op_ == sim::trace::mitem_t::STORE and not top->discarded_) {
                send_store(*top);
            }
            size_ -= top->item_.ins_;
            ins -= top->item_.ins_;
            total_removed += top->item_.ins_;
            data_.pop_front();
            if (data_.empty()) {
                return total_removed;
            }
            top = &data_.front();
        } else if (not top->deps_.empty() and  // store has dep loads
                out_.free_entries() >= n_requests) {
            // and all fit in out buffer, retire
            assert(top->item_.op_ == sim::trace::mitem_t::STORE);
            send_store_and_deps(*top);
            size_ -= top->item_.ins_;
            ins -= top->item_.ins_;
            total_removed += top->item_.ins_;
            data_.pop_front();
            if (data_.empty()) {
                return total_removed;
            }
            top = &data_.front();
        } else {
            unsigned remove = top->item_.ins_ - 1;
            size_ -= remove;
            ins -= remove;
            top->item_.ins_ -= remove;
            assert(top->item_.ins_ == 1);
            total_removed += remove;

            return total_removed;
        }
    }

    assert(ins < top->item_.ins_);

    top->item_.ins_ -= ins;
    size_ -= ins;
    total_removed += ins;

    return total_removed;
}

template <typename Output>
inline
bool
ROB<Output>::check_load_fits_output_buffer(const sim::trace::mitem_t &item)
{
    elem_t elem(item, false, false, 0, NULL);
    return number_of_requests(elem) <= out_.free_entries();
}

template <typename Output>
inline
unsigned
ROB<Output>::get_ins_to_first_waiting_item() const
{
    unsigned ins = 0;
    data_const_iterator it = data_.begin();
    while (it != data_.end() and it->served_ and
            (it->item_.op_ != sim::trace::mitem_t::STORE or it->discarded_)) {
        ins += it->item_.ins_;
        ++it;
    }
    if (it != data_.end()) {
        ins += it->item_.ins_;
    }
    return ins;
}

template <typename Output>
inline
std::size_t ROB<Output>::capacity() const
{
    return max_size_;
}

template <typename Output>
inline
bool ROB<Output>::empty() const
{
    return data_.empty();
}

template <typename Output>
inline
void
ROB<Output>::set_current_wd_id(uint64_t wd_id)
{
    current_wd_id_ = wd_id;
}

template <typename Output>
inline
void
ROB<Output>::print_status() const
{
    Log::warning() << "ROB status:";
    Log::warning() << "Max Size: " << max_size_ << ", current size = " << size_;
    for (const elem_t &elem : data_) {
        Log::warning() << " ROB entry served? " << (elem.served_?"yes":"no")
                       << ", discarded? " << (elem.discarded_?"yes":"no")
                       << ", pending requestas " << elem.pending_
                       << ", dependencies: " << elem.deps_.size()
                       << ", associated item: " << elem.item_;
    }
    pending_loads_.print_status();
}

template <typename T>
inline
OutputBuffer<T>::~OutputBuffer()
{
    if (out_.size() > 0)
        std::cout << "warning: " << out_.size()
                  << " memory accesses left in output buffer" << std::endl;
}


template <typename T>
bool
LDQueue<T>::insert(sim::trace::addr_t addr, unsigned size, T elem)
{
    Log::debug() << "LDQueue::insert: addr = " << std::hex << addr << std::dec << " size " << size;
    std::pair<typename data_map::iterator, bool> ret =
            data_.insert(typename data_map::value_type(addr, access_list()));

    if (ret.second == true) {
        // newly inserted
        ret.first->second.push_back(access_list_elem(size, elem));
        return true;
    } else {
        // outstanding accesses to same address existing
        access_list& q = ret.first->second;
        assert(not q.empty());
        q.emplace_back(size, elem);
        return true;
    }
}


template <typename T>
T
LDQueue<T>::retrieve(sim::trace::addr_t addr, unsigned size)
{
    Log::debug() << "LDQueue::retrieve: addr = " << std::hex << addr << std::dec << " size " << size;
    typename data_map::iterator it = data_.find(addr);

    if (it == data_.end()) {
        Log::warning() << "LDQueue::retrieve: cannot find READ Requests for address " << std::hex << addr << std::dec;
        return static_cast<T>(0);
    }

    access_list& q = it->second;
    for (typename access_list::iterator it2 = q.begin(); it2 != q.end(); ++it2) {
        if (it2->first == size) {
            access_list_elem e = *it2;
            q.erase(it2);
            if (q.empty()) {
                data_.erase(it);
            }
            return e.second;
        }
    }
    Log::warning() << "LDQueue::retrieve: cannot find READ Requests for address "
                 << std::hex << addr << std::dec << " and size " << size;
    return static_cast<T>(0);
}

template<typename T>
void LDQueue<T>::print_status() const
{
    Log::warning() << " - LDQueue elements: " << data_.size();
    for (const auto &elem : data_) {
        Log::warning() << " + Addr " << std::hex << elem.first << std::dec;
        for (const auto &sub : elem.second) {
            Log::warning() << "   - Size " << static_cast<unsigned>(sub.first) << " req " << sub.second->to_string();
        }
    }
}


template<typename Ownertype>
inline
ROAStats::ROAStats(const Ownertype &owner) :
    num_loads_total_(owner, "ROA number of loads total"),
    num_loads_sent_(owner, "ROA number of loads sent"),
    num_loads_received_(owner, "ROA number of loads acks received"),
    num_stores_total_(owner, "ROA number of stores total"),
    num_stores_sent_(owner, "ROA number of stores sent"),
    num_stores_received_(owner, "ROA number of stores acks received"),
    committed_ins_(owner, "ROA total number of committed instructions"),
    stalled_cycles_(owner, "ROA total number of stalled cycles"),
    out_full_cycles_(owner, "ROA total number of output full"),
    load_cycles_(owner, "ROA number of cycles spent waiting for load responses"),
    store_cycles_(owner, "ROA number of cycles spent waiting for store responses"),
    prev_issued_instructions_(0),
    prev_committed_instructions_(0),
    prev_cycle_count_(0),
    prev_stalled_cycles_(0),
    prev_load_instructions_(0),
    prev_store_instructions_(0),
    prev_stores_recv_(0),
    prev_loads_recv_(0),
    prev_store_cycles_(0),
    prev_load_cycles_(0)
{
}

}  // namespace timing_models
}  // namespace cpu
}  // namespace sim

#endif  // CORE_MODULES_CPU_MODELS_TYPES_IMPL_H_
