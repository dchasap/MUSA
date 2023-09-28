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


#ifndef CORE_MODULES_INTERCONNECT_BUS_H_
#define CORE_MODULES_INTERCONNECT_BUS_H_

#include <functional>
#include <list>
#include <map>
#include <stack>
#include <string>

#include "core/engine/Config.h"
#include "core/engine/IModule.h"
#include "core/stats/Average.h"
#include "core/stats/Counter.h"
#include "core/utils/TimeList.h"

namespace sim {
namespace interconnect {

template<typename Message, template<typename> class InterConnect>
class Bus : public engine::Module, public InterConnect<Message> {
public:
    typedef typename InterConnect<Message>::Input Input;
    typedef typename InterConnect<Message>::Output Output;

protected:
    template<typename Dir>
    struct table_entry_t {
        typedef std::list<Dir *> list_t;
        typedef typename std::list<Dir *>::iterator list_ptr_t;

        list_t list_;
        list_ptr_t ptr_;

        table_entry_t();
    };

    typedef std::map<unsigned, table_entry_t<Input> > input_table_t;
    typedef std::map<unsigned, table_entry_t<Output> > output_table_t;

    input_table_t inputs_;
    output_table_t outputs_;

    engine::cycles_t delay_;
    unsigned width_;

    utils::TimeList<typename Message::Request> pending_request_queue_;
    utils::TimeList<typename Message::Ack> pending_ack_queue_;
    utils::TimeList<typename Message::Ack::tag_t> pending_update_queue_;

    template<typename Dir>
    struct routing_table_t {
        typedef std::unordered_multimap<engine::addr_t, Dir*> multimap_t;
        multimap_t multimap_;
    };

    routing_table_t<Input> ack_routing_table_;
    bool use_routing_table_;
    std::list<Input *> expected_inputs_;
    bool use_expected_inputs_;
    bool write_request_failed_;
    bool pipelined_;
    unsigned requests_per_cycle_;
    unsigned num_ports_;

    stats::Counter requests_received_;
    stats::Counter requests_sent_;
    stats::Counter ack_received_;
    stats::Counter ack_sent_;

    engine::cycles_t packet_delay(std::size_t data_size);
    template<typename DirectType, typename MessageType>
    engine::cycles_t read_ports(utils::TimeList<MessageType> &pending,
            std::map<unsigned, table_entry_t<DirectType> > &port_list,
            stats::Counter &counter,
            std::function<bool(DirectType&, MessageType&)> read,
            routing_table_t<DirectType> *routing_table_ptr = NULL,
            std::list<DirectType*> *expected_ports = NULL);

    template<typename DirectType, typename MessageType>
    bool write_ports(utils::TimeList<MessageType> &pending,
            std::map<unsigned, table_entry_t<DirectType> >&port_list,
            stats::Counter &counter,
            std::function<bool(DirectType&)> can_write,
            std::function<bool(DirectType&, const MessageType&)> write,
            routing_table_t<DirectType> *routing_table_ptr = NULL);

    stats::Average avg_data_size_;

public:
    Bus(engine::Simulator &, engine::Config);
    ~Bus();

    /** InterConnect interface */
    Input &input(unsigned);
    Output &output(unsigned);

    void notify_wakeup(void *caller_ptr);

    /** Module interface */
    bool reset();
    void start();
    void end();
    const std::string get_name() const;
    void print_hardware_counters(engine::cycles_t time, bool propagate = false, unsigned overwrite_cpu_id = 0);
    virtual void print_status() const override;
};

}  // namespace interconnect
}  // namespace sim

#include "Bus-impl.h"

#endif  // CORE_MODULES_INTERCONNECT_BUS_H_
