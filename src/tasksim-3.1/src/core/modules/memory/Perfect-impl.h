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


#ifndef CORE_MODULES_MEMORY_PERFECT_IMPL_H_
#define CORE_MODULES_MEMORY_PERFECT_IMPL_H_

#include <memory>
#include <string>

#include "core/engine/Config.h"
#include "core/stats/VulnerabilityStat.h"
#include "core/utils/instrumentation/ParaverDefs.h"
#include "core/utils/Log.h"


namespace sim {
namespace memory {

using sim::utils::Log;

template<typename Message, template<typename> class Interconnect>
inline
Perfect<Message, Interconnect>::Perfect(sim::engine::Simulator &sim,
                                        sim::engine::Config conf,
                                        unsigned id) :
    engine::IModule<Message>(sim, engine::ModuleType::RAM),
    in_(Input(*this)),
    max_input_buffer_size_(conf.get_value<std::size_t>("input-buffer")),
    latency_(conf.get_value<unsigned>("latency") * sim.get_config().get_value<unsigned>("cpu_frequency") / 1000),
    max_concurrent_(ceil(conf.get_value<float>("bandwidth") * latency_ / (conf.get_value<unsigned>("request-size") *
            1e6 * sim.get_config().get_value<unsigned>("cpu_frequency")))),
    id_(id),
    stats_(*this),
    blocked_(false),
    last_nonblocked_cycle_(0)
{
    // Max concurrent_requests := Bandwidth[bytes/s] * latency[cycles] / ( request_size[bytes] * cpu_freq[cycles/s])
    this->instrumentation_id_ = sim.get_instrumentation().add_module(*this);
    if (max_concurrent_ == 0) {
        Log::error() << "[ERROR] This configuration has a lower bandwidth than possible.";
        exit(-1);
    }
    Log::debug2() << "Max concurrent RAM requests = " << max_concurrent_;
    stats_.vulnerability_ = sim::stats::VulnerabilityStat::template create_from_config<Perfect>(*this, conf);
}

template<typename Message, template<typename> class Interconnect>
inline
Perfect<Message, Interconnect>::~Perfect()
{
    if (stats_.vulnerability_) {
        stats_.vulnerability_->finalize(this->get_simulator().get_clock());
    }
}

template<typename Message, template<typename> class Interconnect>
inline
bool Perfect<Message, Interconnect>::reset()
{
    return true;
}

template<typename Message, template<typename> class Interconnect>
inline
void Perfect<Message, Interconnect>::start()
{
    // We do not have any pending requests.
    if (pending_.empty()) {
        return;
    }
    // First pending request is due in the future.
    if (pending_.front().time_ > this->get_simulator().get_clock()) {
        return;
    }

    // Check if we can write back the Ack.
    while (not(pending_.empty()) and
            pending_.front().time_ <= this->get_simulator().get_clock()) {
        bool success = in_.write_ack(typename Message::Ack(pending_.front().data_));
        if (success) {
            uint64_t user_code_id = pending_.front().data_.get_user_code_id();
            stats_.latency[user_code_id] += latency_;
            stats_.count[user_code_id]++;
            pending_.pop_front();
        }
    }
}

template<typename Message, template<typename> class Interconnect>
inline
void Perfect<Message, Interconnect>::end()
{
    // We read from the buffer
    while ((input_buffer_.size() < max_input_buffer_size_) and in_.can_read_request()) {
        typename Message::Request request;
        if (in_.read_request(request)) {
            input_buffer_.emplace_back(std::move(request));
        }
    }

    // If we can, we take care of a request.
    if (input_buffer_.size() > 0 && pending_.size() < max_concurrent_) {
        typename Message::Request req = input_buffer_.front();
        uint64_t user_code_id = req.get_user_code_id();
        if (stats_.vulnerability_) {
            stats_.vulnerability_->mem_access(this->get_simulator().get_clock(), req);
        }
        if (req.get_op() == Message::Request::op_t::READ) {
            stats_.read_requests[user_code_id]++;
        } else if (req.get_op() == Message::Request::op_t::WRITE) {
            stats_.write_requests[user_code_id]++;
        }
        pending_.emplace_back(this->get_simulator().get_clock() + latency_, std::move(req));
        input_buffer_.pop_front();
    }
    stats_.occupancy.sample(pending_.size(), this->get_simulator().get_clock());

    if (not blocked_ and input_buffer_.size()) {
        // So, if we were not blocked, and have pending requests in the input buffer:
        blocked_ = true;
        last_nonblocked_cycle_ = this->get_simulator().get_clock() - 1;
    }
    if (blocked_ and input_buffer_.size() == 0) {
        // We where blocked, but have no more blocked Requests:
        blocked_ = false;
        stats_.blocked_cycles += this->get_simulator().get_clock() - last_nonblocked_cycle_ - 1;
    }

    set_next_cycle();
}

template<typename Message, template<typename> class Interconnect>
inline
bool Perfect<Message, Interconnect>::inspect(const typename Message::Request &)
{
    return true;
}

template<typename Message, template<typename> class Interconnect>
inline
bool Perfect<Message, Interconnect>::inspect(const typename Message::Ack &)
{
    return true;
}

template<typename Message, template<typename> class Interconnect>
inline
void Perfect<Message, Interconnect>::print_hardware_counters(sim::engine::cycles_t time, bool propagate,
        unsigned overwrite_cpu_id)
{
    sim::utils::instrumentation::SelectedInstrumentation& instrumentation =
            this->get_simulator().get_instrumentation();
    unsigned instrumentation_id = (overwrite_cpu_id != 0) ? overwrite_cpu_id : this->instrumentation_id_;
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {sim::utils::instrumentation::DRAM_LATENCY,
                stats_.latency.get_value()}},
            *this);
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {sim::utils::instrumentation::DRAM_COUNT,
                stats_.count.get_value()}},
            *this);

    uint64_t occ = static_cast<uint64_t>(stats_.occupancy.get_local_average());
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {sim::utils::instrumentation::DRAM_OCCUPANCY, occ}},
            *this);
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {sim::utils::instrumentation::DRAM_BLOCKED_CYCLES,
                stats_.blocked_cycles.get_value()}},
            *this);
}

