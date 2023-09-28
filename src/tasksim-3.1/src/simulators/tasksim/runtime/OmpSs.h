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


// Authors: Thomas Grass (thomas.grass@bsc.es)
//          Alejandro Rico (alejandro.rico@bsc.es)

#ifndef SIMULATORS_TASKSIM_RUNTIME_OMPSS_H_
#define SIMULATORS_TASKSIM_RUNTIME_OMPSS_H_

#include <memory>
#include <string>
#include <vector>

#include "core/engine/EventBuffer.h"
#include "core/logic/coherence/single/types.h"
#include "core/engine/DModule.h"
#include "core/engine/types.h"
#include "core/modules/cpu/Memory.h"
#include "core/trace/types.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/ts/types.h"
#include "core/trace/MemParser.h"
#include "simulators/tasksim/runtime/ModeSelector.h"


namespace sim {

namespace engine {
class Simulator;
class Module;
}  // namespace engine

namespace runtime {

class ThreadState;

class OmpSs {
public:
    typedef sim::cpu::Memory<sim::cpu::timing_models::ROA,
                        sim::logic::coherence::single::Message> memory_cpu_t;
    typedef sim::engine::DModule burst_cpu_t;
    typedef sim::engine::EventBuffer<sim::trace::ts::phase_t> burst_buffer_t;
    typedef sim::engine::EventBuffer<sim::trace::mitem_t> mem_buffer_t;

private:
    /**
     * This structure represents a buffer for phases ("bursts"), which are to be
     * processed by a BurstCpu. For each simulated BurstCpu there is a separate
     * buffer.
     */
    struct thread_buffer_t {
        burst_buffer_t burst_;
        mem_buffer_t mem_;

        trace::ts::phase_t current_phase_;

        thread_buffer_t(OmpSs &,
                void (OmpSs::*)(burst_buffer_t &),
                void (OmpSs::*)(mem_buffer_t &),
                std::size_t,
                std::size_t,
                unsigned);
    };

    sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> input_trace_;
    sim::trace::MemParser<sim::trace::ts::FileTraceReader<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal>> mem_parser_;

    sim::engine::Simulator &sim_;

    static const unsigned Burst_Buffer_Size_ = 1;
    static const unsigned Mem_Buffer_Size_ = 1024;
    /** Vector of buffers buffering bursts for each BurstCpu instance */
    std::vector<thread_buffer_t*> thread_buffers_;

    /** States of all threads in the system */
    std::vector<ThreadState*> thread_states_;

    /** Number of threads in the system */
    unsigned num_threads_;

    /** Returns the memory cpu module that corresponds to a given thread */
    memory_cpu_t* get_memory_module(unsigned thread_id);
    /** Returns the burst cpu module that corresponds to a given thread */
    burst_cpu_t* get_burst_module(unsigned thread_id);
    /** Checks if the buffers are in draining mode. */
    bool check_if_buffers_are_ready(unsigned thread_id);
    /** Performs checks after we process the last event of a task. */
    void finish_task(unsigned task_id, unsigned thread_id);
    void process_phase(unsigned task_id, unsigned thread_id, trace::ts::event_t &event);
    bool process_create_task(unsigned task_id, unsigned &thread_id, const trace::ts::event_t &event);
    bool process_task_wait(unsigned task_id, unsigned &thread_id, const trace::ts::event_t &event);
    bool process_wait_on(unsigned task_id, unsigned &thread_id, const trace::ts::event_t &event);
    void process_set_lock(unsigned task_id, unsigned thread_id, const trace::ts::event_t &event);
    void process_unset_lock(unsigned task_id, unsigned thread_id, const trace::ts::event_t &event);

    /** Generate the instrumentation events for a thread switching tasks. */
    void instrumentation_switch_task(unsigned thread_id, unsigned task_id);

    /** Variable to store the id the thread that is currently entering OmpSs.
     *  This is require to restore the correct thread id in processNextEvent
     *  when a task is resumed in a different thread than the one where it
     *  was suspended.
     */
    unsigned my_thread_id_;

    /** Lock map, it holds the id of the thread that has a lock on the address. */
    std::unordered_map<unsigned, unsigned> locks;

    /** Method to fill the burst buffer with events */
    void fill_burst_buffer(burst_buffer_t &);

    /** Method to insert instructions that write task in/out information into the cpu */
    void forward_runtime_info_to_cpu(std::size_t, unsigned, unsigned int);

    /** Method to insert instruction signaling the end of the task */
    void insert_task_end_instruction(mem_buffer_t &, unsigned int);

    /** Method to fill the mem buffer with events */
    void fill_mem_buffer(mem_buffer_t &);

    /** Create thread states of all threads in the system */
    void createThreadStates(unsigned num_threads, const sim::engine::Config &config);

    /** Current memory subtraces of all memory event buffers */
    std::vector<const trace::ts::subtrace_t*> memory_subtraces_;

    /** Reference tot the ModeSelector selected by the simulator. */
    std::unique_ptr<ModeSelector> mode_selector_;

    /* Getters for the drain state of the cpus associated with a buffer:*/
    engine::drain_state_t get_drain_state(mem_buffer_t &buffer);
    engine::drain_state_t get_drain_state(burst_buffer_t &buffer);

    /** Checks if a task has migrated to another thread.*/
    bool check_thread_migration(unsigned int &thread_id);

    /** Identifies if we are currently on parallel mode or only
     *  the master is creating tasks.
     */
    bool parallel_section_;

    /** Identifies if we want to measure all the application or
     *  only tye parallel sections:
     */
    sim::runtime::Measure  measure_;

    /** SpeedUp to be aplied to all events from the master task, */
    float master_speedup_ratio_;

    // unsigned first_task_id_;

    /* Flags telling whether to pass certain information from the Runtime to CPUs*/
    bool forward_task_start_;
    bool forward_task_end_;

    std::vector<sim::runtime::TaskDeps> current_task_deps_;

    // Finished tasks counter for safety checks.
    unsigned finished_tasks_;

    /** Wether to copy or not the Task dependencies. */
    bool copy_deps_;

    std::unordered_map<unsigned, unsigned> phase_count_per_wd_;

    unsigned tracing_freq_mhz_;

public:
    OmpSs(sim::engine::Simulator &, const sim::engine::Config &, const std::string &trace_file,
          unsigned num_threads, unsigned master_id = 1);
    ~OmpSs();

    /** Definition of [] operator. This is used to bind CPUs to output buffers */
    thread_buffer_t &operator[](unsigned n);

    /** TODO: documentation */
    bool process_next_event(unsigned task_id, unsigned thread_id);

    /** Function called by the SimSmpThread before starting a task, for debug and Stats purposes. */
    void task_start(unsigned task_id, unsigned thread_id);
    /** Function called by the SimSmpThread before after a task ends, for debug and Stats purposes. */
    void task_end(unsigned task_id, unsigned thread_id);
    /** Function to get the current simulation clock, useful for timing schedulers. */
    sim::engine::cycles_t get_simulation_clock();

    /** Prints number of finished tasks and the number of tasks present in the trace. */
    void print_finish_message() const;
};

}  // namespace runtime
}  // namespace sim

#endif  // SIMULATORS_TASKSIM_RUNTIME_OMPSS_H_
