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


#ifndef CORE_MODULES_MEMORY_CACHE_IMPL_H_
#define CORE_MODULES_MEMORY_CACHE_IMPL_H_

#include <string>

#include "core/engine/Config.h"
#include "core/engine/IModule.h"
#include "core/utils/instrumentation/ParaverDefs.h"
#include "CacheStats.h"

#include "core/utils/Log.h"
#include "core/utils/macros.h"

namespace sim {
namespace memory {

using sim::utils::Log;

template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline Cache<Message, CacheType, Upper, Lower>::Cache(sim::engine::Simulator &simulator,
        sim::logic::coherence::single::Directory<Message> &directory,
        sim::engine::Config cache_config, unsigned id) :
    engine::IModule<Message>(simulator, engine::ModuleType::CACHE),
    directory_(directory),
    cache_id_(id),
    level_(cache_config.get_value<unsigned>("level")),
    stats_(*this),
    cache_(id, stats_, simulator, directory, cache_config),
    write_request_failed_(false),
    latency_(cache_config.get_value<engine::cycles_t>("latency")),
    stall_start_cycle_(0),
    in_(1, Input(*this)),
    out_(Output(*this, cache_config.get_value<std::size_t>("num-ports")))
{
    Log::debug2() << "Creating cache at level " << level_;
    this->instrumentation_id_ = simulator.get_instrumentation().add_module(*this);
    directory_.register_cache(*this);
    vulnerability_ = sim::stats::VulnerabilityStat::template create_from_config<Cache>(*this, cache_config);
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline Cache<Message, CacheType, Upper, Lower>::~Cache()
{
    // write the statistic here...
    Log::debug2() << latency_ << '\t' << stats_.read.hit.get_value()
                  << '\t' << stats_.read.miss.get_value()
                  << '\t' << stats_.read.half_miss.get_value();
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline bool Cache<Message, CacheType, Upper, Lower>::reset()
{
    return true;
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline void Cache<Message, CacheType, Upper, Lower>::start()
{
    if (cache_.is_stalled() == false) {
        /* Process requests scheduled for this cycle */
        while (input_buffer_.empty() == false) {
            // We only process one request per buffer:
            if (input_buffer_.front().time_ <= this->get_simulator().get_clock()) {
                typename Message::Request req = input_buffer_.front().data_.second;
                if (cache_.process_request(input_buffer_.front().data_.first, req) == false) {
                    // in_[i].signal_miss();
                }
                if (cache_.is_stalled()) {
                    stall_start_cycle_ = this->get_simulator().get_clock();
                    break;
                }
                if (vulnerability_) {
                    vulnerability_->mem_access(this->get_simulator().get_clock(), req);
                }
                uint64_t user_code_id = req.get_user_code_id();
                input_buffer_.pop_front();
                stats_.requests_processed[user_code_id]++;
            } else {
                break;
            }
        }
    }

    bool was_stalled = cache_.is_stalled();
    int c = 0;
    while (cache_.is_empty_output_request_buffer() == false) {
        if (cache_.send_request(out_)) {
            stats_.requests_sent++;
            c++;
        } else {
            write_request_failed_ = true;
            break;
        }
    }
    Log::debug() << "[CACHE] ID " <<  cache_id_ << " sent " << c << " REQs at " << this->get_simulator().get_clock();
    c = 0;
    for (unsigned i = 0; i < in_.size(); ++i) {
        while (cache_.send_ack(i, in_[i])) {
            stats_.acks_sent++;
            c++;
        }
    }
    Log::debug() << "[CACHE] ID " <<  cache_id_ << " sent " << c << " ACKs at " << this->get_simulator().get_clock();

    if (was_stalled and !cache_.is_stalled()) {
        /* not stalled anymore */
        engine::cycles_t advance = this->get_simulator().get_clock() + 1 - stall_start_cycle_;
        stats_.stalled_cycles += advance;
        for (auto it = input_buffer_.begin(); it != input_buffer_.end(); ++it) {
            (*it).time_ += advance;
        }
    }
    return;
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline void Cache<Message, CacheType, Upper, Lower>::end()
{
    if ((cache_.is_stalled() == false) and input_buffer_.size() < cache_.get_mshr_size()) {
        for (unsigned i = 0; i < in_.size(); ++i) {
            while ((input_buffer_.size() < cache_.get_mshr_size()) and in_[i].can_read_request()) {
                buffer_entry_t req;
                bool success = in_[i].read_request(req.second);
                assert(success == true);
                _unused(success);
                uint64_t user_code_id = req.second.get_user_code_id();
                stats_.requests_received[user_code_id]++;
                input_buffer_.emplace_back(this->get_simulator().get_clock() + latency_, std::move(req));
            }
        }
    }
    /* Process any ack from upper levels */
    bool was_stalled = cache_.is_stalled();
    while (out_.can_read_ack()) {
        typename Message::Ack ack_req;
        bool success = out_.read_ack(ack_req);
        assert(success == true);
        _unused(success);
        stats_.acks_received++;
        cache_.process_ack(ack_req);
    }

    if (was_stalled and !cache_.is_stalled()) {
        /* not stalled anymore */
        engine::cycles_t advance = this->get_simulator().get_clock() + 1 - stall_start_cycle_;
        for (auto it = input_buffer_.begin(); it != input_buffer_.end(); ++it) {
            (*it).time_ += advance;
        }
    }

    set_next_cycle();
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline
void Cache<Message, CacheType, Upper, Lower>::print_hardware_counters(engine::cycles_t time, bool propagate,
        unsigned overwrite_cpu_id)
{
    uint64_t miss_id, hit_id, read_id, write_id, access_id;
    uint64_t load_miss_id, store_miss_id;

    unsigned instrumentation_id = (overwrite_cpu_id != 0) ? overwrite_cpu_id : this->instrumentation_id_;

    // First we gather the event ids depending on the cache level:
    switch (level_) {
        case 1:
            miss_id = sim::utils::instrumentation::PAPI_L1_DCM;
            hit_id = sim::utils::instrumentation::PAPI_L1_DCH;
            read_id = sim::utils::instrumentation::PAPI_L1_TCR;
            write_id = sim::utils::instrumentation::PAPI_L1_TCW;
            access_id = sim::utils::instrumentation::PAPI_L1_TCA;
            load_miss_id = sim::utils::instrumentation::PAPI_L1_LDM;
            store_miss_id = sim::utils::instrumentation::PAPI_L1_STM;
            break;
        case 2:
            miss_id = sim::utils::instrumentation::PAPI_L2_DCM;
            hit_id = sim::utils::instrumentation::PAPI_L2_DCH;
            read_id = sim::utils::instrumentation::PAPI_L2_TCR;
            write_id = sim::utils::instrumentation::PAPI_L2_TCW;
            access_id = sim::utils::instrumentation::PAPI_L2_TCA;
            load_miss_id = sim::utils::instrumentation::PAPI_L2_LDM;
            store_miss_id = sim::utils::instrumentation::PAPI_L2_STM;
            break;
        case 3:
            miss_id = sim::utils::instrumentation::PAPI_L3_DCM;
            hit_id = sim::utils::instrumentation::PAPI_L3_DCH;
            read_id = sim::utils::instrumentation::PAPI_L3_TCR;
            write_id = sim::utils::instrumentation::PAPI_L3_TCW;
            access_id = sim::utils::instrumentation::PAPI_L3_TCA;
            load_miss_id = sim::utils::instrumentation::PAPI_L3_LDM;
            store_miss_id = sim::utils::instrumentation::PAPI_L3_STM;
            break;
        default:
            // For victim cache:
            return;
    }

    // Then we gather the total event values:
    uint64_t total_hits =  stats_.read.hit.get_value() +
                           stats_.write.hit.get_value() +
                           stats_.flush.hit.get_value() +
                           stats_.invalidate.hit.get_value();
    uint64_t total_misses = stats_.read.miss.get_value() +
                            stats_.write.miss.get_value() +
                            stats_.flush.miss.get_value() +
                            stats_.invalidate.miss.get_value();

    uint64_t total_reads = stats_.read.hit.get_value() +
                           stats_.read.miss.get_value();

    uint64_t total_writes = stats_.write.hit.get_value() +
                            stats_.write.miss.get_value();

    uint64_t total_accesses = total_reads + total_writes;
    uint64_t load_misses = stats_.read.miss.get_value();
    uint64_t store_misses = stats_.write.miss.get_value();

    // Then we add an event for the delta:
    utils::instrumentation::SelectedInstrumentation& instrumentation =
            this->get_simulator().get_instrumentation();

    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {hit_id, total_hits}},
            *this);

    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {miss_id, total_misses}},
            *this);


    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {read_id, total_reads}},
            *this);


    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {write_id, total_writes}},
            *this);


    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {access_id, total_accesses}},
            *this);

    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {load_miss_id, load_misses}},
            *this);

    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {store_miss_id, store_misses}},
            *this);

    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            this->instrumentation_id_,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {utils::instrumentation::CACHE_FREE_LINES, cache_.get_free_lines()}},
            *this);

    if (propagate) {
        out_.get_peer().print_hardware_counters(time, true, overwrite_cpu_id);
    }
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline bool Cache<Message, CacheType, Upper, Lower>::inspect(const typename Message::Request &)
{
    return true;
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline bool Cache<Message, CacheType, Upper, Lower>::inspect(const typename Message::Ack &ack)
{
    return cache_.inspect(ack);
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline
const unsigned Cache<Message, CacheType, Upper, Lower>::get_id() const
{
    return cache_id_;
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline
const unsigned Cache<Message, CacheType, Upper, Lower>::get_level() const
{
    return level_;
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline
const std::string Cache<Message, CacheType, Upper, Lower>::get_name() const {
    return std::string("CacheL" + std::to_string(level_));
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline
void Cache<Message, CacheType, Upper, Lower>::print_status() const
{
    Log::warning() << "[STATUS] Cache L" << level_ << "with ID " << get_id() << " deadlock report:";
    Log::warning() << " - Last write request failed? " << write_request_failed_;
    for (const auto& req : input_buffer_) {
        Log::warning() << " - Request pending at " << req.time_ << " from port " << req.data_.first
                       << " req: " << req.data_.second;
    }
    cache_.print_status();
}


template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower>
inline
void Cache<Message, CacheType, Upper, Lower>::set_next_cycle()
{
    // First we check if we have some ACKs pending.
    if (cache_.is_empty_output_ack_buffer() == false) {
        this->set_module_activity(engine::NEXT);
        return;
    }

    // Check if we have to send Requests and we have not failed at it.
    if ((cache_.is_empty_output_request_buffer() == false) and (write_request_failed_ == false)) {
        this->set_module_activity(engine::NEXT);
        return;
    }

    // Check if the input buffers have any pending requests in them:
    for (unsigned i = 0; i < in_.size(); ++i) {
        if (in_[i].can_read_request()) {
            this->set_module_activity(engine::NEXT);
            return;
        }
    }
    // Else we will awake when we "finish" processing the oldest Request.
    sim::engine::cycles_t next_cycle = engine::NEVER;
    if (cache_.is_stalled() == false) {
        if ((input_buffer_.empty() == false) &&
                 input_buffer_.front().time_ < next_cycle) {
            next_cycle = input_buffer_.front().time_;
        }
    }

    // We set next cycle acordiongly:
    if (next_cycle == engine::NEVER) {
        this->set_module_activity(engine::NEVER);
    } else if (next_cycle <= this->get_simulator().get_clock()) {
        // This is done in case some of the buffers are congested, we should not go back in time.
        this->set_module_activity(engine::NEXT);
    } else {
        this->set_module_activity(next_cycle - this->get_simulator().get_clock());
    }
}

}  // namespace memory
}  // namespace sim

#endif  // CORE_MODULES_MEMORY_CACHE_IMPL_H_
