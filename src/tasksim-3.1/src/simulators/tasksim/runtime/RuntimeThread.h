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

#ifndef SIMULATORS_TASKSIM_RUNTIME_RUNTIMETHREAD_H_
#define SIMULATORS_TASKSIM_RUNTIME_RUNTIMETHREAD_H_

#include "core/basethread_decl.hpp"

#include "SimBaseThread.h"

namespace sim {
namespace runtime {

class RuntimeThread {
public:
    typedef void* (*func_ptr_t)(void*);

    static unsigned MAIN_TASK_ID;

private:
    static const unsigned MASTER_THREAD_ID = 0;

    /** Thread ID */
    unsigned int _id;

    nanos::ext::SimBaseThread * _thread;

    /** Thread is currently running */
    bool            _started;

    intptr_t*       _masterState;
    bool            _masterStateSaved;

    static void initMaster(void * rtThread);
    static void finiMaster(void);

    void initMasterContext();

public:
    RuntimeThread(): _id(-1), _thread(NULL), _started(false), _masterState(0), _masterStateSaved(false) { }

    void startOfCycle();

    /** Associate SimSmpThread with this RuntimeThread and take over its ID */
    void setRuntimeThread(void * th);
    /** Read accessor for SimSmpThread */
    nanos::BaseThread* getThread() { return _thread; }
    /** Mark thread as started */
    void start() { _started = true; }
    /** Read accessor for thread ID */
    unsigned int getId() { return _id; }

    /** TODO(XXX): documentation */
    static void saveRTState(void * ctx, void * arg, intptr_t* state);
    /** TODO(XXX): documentation */
    static void saveSimState(void * ctx, void * arg, intptr_t* state);
    /** TODO(XXX): documentation */
    static void doNothing(void * oldArg, void * newArg, intptr_t* state);
    /** TODO(XXX):: documentation */
    void restoreSimState();
    /** TODO(XXX): documentation */
    void restoreRTState();
};

}  // namespace runtime
}  // namespace sim

#endif  // SIMULATORS_TASKSIM_RUNTIME_RUNTIMETHREAD_H_
