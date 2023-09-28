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


/*
 * thread_state.h
 *
 *  Created on: Nov 13, 2012
 *      Author: tgrass
 */

#ifndef SIMULATORS_TASKSIM_RUNTIME_THREADSTATE_H_
#define SIMULATORS_TASKSIM_RUNTIME_THREADSTATE_H_

#include <nextsim/core/engine/Simulator.h>
#include <nextsim/core/trace/ts/types.h>
#include <nextsim/core/engine/types.h>
#include "RuntimeThread.h"
#include "OmpSs.h"

namespace sim {
namespace runtime {

/**
 * The state of a thread. The state encompasses information about
 * the current state of the thread (running, idle, ...)
 */

class ThreadState {
private:
    typedef enum { IDLE, RUNNING, HALTED } StateType;
    /** TODO documentation */
    engine::Simulator &sim_;

    /** Burst buffer associated to this thread */
    OmpSs::burst_buffer_t &buffer_;

    /** Thread(s) of this CPU */
    RuntimeThread thread_;

    /** Current state_ of this CPU */
    StateType state_;

    /** Number of cycles for an idle loop. */
    sim::engine::cycles_t idle_cycles_;

    /** Number of cycles for an active wait on a lock. */
    sim::engine::cycles_t lock_cycles_;

    /** Number of cycles for  thread migration delay. */
    sim::engine::cycles_t migration_cycles_;

public:
    /** Constructor */
    ThreadState(engine::Simulator& sim, OmpSs& sim_runtime , OmpSs::burst_buffer_t &buffer,
            sim::engine::cycles_t idle_cycles, sim::engine::cycles_t lock_cycles,
            sim::engine::cycles_t migration_cycles) :
        sim_(sim),
        buffer_(buffer),
        thread_(),
        state_(IDLE),
        idle_cycles_(idle_cycles),
        lock_cycles_(lock_cycles),
        migration_cycles_(migration_cycles)
    {}

    ~ThreadState() {}

    /** Mode switch to IDLE */
    void setToIdle() {
        state_ = IDLE;
        buffer_.push_event({0, sim::trace::ts::NO_SUBTRACE, idle_cycles_});  // Push an idle phase (burst_id=0)
    }
    /** Mode switch to ACTIVE_WAIT */
    void setToLockWait() {
        state_ = RUNNING;
        buffer_.push_event({9999, sim::trace::ts::NO_SUBTRACE, lock_cycles_});  // Push an active wait phase
    }
    /** Mode switch to RUNNING */
    void setToRunning() {
        state_ = RUNNING;
    }
    /** Mode switch to HALTED */
    void setToHalted() {
        state_ = HALTED;
        if (thread_.getId() == 0) {
            sim_.finish();
        }
    }
    /** Add a delay for migrating a task to a different thread */
    void thread_migration_delay() {
        state_ = IDLE;
        buffer_.push_event({9999, sim::trace::ts::NO_SUBTRACE, migration_cycles_});  // Push a task migration delay
    }
    /** Action taken at start of cycle. It switches to the user-level thread in RuntimeThread */
    void start_of_cycle() {
        if (state_ != HALTED) {
            thread_.startOfCycle();
        }
    }
    /** Get thread of this CPU */
    RuntimeThread *getThread() { return &thread_; }
};

}  // namespace runtime
}  // namespace sim

#endif  // SIMULATORS_TASKSIM_RUNTIME_THREADSTATE_H_
