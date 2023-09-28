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

#include <string>
#include "Transaction.h"

#include "core/utils/Log.h"

#ifndef CORE_MODULES_MEMORY_DRAMSIM2CONTROLLER_IMPL_H_
#define CORE_MODULES_MEMORY_DRAMSIM2CONTROLLER_IMPL_H_

namespace sim {
namespace memory {

using sim::utils::Log;

template<typename Message, template<typename> class Interconnect>
inline
Dramsim2Controller<Message, Interconnect>::Dramsim2Config::Dramsim2Config(const engine::Config &config) :
    path_(getenv("TMPDIR")!= NULL ? getenv("TMPDIR") : "/tmp"),
    dram_ini_(lock_config_file(path_, "tasksim_dramsim_dram", ".ini")),
    system_ini_(lock_config_file(path_, "tasksim_dramsim_system", ".ini")),
    trace_file_("tasksim_dram"),
    megs_of_ram_(config.get_value<unsigned>("megs_of_ram")),
    in_(Input(*this))
{
    // Here we should generate the configuration files:
    std::ofstream system_ini_file(system_ini_);
    for (unsigned i = 0; i < system_ini_options.size(); i++) {
        if (config.key_exists(system_ini_options[i].first)) {
            system_ini_file << system_ini_options[i].first << "="
                            << config.get_value<std::string>(system_ini_options[i].first) << std::endl;
            Log::debug() << "Overriding Dramsim system configuration option " << system_ini_options[i].first
                         << " with value " << config.get_value<std::string>(system_ini_options[i].first);
        } else {
            system_ini_file << system_ini_options[i].first << "=" << system_ini_options[i].second << std::endl;
        }
    }
    system_ini_file.close();


    std::ofstream dram_ini_file(dram_ini_);
    for (unsigned i = 0; i < dram_ini_options.size(); i++) {
        if (config.key_exists(dram_ini_options[i].first)) {
            dram_ini_file << dram_ini_options[i].first << "="
                          << config.get_value<std::string>(dram_ini_options[i].first) << std::endl;
            Log::debug() << "Overriding Dramsim dram configuration option " << dram_ini_options[i].first
                         << " with value " << config.get_value<std::string>(dram_ini_options[i].first);
        } else {
            dram_ini_file << dram_ini_options[i].first << "=" << dram_ini_options[i].second << std::endl;
        }
    }
    dram_ini_file.close();
}

template<typename Message, template<typename> class Interconnect>
inline
std::string Dramsim2Controller<Message, Interconnect>::Dramsim2Config::lock_config_file(const std::string &path,
        const std::string &prefix,
        const std::string &sufix)
{
    std::string file_name;
    int index = 1;
    bool found = false;
    while (!found) {
        std::ostringstream name_stream;
        name_stream << path << "/" << prefix << "-";
        name_stream.width(4);
        name_stream.fill('0');
        name_stream << index;
        name_stream << sufix;
        file_name = name_stream.str();

        FILE *file_desc = fopen(file_name.c_str(), "wbx");
        if (file_desc == NULL) {
            // File failed to open
            // The x flag makes the open fail if the file already exists.
            index++;
        } else {
            // File created.
            fclose(file_desc);
            file_desc = NULL;
            found = true;
        }
    }
    return file_name;
}

template<typename Message, template<typename> class Interconnect>
inline
Dramsim2Controller<Message, Interconnect>::Dramsim2Config::~Dramsim2Config()
{
    unlink(system_ini_.c_str());
    unlink(dram_ini_.c_str());
}

template<typename Message, template<typename> class Interconnect>
inline
Dramsim2Controller<Message, Interconnect>::Dramsim2Controller(engine::Simulator &sim,
        const engine::Config &config,
        logic::memory::MMU *mmu,
        unsigned id) :
    engine::IModule<Message>(sim, engine::ModuleType::RAM),
    config_(config),
    input_buffer_size_(config.get_value<std::size_t>("input-buffer")),
    dram_(config_.dram_ini_, config_.system_ini_, config_.path_, config_.trace_file_, config_.megs_of_ram_),
    dram_cycle_(0),
    stats_(*this),
    mmu_(mmu),
    id_(id),
    in_(Input(*this))
{
    this->instrumentation_id_ = sim.get_instrumentation().add_module(*this);
    // Connect the return functions:
    DRAMSim::TransactionCompleteCB *read_cb =
            new Callback<Dramsim2Controller, void, unsigned, uint64_t, uint64_t>(this,
                                                                                 &Dramsim2Controller::read_complete);
    DRAMSim::TransactionCompleteCB *write_cb =
            new Callback<Dramsim2Controller, void, unsigned, uint64_t, uint64_t>(this,
                                                                                 &Dramsim2Controller::write_complete);
    dram_.RegisterCallbacks(read_cb, write_cb, NULL);
    dram_.setCPUClockSpeed(1000000000ull);
    if (mmu_) {
        mmu_->register_dram_size(config_.megs_of_ram_ * 1024 * 1024);
    }
}


template<typename Message, template<typename> class Interconnect>
inline
Dramsim2Controller<Message, Interconnect>::~Dramsim2Controller()
{}

template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::advance_dram_cycle()
{
    dram_.update();
    dram_cycle_++;
}

template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::update_dram_cycle()
{
    sim::engine::cycles_t current_cycle = this->get_simulator().get_clock();
    while (current_cycle > dram_cycle_) {
        advance_dram_cycle();
    }
}

template<typename Message, template<typename> class Interconnect>
inline
bool Dramsim2Controller<Message, Interconnect>::reset()
{
    return true;
}


template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::start()
{
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
void Dramsim2Controller<Message, Interconnect>::end()
{
    // We try to read the bus for a request:

    while (input_buffer_.size() < input_buffer_size_ and in_.can_read_request()) {
        bool ret = in_.read_request(input_buffer_);
        assert(ret == true);
        /* Grab Statistics */
        switch (input_buffer_.back().get_op()) {
            case Message::Request::READ:
            case Message::Request::PREFETCH:
                stats_.read_requests++; break;
            case Message::Request::WRITE:
                stats_.write_requests++; break;
            default:
                break;
        }
    }

    update_dram_cycle();

    if (input_buffer_.size()) {
        typename Message::Request &request = input_buffer_.front();
        // First we get the memory address and check if the RAM module will accept the petition.
        uint64_t addr = request.get_tag();
        if (dram_.willAcceptTransaction(addr)) {
            // If we accept the petition, we send it
            DRAMSim::TransactionType trans_type;
            // Fer un vector amb els valors ja que son predefinits.
            switch (request.get_op()) {
                case Message::Request::READ:
                    trans_type = DATA_READ;
                    break;
                case Message::Request::PREFETCH:
                    trans_type = DATA_READ;
                    break;
                case Message::Request::WRITE:
                    trans_type = DATA_WRITE;
                    break;
                default:
                    std::abort();
                    break;
            }
            DRAMSim::Transaction trans(trans_type, addr, NULL);
            dram_.addTransaction(trans);
            request_buffer_[addr].emplace_back(this->get_simulator().get_clock(), typename Message::Request(request));
            if (mmu_ != NULL) {
                mmu_->notify_access(request);
            }
            input_buffer_.pop_front();
        }
    }
    // We do advance the cycle to make sure acks are sent on the right cycle:
    advance_dram_cycle();

    stats_.occupancy.sample(/*input_buffer_.size() +*/ request_buffer_.size(), this->get_simulator().get_clock());
    // Calculate next cycle:
    if (request_buffer_.size() > 0 || output_buffer_.size() > 0 || input_buffer_.size()) {
        this->set_module_activity(engine::NEXT);
    } else {
        this->set_module_activity(engine::NEVER);
    }
    return;
}


template<typename Message, template<typename> class Interconnect>
inline
bool Dramsim2Controller<Message, Interconnect>::inspect(const typename Message::Request &req)
{
    update_dram_cycle();
    return input_buffer_.size() < input_buffer_size_;
}


template<typename Message, template<typename> class Interconnect>
inline
bool Dramsim2Controller<Message, Interconnect>::inspect(const typename Message::Ack &)
{
    assert(0);
    return false;
}

template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::read_complete(unsigned id, uint64_t addr, uint64_t dramsim_cycle)
{
    return_request(id, addr, dramsim_cycle);
    return;
}

template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::write_complete(unsigned id, uint64_t addr, uint64_t dramsim_cycle)
{
    return_request(id, addr, dramsim_cycle);
    return;
}

template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::return_request(unsigned id, uint64_t addr, uint64_t dramsim_cycle)
{
    assert(request_buffer_.find(addr) != request_buffer_.end());
    assert(request_buffer_[addr].size() > 0);
    stats_.access_time.sample(this->get_simulator().get_clock() - request_buffer_[addr].front().time_);
    output_buffer_.push_back(typename Message::Ack(request_buffer_[addr].front().data_));

    assert(this->get_simulator().get_clock() > request_buffer_[addr].front().time_);

    stats_.latency += this->get_simulator().get_clock() - request_buffer_[addr].front().time_;
    stats_.count++;

    request_buffer_[addr].front().data_.get_data_size();
    request_buffer_[addr].pop_front();
    if (request_buffer_[addr].empty()) {
        request_buffer_.erase(addr);
    }
    return;
}

template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::power_callback(unsigned, uint64_t, uint64_t dramsim_cycle) {
    return;
}

template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::print_hardware_counters(sim::engine::cycles_t time, bool propagate,
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
}
template<typename Message, template<typename> class Interconnect>
inline
const std::string Dramsim2Controller<Message, Interconnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename Message, template<typename> class Interconnect>
inline
unsigned Dramsim2Controller<Message, Interconnect>::get_id() const
{
    return id_;
}

template<typename Message, template<typename> class Interconnect>
inline
void Dramsim2Controller<Message, Interconnect>::print_status() const
{
    Log::warning() << "[STATUS] Ramulator memory module with id " << get_id() << " deadlock report:";
    Log::warning() << " - Have " << input_buffer_.size() << " requests in queue:";
    for (const auto &req : input_buffer_) {
        Log::warning() << " Request " << req;
    }
    // request buffer stuff here:
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

}  // namespace memory
}  // namespace sim


#endif  // CORE_MODULES_MEMORY_DRAMSIM2CONTROLLER_IMPL_H_
