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


#ifndef CORE_LOGIC_COHERENCE_SINGLE_CONTROLLER_IMPL_H_
#define CORE_LOGIC_COHERENCE_SINGLE_CONTROLLER_IMPL_H_

#include <algorithm>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/utils/macros.h"
#include "core/utils/Log.h"
#include "core/stats/Counter.h"
#include "core/logic/memory/MSHR.h"

namespace sim {
namespace logic {
namespace coherence {
namespace single {

using sim::utils::Log;

template<typename Message>
inline
Controller<Message>::ack_entry_t::ack_entry_t(Ack &&ack, typename MSHR::entry_t *mshr_entry) :
    ack_(std::move(ack)),
    mshr_entry_(mshr_entry)
{}

// Public functions:
template<typename Message>
inline
Controller<Message>::Controller(std::size_t cpu_id, sim::memory::CacheStats &stats, sim::engine::Simulator &simulator,
        sim::logic::coherence::single::Directory<Message> &directory, const engine::Config &cache_config) :
    coherence::Controller<engine::addr_t, CacheLine>(cpu_id, simulator, cache_config),
    simulator_(simulator),
    mask_(~(this->line_size_ - 1)),
    directory_(directory),
    mshr_(sim::engine::Config(simulator.get_config(), cache_config.get_value<std::string>("mshr")), this->line_size_),
    victim_cache_(simulator,
            std::max(cache_config.get_value<std::size_t>("victim-lines"), 4UL),
            std::max(cache_config.get_value<std::size_t>("victim-lines"), 4UL),
            this->line_size_,
            true,
            sim::engine::Config()),
    stalled_(false),
    victim_cache_affinity_(cache_config.key_exists("victim-cache-affinity") ?
            sim::runtime::string_to_task_dep.at(cache_config.get_value<std::string>("victim-cache-affinity")) :
            task_dep_t::ANY),
    max_requests_(cache_config.get_value<std::size_t>("num-ports")),
    output_request_buffer_(),
    output_ack_buffer_(1),
    stats_(stats)
{
    /* initialize statistics for all types of accesses */
    for (auto it = sim::runtime::string_to_task_dep.begin(); it != sim::runtime::string_to_task_dep.end(); ++it) {
        stats_.hit_by_access_type[it->second] += 0;
        stats_.miss_by_access_type[it->second] += 0;
        stats_.requests_by_access_type[it->second] += 0;
    }
}


template<typename Message>
inline
Controller<Message>::~Controller()
{
    assert(pending_requests_.size()  == 0);
    // How do we set a for inside the assert?
    // assert(not(output_request_buffer_));
    assert(mshr_.get_size() == mshr_.get_free_size());
}


template<typename Message>
inline
bool Controller<Message>::process_request(unsigned input_port, const Request &request)
{
    const sim::engine::addr_t addr = request.get_tag();
    Log::debug2() << "[CacheL" << level_ << ":" << this->cpu_id_
                  << "] process_request addr = 0x" << std::hex << addr << std::dec
                  << " , size= " << request.get_size() << ", op=" << static_cast<unsigned>(request.get_op());

    assert(request.get_op() < Request::op_t::LAST);
    typename Ack::op_t op = (Req_Handlers_[static_cast<unsigned>(request.get_op())])(*this, input_port, request);
    assert(op < Ack::op_t::LAST);

    if (not stalled_ and is_special_request(request)) {
        process_special_request(request);
    }

    if (op == Ack::op_t::MISS) {
#ifdef ENABLE_COMPULSORY_MISS
        // UGI: perfect place to see if it is compulsory or non-compuslory miss
        if (request.get_op() == Request::op_t::READ) {
            if (requested_lines_.find(line_addr) == requested_lines_.end())
                stats_.read.comp_miss++;
            else
                stats_.read.non_comp_miss++;
            requested_lines_.insert(addr & mask_);
        }
#endif
        Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_
                      << "] process_request is a MISS.";
        return false;
    } else {
        Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_
                      << "] process_request is a HIT.";
        add_output_ack(input_port, Ack(request));
        return true;
    }
}


template<typename Message>
inline
void Controller<Message>::process_ack(const Ack &ack)
{
    Log::debug2() << "Controller::process_ack L" << this->level_ << " addr = 0x" << std::hex << ack.get_tag() << std::dec
                  << " , size= " << ack.get_size() << ", op=" << static_cast<unsigned>(ack.get_op());
    assert(ack.get_op() < Ack::op_t::MISS); /* Ack::op_t::MISS is only used internally to the controller */
    (Ack_Handlers_[static_cast<unsigned>(ack.get_op())])(*this, ack);
}


template<typename Message>
template<typename Output>
inline
bool Controller<Message>::send_request(Output &output)
{
    /* FCSC:
     * Here we choose the request to send.
     * 1st: If victim cache is full and we are stalled, send a victim line.
     * 2nd: If there is a MSHR request, sent it instead.
     * 3rd: Try to empty the victim cache.
     */
    if (output_request_buffer_.empty()) {
        // We do not have a pending request on the buffer.
        if (pending_requests_.size()) {
            // We can send requests
            typename MSHR::entry_t& mshr_entry = *pending_requests_.front();
            output_request_buffer_.emplace_back(Request(mshr_entry.request_, Request::op_t::READ,
                    this->line_size_));
            output_request_buffer_.back().set_fill(mshr_entry.fill_);
            mshr_entry.state_.value_ = MSHR::MSHRState::SENT;
            // mshr_entry.state_.time_ = stats_.get_time();
            // And remove it from the pending list.
            pending_requests_.pop_front();
            Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Sending request from pending_request queue.";
        } else if (this->victim_cache_.has_pending_writes()) {
            // We can send some of the victim lines:
            auto pending_write = victim_cache_.get_pending_write();
						//Mitos: creating a request here, need to add arguments for pc and op
            output_request_buffer_.emplace_back(Request(Request::op_t::WRITE, pending_write.first,
                    0,
										0, // pc
										0, // op
										this->line_size_, 0, pending_write.second.get_dep_type(), pending_write.second.task_type_,
                    -1, 0));
            // We mark the line as clean.
            victim_cache_.mark_as_clean(pending_write.first);
            // And mark that this write has no associated requests.
            assert(evict_table_.find(pending_write.first) == evict_table_.end());
            evict_table_[pending_write.first] = 1;
            Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Sending request from victim cache.";
        } else {
            // We got no misses and the victim cache is empty.
            return false;
        }
    } else {
         Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Request from output_request_buffer";
    }

    // We try to send a request on the buffer.
    bool success = output.write_request(output_request_buffer_.front());
    if (success) {
        Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_
                      << "] Sent request for address 0x"
                      << output_request_buffer_.front() << " at cycle "
                      << simulator_.get_clock();
        // The request went through, we clear the buffer.
        output_request_buffer_.pop_front();
        Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Still " << output_request_buffer_.size() << " pending requests.";
        stalled_ = false;
        return true;
    } else {
        return false;
    }
}


template<typename Message>
template<typename Input>
inline
bool Controller<Message>::send_ack(unsigned input_port_index, Input &input)
{
    assert(input_port_index < output_ack_buffer_.size());
    /** Check that we have ACKs to send: */
    if (output_ack_buffer_[input_port_index].empty()) {
        return false;
    }

    ack_entry_t &ack_entry = output_ack_buffer_[input_port_index].front();
    /* We can try to write before processing the request because
     * we hold a reference to the entry.
     */
    bool ret = input.write_ack(ack_entry.ack_);
    /** We check if we could write the ACK. */
    if (ret == false) {
        return false;
    }

    /* Get the MSHR entry with the pending requests */
    if (ack_entry.mshr_entry_ != NULL) {
        assert(ack_entry.mshr_entry_->list_.empty() == false);
        /* Remove the sub-entry associated to the request */
        assert(ack_entry.mshr_entry_->state_.value_ == MSHR::MSHRState::READY);
        // We just send the appropiate Ack for this sub_entry:
        ack_entry.mshr_entry_->list_.pop_front();

        /* If there are no more pending sub-entries, remove the MSHR entry */
        if (ack_entry.mshr_entry_->list_.empty() == true) {
            /* If one of the requests was a write request, the line should be dirty. */
            if (ack_entry.mshr_entry_->dirty_) {
                this->cache_.mark_as_dirty(ack_entry.mshr_entry_->tag_ & mask_);
            } else {
                this->cache_.mark_as_clean(ack_entry.mshr_entry_->tag_ & mask_);
            }
            mshr_.remove_entry(*ack_entry.mshr_entry_);
            stalled_ = false;
        }
    }

    output_ack_buffer_[input_port_index].pop_front();
    return true;
}


template<typename Message>
inline
bool Controller<Message>::is_stalled()
{
    return stalled_;
}


template<typename Message>
inline
size_t Controller<Message>::get_free_mshr_size() const
{
    return mshr_.get_free_size();
}


template<typename Message>
inline
size_t Controller<Message>::get_mshr_size() const
{
    return mshr_.get_size();
}


template<typename Message>
inline
bool Controller<Message>::inspect(const Ack &ack)
{
    if (Ack::op_t::READ == ack.get_op()) {
        return this->mshr_lookup(ack.get_tag());
    } else if (Ack::op_t::WRITE == ack.get_op()) {
        if (evict_table_lookup(ack.get_tag()) && evict_table_[ack.get_tag()] != 0) {
            evict_table_[ack.get_tag()] = 0;
            stalled_ = false;
            return true;
        }
    }
    return false;
}


template<typename Message>
inline
bool Controller<Message>::is_empty_output_request_buffer()
{
    // Called externally to verify if there is something to send to the next level of memory.
    return (output_request_buffer_.empty() and pending_requests_.size() == 0 and
            (victim_cache_.get_set_free_size(0) == victim_cache_.get_set_size(0) ));
}


template<typename Message>
inline
void Controller<Message>::clear_buffers()
{
    output_request_buffer_.clear();
    pending_requests_.clear();
    output_ack_buffer_.clear();
    mshr_.clear_entries();
}

template<typename Message>
inline
size_t Controller<Message>::get_free_lines()
{
    return this->cache_.get_free_lines();
}

template<typename Message>
inline
void Controller<Message>::print_status() const
{
    Log::warning() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Logic::cache status:";
    Log::warning() << " - Pending requests: " << output_request_buffer_.size()
                   << " of " << max_requests_;
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
    Log::warning() << " - EvictTable size " << evict_table_.size() << " contents:";
    for (auto &elem : evict_table_) {
        Log::warning() << std::hex << elem.first << ":" << elem.second << std::dec;
    }
}

template<typename Message>
inline
bool Controller<Message>::is_empty_output_ack_buffer()
{
    for (auto &ack_buffer : output_ack_buffer_) {
        if (ack_buffer.empty() == false) {
            return false;
        }
    }
    return true;
}


// Private functions here:
template<typename Message>
template<typename ...Args>
inline
void Controller<Message>::add_output_request(Args... args)
{
    Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Add output request.";
    add_output_request(Request(args...));
}


template<typename Message>
inline
void Controller<Message>::add_output_request(Request &&req)
{
    Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Add output request for " << req;
    if (output_request_buffer_.size() >= max_requests_) {
        Log::error() << "[ERROR] Cache Controller output request buffer limit surpassed.";
        exit(-1);
    }
    output_request_buffer_.push_back(std::move(req));
}


template<typename Message>
inline
void Controller<Message>::add_output_ack(unsigned in, Ack && ack, typename MSHR::entry_t *mshr_entry)
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
    // assert(output_ack_buffer_[in].size() <= mshr_.get_size());
}


template<typename Message>
inline
void Controller<Message>::allocate(const Request &request)
{
    const sim::engine::addr_t line_addr = request.get_tag() & mask_;
    bool ret = this->cache_.insert(line_addr, CacheLine(CacheLine::ALLOCATED, request.get_dep_type(),
                                                        request.get_task_type()));
    assert(ret == true);
    _unused(ret);
}


template<typename Message>
inline
bool Controller<Message>::insert(const Request &request)
{
    Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Processing insert " << request;
    const sim::engine::addr_t addr = request.get_tag();
    const sim::engine::addr_t line_addr = addr & mask_;

    if (mshr_.get_free_size() == 0) {
        stats_.mshr_overcapacity_events++;
        stalled_ = true;
        Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] MSHR is full.";
        return false;
    }

