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

#include "OmpSs.h"

#include <functional>
#include <string>
#include <vector>

#include "core/engine/Simulator.h"
#include "core/engine/Config.h"
#include "core/logic/coherence/single/Controller.h"
#include "core/stats/TimeStatistics.h"
#include "core/trace/ts/nanos.h"
#include "core/trace/ompss/types.h"
#include "core/utils/Log.h"
#include "simulators/tasksim/runtime/runtimeInitDefs.h"
#include "simulators/tasksim/runtime/ThreadState.h"
#include "simulators/tasksim/runtime/NanoxBridge.h"
#include "simulators/tasksim/runtime/smp/SimSmpThread.h"

namespace sim {
namespace runtime {

OmpSs::thread_buffer_t::thread_buffer_t(OmpSs &provider,
                                  void (OmpSs::*burst_provider)(burst_buffer_t &),
                                  void (OmpSs::*mem_provider)(mem_buffer_t &),
                                  std::size_t burst_buffer_size,
                                  std::size_t mem_buffer_size, unsigned id) :
    burst_(std::bind(burst_provider, &provider, std::placeholders::_1), burst_buffer_size, id),
    mem_(std::bind(mem_provider, &provider, std::placeholders::_1), mem_buffer_size, id)
{
}


OmpSs::OmpSs(engine::Simulator &sim, const sim::engine::Config &config,
             const std::string &trace_file,
             unsigned num_threads, unsigned master_id) :
        input_trace_(trace_file.c_str()),
        mem_parser_(input_trace_, config),
        sim_(sim),
        num_threads_(num_threads),
        memory_subtraces_(num_threads, NULL),
        parallel_section_(false),
        measure_(runtime::Measure::FULL_APPLICATION),
        master_speedup_ratio_(1.0),
        // first_task_id_(first_task_id)
        forward_task_start_(config.get_value_with_default<bool>("forward_task_start_to_cpu", false)),
        forward_task_end_(config.get_value_with_default<bool>("forward_task_end_to_cpu", false)),
        current_task_deps_(num_threads),
        finished_tasks_(0),
        copy_deps_(config.get_value_with_default<std::string>("copy_deps", "no") == "yes"),
        tracing_freq_mhz_(sim.get_config().get_value<unsigned>("tracing_cpu_freq"))
{
    // Initiate the mode_selector.
    sim::runtime::Policy mode_selector_policy = sim::runtime::get_policy(
            config.get_value<std::string>("mode_selector") );
    mode_selector_ = std::unique_ptr<ModeSelector>(new ModeSelector(sim,
            sim::engine::Config(config, "ModeSelector"),
            mode_selector_policy, num_threads));

    // Measure
    measure_ = sim::runtime::get_measure(config.get_value<std::string>("measure"));
    // Master speedup from configuration file:
    master_speedup_ratio_ = config.get_value<float>("master_speedup_ratio");
    // Instantiate CPU states
    createThreadStates(num_threads_, config);
    // Set pointer to simulator runtime
    bridge.setOmpSs(this);
    // Initialize Threads in NANOX bridge
    bridge.initThreads(&thread_states_);
    // Initialize NANOX bridge
    bridge.init(config);
    // Set the task where to start simulation
    bridge.setMainTask(master_id);

    // Add function names to the instrumentation:
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_TRACE_READ);
    for (unsigned i = 0; i < input_trace_.get_number_of_task_names(); i++) {
        sim_.get_instrumentation().add_function_name(input_trace_.get_task_name(i));
    }
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_TRACE_READ);
}

OmpSs::~OmpSs()
{
    // This is mantained separate so it can be called in case of SIGABORT.
    print_finish_message();

    for (thread_buffer_t *thread_buffer : thread_buffers_) {
        delete thread_buffer;
    }
    for (ThreadState *thread_state : thread_states_) {
        delete thread_state;
    }
}

void OmpSs::fill_burst_buffer(burst_buffer_t &buffer)
{
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);
    unsigned thread_id = buffer.get_id();

    // Inform ModeSelector that a burst just ended simulation
    PhaseSignature signature(0, 0, thread_id, 0, 0, 0, 0);
    mode_selector_->end_burst_phase(signature);
    get_memory_module(thread_id)->set_current_wd_id(signature.task_id_);

    assert(thread_id < thread_buffers_.size());
    assert(thread_buffers_[thread_id] != NULL);

    my_thread_id_ = thread_id;
    thread_states_[thread_id]->start_of_cycle();
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
}

