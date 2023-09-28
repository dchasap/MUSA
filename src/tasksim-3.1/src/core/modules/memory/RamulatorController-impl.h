/*
 * Copyright (c) 2016, Barcelona Supercomputing Center
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

#include <string.h>
#include <cstdlib>
#include <map>
#include <string>

#include "core/utils/Log.h"

#ifndef CORE_MODULES_MEMORY_RAMULATORCONTROLLER_IMPL_H_
#define CORE_MODULES_MEMORY_RAMULATORCONTROLLER_IMPL_H_


namespace sim {
namespace memory {

using sim::utils::Log;

template<typename Message, template<typename> class Interconnect>
inline
RamulatorController<Message, Interconnect>::RamulatorConfig::RamulatorConfig(const engine::Config &config) :
    ramulator_ini_(lock_config_file("tasksim_ramulator_dram", ".ini"))
{
    // Here we should generate the configuration files:
    std::ofstream dram_ini_file(ramulator_ini_);
    for (unsigned i = 0; i < dram_ini_options.size(); i++) {
        if (config.key_exists(dram_ini_options[i].first)) {
            dram_ini_file << dram_ini_options[i].first << " = "
                          << config.get_value<std::string>(dram_ini_options[i].first) << std::endl;
            Log::debug() << "Overriding Ramulator Dram configuration option " << dram_ini_options[i].first
                         << " with value " << config.get_value<std::string>(dram_ini_options[i].first);
        } else {
            dram_ini_file << dram_ini_options[i].first << " = " << dram_ini_options[i].second << std::endl;
        }
    }
    dram_ini_file.close();
}

template<typename Message, template<typename> class Interconnect>
inline
std::string RamulatorController<Message, Interconnect>::RamulatorConfig::lock_config_file(const std::string &prefix,
                                                                                          const std::string &suffix)
{
    std::ostringstream name_stream;
    name_stream << (getenv("TMPDIR") != NULL ? getenv("TMPDIR") : "/tmp") << "/" << prefix << "XXXXXX" << suffix;
    char *file_name = strdup(name_stream.str().c_str());

    int fd = mkstemps(file_name, suffix.size());
    assert(fd != -1);
    close(fd);
    return std::string(std::move(file_name));
}

template<typename Message, template<typename> class Interconnect>
inline
RamulatorController<Message, Interconnect>::RamulatorConfig::~RamulatorConfig()
{
    unlink(ramulator_ini_.c_str());
}



template<typename Message, template<typename> class Interconnect>
inline
RamulatorController<Message, Interconnect>::RamulatorController(engine::Simulator &sim,
        const engine::Config &config,
        const engine::Config &config_L3,
        unsigned int tcpus,
        logic::memory::MMU *mmu,
        unsigned id) :
    engine::IModule<Message>(sim, engine::ModuleType::RAM),
    config_(config),
    ramulator_config_(config_.ramulator_ini_),
    input_buffer_size_(128),
    num_cpus_(tcpus),
    mem_(nullptr),
    read_cb_func(std::bind(&RamulatorController::read_complete, this, std::placeholders::_1)),
    write_cb_func(std::bind(&RamulatorController::write_complete, this, std::placeholders::_1)),
    dram_cycle_(0),
    stats_(*this),
    mmu_(mmu),
    id_(id),
    pending_reads_(0),
    pending_writes_(0),
    blocked_(false),
    last_nonblocked_cycle_(0),
    frequency_ratio_(0),
    in_(Input(*this))
{
    // Declaration of statistics
    Stats::statlist.output(config.get_value<std::string>("full-path"));
    const std::string& std_name = ramulator_config_["standard"];
    assert(name_to_func.find(std_name) != name_to_func.end() && "unrecognized standard name");
    // num_cpus+1 to count the memory acceses (writes) from the last level of cache
    ramulator_config_.set_core_num(num_cpus_ + 1);
    mem_.reset(name_to_func.at(std_name)(ramulator_config_, config_L3.get_value<unsigned>("line-size")));
    frequency_ratio_ = sim.get_config().get_value<unsigned>("cpu_frequency") * (mem_->clk_ns() / 1000.);
    this->instrumentation_id_ = sim.get_instrumentation().add_module(*this);

    stats_.vulnerability_ = sim::stats::VulnerabilityStat::template create_from_config<RamulatorController>(*this, config);
}


template<typename Message, template<typename> class Interconnect>
inline
RamulatorController<Message, Interconnect>::~RamulatorController()
{
    mem_->finish();
    Stats::statlist.printall();

    if (stats_.vulnerability_) {
        stats_.vulnerability_->finalize(this->get_simulator().get_clock());
    }
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::advance_dram_cycle()
{
    dram_cycle_++;
    Stats::curTick++;
    mem_->tick();
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::update_dram_cycle()
{
    sim::engine::cycles_t current_cycle = this->get_simulator().get_clock();
    while (current_cycle > static_cast<sim::engine::cycles_t>(dram_cycle_ * frequency_ratio_ + 1e-9)) {
        advance_dram_cycle();
    }
}

template<typename Message, template<typename> class Interconnect>
inline
bool RamulatorController<Message, Interconnect>::reset()
{
    return true;
}


template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::start()
{
    update_dram_cycle();
    unsigned rejected = 0;
    while (rejected < input_buffer_.size()) {
        typename Message::Request &request = input_buffer_[rejected];

        // Using the memory address, we check if the RAM module will accept the petition.
        uint64_t addr = request.get_tag();
        bool accepted = false;

        int core_id = request.get_issuer();
        if (core_id > num_cpus_ or core_id < -1) {
            core_id = 0;
        } else if (core_id == -1) {
            core_id = ramulator_config_.get_core_num() - 1;
        }

        switch (request.get_op()) {
            case Message::Request::op_t::PREFETCH:
            case Message::Request::op_t::READ:
            {
                ramulator::Request req_read(addr, ramulator::Request::Type::READ, read_cb_func, core_id);
                accepted = mem_->send(req_read);
                pending_reads_++;
                break;
            }
            case Message::Request::op_t::WRITE:
            {
                ramulator::Request req_write(addr, ramulator::Request::Type::WRITE, write_cb_func, core_id);
                accepted = mem_->send(req_write);
                pending_writes_++;
                break;
            }
            default:
                std::abort();
                break;
        }

        // If accepted, pop the request from the input buffer and into the buffer waiting for Ramulator to complete it.
        if (accepted) {
            request_buffer_[addr].emplace_back(this->get_simulator().get_clock(), typename Message::Request(request));
            if (mmu_ != NULL) {
                mmu_->notify_access(request);
            }
            if (stats_.vulnerability_) {
                stats_.vulnerability_->mem_access(this->get_simulator().get_clock(), request);
            }
            input_buffer_.erase(input_buffer_.begin()+rejected);
        } else {
            rejected++;
        }
    }
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
    /* Send any data that has been written */
    while (not(output_buffer_.empty())) {
        bool success = in_.write_ack(output_buffer_.front());
        if (success) {
            output_buffer_.pop_front();
        } else {
            break;
        }
    }
}