    if (this->cache_.is_full(line_addr)) {
        Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Cache is full.";
        // We must make a line empty:
        // Check if we can first:
        if (not this->cache_.can_evict(line_addr)) {
            // We simply cant:
            stalled_ = true;
            Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Cannot evict cache line.";
            return false;
        }
        // Now we sure can. So we snoop the victim:
        const std::pair<engine::addr_t, CacheLine> *evict_candidate = this->cache_.inspect_evict(line_addr);
        assert(evict_candidate != NULL);

        // We check if the victim cache policies accept the line into the cache:
        if (not insertable_into_victim_cache(evict_candidate->second)) {
            // If the victim cannot be inserted in the victim cache due to allocation policy,
            // we need to send it to the output_request_buffer_ if is dirty()
            if (is_request_buffer_full() && evict_candidate->second.dirty()) {
                // We have to wait for the output buffer to be cleared.
                stalled_ = true;
                Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Evict candidate is dirty and cannot written to victim cache or output.";
                return false;
            } else {
                /* Either the buffer is empty or we do not need to us it so we evict the line. */
                std::pair<engine::addr_t, CacheLine> victim = this->cache_.evict(line_addr);
                /* And if we need to send it, we do it. */
                if (victim.second.dirty()) {
                    Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Evicting line from main cache.";
                    add_output_request(Request::op_t::WRITE, victim.first, 0, this->line_size_, 0, 0);
                    assert(evict_table_.find(victim.first) == evict_table_.end());
                    evict_table_[victim.first] = 1;
                }
            }
        } else {
            // The line is insertable into the victim cache, so we try it.
            if (victim_cache_.insert(evict_candidate->first & mask_, evict_candidate->second)) {
                // We did insert the line.
                // Now we evict the line we just inserted in the victim cache.
                std::pair<engine::addr_t, CacheLine> evicted = this->cache_.evict(line_addr);
                assert(evicted.first == evict_candidate->first);
                /* Just to avoid unused warnings: */
                _unused(evicted);
                // stats_.victim_insert++;
            } else {
                // We couldn't insert the line into the victim cache.
                // First we look if a line is evictable: (lines on the victim cache will always be valid or dirty).
                if (victim_cache_.inspect_evict(addr & mask_)->second.dirty() and is_request_buffer_full()) {
                    // We need to evict a line but we cannot do it...
                    Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Cannot evict line.";
                    stalled_ = true;
                    return false;
                } else {
                    // Even if we snooped it, we need to really evict it:
                    std::pair<engine::addr_t, CacheLine> victim = victim_cache_.evict(addr & mask_);
                    /* If the line is dirty, we write it back. */
                    if (victim.second.dirty()) {
                        Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Evicting line from victim cache..";
                        add_output_request(Request::op_t::WRITE, victim.first, 0, this->line_size_, 0, 0);
                        evict_table_[victim.first] = 1;
                    }
                    /* Now that the victim cache has space left, we evict from the cache. */
                    std::pair<engine::addr_t, CacheLine> evicted = this->cache_.evict(line_addr);
                    victim_cache_.insert(evicted.first, evicted.second);
                }
            }
        }
    }
    Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Allocating line from " << request;
    // A line is now empty:
    allocate(request);
    return true;
}


template<typename Message>
inline
typename Message::Ack::op_t Controller<Message>::read_req(unsigned in, const Request& request)
{
    Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] read_req for " << request;
		//std::cout << "[CacheL" << level_ << ":" << this->cpu_id_ << "] read_req for " << request << std::endl;
    const engine::addr_t addr = request.get_tag();
    const sim::engine::addr_t line_addr = addr & mask_;
    uint64_t user_code_id = request.get_user_code_id();
    CacheLine *line = this->cache_.access(line_addr);
    if (line != NULL && (not line->locked())) {
        assert(line->state_ != CacheLine::INVALID);
        this->cache_.signal_event(request, cache_access_outcome_t::HIT);
#ifdef ENABLE_MITOS
				//std::cout << "\tHIT@" << std::hex << request.get_tag() << std::dec << std::endl;
				utils::instrumentation::SelectedInstrumentation& instrumentation = this->simulator_.get_instance()->get_instrumentation();

				uint64_t latency = 0; // latency is fixed, need to lookitup somehow
				sim::engine::cycles_t time = this->simulator_.get_clock();
				
				//std::cout << "pre-op:" << request.get_op_name() << std::endl;
				//std::cout << "pre-ip:" << request.get_ip() << std::endl;

				instrumentation.add_precise_memory_event(
            utils::instrumentation::InstrumentationItem {
            static_cast<unsigned int>(this->cpu_id_),
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::PreciseMemoryEvent {static_cast<uint8_t>(request.get_op_name()), // op
																												request.get_ip(), // ip	
																												request.get_tag(), 
																												latency, 
																												this->level_, 
																												static_cast<uint64_t>(request.get_op())}});
#endif

        stats_.read.hit[user_code_id]++;
        stats_.cache_read_hit_by_task_type[request.get_task_type()]++;
        stats_.update_access(true, request.get_dep_type(), request.get_task_type());
        /* TODO: to add per-set statistics, use this->cache_.get_set(addr & mask) */
        return Ack::op_t::READ; /* Cache hit */
    }