void OmpSs::forward_runtime_info_to_cpu(std::size_t buffer_id, unsigned task_id, unsigned int task_name_id)
{
    if (not forward_task_start_) {
        return;
    }
    mem_buffer_t *buffer = &thread_buffers_[buffer_id]->mem_;
    sim::trace::ts::create_task_t t_id;
    t_id.wd_id = task_id;

    bool success = false;
    _unused(success);

    trace::mitem_t mitem_task_type;
    mitem_task_type.addr_ = engine::special_address_t::TASK_ID;
    mitem_task_type.size_ = 8;
    mitem_task_type.data_ = task_name_id;
    mitem_task_type.op_ = trace::mitem_t::STORE;
    mitem_task_type.ins_ = 1;
    mitem_task_type.flags_ = 0;

    success = buffer->push_event(mitem_task_type);
    assert(success == true);

    const std::vector<trace::ts::dep_t> *deps = NULL;
    if (t_id.wd_id != 1) {
        sim::stats::Time_Stats.phaseIn(sim::engine::REGION_TRACE_READ);
        deps = &input_trace_.get_task_dependencies(t_id);
        sim::stats::Time_Stats.phaseOut(sim::engine::REGION_TRACE_READ);
    }

    current_task_deps_[buffer_id].clear();
    if (deps) {
        for (size_t i = 0; i < deps->size(); i++) {
            /* total size of current dependency */
            /* VD: I assume that array is continuous, even if it is multidimensional */
            /* TODO: revisit this part to support non-continuous arrays*/
            size_t total_size = 1;

            for (size_t j = 0; j < (*deps)[i].dimensions.size(); j++) {
                total_size *= (*deps)[i].dimensions[j].size;
            }

            /* Add dependency to the dependency list of the current task */
            current_task_deps_[buffer_id].current_.start_addr_ = (trace::addr_t)(*deps)[i].address +
                                                              (trace::addr_t)(*deps)[i].offset;
            current_task_deps_[buffer_id].current_.end_addr_ = current_task_deps_[buffer_id].current_.start_addr_ +
                                                            total_size - 1;
            current_task_deps_[buffer_id].current_.concurrent_ = (*deps)[i].flags.concurrent;

            if ((*deps)[i].flags.input) {
                if ((*deps)[i].flags.output) { /* in_out */
                    current_task_deps_[buffer_id].current_.type_ = runtime::task_dep_t::INOUT;
                } else { /* in */
                    current_task_deps_[buffer_id].current_.type_ = runtime::task_dep_t::IN;
                }
            } else {
                if ((*deps)[i].flags.output) { /* out */
                    current_task_deps_[buffer_id].current_.type_ = runtime::task_dep_t::OUT;
                } else { /* nothing */
                    assert(0);
                }
            }

            current_task_deps_[buffer_id].insert();
        }
    }

    current_task_deps_[buffer_id].current_.task_id_ = task_name_id;
}

void OmpSs::insert_task_end_instruction(mem_buffer_t & buffer, unsigned int name_id)
{
    if (forward_task_end_) {
        trace::mitem_t mitem_end;
        mitem_end.addr_ = engine::special_address_t::TASK_END;
        mitem_end.size_ = 1;
        mitem_end.data_ = name_id;
        mitem_end.op_ = trace::mitem_t::STORE;
        mitem_end.ins_ = 1;
        mitem_end.flags_ = 0;
//        std::cout << "insert task end instruction" << std::endl;
        bool success = buffer.push_event(mitem_end);
        assert(success == true);
        if (not success) abort();
        _unused(success);
    }
}

