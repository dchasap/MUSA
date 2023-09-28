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


#ifndef CORE_MODULES_CPU_MEMORY_IMPL_H_
#define CORE_MODULES_CPU_MEMORY_IMPL_H_

#include <string>
#include "models/ROA.h"
#include "core/utils/Log.h"
#include "core/utils/macros.h"

namespace sim {
namespace cpu {

using sim::utils::Log;

template<typename TimingModel, typename MemoryAccess>
Memory<TimingModel, MemoryAccess>::Memory(engine::Simulator &sim,
                                                            const engine::Config &config,
                                                            unsigned cpu_id):
        engine::DModule(sim, engine::ModuleType::CPU),
        model_(cpu_id, sim, config, &mem_in_),
        sim_(sim),
        cpu_id_(cpu_id),
        past_activity_(false),
        load_size_hist_(*this, "Memory CPU load size histogram", 0, 32, 8),
        store_size_hist_(*this, "Memory CPU store size histogram", 0, 32, 8),
        dl1_(*this, config.get_value<unsigned>("num-ports"))
{
    instrumentation_id_ = sim.get_instrumentation().add_module(*this);
    vulnerability_ = sim::stats::VulnerabilityStat::template create_from_config<Memory>(*this, config);
}

template<typename TimingModel, typename MemoryAccess>
Memory<TimingModel, MemoryAccess>::~Memory()
{
    if (vulnerability_) {
        vulnerability_->finalize(this->get_simulator().get_clock());
    }
}

template<typename TimingModel, typename MemoryAccess>
void
Memory<TimingModel, MemoryAccess>::send_mem_request()
{
    Log::debug() << sim_.get_clock() << ": Memory::send_mem_request()";
    bool write_success = true;
    // The dl1_ port might accept more than one Request per cycle.
    while (write_success) {
        write_success = false;
        // If we have a pending request, we send it.
        if (model_.has_mem_request()) {
            sim::logic::coherence::single::Request &request = model_.peek_mem_request();
            request.set_user_code_id(current_wd_id_);
            request.set_issuer(cpu_id_);
            assert(request.get_op() < MemoryAccess::Request::op_t::LAST);
            if (dl1_.write_request(request)) {
                if (vulnerability_) {
                    vulnerability_->mem_access(this->get_simulator().get_clock(), request);
                }
                typename MemoryAccess::Request::op_t op = request.get_op();
                switch (op) {
                case MemoryAccess::Request::op_t::READ:
                    load_size_hist_.sample(request.get_size());
                    break;
                case MemoryAccess::Request::op_t::WRITE:
                    store_size_hist_.sample(request.get_data_size());
                    break;
                default:
                    break;
                }
                model_.pop_mem_request();
            }
        }
    }
}

template<typename TimingModel, typename MemoryAccess>
void
Memory<TimingModel, MemoryAccess>::get_mem_response()
{
    while (dl1_.can_read_ack()) {
        typename MemoryAccess::Ack mem_response;
        bool success = dl1_.read_ack(mem_response);
        assert(success == true);
        _unused(success);
        Log::debug() << "Reading ack from address " << mem_response.get_tag();
        Log::debug() << sim_.get_clock() << ": CPU " << cpu_id_ << ": Reading ack "
                     << mem_response;
        model_.ack_mem_response(mem_response);
    }
}

template<typename TimingModel, typename MemoryAccess>
void
Memory<TimingModel, MemoryAccess>::start(void)
{
    Log::debug() << sim_.get_clock() << ": Memory::start()";
    Log::debug() << "MemoryCPU: mem_in_= " << mem_in_;
    send_mem_request();
}

template<typename TimingModel, typename MemoryAccess>
void
Memory<TimingModel, MemoryAccess>::end(void)
{
    Log::debug() << sim_.get_clock() << ": Memory::end()";

    // First we get the responses:
    get_mem_response();

    // Them we run the timing model:
    sim::engine::cycles_t model_next_cycle = model_.cycle();

    // And then we evaluate the next cycle:
    if (drain_state_ == engine::drain_state_t::DRAINING or model_.has_mem_request()) {
        set_module_activity(sim::engine::NEXT);
    } else {
        set_module_activity(model_next_cycle);
    }

    // TODO(who?): add support for CPU model switching
    // if (model_.finished()) {
    //     this->switch_request_ = true;
    // }
}

template<typename TimingModel, typename MemoryAccess>
inline
void
Memory<TimingModel, MemoryAccess>::context_switch(engine::addr_t stack_ptr)
{
    model_.context_switch(stack_ptr);
}

template<typename TimingModel, typename MemoryAccess>
inline
void
Memory<TimingModel, MemoryAccess>::print_hardware_counters(engine::cycles_t time,
        bool propagate, unsigned overwrite_cpu_id)
{
    // We delegate this to the Timing Model as it has all the data and acces
    // To the simulator
    unsigned instrumentation_id = (overwrite_cpu_id != 0) ? overwrite_cpu_id : this->instrumentation_id_;
    model_.print_hardware_counters(time, instrumentation_id);
    if (propagate) {
        dl1_.get_peer().print_hardware_counters(time, true, overwrite_cpu_id);
    }
}

template<typename TimingModel, typename MemoryAccess>
bool
Memory<TimingModel, MemoryAccess>::drain()
{
    Log::debug() << sim_.get_clock() << ": Memory::drain()";
    if (drain_state_ == engine::drain_state_t::DRAINED) {
        Log::debug() << "Memory CPU model drained";
        return true;
    } else if (model_.finished() and mem_in_->empty()) {
        model_.end_of_phase();
        drain_state_ = engine::drain_state_t::DRAINED;
        Log::debug() << "Memory CPU model drained";
        return true;
    } else {
        drain_state_ = engine::drain_state_t::DRAINING;
        Log::debug() << "Memory CPU model draining";
        Log::debug() << model_.finished() << " " << sim_.get_clock() << " " << mem_in_->empty();
        return false;
    }
}

template<typename TimingModel, typename MemoryAccess>
void
Memory<TimingModel, MemoryAccess>::drain_resume()
{
    if (drain_state_ == engine::drain_state_t::DRAINED) {
        model_.start_of_phase();
        drain_state_ = engine::drain_state_t::RUNNING;
        Log::debug() << "Memory::drain_resume() RUNNING";
        set_module_activity(1);
    }
}

template<typename TimingModel, typename MemoryAccess>
const unsigned
Memory<TimingModel, MemoryAccess>::get_id() const {
    return cpu_id_;
}

template<typename TimingModel, typename MemoryAccess>
inline
const std::string Memory<TimingModel, MemoryAccess>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename TimingModel, typename MemoryAccess>
inline
bool Memory<TimingModel, MemoryAccess>::cpu_activity_poll()
{
    past_activity_ = model_.cpu_activity_poll();
    if (past_activity_ == true) {
        past_activity_ = false;
        return true;
    } else {
        return false;
    }
}

template<typename TimingModel, typename MemoryAccess>
inline
void Memory<TimingModel, MemoryAccess>::print_status() const
{
    Log::warning() << "[STATUS] Memory cpu " << get_id() << " deadlock report:";
    Log::warning() << "         Model has a request? " << model_.has_mem_request();
    model_.print_status();
}

template<typename TimingModel, typename MemoryAccess>
inline
void Memory<TimingModel, MemoryAccess>::set_current_wd_id(uint64_t wd_id)
{
    current_wd_id_ = wd_id;
    model_.set_current_wd_id(wd_id);
}

}  // namespace cpu
}  // namespace sim


#endif  // CORE_MODULES_CPU_MEMORY_IMPL_H_