    stats_.read_not_hit++;

    /* do Victim Cache check first, before going to MSHR */
    CacheLine *victim_line = victim_cache_.access(line_addr);
    if (victim_line != NULL) {
        Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] read_req Restoring from victim cache.";
        /* We got a hit in the victim cache. We need to swap a line from cache with the one from victim cache */
        assert(line == NULL);
        if (not this->cache_.can_evict(line_addr)) {
            stalled_ = true;
            return Ack::op_t::MISS;
        }
        std::pair<engine::addr_t, CacheLine> cache_line = this->cache_.evict(line_addr);

        bool victim_removed = victim_cache_.remove(line_addr);
        assert(victim_removed != false);
        _unused(victim_removed);

        bool inserted = this->cache_.insert(line_addr, *victim_line);
        assert(inserted == true);
        _unused(inserted);

        if (insertable_into_victim_cache(cache_line.second)) {
            assert(cache_line.second.locked() == false);
            inserted = victim_cache_.insert(cache_line.first & mask_, cache_line.second);
            assert(inserted == true);
//                stats_.victim_insert++;
            stats_.victim_insert_on_read_by_task_type[request.get_task_type()]++;
        }

        stats_.victim_swap[user_code_id]++;
        stats_.victim_swap_by_task_type[request.get_task_type()]++;