void OmpSs::fill_mem_buffer(mem_buffer_t &buffer)
{
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);
    unsigned buffer_id = buffer.get_id();
    if (memory_subtraces_[buffer_id] == NULL) {
        sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
        return;
    }

    if (get_drain_state(buffer) == engine::drain_state_t::RUNNING) {
        sim::stats::Time_Stats.phaseIn(sim::engine::REGION_TRACE_READ);
        bool last = false;
        while (buffer.space_left() > 1) {
            trace::mitem_t mitem = mem_parser_.get_next_mem_item(*memory_subtraces_[buffer_id]);
            // void* stack_ptr = memory_subtraces_[buffer_id]->stack;
            mitem.flags_ = sim::runtime::TaskDepConv::pack(current_task_deps_[buffer_id].get_task_dep_type(mitem.addr_));
            mitem.data_ = memory_subtraces_[buffer_id]->name_id;
            // mitem.data_ = thread_buffers_[buffer_id]->current_phase_.burst_id;

            buffer.push_event(mitem);
            if (mitem.op_ == trace::mitem_t::op_t::INVALID) {
                last = true;
                insert_task_end_instruction(buffer, memory_subtraces_[buffer_id]->name_id);
                break;
            }
        }
        sim::stats::Time_Stats.phaseOut(sim::engine::REGION_TRACE_READ);
        // If subtrace finished: process next phase
        if (last) {
            dynamic_cast<memory_cpu_t*>(buffer.get_remote_module())->drain();
            thread_buffers_[buffer_id]->current_phase_ = {/*burst_id*/(unsigned)-1, sim::trace::ts::NO_SUBTRACE, /*duration*/ 0};
        }
    } else if (get_drain_state(buffer) ==
                engine::drain_state_t::DRAINING) {
        memory_cpu_t *mem_cpu = dynamic_cast<memory_cpu_t*>(buffer.get_remote_module());
        if (mem_cpu->drain()) {
            const unsigned int wd_id = memory_subtraces_[buffer_id]->wd_id;
            PhaseSignature signature(0,
                    memory_subtraces_[buffer_id]->name_id,
                    buffer_id,
                    input_trace_.get_task_trace_id_by_wd_id(wd_id),
                    mem_cpu->get_ins_current_phase(),
                    0,
                    wd_id);
            mode_selector_->end_detailed_phase(signature);
            mem_cpu->set_current_wd_id(0);
            get_burst_module(buffer_id)->drain_resume();
        }
    }
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
}

OmpSs::thread_buffer_t& OmpSs::operator[](unsigned thread_id)
{
    if (thread_buffers_.size() <= thread_id) {
        thread_buffers_.resize(thread_id + 1, NULL);
    }
    if (thread_buffers_[thread_id] == NULL) {
        thread_buffers_[thread_id] = new thread_buffer_t(*this,
                &OmpSs::fill_burst_buffer,
                &OmpSs::fill_mem_buffer,
                Burst_Buffer_Size_,
                Mem_Buffer_Size_,
                thread_id);
    }

    return *thread_buffers_[thread_id];
}

sim::runtime::OmpSs::memory_cpu_t*
OmpSs::get_memory_module(unsigned thread_id)
{
    assert(thread_id < thread_buffers_.size());
    assert(thread_buffers_[thread_id] != NULL);
    return dynamic_cast<memory_cpu_t*>(thread_buffers_[thread_id]->mem_.get_remote_module());
}

sim::runtime::OmpSs::burst_cpu_t*
OmpSs::get_burst_module(unsigned thread_id)
{
    assert(thread_id < thread_buffers_.size());
    assert(thread_buffers_[thread_id] != NULL);
    return dynamic_cast<burst_cpu_t*>(thread_buffers_[thread_id]->burst_.get_remote_module());
}

void
OmpSs::createThreadStates(unsigned num_threads, const sim::engine::Config &config)
{
    using sim::engine::cycles_t;
    cycles_t idle_cycles = config.get_value<cycles_t>("idle_cycles");
    cycles_t lock_cycles = config.get_value<cycles_t>("lock_cycles");
    cycles_t migration_cycles = config.get_value<cycles_t>("thread_migration_cycles");
    for (unsigned int i = 0; i < num_threads; ++i) {
        thread_states_.push_back(new ThreadState(sim_,
                *this,
                (*this)[i].burst_,
                idle_cycles,
                lock_cycles,
                migration_cycles));
    }
}

