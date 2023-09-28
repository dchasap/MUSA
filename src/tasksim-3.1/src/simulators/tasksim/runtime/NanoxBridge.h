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


#ifndef SIMULATORS_TASKSIM_RUNTIME_NANOXBRIDGE_H_
#define SIMULATORS_TASKSIM_RUNTIME_NANOXBRIDGE_H_

#include <cassert>
#include <vector>
#include "core/basethread_decl.hpp"

#include "core/engine/Config.h"
#include "core/trace/ts/Trace.h"
#include "simulators/tasksim/runtime/smp/SimSmpPlugin.h"
#include "simulators/tasksim/runtime/ThreadState.h"
#include "simulators/tasksim/runtime/RuntimeThread.h"

namespace sim {
namespace runtime {

class NanoxInstrumentation;
class OmpSs;

class NanoxBridge {
private:
    // Disable copy constructor and = operator
    NanoxBridge(const NanoxBridge &runtime) { }
    const NanoxBridge & operator= (const NanoxBridge &runtime);

    /**
     * Pointers to CPUStates. The actual CPUStates are maintained in
     * OmpSs.
     */
    std::vector<sim::runtime::ThreadState*> *thread_states_;

    /**
     * The runtime threads. These give NANOX the illusion of running on a
     * multithreaded system.
     * */
    std::vector<sim::runtime::RuntimeThread*> runtime_threads_;

    /** Pointer to simulator runtime */
    OmpSs* sim_runtime_;

    ::nanos::ext::SimSmpPlugin nanos_plugin_;

    /** True after init() has been called */
    bool _init;

    /** True after fini() has been called */
    bool _fini;

public:
    NanoxBridge(): thread_states_(NULL), nanos_plugin_(),  _init(false), _fini(false) {}
    virtual ~NanoxBridge() { }

    /** Initialize NANOX bridge */
    virtual void init(const sim::engine::Config& config);
    /** Finalize NANOX bridge */
    virtual void fini();

    /** Set pointer to simulator runtime */
    void setOmpSs(sim::runtime::OmpSs* sim_runtime);
    /** Get pointer to simulator runtime */
    sim::runtime::OmpSs* getSimRuntime();

    // TODO(TG):
    /** Create and submit new task */
    /** Creates task througth NanoxAPI */
    virtual void createTask(unsigned int task_id, const std::vector<trace::ts::dep_t> &deps, bool copy_deps);
    /** Invokes nanos__wg_wait_completion througth NanoxAPI */
    virtual void waitForTaskGroup(unsigned int task_group);
    /** Invokes nanos_wait_on througth NanoxAPI */
    virtual void waitOn(const std::vector<trace::ts::dep_t> &deps);

    /** Initializes thread_states_ and runtime_threads with the suplied thread_states */
    void initThreads(std::vector<sim::runtime::ThreadState*> *thread_states);

    /** Returns the i-th ThreadState */
    sim::runtime::ThreadState* getThreadState(unsigned int i) const;
    /** Invokes start() on the i-th RuntimeThread */
    void startRuntimeThread(int i);
    /** Sets the i-th RuntimeThread to tsth */
    void setRuntimeThread(unsigned int i, nanos::BaseThread* tsth);
    /** Returns the i-th RuntimeThread */
    RuntimeThread* getRuntimeThread(unsigned int i);
    // Sets the main task id at RuntimeThread::MAIN_TASK_ID
    void setMainTask(unsigned id);
};

extern NanoxBridge bridge;

}  // namespace runtime
}  // namespace sim

#endif  // SIMULATORS_TASKSIM_RUNTIME_NANOXBRIDGE_H_
