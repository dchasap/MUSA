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

#ifndef CORE_MODULES_CPU_MODELS_ROA_H_
#define CORE_MODULES_CPU_MODELS_ROA_H_

#include <deque>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "core/engine/Config.h"
#include "core/engine/EventBuffer.h"
#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Message.h"
#include "core/trace/mem/types.h"
#include "core/modules/cpu/models/types.h"

/**
 * ROA implements the Reorder buffer Occupancy Analysis cpu model
 * proposed by S. Cho group at UPitt
 */

namespace sim {
namespace cpu {
namespace timing_models {

class ROA {
private:
    typedef OutputBuffer<sim::logic::coherence::single::Request> OutputBuffer_t;
    typedef ROB<OutputBuffer_t> ROB_t;
    typedef LDQueue<ROB_t::elem_t*> LDQueue_t;
    typedef sim::engine::EventBuffer<sim::trace::mitem_t> mem_buffer_t;

public:
    /** ROA constructor. */
    ROA(unsigned id, sim::engine::Simulator &sim, const engine::Config &config, mem_buffer_t **mem_in);
    /** We perform all tasks up to current cycle. */
    sim::engine::cycles_t cycle();
    /** Returns wether we have any pending memory request to send. */
    bool has_mem_request() const;
    /** Returns the first pending memory request. */
    sim::logic::coherence::single::Request get_mem_request();
    sim::logic::coherence::single::Request& peek_mem_request();
    void pop_mem_request();
    /** Processes the Memory system Ack. */
    void ack_mem_response(const sim::logic::coherence::single::Ack &ack);
    /** Returns wether the current phase has finished. */
    bool finished() const;

    /** Functions necessary for Stats: */
    /** Returns CPU index. */
    unsigned get_id() const;
    /** Returns module name. */
    const std::string get_name() const;

    /** TaskPoint enabling functions: */
    /** Called by simulator runtime at start of detailed phase. */
    void start_of_phase();
    /** Called by simulator runtime at end of detailed phase. */
    void end_of_phase();
    /** Returns number of instructions executed in current detailed phase. */
    uint64_t get_ins_current_phase() const;

    /** Sets current wd_id for stats. */
    void set_current_wd_id(uint64_t wd_id);

    /** Prints Status and contents of ROB/LDqueue for deadlock detection. */
    void print_status() const;

    /** Generates events for hardware counters in the instrumentation trace. */
    void print_hardware_counters(sim::engine::cycles_t time, const unsigned int cpu_instrumentation_id);

    /** Signals a context switch. */
    void context_switch(engine::addr_t);

    // Checks whether instructions have been committed since the last poll.
    bool cpu_activity_poll();

private:
    /** Next memory operation to be issued. */
    sim::trace::mitem_t next_item_;
    /** Output buffer poor memory requests. */
    OutputBuffer_t out_;
    /** ReorderBuffer. */
    ROB_t rob_;

    /** Instruction commit rate. */
    int commit_rate_;
    sim::engine::cycles_t last_issue_cycle_;
    sim::engine::cycles_t last_cycle_;
    /** Stores wether the instruction commit is stalled. */
    bool commit_stalled_;
    /** Stores wether the instruction issue is stalled. */
    bool issue_stalled_;

    /** Queue with the cycle load instructions have been issued. */
    std::queue<engine::cycles_t> cycle_load_req_;
    /** Queue with the cycle load instructions Ack have been received. */
    std::queue<engine::cycles_t> cycle_load_resp_;
    /** Queue with the cycle store instructions have been issued. */
    std::queue<engine::cycles_t> cycle_store_req_;
    /** Queue with the cycle store instructions Ack have been received. */
    std::queue<engine::cycles_t> cycle_store_resp_;

    /** CPU index. */
    unsigned id_;

    /** Reference to the simulator. */
    sim::engine::Simulator &sim_;

    // mitem_t buffer in simulator runtime
    mem_buffer_t **mem_in_;

    /** TaskPoint info: Counter for committed instructions in current phase. */
    uint64_t ins_current_phase_;

    /** Holds the CPU/ROA statistics. */
    ROAStats stats_;

    /** Work Descriptor of the current task being executed. */
    uint64_t current_wd_id_;
    unsigned past_committed_instructions_;

    /** Execute the instruction commit phsae. */
    void commit();
    /** Execute the instruction issue phase. */
    void issue();

    unsigned insert_partial_item();

    /** Returns the next cycle in which we can commit instructions. */
    sim::engine::cycles_t next_commit_cycle() const;
    /** Returns the next cycle in which we can issue instructions. */
    sim::engine::cycles_t next_issue_cycle() const;
    /** Returns the next cycle in which we can do something. */
    sim::engine::cycles_t compute_next_cycle();

    /** Updates the statistics. */
    void update_stats();
};

}  // namespace timing_models
}  // namespace cpu
}  // namespace sim

#endif  // CORE_MODULES_CPU_MODELS_ROA_H_