bool
OmpSs::check_if_buffers_are_ready(unsigned thread_id)
{
    sim::engine::drain_state_t burst_state = get_drain_state(thread_buffers_[thread_id]->burst_);
    sim::engine::drain_state_t mem_state = get_drain_state(thread_buffers_[thread_id]->mem_);

    if (burst_state == engine::drain_state_t::DRAINING ||
            mem_state == engine::drain_state_t::DRAINING) {
        return false;
    }
    if (burst_state == engine::drain_state_t::RESUMING &&
            mem_state == engine::drain_state_t::DRAINED) {
        get_burst_module(thread_id)->drain_resume();
        return false;
    }
    if (mem_state == engine::drain_state_t::RESUMING &&
            burst_state == engine::drain_state_t::DRAINED) {
        get_memory_module(thread_id)->drain_resume();
        return false;
    }
    return true;
}

bool
OmpSs::process_next_event(unsigned task_id, unsigned thread_id)
{
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);
    // If at least one CPU model is draining, do nothing
    if (check_if_buffers_are_ready(thread_id) == false) {
        sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
        return true;
    }

    /**
     * This loop keeps processing events until a phase is inserted in
     * the corresponding event buffer.
     * If we have a synchronization or scheduling event, we process it,
     * and continue until we get a phase to insert in the buffer.
     */
    bool event_inserted = false;
    do {
        trace::ts::event_t event;
        sim::stats::Time_Stats.phaseIn(sim::engine::REGION_TRACE_READ);
        bool ret = input_trace_.get_task_event(task_id, event);
        sim::stats::Time_Stats.phaseOut(sim::engine::REGION_TRACE_READ);
        sim::utils::Log::info() << "Processing event " << event << " for task " << task_id << " at thread " << thread_id;
        if (ret == false) {
            /** Reached end of task trace */
            finish_task(task_id, thread_id);
            sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
            return false;
        }
        switch (event.type) {
        case trace::ts::PHASE: {
            process_phase(task_id, thread_id, event);
            event_inserted = true;
            break;
        }
        case trace::ts::CREATE_TASK: {
            event_inserted = process_create_task(task_id, thread_id, event);
            break;
        }
        case trace::ts::TASK_WAIT: {
            event_inserted = process_task_wait(task_id, thread_id, event);
            break;
        }
        case trace::ts::WAIT_ON: {
            event_inserted = process_wait_on(task_id, thread_id, event);
            break;
        }
        case trace::ts::SET_LOCK: {
            process_set_lock(task_id, thread_id, event);
            break;
        }
        case trace::ts::UNSET_LOCK: {
            process_unset_lock(task_id, thread_id, event);
            break;
        }
        default: {
            std::cerr << "[ERROR] " << sim_.get_clock()
                      << " Read unsupported event from trace: " << event.type << std::endl;
            exit(-1);
        }
        }
    } while (not event_inserted);
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
    return true;
}

