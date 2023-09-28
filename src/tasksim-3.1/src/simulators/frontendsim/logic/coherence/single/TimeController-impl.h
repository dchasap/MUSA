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


#ifndef SIMULATORS_FRONTENDSIM_LOGIC_COHERENCE_SINGLE_TIMECONTROLLER_IMPL_H_
#define SIMULATORS_FRONTENDSIM_LOGIC_COHERENCE_SINGLE_TIMECONTROLLER_IMPL_H_

#include <list>
#include <set>
#include <utility>
#include <vector>

#include "core/utils/macros.h"
#include "core/utils/Log.h"


namespace sim {
namespace logic {
namespace coherence {
namespace single {

using sim::utils::Log;


template<typename Message>
inline
TimeController<Message>::ack_entry_t::ack_entry_t(Ack &&ack, typename MSHR::entry_t *mshr_entry) :
    ack_(std::move(ack)),
    mshr_entry_(mshr_entry)
{}


template<typename Message>
typename TimeController<Message>::req_handler_t TimeController<Message>::Req_Handlers_[] = {
        &TimeController<Message>::read_req,          /* READ_REQ */
        &TimeController<Message>::write_req,         /* WRITE_REQ */
        &TimeController<Message>::flush_req,         /* FLUSH_REQ */
        &TimeController<Message>::invalidate_req,    /* INVALIDATE_REQ */
        &TimeController<Message>::prefetch_req,      /* PREFETCH_REQ */
        &TimeController<Message>::clear_req,         /* CLEAR_REQ */
        NULL
};


template<typename Message>
typename TimeController<Message>::ack_handler_t TimeController<Message>::Ack_Handlers_[] = {
        &TimeController<Message>::data_ack,          /* READ_ACK */
        &TimeController<Message>::write_ack,         /* WRITE_ACK */
        &TimeController<Message>::null_ack,          /* FLUSH_ACK */
        &TimeController<Message>::null_ack,          /* INVALIDATE_ACK */
        &TimeController<Message>::prefetch_ack,      /* PREFETCH_ACK */
        &TimeController<Message>::clear_ack,         /* CLEAR_ACK */
        &TimeController<Message>::miss_ack,          /* MISS_ACK */
        NULL
};


template<typename Message>
inline
TimeController<Message>::TimeController(std::size_t cpu_id, sim::memory::CacheStats &stats,
        sim::engine::Simulator &simulator, const sim::engine::Config &cache_config) :
    coherence::Controller<engine::addr_t, CacheLine>(cpu_id, simulator, cache_config),
    simulator_(simulator),
    mask_(~(this->line_size_ - 1)),
    mshr_(sim::engine::Config(simulator.get_config(), cache_config.get_value<std::string>("mshr")), this->line_size_),
    stalled_(false),
    output_ack_buffer_(mshr_.get_size()),
    stats_(stats)
{
}


template<typename Message>
inline
TimeController<Message>::~TimeController()
{
}


template<typename Message>
inline
void TimeController<Message>::add_output_request(Request &&request)
{
    output_request_buffer_.emplace_back(std::move(request));
    assert(output_request_buffer_.size() <= mshr_.get_size());
}


template<typename Message>
inline
void TimeController<Message>::add_output_ack(unsigned in, Ack &&ack, typename MSHR::entry_t *mshr_entry)
{
    output_ack_buffer_[in].emplace_back(ack_entry_t(std::move(ack), mshr_entry));
#if !defined(NDEBUG) && 0
    if (output_ack_buffer_[in].back().mshr_entry_ != NULL) {
        /* Check that the MSHR entry has pending requests */
        assert(output_ack_buffer_[in].back().mshr_entry_->list_.empty() == false);
        typename std::list<ack_entry_t>::const_iterator i;
        for (i = output_ack_buffer_[in].begin(); i != output_ack_buffer_[in].end(); ++i) {
            typename std::list<ack_entry_t>::const_iterator next = i; next++;
            if (next == output_ack_buffer_[in].end()) break;
            assert(i->mshr_entry_ != output_ack_buffer_[in].back().mshr_entry_);
        }
    }
#endif
    //assert(output_ack_buffer_[in].size() <= mshr_.get_size());
}


template<typename Message>
inline
void TimeController<Message>::allocate(const engine::addr_t &addr)
{
    bool ret = this->cache_.insert(addr & mask_, CacheLine(CacheLine::ALLOCATED));
    assert(ret == true);
    _unused(ret);
}


template<typename Message>
inline
bool TimeController<Message>::evict(const engine::addr_t &addr,
        typename MSHR::entry_t &mshr_entry)
{
    /* Check if there is still free space in the cache. If so, allocate the line */
    if (this->cache_.is_full(addr & mask_) == false) {
        allocate(addr);
        return false;
    }

    /* Evict a line and allocate a new one */
    std::pair<engine::addr_t, CacheLine> ret = this->cache_.evict(addr & mask_);
    allocate(addr);

    /* If the cacheline is not dirty, the miss request can be issued */
    if (ret.second.state_ != CacheLine::DIRTY) {
        return false;
    }

    /* Mark the MSHR as pending */
    evict_table_[ret.first] = &mshr_entry;

    /* Send the write-back request */
    add_output_request(Request(Request::op_t::WRITE, ret.first, 0, this->line_size_));
    return true;
}


template<typename Message>
inline
bool TimeController<Message>::process_request(unsigned input_port, const Request &request)
{
    Log::debug4() << "Controller::process_request; L" << this->level_ << " addr=" << request.get_tag()
                  << " , size= " << request.get_size() << ", op=" << request.get_op();

    assert(request.get_op() < Request::op_t::LAST);
    typename Ack::op_t op = (Req_Handlers_[static_cast<unsigned>(request.get_op())])(*this, input_port, request);
    assert(op < Ack::op_t::LAST);

    if (op == Ack::op_t::MISS) {
#ifdef ENABLE_COMPULSORY_MISS
        // UGI: perfect place to see if it is compulsory or non-compuslory miss
        if (request.get_op() == Request::op_t::READ) {
            if (requested_lines_.find(req.get_tag() & mask_) == requested_lines_.end())
                stats_.read.comp_miss++;
            else
                stats_.read.non_comp_miss++;
            requested_lines_.insert(req.get_tag() & mask_);
            stats_.read.miss++;
        }
#endif
        return false;
    }
    add_output_ack(input_port, Ack(request));
    return true;
}


template<typename Message>
inline
void TimeController<Message>::process_ack(const Ack &ack)
{
    assert(ack.get_op() < Ack::op_t::MISS); /* Ack::op_t::MISS is only used internally to the controller */
    (Ack_Handlers_[static_cast<unsigned>(ack.get_op())])(*this, ack);
}


template<typename Message>
template<typename Output>
inline
bool TimeController<Message>::send_request(Output &out)
{
    if (output_request_buffer_.empty()) {
        return false;
    }
    bool ret = out.write_request(output_request_buffer_.front());
    if (ret == false) {
        return false;  /* Could not write, try next time */
    }
    output_request_buffer_.pop_front();
    return true;
}


template<typename Message>
template<typename Input>
inline
bool TimeController<Message>::send_ack(unsigned n, Input &in)
{
    assert(n < output_ack_buffer_.size());
    if (output_ack_buffer_[n].empty()) {
        return false;
    }

    ack_entry_t &ack_entry = output_ack_buffer_[n].front();
    /* We can try to write before processing the request because
     * we hold a reference to the entry.
     */
    bool ret = in.write_ack(ack_entry.ack_);
    if (ret == false) {
        return false;
    }

    /* Get the MSHR entry with the pending requests */
    if (ack_entry.mshr_entry_ != NULL) {
        assert(ack_entry.mshr_entry_->list_.empty() == false);
        /* Remove the sub-entry associated to the request */
        assert(ack_entry.mshr_entry_->state_.value_ == MSHR::MSHRState::READY);
        assert(ack_entry.mshr_entry_->list_.begin()->request_.get_tag() == ack_entry.ack_.get_tag());
        assert(ack_entry.mshr_entry_->list_.begin()->request_.get_size() == ack_entry.ack_.get_size());
        assert((ack_entry.ack_.get_op() != Ack::op_t::READ) ||
                ack_entry.mshr_entry_->list_.begin()->request_.get_size() == ack_entry.ack_.get_data_size());
        ack_entry.mshr_entry_->list_.pop_front();

        /* If there are no more pending sub-entries, remove the MSHR entry */
        if (ack_entry.mshr_entry_->list_.empty() == true) {
            mshr_.remove_entry(*ack_entry.mshr_entry_);
        }
    }

    output_ack_buffer_[n].pop_front();
    return true;
}


template<typename Message>
inline
typename Message::Ack::op_t TimeController<Message>::read_req(unsigned input_port, const Request &request)
{
    sim::engine::addr_t addr = request.get_tag();
    sim::engine::addr_t line_addr = addr & mask_;
    CacheLine *line = this->cache_.access(line_addr);
    if (line != NULL && line->state_ != CacheLine::ALLOCATED) {
        assert(line->state_ != CacheLine::INVALID);
        stats_.read.hit++;
        /* TODO: to add per-set statistics, use this->cache_.get_set(addr & mask) */
        return Ack::op_t::READ; /* Cache hit */
    }
    /* Cache miss -- check the MSHR for a half-miss */
    typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(addr);
    bool new_mshr_entry = (mshr_entry == NULL);
    if (mshr_entry == NULL) {
        if (!this->cache_.can_evict(addr)) {
            stalled_ = true;
            return Ack::op_t::MISS;
        }
        assert(line == NULL);
        mshr_entry = mshr_.allocate_entry(addr, request);
        if (mshr_entry == NULL) {
            stalled_ = true;
            return Ack::op_t::MISS;
        }
    }
    else {
        if (line == NULL) {
            stalled_ = true;
            return Ack::op_t::MISS;
        }
        assert(line != NULL);
        assert(line->state_ == CacheLine::ALLOCATED);
        stats_.read.half_miss++;
    }
    mshr_entry->list_.emplace_back(typename MSHR::subentry_t(input_port, request));

    if (new_mshr_entry) {
        /* Full miss, evict a line and create read request */
        if (evict(addr, *mshr_entry) == true) {
            mshr_entry->state_ = MSHR::MSHRState::PENDING;
            return Ack::op_t::MISS;
        }
    }
    mshr_request(*mshr_entry);
    return Ack::op_t::MISS;
}


template<typename Message>
inline
typename Message::Ack::op_t TimeController<Message>::write_req(unsigned input_port, const Request &request)
{
    sim::engine::addr_t addr = request.get_tag();
    sim::engine::addr_t line_addr = addr & mask_;
    CacheLine *line = this->cache_.access(line_addr);
    if (line != NULL && line->state_ != CacheLine::ALLOCATED) { /* Cache hit */
        line->state_ = CacheLine::DIRTY;
        stats_.write.hit++;;
        return Ack::op_t::WRITE;
    }
    /* Cache miss -- check MSHR for a half-miss */
    typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(addr);
    bool new_mshr_entry = (mshr_entry == NULL);
    if (mshr_entry == NULL) {
        if (!this->cache_.can_evict(addr)) {
            stalled_ = true;
            return Ack::op_t::MISS;
        }
        assert(line == NULL);
        mshr_entry = mshr_.allocate_entry(addr, request);
        assert(mshr_entry != NULL);
        stats_.write.miss++;
    }
    else {
        assert(line != NULL);
        assert(line->state_ == CacheLine::ALLOCATED);
        stats_.write.half_miss++;
    }
    mshr_entry->list_.emplace_back(typename MSHR::subentry_t(input_port, request));
    if (new_mshr_entry) { /* This is a new entry in the MSHR */
        /* Full miss, evict a line and create read request */
        if (evict(addr, *mshr_entry) == true) {
            mshr_entry->state_ = MSHR::MSHRState::PENDING;
            return Ack::op_t::MISS;
        }
    }

    mshr_request(*mshr_entry);
    return Ack::op_t::MISS;
}


template<typename Message>
inline
typename Message::Ack::op_t TimeController<Message>::flush_req(unsigned input_port, const Request &request)
{
    const sim::engine::addr_t addr = request.get_tag();
    const sim::engine::addr_t line_addr = addr & mask_;
    CacheLine *line = this->cache_.access(line_addr);
    if (line != NULL && line->state_ != CacheLine::ALLOCATED) {
        if (line->state_ == CacheLine::DIRTY) {
            add_output_request(Request(Request::op_t::WRITE, line_addr, 0, this->line_size_));
        }
        this->cache_.remove(addr);
        add_output_request(Request(request));
        stats_.flush.hit++;
    } else {
        stats_.flush.miss++;
    }
    return Ack::op_t::FLUSH;
}


template<typename Message>
inline
typename Message::Ack::op_t TimeController<Message>::invalidate_req(unsigned input_port, const Request &request)
{
    const sim::engine::addr_t addr = request.get_tag();
    const sim::engine::addr_t line_addr = addr & mask_;
    CacheLine *line = this->cache_.access(line_addr);
    if (line != NULL && line->state_ != CacheLine::ALLOCATED) {
        this->cache_.remove(line_addr);
        stats_.invalidate.hit++;
    }  else {
        stats_.invalidate.miss++;
    }
    add_output_request(Request(request));
    return Ack::op_t::INVALIDATE;
}


template<typename Message>
inline
typename Message::Ack::op_t TimeController<Message>::prefetch_req(unsigned input_port, const Request &request)
{
    assert(0);
    return Ack::op_t::READ;  // to avoid warning
}


template<typename Message>
inline
typename Message::Ack::op_t TimeController<Message>::clear_req(unsigned input_port, const Request &request)
{
    const sim::engine::addr_t addr = request.get_tag();
    std::size_t size = request.get_size();
    unsigned pending_write_acks = 0;
    unsigned pending_read_acks = 0;
    for (engine::addr_t line_addr = addr & mask_; line_addr < addr + size; line_addr += this->line_size_) {
        CacheLine *line = this->cache_.access(line_addr);
        if (line == NULL) continue;
        if (line->state_ == CacheLine::VALID) {
            this->cache_.remove(line_addr);
            continue;
        }
        assert(line->state_ != CacheLine::ALLOCATED);
        assert(line->state_ != CacheLine::INVALID);

        if (line->state_ == CacheLine::DIRTY) {
            if (!evict_table_lookup(line_addr)) {
                add_output_request(Request(Request::op_t::WRITE, line_addr, 0, this->line_size_));
                evict_table_[line_addr] = NULL;
                this->cache_.remove(line_addr);
            }
            pending_write_acks++;
        }

        if (line->state_ == CacheLine::ALLOCATED)
            pending_read_acks++;
    }

    clear_request_list_.push_back(clear_request_t(input_port, pending_write_acks, pending_read_acks, request));
    process_clear_request(--clear_request_list_.end());

    return Ack::op_t::MISS;
}


template<typename Message>
inline
bool TimeController<Message>::mshr_lookup(const engine::addr_t &addr)
{
    typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(addr);
    if ((mshr_entry == NULL) or (mshr_entry->state_.value_ != MSHR::MSHRState::SENT)) {
        return false;
    } else {
        mshr_entry->state_.value_ = MSHR::MSHRState::INSPECTED;
        return true;
    }
}

template<typename Message>
inline
bool TimeController<Message>::evict_table_lookup(const engine::addr_t &addr)
{
    typename EvictTable::const_iterator i = evict_table_.find(addr);
    if (i == evict_table_.end()) return false;

    return true;
}


template<typename Message>
inline
void TimeController<Message>::process_clear_request(typename ClearRequestList::iterator clear_request_iterator)
{
    if (clear_request_iterator->pending_write_acks_ + clear_request_iterator->pending_read_acks_ > 0) {
        return;
    }

    if (this->last_level_) {
        add_output_ack(clear_request_iterator->port_, Ack(clear_request_iterator->request_));
        clear_request_list_.erase(clear_request_iterator);
    } else {
        add_output_request(Request(clear_request_iterator->request_));
    }
}


template<typename Message>
inline
void TimeController<Message>::data_ack(const Ack &ack)
{
    const sim::engine::addr_t addr = ack.get_tag();
    /* Get the MSHR entry with the pending requests */
    typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(addr);
    assert(mshr_entry != NULL);
    assert(mshr_entry->state_.value_ != MSHR::MSHRState::READY);
    stalled_ = false;

    /* Add an ack entry for each pending request */
    unsigned write_count = 0;

    for (auto i = mshr_entry->list_.begin(); i != mshr_entry->list_.end(); ++i) {
        assert(i->port_ < output_ack_buffer_.size());
        assert((i->request_.get_op() != Request::op_t::READ) || (i->request_.get_data_size() == 0) );
        assert((i->request_.get_op() == Request::op_t::READ) || (i->request_.get_size() == 0) );
        if (i->request_.get_op() == Request::op_t::WRITE) {
            write_count++;
        }
        // Append time stamps that you just got...
        i->request_.touch(ack);

        add_output_ack(i->port_, Ack(i->request_), mshr_entry);
    }

    CacheLine *line = this->cache_.access(addr & mask_);
    if (line == NULL) {
        return;
    }
    assert(line != NULL);
    assert(line->state_ == CacheLine::ALLOCATED);
    line->state_ = (write_count > 0) ? CacheLine::DIRTY : CacheLine::VALID;
    /* Set the entry as ready, but do not remove it */
    mshr_entry->state_.value_ = MSHR::MSHRState::READY;
    stats_.miss_time.sample(simulator_.get_clock() - mshr_entry->state_.time_);
    stats_.miss_time_per_task_type[mshr_entry->request_.get_task_type()].sample(simulator_.get_clock() - mshr_entry->state_.time_);

    typename ClearRequestList::iterator it = clear_request_list_.begin();
    while (it != clear_request_list_.end()) {
        typename ClearRequestList::iterator curr = it;
        ++it;
        sim::engine::addr_t clear_addr = curr->request_.get_tag();
        std::size_t clear_size = curr->request_.get_size();
        if ((addr >= (clear_addr & mask_)) && (addr < clear_addr + clear_size)) {
            curr->pending_read_acks_--;
            if (line->state_ == CacheLine::DIRTY) {
                add_output_request(Request(Request::op_t::WRITE, addr, 0, this->line_size_));
                evict_table_[addr] = NULL;
                this->cache_.remove(addr);
                curr->pending_write_acks_++;
            }
            process_clear_request(curr);
        }
    }
}


template<typename Message>
inline
void TimeController<Message>::write_ack(const Ack &ack)
{
    const sim::engine::addr_t addr = ack.get_tag();
    /* Check that there are pending write requests */
    typename EvictTable::const_iterator i = evict_table_.find(addr);
    assert(i != evict_table_.end());

    if (i->second != NULL)
    {
        /* Find the MSHR entry associated to the eviction */
        typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(i->second->tag_);
        assert(mshr_entry == i->second);
        _unused(mshr_entry);

        mshr_evict_ack(*(i->second));
    }

    evict_table_.erase(i);

    typename ClearRequestList::iterator it = clear_request_list_.begin();
    while (it != clear_request_list_.end()) {
        typename ClearRequestList::iterator curr = it;
        ++it;

        sim::engine::addr_t clear_addr = curr->request_.get_tag();
        std::size_t clear_size = curr->request_.get_size();
        if ((addr >= (clear_addr & mask_)) && (addr < clear_addr + clear_size)) {
            curr->pending_write_acks_--;
            process_clear_request(curr);
        }
    }
}

template<typename Message>
inline
void TimeController<Message>::null_ack(const Ack &ack)
{
}

template<typename Message>
inline
void TimeController<Message>::prefetch_ack(const Ack &ack)
{
    assert(0);
}

template<typename Message>
inline
void TimeController<Message>::clear_ack(const Ack &ack)
{
    for (typename ClearRequestList::iterator it = clear_request_list_.begin(); it != clear_request_list_.end(); ++it) {
        if ((ack.get_tag() == it->request_.get_tag()) && (ack.get_size() == it->request_.get_size())) {
            add_output_ack(it->port_, Ack(it->request_));
            clear_request_list_.erase(it);
            return;
        }
    }
    Log::error() << "[ERROR] Received clear request for addres " << ack.get_tag() << " and size " << ack.get_size()
                 << " but we did not generate/relly it.";
    exit(-1);
}

template<typename Message>
inline
void TimeController<Message>::miss_ack(const Ack &ack)
{
}

template<typename Message>
inline
void TimeController<Message>::mshr_request(typename MSHR::entry_t &mshr_entry)
{
    switch (mshr_entry.state_.value_) {
        case MSHR::MSHRState::INVALID:
            add_output_request(Request(Request::op_t::READ, mshr_entry.tag_, this->line_size_));
            mshr_entry.state_.value_ = MSHR::MSHRState::SENT;
            mshr_entry.state_.time_ = simulator_.get_clock();
            break;
        case MSHR::MSHRState::PENDING:
        case MSHR::MSHRState::SENDING:
        case MSHR::MSHRState::SENT:
        case MSHR::MSHRState::INSPECTED:
            break;
        case MSHR::MSHRState::READY:
            assert(0);
            break;
    }
}

template<typename Message>
inline
void TimeController<Message>::mshr_evict_ack(typename MSHR::entry_t &mshr_entry)
{
    switch (mshr_entry.state_.value_) {
        case MSHR::MSHRState::PENDING:
            add_output_request(Request(Request::op_t::READ, mshr_entry.tag_, this->line_size_));
            mshr_entry.state_ = MSHR::MSHRState::SENT;
            mshr_entry.state_.time_ = simulator_.get_clock();
            break;
        case MSHR::MSHRState::SENDING:
        case MSHR::MSHRState::SENT:
        case MSHR::MSHRState::INSPECTED:
            break;
        case MSHR::MSHRState::INVALID:
        case MSHR::MSHRState::READY:
            assert(0);
            break;
    }
}

template<typename Message>
inline
bool TimeController<Message>::is_stalled()
{
    return stalled_;
}

template<typename Message>
inline
bool TimeController<Message>::inspect(const Ack &ack)
{
    if (Ack::op_t::READ == ack.get_op()) {
        return this->mshr_lookup(ack.get_tag());
    } else if (Ack::op_t::WRITE == ack.get_op()) {
        return this->evict_table_lookup(ack.get_tag());
    }
    return false;
}

template<typename Message>
inline
size_t TimeController<Message>::get_free_mshr_size() const
{
    return mshr_.get_free_size();
}

template<typename Message>
inline
size_t TimeController<Message>::get_mshr_size() const
{
    return mshr_.get_size();
}

template<typename Message>
inline
bool TimeController<Message>::is_empty_output_request_buffer()
{
    return output_request_buffer_.empty();
}

template<typename Message>
inline
bool TimeController<Message>::is_empty_output_ack_buffer()
{
    for (auto &ack_buffer : output_ack_buffer_) {
        if (ack_buffer.empty() == false) {
            return false;
        }
    }
    return true;
}

template<typename Message>
inline
size_t TimeController<Message>::get_free_lines()
{
    return this->cache_.get_free_lines();
}

template<typename Message>
inline
void TimeController<Message>::print_status() const
{
    Log::warning() << " - Logic::cache status:";
    Log::warning() << " - Pending requests: " << output_request_buffer_.size();
    for (const auto& req : output_request_buffer_) {
        Log::warning() << " + " << req;
    }
    for (unsigned i = 0 ; i < output_ack_buffer_.size(); i++) {
        Log::warning() << " - Port " << i << " has " << output_ack_buffer_[i].size() << " pending ACKs.";
        for (const auto& ack : output_ack_buffer_[i]) {
            Log::warning() << "   - Ack: " << ack.ack_;
        }
    }
    Log::warning() << " - MSHR status:";
    mshr_.print_status();
}

}  // namespace single
}  // namespace coherence
}  // namespace logic
}  // namespace sim


#endif  // SIMULATORS_FRONTENDSIM_LOGIC_COHERENCE_SINGLE_TIMECONTROLLER_IMPL_H_
