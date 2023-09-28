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

#include "ROA.h"

#include <cassert>
#include <deque>
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

ROA::ROA(unsigned id, engine::Simulator &sim, const engine::Config &config, mem_buffer_t **mem_in) :
    next_item_(),
    out_(config.get_value<unsigned>("out_buff_size")),
    rob_(config.get_value<unsigned>("rob_size"), config.get_value<unsigned>("request-size"), out_),
    commit_rate_(config.get_value<unsigned>("commit_rate")),
    last_issue_cycle_(0),
    last_cycle_(0),
    commit_stalled_(false),
    issue_stalled_(false),
    id_(id),
    sim_(sim),
    mem_in_(mem_in),
    ins_current_phase_(0),
    stats_(*this),
    current_wd_id_(0),
    past_committed_instructions_(0)
{
}


void
ROA::commit()
{
    Log::debug2() << "ROA::commit()";
    if (rob_.empty()) return;

    unsigned ins = commit_stalled_ ?
            commit_rate_ :
            commit_rate_ * static_cast<unsigned>(sim_.get_clock() - last_cycle_);
    Log::debug2() << "ROA::commit(): ins = " << ins;
    Log::debug2() << "commit_rate_ =" << commit_rate_;
    assert(ins > 0);

    Log::debug2() << sim_.get_clock() << ":retire " << ins << " ins (";

    unsigned retired = rob_.retire(ins);

    stats_.committed_ins_[current_wd_id_] += retired;

    Log::debug2() << retired << ") retired";
    Log::debug2() << "remaining in rob: " << rob_.capacity() - rob_.free_entries();

    commit_stalled_ = (retired == 0);
}


void
ROA::issue()
{
    Log::debug2() << "ROA::issue()";
    Log::debug2() << "next_item_.op = " << static_cast<unsigned>(next_item_.op_);
    if (next_item_.op_ == sim::trace::mitem_t::INVALID) {
        Log::debug2() << "ROA::issue(): mem_in_->read()";
        bool next = (*mem_in_)->read(next_item_);
        if (not next or (next_item_.op_ == sim::trace::mitem_t::INVALID)) {
            return;  // no more items
        }
    }

    Log::debug2() << "ROA::issue(): next request valid\n";

    // If we cannot issue instructions we stall
    if (rob_.free_entries() == 0) {
        issue_stalled_ = true;
        return;
    }
    // If we were stalled and no longer should be, we should assume we start to work this cycle.
    if (issue_stalled_ && rob_.free_entries() > 0) {
        issue_stalled_ = false;
        last_issue_cycle_ = sim_.get_clock() - 1;
    }

    // TODO(???): could this be less strict and check ldq occupancy instead of outbuffer?
    if (out_.full()) {
        Log::debug2() << sim_.get_clock() << ": out_ full";
        if (next_item_.ins_ > 1) {
            issue_stalled_ = (insert_partial_item() == 1);
            if (not issue_stalled_) {
                last_issue_cycle_ = sim_.get_clock();
            }
        } else {
            issue_stalled_ = true;
        }
        return;
    } else  {
        issue_stalled_ = false;
    }

    Log::debug() << sim_.get_clock() << ":ROA[" << id_ << "]: " << std::hex << next_item_.addr_ << std::dec
                << ", wd: " << current_wd_id_
                << ", op:"<< static_cast<unsigned>(next_item_.op_)
                << ", sz: " << static_cast<unsigned>(next_item_.size_)
                << ", ins: " << static_cast<unsigned>(next_item_.ins_)
                << ", fe: " << rob_.free_entries();

    assert(next_item_.ins_ > 0);
    assert((next_item_.op_ != sim::trace::mitem_t::INVALID) and not out_.full());

    sim::trace::mitem_t& item = next_item_;
    if (rob_.free_entries() >= item.ins_) {
        // item fits in ROB
        if (item.op_ == sim::trace::mitem_t::LOAD) {
            // Special check for loads as they can sprawl over several cache lines
            if (rob_.check_load_fits_output_buffer(item)) {
                Log::debug2() << sim_.get_clock() << ":ROA[" << id_ << "]: issue LOAD";
                stats_.num_loads_total_[current_wd_id_]++;
                rob_.insert(item);
            } else {
                // We wait until the Output Buffer is not so full.
                return;
            }
        } else if (item.op_ == sim::trace::mitem_t::STORE) {
            Log::debug2() << sim_.get_clock() << ":ROA[" << id_ << "]: issue STORE";
            stats_.num_stores_total_[current_wd_id_]++;
            rob_.insert(item);
        } else if (item.op_ == sim::trace::mitem_t::NOMEM) {
            Log::debug2() << sim_.get_clock() << ":ROA[" << id_ << "]: issue NOMEM";
            rob_.insert(item);
        } else {
            std::cerr << "Empty memory chunk detected at ROA." << std::endl;
            Log::debug2() << "Unknown memory instruction type " << static_cast<unsigned>(item.op_) << "-";
            // exit(1);
        }
        last_issue_cycle_ = sim_.get_clock();
        bool next = (**mem_in_).read(next_item_);
        if (not next) {
            next_item_ = sim::trace::mitem_t();
        }
    } else {  // item does not fit free space in ROB
        assert(rob_.free_entries() <= rob_.capacity());
        /* arico: this check is for efficiency. If there are enough instructions to be committed
         *        in the next cycle, we do not insert any instruction in the ROB. Whenever the
         *        ROB is emptied to the point where there are not enough instructions to fulfill the
         *        commit rate in the next cycle, then we insert a partial item. This way the ROB
         *        is filled at once instead of introducing ISSUE_RATE instructions every cycle. This
         *        means that statistics like average ROB occupancy are not valid. This 'if' statement
         *        must be removed to correctly collect such microarchitectural statistics.
         *        The consequence is that issue is not really stalled in this case, but issues are
         *        not executed at this time but coalesced and inserted as late as possible for the sake
         *        of performance and as long as timing is not affected.
         */
        if ((rob_.capacity() - rob_.free_entries()) < static_cast<unsigned>(commit_rate_)) {
            assert(item.ins_ > rob_.free_entries());
            insert_partial_item();
            last_issue_cycle_ = sim_.get_clock();
        }
    }
}