void
OmpSs::process_phase(unsigned task_id, unsigned thread_id, trace::ts::event_t &event)
{
    /** Check if we have the subtrace_stream */
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_TRACE_READ);
    const sim::trace::ts::subtrace_t &subtrace = input_trace_.get_task_trace_by_wd_id(task_id);
    const int subtrace_id = input_trace_.get_task_trace_id_by_wd_id(task_id);
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_TRACE_READ);

    unsigned name_id = subtrace.name_id;

    if (measure_ == sim::runtime::Measure::PARALLEL_SECTIONS and (
            (parallel_section_ == false and event.u.phase.burst_id == sim::trace::ompss::phase_id_t::WD_ID_PHASE) or
            (parallel_section_ == false and event.u.phase.burst_id == sim::trace::ompss::phase_id_t::USER_CODE_PHASE) or
            (event.u.phase.subtrace_id == sim::trace::ts::subtrace_id_t::EMPTY_SUBTRACE and
             event.u.phase.burst_id == sim::trace::ompss::USER_CODE_PHASE))) {
        event.u.phase.duration = 1;
    } else if (name_id == 0 && master_speedup_ratio_ < 1e-12) {
        event.u.phase.duration = 1;
    } else if (name_id == 0) {
        event.u.phase.duration /= master_speedup_ratio_;
    }

    // We do not need to simulate Trace wait_completion event durations.
    if (event.u.phase.burst_id == 2) {
        event.u.phase.duration = std::min(event.u.phase.duration, uint64_t(100));
    }

    assert(event.u.phase.subtrace_id < 0 or event.u.phase.subtrace_id == subtrace_id);

    thread_buffers_[thread_id]->current_phase_ = event.u.phase;
    if (task_id == 1 and event.u.phase.burst_id == sim::trace::ompss::USER_CODE_PHASE) {
        thread_buffers_[thread_id]->current_phase_.burst_id = 0;
    }

    PhaseSignature signature(event.u.phase.duration, name_id, thread_id, subtrace_id,
            0, 0, task_id);

    // We add the instrumentation event for the Phase ID
    sim_.get_instrumentation().add_event(
            sim::utils::instrumentation::InstrumentationItem {
            get_burst_module(thread_id)->get_instrumentation_id(),
            0,
            sim_.get_clock(),
            sim::utils::instrumentation::EVENT_RECORD,
            sim::utils::instrumentation::Event{PARAVER_EVENT_TASKSIM, event.u.phase.burst_id}});

    // If the phase has an asociated subtrace, we get the number of instructions.
    if (event.u.phase.subtrace_id != sim::trace::ts::NO_SUBTRACE) {
        const sim::trace::user_code_data_t &user_data = input_trace_.get_user_code_data(task_id);
        const uint64_t phase_instr = user_data.data[phase_count_per_wd_[task_id]++].n_instrs;
        signature.inst_count_ = phase_instr;
    }
    SimulationMode mode = mode_selector_->get_mode(signature);
    // We recheck subtrace_id in case MEMORY always replies MEMORY
    if (mode == BURST or event.u.phase.subtrace_id < 0) {
        if (mode != BURST and event.u.phase.subtrace_id == sim::trace::ts::EMPTY_SUBTRACE) {
            // We need to flush the bbl/mem buffer of one zero.
            sim::trace::mitem_t mitem = mem_parser_.get_next_mem_item(subtrace);
            assert(mitem.addr_ == 0 and mitem.op_ == sim::trace::mitem_t::op_t::INVALID);
            _unused(mitem);
        }

        engine::cycles_t predicted_cycles =  mode_selector_->predict_cycle_count(signature);
        assert(predicted_cycles != 0);
        if (predicted_cycles != engine::NEVER) {
            // We convert to ns so the Burst CPU can convert them back...
            event.u.phase.duration = predicted_cycles * 1000 / tracing_freq_mhz_;
        }
        bool ok = thread_buffers_[thread_id]->burst_.push_event(event.u.phase);
        assert(ok == true);
        _unused(ok);
    } else if (mode == MEMORY) {
        forward_runtime_info_to_cpu(thread_id, task_id, name_id);
        if (get_burst_module(thread_id)->drain()) {
            get_memory_module(thread_id)->drain_resume();
        }
        // on new detailed phase, tell the mem-aware cpu module about the stack pointer (if it changes)
        void* task_stack_ptr = input_trace_.get_task_trace_by_id(subtrace_id).stack;
        if (memory_subtraces_[thread_id] == NULL or memory_subtraces_[thread_id]->stack != task_stack_ptr) {
            get_memory_module(thread_id)->context_switch(reinterpret_cast<engine::addr_t>(task_stack_ptr));
        }
        sim::stats::Time_Stats.phaseIn(sim::engine::REGION_TRACE_READ);
        memory_subtraces_[thread_id] = &input_trace_.get_task_trace_by_id(subtrace_id);
        sim::stats::Time_Stats.phaseOut(sim::engine::REGION_TRACE_READ);

        fill_mem_buffer(thread_buffers_[thread_id]->mem_);
    }
    // We tell the Memory CPU which Task it is executing.
    get_memory_module(thread_id)->set_current_wd_id(signature.task_id_);
}