        /* do access again (for LRU update) */
        line = this->cache_.access(line_addr);
        assert(line != NULL);
				//std::cout << "\tVICTIM_HIT@" << std::hex << request.get_tag() << std::dec << std::endl;
        stats_.victim_read.hit[user_code_id]++;
        stats_.victim_read_hit_by_task_type[request.get_task_type()]++;
        stats_.update_access(true, request.get_dep_type(), request.get_task_type());
        return Ack::op_t::READ;
    } else {
				//std::cout << "\tVICTIM_MISS@" << std::hex << request.get_tag() << std::dec << std::endl;
        stats_.victim_read.miss[user_code_id]++;
        stats_.victim_read_miss_by_task_type[request.get_task_type()]++;
    }

    /* Cache miss -- check the MSHR for a half-miss */
    typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(addr);
    bool new_mshr_entry = (mshr_entry == NULL);

    if (new_mshr_entry) {
        assert(line == NULL);
        // The insert logic is a bit complicated, so we do it out of here:
        if (not insert(request)) {
            return Ack::op_t::MISS;
        }
        mshr_entry = mshr_.allocate_entry(addr, request);
        assert(mshr_entry != NULL);
        this->cache_.signal_event(request, cache_access_outcome_t::MISS);
        stats_.read.miss[user_code_id]++;
        stats_.cache_read_miss_by_task_type[request.get_task_type()]++;
        stats_.update_access(false, request.get_dep_type(), request.get_task_type());
    } else if (mshr_entry->state_.value_ == MSHR::MSHRState::READY) {
#ifdef ENABLE_MITOS
				std::cout << "\tMSHR_HIT@" << std::hex << request.get_tag() << std::dec << std::endl;
				utils::instrumentation::SelectedInstrumentation& instrumentation = this->simulator_.get_instance()->get_instrumentation();

				uint64_t latency = 0; // latency is fixed, need to lookitup somehow
				sim::engine::cycles_t time = this->simulator_.get_clock();
								
				instrumentation.add_precise_memory_event(
            utils::instrumentation::InstrumentationItem {
            static_cast<unsigned int>(this->cpu_id_),
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::PreciseMemoryEvent {static_cast<uint8_t>(request.get_op_name()), // op
																												request.get_ip(), // ip	
																												request.get_tag(), 
																												latency, 
																												this->level_, 
																												static_cast<uint64_t>(request.get_op())}});
#endif
				stats_.read.hit[user_code_id]++;
        stats_.update_access(true, request.get_dep_type(), request.get_task_type());
        stats_.cache_read_hit_by_task_type[request.get_task_type()]++;
        return Ack::op_t::READ; /* Cache hit */
    } else {
        assert(line != NULL);
        assert(line->state_ == CacheLine::ALLOCATED);
        stats_.read.half_miss[user_code_id]++;
        stats_.cache_read_half_miss_by_task_type[request.get_task_type()]++;
        stats_.update_access(true, request.get_dep_type(), request.get_task_type());
    }
    mshr_entry->list_.emplace_back(typename MSHR::subentry_t(in, request));
    if (mshr_entry->list_.size() == 1) {
        mshr_entry->fill_ = request.is_fill();
    }