unsigned
ROA::insert_partial_item()
{
    Log::debug2() << "ROA::insert_partial_item()";
    unsigned inserted = rob_.free_entries() < next_item_.ins_ ?
            rob_.free_entries() : next_item_.ins_ - 1;
    if (inserted > 0) {
        rob_.insert(sim::trace::mitem_t(0x0, 0, sim::trace::mitem_t::NOMEM, 0, 0, inserted));
        next_item_.ins_ -= inserted;
    }
    Log::debug2() << sim_.get_clock() << ":ROA[" << id_ << "]: insert partial item " << std::hex << next_item_.addr_
                 << std::dec << " inserted " << inserted << " left " << next_item_.ins_;
    return next_item_.ins_;  // returns remaining ins
}


engine::cycles_t
ROA::next_commit_cycle() const
{
    Log::debug2() << "ROA::next_commit_cycle()";
    if (commit_stalled_ or rob_.empty()) {
        Log::debug() << "ROB empty";
        return engine::NEVER;
    }

    // Need the following check to avoid floating point exception in case commit_rate_ = 0
    engine::cycles_t next = 0;
    if (commit_rate_ > 0) {
        next = rob_.get_ins_to_first_waiting_item() / commit_rate_;
    }
    return next > 0 ? next : 1;
}


engine::cycles_t
ROA::next_issue_cycle() const
{
    Log::debug2() << "ROA::next_issue_cycle()";
    if (next_item_.op_ == sim::trace::mitem_t::INVALID or issue_stalled_) {
        return engine::NEVER;
    }

    engine::cycles_t next;
    if (rob_.free_entries() >= next_item_.ins_) {
        next = 1;  // fits
    } else if (next_item_.ins_ >= rob_.capacity()) {
        next = (rob_.capacity() - rob_.free_entries()) / commit_rate_;  // wait until max space
    } else {
        next = (next_item_.ins_ - rob_.free_entries()) / commit_rate_;
    }
    return next > 0 ? next : 1;
}


engine::cycles_t
ROA::compute_next_cycle()
{
    Log::debug2() << "ROA::compute_next_cycle()";
    engine::cycles_t commit = next_commit_cycle();
    engine::cycles_t issue = next_issue_cycle();

    Log::debug2() << sim_.get_clock() << ": next commit: " << commit;
    Log::debug2() << sim_.get_clock() << ": next issue: " << issue;

    return commit < issue ? commit : issue;
}


