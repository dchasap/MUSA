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


#ifndef CORE_MODULES_MEMORY_DRAM_IMPL_H_
#define CORE_MODULES_MEMORY_DRAM_IMPL_H_

#include <string>

#include "core/utils/macros.h"

namespace sim {
namespace memory {

template<typename OwnerType>
inline
DRAMStats::DRAMStats(const OwnerType &owner) :
        RAMStats(owner),
        open(owner, "Row open event counter (DRAM module)"),
        precharge(owner, "Precharge event counter (DRAM module)")
{}


template<typename Message, template<typename> class Interconnect>
inline
DRAM<Message, Interconnect>::Config::Config(const sim::engine::Config &config) :
    frequency_divider_(config.get_value<float>("frequency-divider")),
    cas_(config.get_value<unsigned>("cas")),
    ras_(config.get_value<unsigned>("ras")),
    precharge_(config.get_value<unsigned>("precharge"))
{
    std::string access_mask = config.get_value<std::string>("access-mask");
    int n = 0;
    for (auto c = access_mask.rbegin(); c != access_mask.rend(); c++) {
        switch (*c) {
            case 'c': column_range_[n] = true; break;
            case 'r': row_range_[n] = true; break;
            case 'B': bank_range_[n] = true; break;
            case 'R': rank_range_[n] = true; break;
        }
        n++;
    }

    if (config.key_exists("addr-range-start"))
        addr_range_start_ = stoull(config.get_value<std::string>("addr-range-start"), NULL, 16);
    else
        addr_range_start_ = sim::engine::addr_t(0);

    if (config.key_exists("addr-range-end"))
        addr_range_end_ = stoull(config.get_value<std::string>("addr-range-end"), NULL, 16);
    else
        addr_range_end_ = sim::engine::addr_t(0xffffffffffffffff);
}


template<typename Message, template<typename> class Interconnect>
inline
DRAM<Message, Interconnect>::DRAM(sim::engine::Simulator &sim, sim::engine::Config config,
        sim::logic::memory::MMU *mmu, unsigned id) :
    sim::engine::IModule<Message>(sim, sim::engine::ModuleType::RAM),
    config_(config),
    input_buffer_size_(config.get_value<std::size_t>("input-buffer")),
    dram_(config_.rank_range_, config_.bank_range_, config_.row_range_, config_.column_range_),
    stats_(*this),
    mmu_(mmu),
    id_(id),
    in_(Input(*this))
{
    this->instrumentation_id_ = sim.get_instrumentation().add_module(*this);
    if (mmu_)
        mmu_->register_dram_size(config_.addr_range_end_ - config_.addr_range_start_ + 1);
}


template<typename Message, template<typename> class Interconnect>
inline
DRAM<Message, Interconnect>::~DRAM()
{}



template<typename Message, template<typename> class Interconnect>
inline
bool DRAM<Message, Interconnect>::reset()
{
    return true;
}


template<typename Message, template<typename> class Interconnect>
inline
void DRAM<Message, Interconnect>::start()
{
    /* Send any data that has been written */
    while (not(output_buffer_.empty()) and (output_buffer_.front().time_ <= this->get_simulator().get_clock())) {
        bool success = in_.write_ack(typename Message::Ack(output_buffer_.front().data_));
        if (success) {
            dram_.unset_busy(output_buffer_.front().data_.get_tag());
            output_buffer_.pop_front();
        }
    }
}


template<typename Message, template<typename> class Interconnect>
inline
void DRAM<Message, Interconnect>::end()
{
    while (input_buffer_.size() < input_buffer_size_ and in_.can_read_request()) {
        bool ret = in_.read_request(input_buffer_);
        assert(ret == true);
        _unused(ret);
        /* Grab Statistics */
        switch (input_buffer_.back().get_op()) {
            case Message::Request::op_t::READ:
            case Message::Request::op_t::PREFETCH:
                stats_.read_requests++; break;
            case Message::Request::op_t::WRITE:
                stats_.write_requests++; break;
            default:
                break;
        }
    }

    if (input_buffer_.empty() == false) {
        /* Check if we can process the request or the bank has some activity going on */
        if (!dram_.is_busy(input_buffer_.front().get_tag())) {
            /* Calculate the delay in handling the request */
            sim::engine::cycles_t delay = config_.cas_;
            if (dram_.get(input_buffer_.front().get_tag()) == false) {
                stats_.open++;
                dram_.invalidate(input_buffer_.front().get_tag());
                if (dram_.precharge(input_buffer_.front().get_tag())) {
                    stats_.precharge++;
                    delay += config_.precharge_;
                }
                delay += config_.ras_;
                dram_.set(input_buffer_.front().get_tag());
            }
            delay *= config_.frequency_divider_;
            stats_.access_time.sample(delay);

            /* Make sure the bank is not used until the data is read */
            dram_.set_busy(input_buffer_.front().get_tag());
            output_buffer_.emplace_back(this->get_simulator().get_clock() + delay,
                                        typename Message::Ack(input_buffer_.front()));
            stats_.latency += delay;
            stats_.count++;
            if (mmu_ != NULL) {
                mmu_->notify_access(input_buffer_.front());
            }
            input_buffer_.pop_front();
        }
    }
    stats_.occupancy.sample(output_buffer_.size(), this->get_simulator().get_clock());
    if ((output_buffer_.empty() == false) && (output_buffer_.front().time_ > this->get_simulator().get_clock())) {
        this->set_module_activity(output_buffer_.front().time_ - this->get_simulator().get_clock());
    } else {
        if (output_buffer_.empty() == true) {
            this->set_module_activity(sim::engine::NEVER);
        } else {
            this->set_module_activity(sim::engine::NEXT);
        }
    }
}


template<typename Message, template<typename> class Interconnect>
inline
bool DRAM<Message, Interconnect>::inspect(const typename Message::Request &req)
{
    if (req.get_tag() < config_.addr_range_start_)
        return false;

    if (req.get_tag() > config_.addr_range_end_)
        return false;

    return true;
}


template<typename Message, template<typename> class Interconnect>
inline
bool DRAM<Message, Interconnect>::inspect(const typename Message::Ack &)
{
    assert(0);
    return false;
}

template<typename Message, template<typename> class Interconnect>
inline
void DRAM<Message, Interconnect>::print_hardware_counters(sim::engine::cycles_t time, bool propagate,
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
const std::string DRAM<Message, Interconnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename Message, template<typename> class Interconnect>
inline
unsigned DRAM<Message, Interconnect>::get_id() const
{
    return id_;
}

template<typename Message, template<typename> class Interconnect>
inline
void DRAM<Message, Interconnect>::print_status() const
{
    Log::warning() << "[STATUS] Perfect memory module with id " << get_id() << " deadlock report:";
    Log::warning() << " - Have " << input_buffer_.size() << " requests in queue:";
    for (const auto &req : input_buffer_) {
        Log::warning() << " Request " << req;
    }
    Log::warning() << " - Have " << output_buffer_.size() << " requests being processed:";
    for (const auto &ack : output_buffer_) {
        Log::warning() << " Ack due on cycle " << ack.time_ << " " << ack.data_;
    }
}

}  // namespace memory
}  // namespace sim

#endif  // CORE_MODULES_MEMORY_DRAM_IMPL_H_