    mshr_request(*mshr_entry);

    return Ack::op_t::MISS;
}


template<typename Message>
inline
typename Message::Ack::op_t Controller<Message>::write_req(unsigned in, const Request &request)
{
    Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] write_req for " << request;
    const sim::engine::addr_t addr = request.get_tag();
    const sim::engine::addr_t line_addr = addr & mask_;
    uint64_t user_code_id = request.get_user_code_id();
    // To prevent a corner case that allows multiple writebacks at once from the same cache line (victim cache).
    if (evict_table_lookup(line_addr)) {
        stalled_ = true;
        return Ack::op_t::MISS;
    }

    CacheLine *line = this->cache_.access(line_addr);
    if (line != NULL && line->state_ != CacheLine::ALLOCATED) { /* Cache hit */
        //  line->state_ = CacheLine::DIRTY;
        this->cache_.mark_as_dirty(line_addr);
        this->cache_.signal_event(request, cache_access_outcome_t::HIT);
        stats_.write.hit[user_code_id]++;;
        stats_.cache_write_hit_by_task_type[request.get_task_type()]++;
        stats_.update_access(true, request.get_dep_type(), request.get_task_type());
        return Ack::op_t::WRITE;
    }

    stats_.write_not_hit++;

    /* do Victim Cache check first, before going to MSHR */
    CacheLine *victim_line = victim_cache_.access(line_addr);
    if (victim_line != NULL) {
        Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] write_req Restoring from victim cache.";
        /* We got a hit in the victim cache. We need to swap a line from cache with the one from victim cache */
        assert(line == NULL);
        if (!this->cache_.can_evict(line_addr)) {
            stalled_ = true;
            return Ack::op_t::MISS;
        }
        std::pair<engine::addr_t, CacheLine> cache_line = this->cache_.evict(line_addr);

        bool victim_removed = victim_cache_.remove(line_addr);
        assert(victim_removed != false);
        _unused(victim_removed);

        bool inserted = this->cache_.insert(line_addr, *victim_line);
        assert(inserted == true);
        _unused(inserted);

        if (insertable_into_victim_cache(cache_line.second)) {
            assert(cache_line.second.locked() == false);
            inserted = victim_cache_.insert(cache_line.first & mask_, cache_line.second);
            assert(inserted == true);
//                stats_.victim_insert++;
            stats_.victim_insert_on_write[user_code_id]++;
            stats_.victim_insert_on_write_by_task_type[request.get_task_type()]++;
        }

        stats_.victim_swap[user_code_id]++;
        stats_.victim_swap_by_task_type[request.get_task_type()]++;

        /* do access again (for LRU update) */
        line = this->cache_.access(line_addr);
        assert(line != NULL);
        // line->state_ = CacheLine::DIRTY;
        this->cache_.mark_as_dirty(line_addr);
        stats_.victim_write.hit[user_code_id]++;
        stats_.victim_write_hit_by_task_type[request.get_task_type()]++;
        stats_.update_access(true, request.get_dep_type(), request.get_task_type());
        return Ack::op_t::WRITE;
    } else {
        stats_.victim_write.miss[user_code_id]++;
        stats_.victim_write_miss_by_task_type[request.get_task_type()]++;
    }

    /* Cache miss -- check the MSHR for a half-miss */
    typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(addr);
    bool new_mshr_entry = (mshr_entry == NULL);

    if (new_mshr_entry) {
        assert(line == NULL);
        // The insert logic is a bit complicated, so we do it out of here:
        if (not insert(request)) {
            return Ack::op_t::MISS;
        }
        mshr_entry = mshr_.allocate_entry(addr, request);
        assert(mshr_entry != NULL);
        mshr_entry->dirty_ = true;
        this->cache_.signal_event(request, cache_access_outcome_t::MISS);
        stats_.write.miss[user_code_id]++;
        stats_.cache_write_miss_by_task_type[request.get_task_type()]++;
        stats_.update_access(false, request.get_dep_type(), request.get_task_type());
    } else if (mshr_entry->state_.value_ == MSHR::MSHRState::READY) {
        // For writes only:
        mshr_entry->dirty_ = true;
        stats_.write.hit[user_code_id]++;
        stats_.update_access(true, request.get_dep_type(), request.get_task_type());
        return Ack::op_t::WRITE; /* Cache hit */
    } else {
        // For writes only:
        mshr_entry->dirty_ = true;
        assert(line != NULL);
        assert(line->state_ == CacheLine::ALLOCATED);
        stats_.write.half_miss[user_code_id]++;
        stats_.cache_write_half_miss_by_task_type[request.get_task_type()]++;
        stats_.update_access(true, request.get_dep_type(), request.get_task_type());
    }
    mshr_entry->list_.emplace_back(typename MSHR::subentry_t(in, request));
    if (mshr_entry->list_.size() == 1)
        mshr_entry->fill_ = true;

    mshr_request(*mshr_entry);

    return Ack::op_t::MISS;
}