engine::cycles_t
ROA::cycle()
{
    Log::debug2() << "ROA::cycle()";
    assert(sim_.get_clock() == 0 or sim_.get_clock() > last_cycle_);

    update_stats();
    commit();
    issue();

    last_cycle_ = sim_.get_clock();
    return compute_next_cycle();
}

bool
ROA::has_mem_request() const
{
    return out_.size() > 0;
}

sim::logic::coherence::single::Request
ROA::get_mem_request()
{
    Log::debug2() << "ROA::get_mem_request()";
    assert(not out_.empty());
    sim::logic::coherence::single::Request request(std::move(out_.front()));
    out_.pop();
    if (request.get_op() == logic::coherence::single::Message::Request::op_t::READ) {
        stats_.num_loads_sent_[current_wd_id_]++;
        cycle_load_req_.push(sim_.get_clock());
        Log::debug2() << sim_.get_clock() << ":ROA[" << id_ << "]:sendLoad " << request.get_tag()
                     << " sz " << request.get_size();
    }
    if (request.get_op() == logic::coherence::single::Request::op_t::WRITE) {
        stats_.num_stores_sent_[current_wd_id_]++;
        cycle_store_req_.push(sim_.get_clock());
        Log::debug2() << sim_.get_clock() << ":ROA[" << id_ << "]:sendStore " << request.get_tag()
                     << " sz " << request.get_size();
    }

    return request;
}

sim::logic::coherence::single::Request&
ROA::peek_mem_request()
{
    assert(not out_.empty());
    return out_.front();
}

void
ROA::pop_mem_request()
{
    assert(not out_.empty());

    const sim::logic::coherence::single::Request &request = out_.front();
    if (request.get_op() == logic::coherence::single::Message::Request::op_t::READ) {
        stats_.num_loads_sent_[current_wd_id_]++;
        cycle_load_req_.push(sim_.get_clock());
    } else if (request.get_op() == logic::coherence::single::Request::op_t::WRITE) {
        stats_.num_stores_sent_[current_wd_id_]++;
        cycle_store_req_.push(sim_.get_clock());
    }
    out_.pop();
}

void
ROA::ack_mem_response(const sim::logic::coherence::single::Ack &ack)
{
    Log::debug2() << "ROA::ack_mem_response(), addr = " << ack.get_tag();
    // Notify load, there are also Acks from writes
    if (ack.get_op() == sim::logic::coherence::single::Ack::op_t::READ) {
        rob_.process_ack(ack);
        cycle_load_resp_.push(sim_.get_clock());
    } else if (ack.get_op() == sim::logic::coherence::single::Ack::op_t::WRITE) {
        cycle_store_resp_.push(sim_.get_clock());
    }
}


bool
ROA::finished() const
{
    return next_item_.op_ == sim::trace::mitem_t::INVALID and
           rob_.empty() and out_.empty();
}


void
ROA::update_stats()
{
    if (commit_stalled_)
        stats_.stalled_cycles_[current_wd_id_] += static_cast<unsigned>(sim_.get_clock() - last_cycle_);
    if (out_.full())
        stats_.out_full_cycles_[current_wd_id_] += static_cast<unsigned>(sim_.get_clock() - last_cycle_);
    while (!cycle_load_resp_.empty() && !cycle_load_req_.empty()) {
        stats_.load_cycles_[current_wd_id_] += (cycle_load_resp_.front() - cycle_load_req_.front());
        cycle_load_resp_.pop();
        cycle_load_req_.pop();
        stats_.num_loads_received_[current_wd_id_]++;
    }
    while (!cycle_store_resp_.empty() && !cycle_store_req_.empty()) {
        stats_.store_cycles_[current_wd_id_] += (cycle_store_resp_.front() - cycle_store_req_.front());
        cycle_store_resp_.pop();
        cycle_store_req_.pop();
        stats_.num_stores_received_[current_wd_id_]++;
    }
}

unsigned ROA::get_id() const
{
    Log::debug2() << "Getting ROA id = " << id_;
    return id_;
}

const std::string ROA::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

void ROA::start_of_phase()
{
    ins_current_phase_ = stats_.committed_ins_[current_wd_id_];
}

void ROA::end_of_phase()
{
    ins_current_phase_ = stats_.committed_ins_[current_wd_id_] - ins_current_phase_;
}

uint64_t ROA::get_ins_current_phase() const
{
    return ins_current_phase_;
}

void ROA::set_current_wd_id(uint64_t wd_id)
{
    current_wd_id_ = wd_id;
    rob_.set_current_wd_id(wd_id);
}