bool
OmpSs::process_create_task(unsigned task_id, unsigned &thread_id, const trace::ts::event_t &event)
{
    // Whenever we create a task we "enter parallel mode"
    parallel_section_ = true;

    // The runtime might swith us to another task, we add the instrumentation events:
    instrumentation_switch_task(0, thread_id);

    unsigned created_wd_id = event.u.create_task.wd_id;

    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_TRACE_READ);
    const std::vector<trace::ts::dep_t> &deps = input_trace_.get_task_dependencies({created_wd_id});
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_TRACE_READ);

    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
    bridge.createTask(created_wd_id, deps, copy_deps_);
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);

    /** Restore the current thread_id in case the task moved to
      * a different thread while on create task */
    bool ret = check_thread_migration(thread_id);
    instrumentation_switch_task(task_id, thread_id);
    return ret;
}

bool
OmpSs::process_task_wait(unsigned task_id, unsigned &thread_id, const trace::ts::event_t &event)
{
    // The runtime might swith us to another task, we add the instrumentation events:
    instrumentation_switch_task(0, thread_id);

    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
    bridge.waitForTaskGroup(1);
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);

    /** Restore the current thread_id in case the task moved to
      * a different thread while on a task_wait */
    bool ret = check_thread_migration(thread_id);
    instrumentation_switch_task(task_id, thread_id);
    // Whenever we exit a task_wait is because all threads have finished and
    // we return to task #1 for more task creation / ending
    parallel_section_ = false;
    return ret;
}


bool
OmpSs::process_wait_on(unsigned task_id, unsigned &thread_id, const trace::ts::event_t &event)
{
    // The runtime might swith us to another task, we add the instrumentation events:
    instrumentation_switch_task(0, thread_id);

    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_TRACE_READ);
    const sim::trace::ts::subtrace_t &subtrace = input_trace_.get_task_trace_by_wd_id(task_id);
    std::vector<trace::ts::dep_t> deps;
    if (not input_trace_.get_wait_on_dependencies(subtrace, deps)) {
        std::cerr << "[ERROR] Wait_on with id = " << subtrace.wait_trace_id << std::endl;
        std::cerr << "        Cannot find dependencies for wait on event on stream id "
                  << event.u.wait_on.subtrace_id << std::endl;
    }
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_TRACE_READ);
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
    bridge.waitOn(deps);
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);
    /** Restore the current thread id in case the task moved to
      * a different thread while on wait on */
    bool ret = check_thread_migration(thread_id);
    instrumentation_switch_task(task_id, thread_id);
    return ret;
}

void
OmpSs::process_set_lock(unsigned task_id, unsigned thread_id, const trace::ts::event_t &event)
{
    unsigned address = event.u.lock.address;
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
    dynamic_cast<nanos::ext::SimSmpThread*>(
            thread_states_[thread_id]->getThread()->getThread())->set_lock(locks, address);
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);
}

void
OmpSs::process_unset_lock(unsigned task_id, unsigned thread_id, const trace::ts::event_t &event)
{
    unsigned address = event.u.lock.address;
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
    dynamic_cast<nanos::ext::SimSmpThread*>(
            thread_states_[thread_id]->getThread()->getThread())->unset_lock(locks, address);
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);
}



void
OmpSs::finish_task(unsigned task_id, unsigned thread_id)
{
    finished_tasks_++;

    // If main task finished: drain both models
    if (task_id == 1) {
        if (get_burst_module(thread_id) != NULL) {
            get_burst_module(thread_id)->drain();
        }
        if (get_memory_module(thread_id) != NULL) {
            get_memory_module(thread_id)->drain();
        }
    }
}

void OmpSs::task_start(unsigned task_id, unsigned thread_id)
{
    unsigned task_type = input_trace_.get_task_trace_by_wd_id(task_id).name_id;
    mode_selector_->task_start(task_id, thread_id, task_type);
    // We print the hardware counters of the associated modules (MemCPU + propagate).
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_INSTRUMENTATION_TRACE);
    get_burst_module(thread_id)->print_hardware_counters(sim_.get_clock());
    if (get_memory_module(thread_id) != NULL) {
        get_memory_module(thread_id)->print_hardware_counters(sim_.get_clock(), true);
    }
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_INSTRUMENTATION_TRACE);
    instrumentation_switch_task(task_id, thread_id);
}