template<typename Message>
inline
typename Message::Ack::op_t Controller<Message>::flush_req(unsigned in, const Request &request)
{
    const sim::engine::addr_t addr = request.get_tag();
    const sim::engine::addr_t line_addr = addr & mask_;
    CacheLine *line = this->cache_.access(line_addr);
    if (line != NULL && line->state_ != CacheLine::ALLOCATED) {
        if (line->state_ == CacheLine::DIRTY) {
            Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Flush request genereates request.";
            add_output_request(Request::op_t::WRITE, line_addr, 0, this->line_size_, 
															request.get_op_name(), request.get_ip());
        }
        this->cache_.remove(addr);
        Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Flush request genereates request.";
        add_output_request(request);
        stats_.flush.hit[request.get_user_code_id()]++;
    } else {
        stats_.flush.miss[request.get_user_code_id()]++;
    }
    return Ack::op_t::FLUSH;
}


template<typename Message>
inline
typename Message::Ack::op_t Controller<Message>::invalidate_req(unsigned in, const Request &request)
{
    const sim::engine::addr_t addr = request.get_tag();
    const sim::engine::addr_t line_addr = addr & mask_;
    CacheLine *line = this->cache_.access(line_addr);
    if (line != NULL && line->state_ != CacheLine::ALLOCATED) {
        this->cache_.remove(line_addr);
        stats_.invalidate.hit[request.get_user_code_id()]++;
    } else {
        stats_.invalidate.miss[request.get_user_code_id()]++;
    }
    Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Invalidate genereates request.";
    add_output_request(request);
    return Ack::op_t::INVALIDATE;
}


