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


#ifndef CORE_LOGIC_COHERENCE_SINGLE_CONTROLLER_H_
#define CORE_LOGIC_COHERENCE_SINGLE_CONTROLLER_H_

#include <list>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Directory.h"
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
class Controller : public coherence::Controller<engine::addr_t, CacheLine> {
protected:
    typedef typename Message::Request Request;
    typedef typename Message::Ack Ack;
    typedef sim::runtime::task_dep_t task_dep_t;
    typedef ClearRequest<Message> clear_request_t;
    typedef sim::logic::memory::Cache<memory::CacheSet<sim::engine::addr_t, CacheLine>> VictimCache;
    typedef typename sim::logic::memory::MSHR<Message> MSHR;
    typedef std::unordered_map<sim::engine::addr_t, unsigned> EvictTable;
    typedef std::list<clear_request_t> ClearRequestList;
    typedef std::function<typename Ack::op_t(Controller&, unsigned, const Request&)> req_handler_t;
    typedef std::function<void(Controller&, const Ack&)> ack_handler_t;
    struct ack_entry_t {
        Ack ack_;
        typename MSHR::entry_t *mshr_entry_;
        ack_entry_t(Ack &&ack, typename MSHR::entry_t *mshr_entry);
    };

public:
    /** Constructor: */
    Controller(std::size_t cpu_id, sim::memory::CacheStats &stats, sim::engine::Simulator &simulator,
               sim::logic::coherence::single::Directory<Message> &directory, const sim::engine::Config &cache_config);
    /** Destructor: */
    ~Controller();
    /** Processes a Request from \input_port Input. */
    bool process_request(unsigned input_port, const Request &request);
    /** Processes an Ack. */
    void process_ack(const Ack &ack);
    /** Sends a request to the next Memory level through Output \output. */
    template<typename Output>
    bool send_request(Output &output);
    /** Sends an Ack to the previous level through Input \input[\input_port_index]  */
    template<typename Input>
    bool send_ack(unsigned input_port_index, Input &input);
    /** Returns whether the Controller is stalled. */
    bool is_stalled();
    /** Returns the MSHR number of free entries. */
    size_t get_free_mshr_size() const;
    /** Returns the MSHR total size. */
    size_t get_mshr_size() const;

    /** Inspects if the Controller is waiting for Ack ack. */
    bool inspect(const Ack &ack);
    /** Returns wheter the Controller has any Request to send. */
    bool is_empty_output_request_buffer();
    /** Returns wheter the Controller has any Ack to send. */
    bool is_empty_output_ack_buffer();

    /** Debug function, to be used only on testing, erases al pending requests from MSHR. */
    void clear_buffers();
    /** Returns the total number of free lines in the Cache. */
    size_t get_free_lines();
    /** Prints current status on the Cache (deadlock detection issues). */
    void print_status() const;

protected:
    /** Adds an output Request to the output_request_buffer_. */
    template<typename ...Args>
    void add_output_request(Args ...args);
    /** Adds an output Request to the output_request_buffer_. */
    void add_output_request(Request &&request);
    /** Adds an Ack to the output_ack_buffer_. */
    void add_output_ack(unsigned input_port, Ack &&ack, typename MSHR::entry_t *mshr_entry = NULL);
    /** Allocates the CacheLine associated to Request. */
    void allocate(const Request &request);
    /** Tries to insert the line into the cache_, evicting lines to the victim cache or
      * output buffer when/if necessary. */
    bool insert(const Request &request);

    /** Request handlers: */
    typename Ack::op_t read_req(unsigned input_port, const Request &request);
    typename Ack::op_t write_req(unsigned input_port, const Request &request);
    typename Ack::op_t flush_req(unsigned input_port, const Request &request);
    typename Ack::op_t invalidate_req(unsigned input_port, const Request &request);
    typename Ack::op_t prefetch_req(unsigned input_port, const Request &request);
    typename Ack::op_t clear_req(unsigned input_port, const Request &request);

    /** Ack handlers: */
    void data_ack(const Ack &ack);
    void write_ack(const Ack &ack);
    void null_ack(const Ack &ack);
    void prefetch_ack(const Ack &ack);
    void clear_ack(const Ack &ack);
    void miss_ack(const Ack &ack);

    /** Checks if the CacheLine can be inserted into the victim cache due to dependency type restrictions. */
    bool insertable_into_victim_cache(const CacheLine &cache_line) const;
    /** Checks if the Request meets specific criteria. */
    bool is_special_request(const Request &request) const;
    /** Processes the reqeust that meets a special criteria. */
    typename Message::Ack::op_t process_special_request(const Request &request);

    /* MSHR related functions */
    void mshr_request(typename MSHR::entry_t &mshr_entry);
    void mshr_evict_ack(typename MSHR::entry_t &mshr_entry);
    bool mshr_lookup(const engine::addr_t addr);

    bool evict_table_lookup(const sim::engine::addr_t addr);
    /** Sends the proper Rquests/Acks based on the ClearRequest. */
    void process_clear_request(typename ClearRequestList::iterator);
    /** Checks if the output_request_buffer contains the maximum number of Requets. */
    bool is_request_buffer_full();

    /** Reference to the simulator. */
    const sim::engine::Simulator &simulator_;
    /** Bitmask to transform request addresses into cache line tag. */
    sim::engine::addr_t mask_;

#ifdef ENABLE_COMPULSORY_MISS
    /** We need this to distinguish compulsory and non-compulsory misses. */
    std::set<sim::engine::addr_t> requested_lines_;
#endif

    /** Coherence directory reference. */
    sim::logic::coherence::single::Directory<Message> &directory_;
    /** Miss Status Handling Registers */
    MSHR mshr_;
    /** Victim cache. */
    VictimCache victim_cache_;
    /** Boolean that indicates if Request processing is stalled. */
    bool stalled_;
    /** Task Input/Output information */
    task_dep_t victim_cache_affinity_;

    /** Maximum number of requests to be sent in one cycle. */
    unsigned max_requests_;
    /** Output Request buffer. */
    std::list<Request> output_request_buffer_;

    /** Stores the Acks to be sent to the previous memory levels. */
    std::vector<std::list<ack_entry_t>> output_ack_buffer_;

    /** List of lines evicted from which the Cache has got no Ack yet. */
    EvictTable evict_table_;
    /** List of Requests sent to the next memory level and it's associated MSHR entry. */
    std::list<typename MSHR::entry_t*> pending_requests_;
    /** Clear Request list. */
    ClearRequestList clear_request_list_;

    /** Request handler function definition. */
    static req_handler_t Req_Handlers_[];
    /** Ack handler function definition: */
    static ack_handler_t Ack_Handlers_[];

    /** Reference to the Cache module statistics. */
    sim::memory::CacheStats &stats_;
};

}  // namespace single
}  // namespace coherence
}  // namespace logic
}  // namespace sim

#include "Controller-impl.h"

#endif  // CORE_LOGIC_COHERENCE_SINGLE_CONTROLLER_H_