void ROA::print_status() const
{
    Log::warning() << "ROA status:";
    Log::warning() << " - OutputBuffer size = " << out_.size();
    Log::warning() << " - OutputBuffer free_entries = " << out_.free_entries();
    Log::warning() << " - LDQueue information:";
    rob_.print_status();
}

void ROA::print_hardware_counters(engine::cycles_t time, const unsigned int cpu_instrumentation_id)
{
    Log::debug2() << "Printing roa hwc on  instrumentation id = " << cpu_instrumentation_id
                 << " roa id = " << this->get_id();
    // First we get the current values:
    uint64_t committed_instructions = stats_.committed_ins_.get_value();
    // Important detail: we do not differentiate between issued an commited
    // that would only be the case with out of order execution.
    uint64_t issued_instructions = committed_instructions;
    uint64_t stalled_cycles = stats_.stalled_cycles_.get_value();
    uint64_t load_instructions = stats_.num_loads_total_.get_value();
    uint64_t store_instructions = stats_.num_stores_total_.get_value();

    // Then we add the instrumentation events for the deltas:
    utils::instrumentation::SelectedInstrumentation& instrumentation = sim_.get_instrumentation();
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::PAPI_TOT_IIS,
                issued_instructions - stats_.prev_issued_instructions_} });
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::PAPI_TOT_INS,
                committed_instructions - stats_.prev_committed_instructions_} });
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::PAPI_TOT_CYC,
                time - stats_.prev_cycle_count_} });
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::PAPI_RES_STL,
                stalled_cycles - stats_.prev_stalled_cycles_} });
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::PAPI_LD_INS,
                load_instructions - stats_.prev_load_instructions_} });
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::PAPI_ST_INS,
                store_instructions - stats_.prev_store_instructions_} });

    uint64_t st_fin = stats_.num_stores_received_.get_value() - stats_.prev_stores_recv_;
    uint64_t ld_fin = stats_.num_loads_received_.get_value() - stats_.prev_loads_recv_;
    /*
    uint64_t st_avg_time = 0;
    if (st_fin) {
        st_avg_time = (stats_.store_cycles_.get_value() - stats_.prev_store_cycles_) / st_fin;
    }
    uint64_t ld_avg_time = 0;
    if (ld_fin) {
        ld_avg_time = (stats_.load_cycles_.get_value() - stats_.prev_load_cycles_) / ld_fin;
    }
    */
    uint64_t amat = 0;
    if (ld_fin + st_fin) {
        amat = (stats_.store_cycles_.get_value() - stats_.prev_store_cycles_ +
                stats_.load_cycles_.get_value() - stats_.prev_load_cycles_) / (st_fin + ld_fin);
    }

    // Memops delay counters:
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::CPU_AMAT,
                amat} });
/*
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::CPU_LD_FIN,
                ld_fin} });

    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::CPU_LD_ATIME,
                ld_avg_time} });

    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::CPU_ST_FIN,
                st_fin} });

    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            cpu_instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {
                utils::instrumentation::CPU_ST_ATIME,
                st_avg_time} });

*/

    // And store the current values for the next iteration:
    stats_.prev_issued_instructions_ = issued_instructions;
    stats_.prev_committed_instructions_ = committed_instructions;
    stats_.prev_cycle_count_ = time;
    stats_.prev_stalled_cycles_ = stalled_cycles;
    stats_.prev_load_instructions_ = load_instructions;
    stats_.prev_store_instructions_ = store_instructions;

    stats_.prev_stores_recv_ = stats_.num_stores_received_.get_value();
    stats_.prev_loads_recv_ = stats_.num_loads_received_.get_value();
    stats_.prev_store_cycles_ = stats_.store_cycles_.get_value();
    stats_.prev_load_cycles_ = stats_.load_cycles_.get_value();
}

void ROA::context_switch(engine::addr_t new_stack_ptr)
{
    // Currently don't store the new stack pointer,
    // though the processor should really store it in a register somewhere
    (void)new_stack_ptr;
}

bool ROA::cpu_activity_poll() {
    unsigned committed = stats_.committed_ins_.get_value();
    if (committed != past_committed_instructions_) {
        past_committed_instructions_ = committed;
        return true;
    } else {
        return false;
    }
}


}  // namespace timing_models
}  // namespace cpu
}  // namespace sim