template<typename Message>
inline
typename Message::Ack::op_t Controller<Message>::prefetch_req(unsigned in, const Request &request)
{
    assert(0);
    return Ack::op_t::READ;  // to avoid warning
}


template<typename Message>
inline
typename Message::Ack::op_t Controller<Message>::clear_req(unsigned in, const Request &request)
{
    const engine::addr_t addr = request.get_tag();
    unsigned pending_write_acks = 0;
    unsigned pending_read_acks = 0;
    for (engine::addr_t line_addr = addr & mask_; line_addr < addr + request.get_size(); line_addr += this->line_size_) {
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
                Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Clear Request genereates request.";
                add_output_request(Request::op_t::WRITE, line_addr, 0, this->line_size_, 
																	0, 0);
                evict_table_[line_addr] = 1;
                this->cache_.remove(line_addr);
            }
            pending_write_acks++;
        }

        if (line->state_ == CacheLine::ALLOCATED)
            pending_read_acks++;
    }

    clear_request_list_.push_back(clear_request_t(in, pending_write_acks, pending_read_acks, request));
    process_clear_request(--clear_request_list_.end());

    return Ack::op_t::MISS;
}


template<typename Message>
inline
void Controller<Message>::data_ack(const Ack &ack)
{
    const sim::engine::addr_t &addr = ack.get_tag();
    /* Get the MSHR entry with the pending requests */
    typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(addr);
    assert(mshr_entry != NULL);
    Log::debug3() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] data_ack for address 0x" << std::hex << ack.get_tag() << std::dec << " at cycle " << simulator_.get_clock();
    assert(mshr_entry->state_.value_ != MSHR::MSHRState::READY);
    stalled_ = false;

    /* Add an ack entry for each pending request */
    for (auto i = mshr_entry->list_.begin(); i != mshr_entry->list_.end(); ++i) {
        assert(i->port_ < output_ack_buffer_.size());
        assert((i->request_.get_op() != Request::op_t::READ) || (i->request_.get_data_size() == 0) );
        assert((i->request_.get_op() == Request::op_t::READ) || (i->request_.get_size() == 0) );
        i->request_.touch(ack);

        add_output_ack(i->port_, Ack(i->request_), mshr_entry);
    }

    CacheLine *line = this->cache_.access(addr & mask_, false);
    assert(line != NULL);
    assert(line->state_ == CacheLine::ALLOCATED);

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
                Log::debug4() << "[CacheL" << level_ << ":" << this->cpu_id_ << "] Clear Request genereates request.";
								// Mitos: this is an ack, we just put 0?
                add_output_request(Request::op_t::WRITE, addr, 0, this->line_size_,
																	0, 0);
                evict_table_[addr] = 1;
                this->cache_.remove(addr);
                curr->pending_write_acks_++;
            }
            process_clear_request(curr);
        }
    }
}


