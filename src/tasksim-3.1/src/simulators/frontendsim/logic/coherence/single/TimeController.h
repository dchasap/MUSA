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

#ifndef SIMULATORS_FRONTENDSIM_LOGIC_COHERENCE_SINGLE_TIMECONTROLLER_H_
#define SIMULATORS_FRONTENDSIM_LOGIC_COHERENCE_SINGLE_TIMECONTROLLER_H_

#include <list>
#include <set>
#include <unordered_map>
#include <vector>
#include <functional>

#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/types.h"
#include "core/logic/coherence/Controller.h"
#include "core/logic/memory/MSHR.h"
#include "core/logic/memory/CacheSet.h"
#include "core/modules/memory/CacheStats.h"
#include "core/runtime/TaskDataDep.h"

namespace sim {
namespace logic {
namespace coherence {
namespace single {

template<typename Message>
class TimeController : public coherence::Controller<engine::addr_t, CacheLine> {
protected:
    typedef typename Message::Request Request;
    typedef typename Message::Ack Ack;
    typedef typename Message::Ack::op_t ack_op_t;
    typedef typename Message::Request::op_t req_op_t;
    typedef const std::vector<engine::cycles_t> stamp_vector_t;
    typedef ClearRequest<Message> clear_request_t;

    /** Reference to the simulator. */
    const sim::engine::Simulator &simulator_;
    /** Bitmask to transform request address into cache line tag. */
    sim::engine::addr_t mask_;

#ifdef ENABLE_COMPULSORY_MISS
    /** We need this to distinguish compulsory and non-compulsory misses. */
    std::set<sim::engine::addr_t> requested_lines_;
#endif

    /* Miss Status Handling Registers */
    typedef typename sim::logic::memory::MSHR<Message> MSHR;
    MSHR mshr_;
    /** Boolean that indicates if Request processing is stalled. */
    bool stalled_;

    std::list<Request> output_request_buffer_;
    void add_output_request(Request &&request);

    struct ack_entry_t {
        Ack ack_;
        typename MSHR::entry_t *mshr_entry_;

        ack_entry_t(Ack &&ack, typename MSHR::entry_t *mshr_entry);
    };

    std::vector<std::list<ack_entry_t> > output_ack_buffer_;
    void add_output_ack(unsigned input_port, Ack &&ack, typename MSHR::entry_t *mshr_entry = NULL);

    typedef std::unordered_map<engine::addr_t, typename MSHR::entry_t *> EvictTable;
    EvictTable evict_table_;

    typedef std::list<clear_request_t> ClearRequestList;
    ClearRequestList clear_request_list_;

    typedef std::function<typename Ack::op_t(TimeController&, unsigned, const Request&)> req_handler_t;
    static req_handler_t Req_Handlers_[];
    typedef std::function<void(TimeController&, const Ack&)> ack_handler_t;
    static ack_handler_t Ack_Handlers_[];

    void allocate(const sim::engine::addr_t &addr);
    bool evict(const sim::engine::addr_t &addr, typename MSHR::entry_t &entry);

    /* Message associated handlers */
    typename Ack::op_t read_req(unsigned, const Request &request);
    typename Ack::op_t write_req(unsigned, const Request &request);
    typename Ack::op_t readwrite_req(unsigned, const Request &request);
    typename Ack::op_t writeback_req(unsigned, const Request &request);
    typename Ack::op_t dirty_req(unsigned, const Request &request);
    typename Ack::op_t flush_req(unsigned, const Request &request);
    typename Ack::op_t invalidate_req(unsigned, const Request &request);
    typename Ack::op_t prefetch_req(unsigned, const Request &request);
    typename Ack::op_t clear_req(unsigned, const Request &request);

    void data_ack(const Ack &ack);
    void write_ack(const Ack &ack);
    void null_ack(const Ack &ack);
    void prefetch_ack(const Ack &ack);
    void clear_ack(const Ack &ack);
    void miss_ack(const Ack &ack);

    /* MSHR related functions */
    void mshr_request(typename MSHR::entry_t &);
    void mshr_evict_ack(typename MSHR::entry_t &);
    bool mshr_lookup(const engine::addr_t &addr);

    bool evict_table_lookup(const engine::addr_t &addr);
    void process_clear_request(typename ClearRequestList::iterator);
    bool is_request_buffer_full();

    sim::memory::CacheStats &stats_;

public:
    TimeController(std::size_t cpu_id, sim::memory::CacheStats &stats, sim::engine::Simulator &simulator,
               const sim::engine::Config &cache_config);
    ~TimeController();

    bool process_request(unsigned input_port, const Request &request);
    void process_ack(const Ack &ack);
    void process_update(const engine::addr_t &);

    template<typename Output>
    bool send_request(Output &out);
    template<typename Input>
    bool send_ack(unsigned, Input &);

    bool is_stalled();
    size_t get_free_mshr_size() const;
    size_t get_mshr_size() const;

    bool inspect(const Message &);
    bool inspect(const Ack &ack);

    bool is_empty_output_request_buffer();
    bool is_empty_output_ack_buffer();

    /** Debug function, to be used only on testing, erases al pending requests from MSHR. */
    void clear_buffers();

    size_t get_free_lines();

    void print_status() const;
};

}  // namespace single
}  // namespace coherence
}  // namespace logic
}  // namespace sim

#include "TimeController-impl.h"

#endif  // SIMULATORS_FRONTENDSIM_LOGIC_COHERENCE_SINGLE_TIMECONTROLLER_H_
