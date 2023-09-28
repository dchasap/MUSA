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


#ifndef CORE_MODULES_INTERCONNECT_BUS_IMPL_H_
#define CORE_MODULES_INTERCONNECT_BUS_IMPL_H_

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>

#include "core/utils/Log.h"

namespace sim {
namespace interconnect {

using sim::utils::Log;

template<typename Message>
inline
static auto get_type_size(Message &t) -> typename std::enable_if<std::is_fundamental<Message>::value, std::size_t>::type
{
    return 1;
}

template<typename Message>
inline
static auto get_type_size(Message &t) -> typename std::enable_if<!std::is_fundamental<Message>::value, std::size_t>::type
{
    return t.get_data_size();
}

template<typename Message, template<typename> class InterConnect>
template<typename Dir>
Bus<Message, InterConnect>::table_entry_t<Dir>::table_entry_t() :
    ptr_(list_.begin())
{}

template<typename Message, template<typename> class InterConnect>
inline
engine::cycles_t Bus<Message, InterConnect>::packet_delay(std::size_t data_size)
{
    if (pipelined_ == true) {
        return delay_ + (data_size + width_ - 1) / width_;
    } else {
        return delay_ * std::max(1ul, ((data_size + width_ - 1) / width_));
    }
}

template<typename Message, template<typename> class InterConnect>
template<typename DirectType, typename MessageType>
inline
engine::cycles_t Bus<Message, InterConnect>::read_ports(utils::TimeList<MessageType> &pending,
        std::map<unsigned, table_entry_t<DirectType>> &port_list,
        stats::Counter &counter,
        std::function<bool(DirectType&, MessageType&)> read,
        routing_table_t<DirectType>* routing_table_ptr,
        std::list<DirectType*>* expected_ports)
{
    if (NULL != expected_ports) {
        typename std::list<DirectType*>::iterator i;
        for (i = expected_ports->begin(); i != expected_ports->end(); ++i) {
            MessageType data;
            if (read(**i,data) == false) {
                continue;
            }
            std::size_t data_size = get_type_size(data);
            uint64_t user_code_id = data.get_user_code_id();

            if (NULL != routing_table_ptr) {
                routing_table_ptr->multimap_.insert(std::make_pair(data.get_tag(), *i));
            }

            expected_ports->erase(i);
            engine::cycles_t delay = packet_delay(data_size);
            pending.push_back(utils::time_list_entry_t<MessageType>(delay + this->get_simulator().get_clock(), data));
            counter[user_code_id]++;
            avg_data_size_.sample(data_size);
            return delay;
        }

        return engine::NEVER;
    }

    for (auto &port : port_list) {
        typename table_entry_t<DirectType>::list_t::iterator i = port.second.ptr_;
        for (++i; ; ++i) {
            if (i == port.second.list_.end()) {
                i = port.second.list_.begin();
            }
            MessageType data;
            if (read(**i,data) == false) {
                if (i == port.second.ptr_) {
                    break;
                }
                continue;
            }
            std::size_t data_size = get_type_size(data);
            uint64_t user_code_id = data.get_user_code_id();

            if (NULL != routing_table_ptr) {
                routing_table_ptr->multimap_.insert(std::make_pair(data.get_tag(), *i));
            }

            port.second.ptr_ = i;
            engine::cycles_t delay = packet_delay(data_size);
            pending.push_back(utils::time_list_entry_t<MessageType>(delay + this->get_simulator().get_clock(), data));
            Log::debug4() << "Reading " << data.get_tag() << " ad delaying " << delay;
            counter[user_code_id]++;
            avg_data_size_.sample(data_size);
            return delay;
        }
    }
    return engine::NEVER;
}

template<typename Message, template<typename> class InterConnect>
template<typename DirectType, typename MessageType>
inline
bool Bus<Message, InterConnect>::write_ports(utils::TimeList<MessageType> &pending,
        std::map<unsigned, table_entry_t<DirectType>> &port_list,
        stats::Counter &counter,
        std::function<bool(DirectType&)> can_write,
        std::function<bool(DirectType&, const MessageType&)> write,
        routing_table_t<DirectType>* routing_table_ptr)
{
    /* Check if there are pending requests */
    if (pending.empty() or port_list.empty()) {
        return true;
    }

    /* Make sure the request is not for the future */
    if (pending.front().time_ > this->get_simulator().get_clock()) {
        return true;
    }

    if (NULL != routing_table_ptr) {
        auto its = routing_table_ptr->multimap_.equal_range(pending.front().data_.get_tag());
        for (auto it = its.first; it != its.second; ++it) {
            DirectType* i = it->second;
            if (can_write(*i) and dynamic_cast<sim::engine::IModule<Message>&>(i->get_peer()).inspect(pending.front().data_) == true) {
                if (write(*i, MessageType(pending.front().data_)) == true) {
                    uint64_t user_code_id = pending.front().data_.get_user_code_id();
                    counter[user_code_id]++;
                    pending.pop_front();
                    routing_table_ptr->multimap_.erase(it);
                    return true;
                }
                return false;
            }
        }
    }

    for (auto &port : port_list) {
        typename table_entry_t<DirectType>::list_t::iterator i;
        i = port.second.ptr_;
        for (++i; ; ++i) {
            if (i == port.second.list_.end()) {
                i = port.second.list_.begin();
            }
            if (can_write(**i) and dynamic_cast<sim::engine::IModule<Message>&>((*i)->get_peer()).inspect(pending.front().data_) == true) {
                if (write(**i, MessageType(pending.front().data_)) == true) {
                    uint64_t user_code_id = pending.front().data_.get_user_code_id();
                    counter[user_code_id]++;
                    pending.pop_front();
                    return true;
                } else {
                    Log::error() << "[ERROR] Cannot write when can_write was true and inspect was true.";
                    exit(-1);
                    return false;
                }
            }
            if (i == port.second.ptr_) {
                break;
            }
        }
    }
    return true;
}

template<typename Message, template<typename> class InterConnect>
inline
Bus<Message, InterConnect>::Bus(engine::Simulator &sim, engine::Config conf) :
    engine::Module(sim, sim::engine::IN),
    delay_(conf.get_value<unsigned>("latency")),
    width_(conf.get_value<unsigned>("width")),
    use_routing_table_(false),
    use_expected_inputs_(false),
    write_request_failed_(false),
    pipelined_(conf.get_value_with_default<bool>("pipelined", true)),
    requests_per_cycle_(conf.get_value_with_default<unsigned>("req-per-cycle", 1)),
    num_ports_(conf.get_value<unsigned>("num-ports")),
    requests_received_(*this, "Requests received"),
    requests_sent_(*this, "Requests sent"),
    ack_received_(*this, "Ack received"),
    ack_sent_(*this, "Ack sent"),
    avg_data_size_(*this, "AvgDataSize")
{
    if (conf.key_exists("use-routing-table")) {
        use_routing_table_ = (1 == conf.get_value<unsigned>("use-routing-table"));
    }

    if (conf.key_exists("use-expected-inputs")) {
        use_expected_inputs_ = (1 == conf.get_value<unsigned>("use-expected-inputs"));
    }
}

template<typename Message, template<typename> class InterConnect>
inline
Bus<Message, InterConnect>::~Bus()
{
    typename input_table_t::const_iterator in;
    for (in = inputs_.begin(); in != inputs_.end(); ++in) {
        typename table_entry_t<Input>::list_t::const_iterator i;
        for (i = in->second.list_.begin(); i != in->second.list_.end(); ++i) {
            delete (*i);
        }
    }

    typename output_table_t::const_iterator out;
    for (out = outputs_.begin(); out != outputs_.end(); ++out) {
        typename table_entry_t<Output>::list_t::const_iterator i;
        for (i = out->second.list_.begin(); i != out->second.list_.end(); ++i) {
            delete (*i);
        }
    }
}

template<typename Message, template<typename> class InterConnect>
inline
typename Bus<Message, InterConnect>::Input &Bus<Message, InterConnect>::input(unsigned priority)
{
    Input* inp_ptr = new Input(*this);
    inputs_[priority].list_.push_back(inp_ptr);
    return *(inputs_[priority].list_.back());
}

template<typename Message, template<typename> class InterConnect>
inline
typename Bus<Message, InterConnect>::Output &Bus<Message, InterConnect>::output(unsigned priority)
{
    outputs_[priority].list_.push_back(new Output(*this, num_ports_));
    return *(outputs_[priority].list_.back());
}

template<typename Message, template<typename> class InterConnect>
inline
void Bus<Message, InterConnect>::notify_wakeup(void *caller_ptr)
{
    if (use_expected_inputs_) {
        Input* inp_ptr = static_cast<Input*>(caller_ptr);
        expected_inputs_.push_back(inp_ptr);
    }
}

template<typename Message, template<typename> class InterConnect>
inline
bool Bus<Message, InterConnect>::reset()
{
    for (auto &out : outputs_) {
        out.second.ptr_ = out.second.list_.empty() ? out.second.list_.end() : out.second.list_.begin();
    }
    for (auto &in : inputs_) {
        in.second.ptr_ = in.second.list_.empty() ? in.second.list_.end() : in.second.list_.begin();
    }
    return true;
}

template<typename Message, template<typename> class InterConnect>
inline
void Bus<Message, InterConnect>::start()
{
    unsigned c = 0;
    write_request_failed_ = false;
    while (not write_request_failed_ and not pending_request_queue_.empty() and
            (pending_request_queue_.front().time_ <= this->get_simulator().get_clock()) and (c < requests_per_cycle_)) {
        write_request_failed_ = !write_ports<Output, typename Message::Request>(
                pending_request_queue_, outputs_, requests_sent_,
                &Output::can_write_request,
                &Output::write_request, NULL);
        c++;
    }
    Log::debug() << "[BUS] At cycle " << this->get_simulator().get_clock() << " we sent " << c << " REQs.";
    bool ack_write_failed = false;
    c = 0;
    while (not ack_write_failed and not pending_ack_queue_.empty() and
            (pending_ack_queue_.front().time_ <= this->get_simulator().get_clock()) and (c < requests_per_cycle_)) {
        ack_write_failed = !write_ports<Input, typename Message::Ack>(
                pending_ack_queue_, inputs_, ack_sent_,
                &Input::can_write_ack,
                &Input::write_ack,
                use_routing_table_ ? &ack_routing_table_ : NULL);
        c++;
    }
    Log::debug() << "[BUS] At cycle " << this->get_simulator().get_clock() << " we sent " << c << " ACKs.";
}

template<typename Message, template<typename> class InterConnect>
inline
void Bus<Message, InterConnect>::end()
{
    /* TODO: create a generic bus, and make a full duplex bus as a combination of two different buses */

    /* Read the request channel */
    unsigned c = 0;
    bool reached_limit = false;
    for (c = 0; c < requests_per_cycle_; c++) {
        sim::engine::cycles_t delay =
                read_ports<Input, typename Message::Request>(pending_request_queue_,
                        inputs_,
                        requests_received_,
                        static_cast<bool(Input::*)(typename Message::Request&)>(&Input::read_request),
                        use_routing_table_ ? &ack_routing_table_ : NULL,
                        use_expected_inputs_ ? &expected_inputs_ : NULL);
        // If we did not read anything:
        if (delay == sim::engine::NEVER) {
            break;
        }
    }
    if (c == requests_per_cycle_) {
        reached_limit = true;
    }
    Log::debug() << "[BUS] At cycle " << this->get_simulator().get_clock() << " we read " << c << " REQs";

    /* Read the Ack channel */
    for (c = 0; c < requests_per_cycle_; c++) {
        sim::engine::cycles_t delay = read_ports<Output, typename Message::Ack>(pending_ack_queue_,
                outputs_, ack_received_,
                static_cast<bool(Output::*)(typename Message::Ack&)>(&Output::read_ack),
                NULL, NULL);
        // If we did not read anything:
        if (delay == sim::engine::NEVER) {
            break;
        }
    }
    if (c == requests_per_cycle_) {
        reached_limit = true;
    }
    Log::debug() << "[BUS] At cycle " << this->get_simulator().get_clock() << " we read " << c << " ACKs";

    sim::engine::cycles_t next_cycle = sim::engine::NEVER;
    if ((!pending_request_queue_.empty()) && (!write_request_failed_)) {
        if (pending_request_queue_.front().time_ > this->get_simulator().get_clock()) {
            next_cycle = pending_request_queue_.front().time_;
        } else {
            next_cycle = this->get_simulator().get_clock() + 1;
        }
    }

    if ((!pending_ack_queue_.empty()) && (pending_ack_queue_.front().time_ < next_cycle)) {
        if (pending_ack_queue_.front().time_ > this->get_simulator().get_clock()) {
            next_cycle = pending_ack_queue_.front().time_;
        } else {
            next_cycle = this->get_simulator().get_clock() + 1;
        }
    }
    assert(next_cycle > this->get_simulator().get_clock());
    if (reached_limit) {
        this->set_module_activity(sim::engine::NEXT);
    } else if (next_cycle == sim::engine::NEVER) {
        this->set_module_activity(sim::engine::NEVER);
    } else {
        this->set_module_activity(next_cycle - this->get_simulator().get_clock());
    }

    write_request_failed_ = false;
}

template<typename Message, template<typename> class InterConnect>
inline
const std::string Bus<Message, InterConnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename Message, template<typename> class InterConnect>
inline
void Bus<Message, InterConnect>::print_hardware_counters(engine::cycles_t time, bool propagate, unsigned overwrite_cpu_id)
{
    if (propagate) {
        // First we iterate by priority:
        for (auto it = outputs_.begin(); it != outputs_.end(); it++) {
            for (auto it2 = it->second.list_.begin(); it2 != it->second.list_.end(); it2++) {
                (*it2)->get_peer().print_hardware_counters(time, propagate, overwrite_cpu_id);
            }
        }
    }
}

template<typename T, template<typename> class InterConnect>
inline
void Bus<T, InterConnect>::print_status() const
{
    Log::warning() << "[STATUS] Interconnect Bus with id " << get_id() << " deadlock report:";
    Log::warning() << "         Have " << pending_request_queue_.size() << " pending requests";
    for (const auto &req : pending_request_queue_) {
        Log::warning() << " Request due on cycle " << req.time_ << " " << req.data_;
    }
    Log::warning() << "         Have " << pending_ack_queue_.size() << " pending acks";
    for (const auto &ack : pending_ack_queue_) {
        Log::warning() << " Ack due on cycle " << ack.time_ << " " << ack.data_;
    }
}

}  // namespace interconnect
}  // namespace sim

#endif  // CORE_MODULES_INTERCONNECT_BUS_IMPL_H_