template<typename Message>
inline
void Controller<Message>::write_ack(const Ack &ack)
{
    const sim::engine::addr_t addr = ack.get_tag();
    /* Check that there are pending write requests */
    if (evict_table_lookup(addr)) {
        evict_table_.erase(addr);
        stalled_ = false;
    }

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
void Controller<Message>::null_ack(const Ack &ack)
{
}


template<typename Message>
inline
void Controller<Message>::prefetch_ack(const Ack &ack)
{
    assert(0);
}


template<typename Message>
inline
void Controller<Message>::clear_ack(const Ack &ack)
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
void Controller<Message>::miss_ack(const Ack &ack)
{
}


template<typename Message>
inline
bool Controller<Message>::insertable_into_victim_cache(const CacheLine &cache_line) const
{
    if ((cache_line.dep_type_ & victim_cache_affinity_) != 0) {
        return true;
    } else {
        return false;
    }
}


template<typename Message>
inline
bool Controller<Message>::is_special_request(const Request &request) const
{
    const sim::engine::addr_t addr = request.get_tag();
    if (addr == engine::special_address_t::TASK_ID or
            addr == engine::special_address_t::TASK_END or
            addr == engine::special_address_t::TASK_DEP_ADDR or
            addr == engine::special_address_t::TASK_DEP_SIZE or
            addr == engine::special_address_t::TASK_DEP_TYPE) {
        return true;
    } else {
        return false;
    }
}


template<typename Message>
inline
typename Message::Ack::op_t Controller<Message>::process_special_request(const Request &request)
{
    assert(is_special_request(request));
    if (stalled_) {
        return Ack::op_t::MISS;
    }
    stats_.setup_task_io_requests++;

    switch (request.get_tag()) {
        case engine::special_address_t::TASK_END:
        case engine::special_address_t::TASK_ID:
            if (is_request_buffer_full()) {
                stalled_ = true;
                return Ack::op_t::MISS;
            } else {
                add_output_request(Request(request));
                evict_table_[request.get_tag()] = 1;
                this->cache_.signal_event(request);
                return Ack::op_t::MISS;
            }
            break;
        default:
            assert(0);
            return Ack::op_t::MISS;
    }

}


template<typename Message>
inline
void Controller<Message>::mshr_request(typename MSHR::entry_t &mshr_entry)
{
    switch (mshr_entry.state_.value_) {
        case MSHR::MSHRState::INVALID:
            pending_requests_.push_back(&mshr_entry);
            mshr_entry.state_.value_ = MSHR::MSHRState::SENDING;
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
void Controller<Message>::mshr_evict_ack(typename MSHR::entry_t &mshr_entry)
{
    switch (mshr_entry.state_.value_) {
        case MSHR::MSHRState::PENDING:
            pending_requests_.push_back(&mshr_entry);
            mshr_entry.state_.value_ = MSHR::MSHRState::SENDING;
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
bool Controller<Message>::mshr_lookup(const engine::addr_t addr)
{
    typename MSHR::entry_t *mshr_entry = mshr_.lookup_entry(addr);
    if ((mshr_entry == NULL) or (mshr_entry->state_.value_ != MSHR::MSHRState::SENT)) {
        return false;
    } else {
        mshr_entry->state_.value_ =  MSHR::MSHRState::INSPECTED;
        return true;
    }
}

template<typename Message>
inline
bool Controller<Message>::evict_table_lookup(const sim::engine::addr_t addr)
{
    return (evict_table_.find(addr) != evict_table_.end());
}


template<typename Message>
inline
void Controller<Message>::process_clear_request(typename ClearRequestList::iterator clear_request_iterator)
{
    if (clear_request_iterator->pending_write_acks_ + clear_request_iterator->pending_read_acks_ > 0) {
        return;
    }

    if (this->last_level_) {
        add_output_ack(clear_request_iterator->port_, Ack(clear_request_iterator->request_));
        clear_request_list_.erase(clear_request_iterator);
    } else {
        add_output_request(clear_request_iterator->request_);
    }
    /*if (line != NULL && line->state_ == CacheLine::ALLOCATED) return;

    if (line == NULL || line->state_ == CacheLine::VALID) {
        if (line != NULL)
            this->cache_.remove(addr);

        if (this->last_level_)
        {
            std::pair<typename ClearTable::iterator, typename ClearTable::iterator> ret = clear_table_.equal_range(addr);
            for (typename ClearTable::iterator i = ret.first; i != ret.second; ++i)
                add_output_ack(i->second.port_, Ack(Ack::op_t::CLEAR, i->second.addr_, i->second.size_, 0));

            clear_table_.erase(addr);
            return;
        }

        add_output_request(Request::op_t::CLEAR, addr, this->line_size_);
        return;
    }

    assert(line->state_ != CacheLine::INVALID);

    add_output_request(Request::op_t::WRITE, addr, 0, this->line_size_);
    evict_table_[addr] = 1;
    this->cache_.remove(addr);*/
}


template<typename Message>
inline
bool Controller<Message>::is_request_buffer_full()
{
    return output_request_buffer_.size() == max_requests_;
}


// Static member initialization
template<typename Message>
typename Controller<Message>::req_handler_t Controller<Message>::Req_Handlers_[] = {
        &Controller<Message>::read_req,          /* READ_REQ */
        &Controller<Message>::write_req,         /* WRITE_REQ */
        &Controller<Message>::flush_req,         /* FLUSH_REQ */
        &Controller<Message>::invalidate_req,    /* INVALIDATE_REQ */
        &Controller<Message>::prefetch_req,      /* PREFETCH_REQ */
        &Controller<Message>::clear_req,         /* CLEAR_REQ */
        NULL
};


template<typename Message>
typename Controller<Message>::ack_handler_t Controller<Message>::Ack_Handlers_[] = {
        &Controller<Message>::data_ack,          /* READ_ACK */
        &Controller<Message>::write_ack,         /* WRITE_ACK */
        &Controller<Message>::null_ack,          /* FLUSH_ACK */
        &Controller<Message>::null_ack,          /* INVALIDATE_ACK */
        &Controller<Message>::prefetch_ack,      /* PREFETCH_ACK */
        &Controller<Message>::clear_ack,         /* CLEAR_ACK */
        &Controller<Message>::miss_ack,          /* MISS_ACK */
        NULL
};


}  // namespace single
}  // namespace coherence
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_COHERENCE_SINGLE_CONTROLLER_IMPL_H_