template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::end()
{
    /* Read requests at the input ports */
    while ((input_buffer_.size() < input_buffer_size_) and in_.can_read_request()) {
        bool success = in_.read_request(input_buffer_);
        assert(success == true);
        // Got one, update statistics
        switch (input_buffer_.back().get_op()) {
            case Message::Request::op_t::READ:
            case Message::Request::op_t::PREFETCH:
                stats_.read_requests++; break;
            case Message::Request::op_t::WRITE:
                stats_.write_requests++; break;
            default:
                std::cerr << "Ram received an unknown request type with ID "
                          << static_cast<int>(input_buffer_.back().get_op()) << std::endl;
                break;
        }
    }


    stats_.occupancy.sample(request_buffer_.size(), this->get_simulator().get_clock());
    stats_.read_occupancy.sample(pending_reads_, this->get_simulator().get_clock());
    stats_.write_occupancy.sample(pending_writes_, this->get_simulator().get_clock());

    set_next_cycle();
    return;
}


template<typename Message, template<typename> class Interconnect>
inline
bool RamulatorController<Message, Interconnect>::inspect(const typename Message::Request &req)
{
    update_dram_cycle();
    return input_buffer_.size() < input_buffer_size_;
}


template<typename Message, template<typename> class Interconnect>
inline
bool RamulatorController<Message, Interconnect>::inspect(const typename Message::Ack &)
{
    assert(0);
    return false;
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::read_complete(ramulator::Request& req)
{
    return_request(req);
    return;
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::write_complete(ramulator::Request& req)
{
    return_request(req);
    return;
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::return_request(ramulator::Request& req)
{
    auto &sent_requests = request_buffer_.at(req.addr);
    assert(sent_requests.size() > 0);
    if (this->get_simulator().get_clock() < sent_requests.front().time_) {
        std::cerr << "ERROR: Getting a reply from Ramulator before the request was sent" << std::endl;
        std::abort();
    }

    engine::cycles_t latency = this->get_simulator().get_clock() - sent_requests.front().time_;
    stats_.access_time.sample(latency);
    if (sent_requests.front().data_.get_op() == sim::logic::coherence::single::Message::Request::op_t::WRITE) {
        stats_.write_time.sample(latency);
        pending_writes_--;
    } else {
        stats_.read_time.sample(latency);
        pending_reads_--;
    }
    stats_.latency += latency,
    stats_.count++;

    // send Ack(initial Request) back
    output_buffer_.push_back(typename Message::Ack(sent_requests.front().data_));

    sent_requests.pop_front();
    if (sent_requests.empty()) {
        request_buffer_.erase(req.addr);
    }
    return;
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::power_callback(unsigned, uint64_t, uint64_t dramsim_cycle) {
    return;
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::print_hardware_counters(sim::engine::cycles_t time, bool propagate,
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
const std::string RamulatorController<Message, Interconnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename Message, template<typename> class Interconnect>
inline
unsigned RamulatorController<Message, Interconnect>::get_id() const
{
    return id_;
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::print_status() const
{
    Log::warning() << "[STATUS] Ramulator memory module with id " << get_id() << " deadlock report:";
    Log::warning() << " - Have " << input_buffer_.size() << " requests in queue:";
    for (const auto &req : input_buffer_) {
        Log::warning() << " Request " << req;
    }
    Log::warning() << "Ramulator pending reads: " << pending_reads_ << " and writes: " << pending_writes_;
    Log::warning() << "Simulator at " << this->get_simulator().get_clock() << " and DRAM at "
                   <<  static_cast<sim::engine::cycles_t>(dram_cycle_ * frequency_ratio_ + 1e-9);
    // request buffer stuff here:
    Log::warning() << "[DRAM:Ramulator] Have " << request_buffer_.size() << " pending requests.";
    for (const auto &item : request_buffer_) {
        Log::warning() << " + Processing request for addr " << item.first << " info:";
        for (const auto &sub : item.second) {
            Log::warning() << "   - Due on cycle " << sub.time_ << ", req:  " << sub.data_;
        }
    }
    Log::warning() << " - Have " << output_buffer_.size() << " acks ready to be sent:";
    for (const auto &ack : output_buffer_) {
        Log::warning() << " + Ack " << ack;
    }
}

template<typename Message, template<typename> class Interconnect>
inline
void RamulatorController<Message, Interconnect>::set_next_cycle()
{
    /* If:
     *  - We have pending Requests on the buffer (input_buffer).
     *  - We have pending ACKs (output_buffer).
     *  - We have Request being processed on Ramulator (request_buffer)
     *  - We have pending Requests on the input ports (in_).
     * Then we have to awake next cycle, otherwise a new Request will awake us eventually.
     */
    if (request_buffer_.size() > 0 || output_buffer_.size() > 0 ||
            input_buffer_.size() > 0 || in_.can_read_request()) {
        this->set_module_activity(engine::NEXT);
    } else {
        this->set_module_activity(engine::NEVER);
    }
    return;
}

}  // namespace memory
}  // namespace sim


#endif  // CORE_MODULES_MEMORY_RAMULATORCONTROLLER_IMPL_H_