template<typename Message, template<typename> class Interconnect>
inline
const std::string Perfect<Message, Interconnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename Message, template<typename> class Interconnect>
inline
unsigned Perfect<Message, Interconnect>::get_id() const
{
    return id_;
}

template<typename Message, template<typename> class Interconnect>
inline
void Perfect<Message, Interconnect>::print_status() const
{
    Log::warning() << "[STATUS] Perfect memory module with id " << get_id() << " deadlock report:";
    Log::warning() << " - Have " << input_buffer_.size() << " requests in queue:";
    for (const auto &req : input_buffer_) {
        Log::warning() << " Request " << req;
    }
    Log::warning() << " - Have " << pending_.size() << " requests being processed:";
    for (const auto &req : pending_) {
        Log::warning() << " Ack due on cycle " << req.time_ << " " << req.data_;
    }
}

template<typename Message, template<typename> class Interconnect>
inline
void Perfect<Message, Interconnect>::set_next_cycle()
{
    if (in_.can_read_request()) {
        // We have a Request in the Input port.
        this->set_module_activity(engine::NEXT);
    } else if (pending_.size() < max_concurrent_ && input_buffer_.size() > 0) {
        // We can process a requests next cycle:
        this->set_module_activity(engine::NEXT);
    } else if (pending_.size() > 0 and pending_.front().time_ <= this->get_simulator().get_clock()) {
        // We can send an Ack next cycle:
        this->set_module_activity(engine::NEXT);
    } else if (pending_.size() > 0) {
        // We have to way until we can send an Ack.
        this->set_module_activity(pending_.front().time_ - this->get_simulator().get_clock());
    } else {
        // We have to wait until we get a new Request.
        this->set_module_activity(engine::NEVER);
    }
}

}  // namespace memory
}  // namespace sim


#endif  // CORE_MODULES_MEMORY_PERFECT_IMPL_H_