void OmpSs::task_end(unsigned task_id, unsigned thread_id)
{
    phase_count_per_wd_.erase(task_id);
    unsigned task_type = input_trace_.get_task_trace_by_wd_id(task_id).name_id;
    mode_selector_->task_end(task_id, thread_id, task_type);
    // We print the hardware counters of the associated modules (MemCPU + propagate).
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_INSTRUMENTATION_TRACE);
    if (get_memory_module(thread_id) != NULL) {
        get_memory_module(thread_id)->print_hardware_counters(sim_.get_clock(), true);
    }
    burst_cpu_t *burst_cpu = get_burst_module(thread_id);
    burst_cpu->print_hardware_counters(sim_.get_clock());
    // We add the instrumentation event for the Phase ID
    sim_.get_instrumentation().add_event(
            sim::utils::instrumentation::InstrumentationItem {
            burst_cpu->get_instrumentation_id(),
            0,
            sim_.get_clock(),
            sim::utils::instrumentation::EVENT_RECORD,
            sim::utils::instrumentation::Event{ PARAVER_EVENT_TASKSIM, 0}});

    instrumentation_switch_task(0, thread_id);
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_INSTRUMENTATION_TRACE);
    // We add a runtime delay to simulate the time needed to get a new task.
    // sim::stats::Time_Stats.phaseOut(sim::engine::REGION_NANOS_RUNTIME);
    dynamic_cast<nanos::ext::SimSmpThread*>(
            thread_states_[thread_id]->getThread()->getThread())->schedule_delay();
    // sim::stats::Time_Stats.phaseIn(sim::engine::REGION_NANOS_RUNTIME);
}

sim::engine::cycles_t OmpSs::get_simulation_clock()
{
    return sim_.get_clock();
}

sim::engine::drain_state_t OmpSs::get_drain_state(mem_buffer_t &buffer)
{
    if (buffer.get_remote_module() != NULL) {
        return dynamic_cast<memory_cpu_t*>(buffer.get_remote_module())->get_drain_state();
    } else {
        return engine::drain_state_t::DRAINED;
    }
}

sim::engine::drain_state_t OmpSs::get_drain_state(burst_buffer_t &buffer)
{
    if (buffer.get_remote_module() != NULL) {
        return dynamic_cast<burst_cpu_t*>(buffer.get_remote_module())->get_drain_state();
    } else {
        return engine::drain_state_t::DRAINED;
    }
}

bool OmpSs::check_thread_migration(unsigned &thread_id)
{
    unsigned int new_thread_id = nanos::myThread->getId();
    if (thread_id != new_thread_id) {
        thread_id = new_thread_id;
        thread_states_[thread_id]->thread_migration_delay();
        return true;
    } else {
        return false;
    }
}

void OmpSs::print_finish_message() const
{
    if (finished_tasks_ != input_trace_.get_total_number_of_tasks()) {
        std::cout << "========================= WARNING ========================" << std::endl;
        std::cout << "Finished " << finished_tasks_ << " of "
                  << input_trace_.get_total_number_of_tasks() << " tasks in trace." << std::endl;
        std::cout << "========================= WARNING ========================" << std::endl;
    }
}

inline
void OmpSs::instrumentation_switch_task(unsigned task_id, unsigned thread_id)
{
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_INSTRUMENTATION_TRACE);
    uint64_t name_id = sim::utils::instrumentation::FUNCTION_NAME::END;
    if (task_id != 0) {
        name_id = sim::utils::instrumentation::FUNCTION_NAME::RESERVED +
                  input_trace_.get_task_trace_by_wd_id(task_id).name_id;
    }
    sim_.get_instrumentation().add_event(
            sim::utils::instrumentation::InstrumentationItem {
            get_burst_module(thread_id)->get_instrumentation_id(),
            0,
            sim_.get_clock(),
            sim::utils::instrumentation::EVENT_RECORD,
            sim::utils::instrumentation::Event{
            sim::utils::instrumentation::FUNCTION_NAME::TYPE, name_id}});
    sim_.get_instrumentation().add_event(
            sim::utils::instrumentation::InstrumentationItem {
            get_burst_module(thread_id)->get_instrumentation_id(),
            0,
            sim_.get_clock(),
            sim::utils::instrumentation::EVENT_RECORD,
            sim::utils::instrumentation::Event{ PARAVER_INFO_WD_ID, task_id}});
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_INSTRUMENTATION_TRACE);
}

}  // namespace runtime
}  // namespace sim
