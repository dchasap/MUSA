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

#include "RuntimeThread.h"

#include <assert.h>

#include "smp_ult.hpp"
#include "schedule.hpp"
#include "simulators/tasksim/runtime/smp/SimSmpThread.h"
#include "simulators/tasksim/runtime/NanoxBridge.h"

#include "core/utils/Log.h"

namespace sim {
namespace runtime {

using sim::utils::Log;

unsigned RuntimeThread::MAIN_TASK_ID = 0;

void
RuntimeThread::startOfCycle()
{
    if (_started) {
        assert(_thread != NULL);
        restoreRTState();
    } else if (_id == MASTER_THREAD_ID) {
        initMasterContext();
        start();
    }
}

void
RuntimeThread::setRuntimeThread(void * th)
{
    assert(th != NULL);
    _thread = static_cast<nanos::ext::SimBaseThread*>(th);
    assert(_thread != NULL);
    _id = _thread->getId();
}

void
RuntimeThread::initMasterContext()
{
    // TG std::cout << "initMasterContext()" << std::endl;
    // TODO(XXX): getStackSize() should be moved to BaseThread
    int stackSize = nanos::ext::SimSmpThread::getStackSize();
    void* stack = new intptr_t[stackSize];
    void* state = ::initContext(stack, stackSize, (void(*)(nanos::WD&)) &initMaster,
            reinterpret_cast<nanos::WD*>(this), reinterpret_cast<void*>(&finiMaster), NULL);
    ::switchStacks(this,
                   0,
                   state,
                   reinterpret_cast<void*>(&saveSimState));
    // This should never be reached, because we never switch
    // back to initMasterContext()
}

void
RuntimeThread::initMaster(void * rtThread)
{
    // TODO(XXX): Need to add an assertion to ensure that we are calling from SMP thread.
    RuntimeThread* rtth = reinterpret_cast<RuntimeThread*>(rtThread);
    nanos::myThread = rtth->getThread();

    // This is executed until end of simulation. We only return once for each task
    ((nanos::ext::SimSmpThread*)rtth->getThread())->work(MAIN_TASK_ID);
}

void
RuntimeThread::finiMaster(void)
{
    // TODO(XXX): Need to add an assertion to ensure that we are calling from SMP thread.
    sim::runtime::bridge.fini();

    // The execution of this function moved to CPU[0] if it started in a different one
    // Thus, all other threads were joined and now the execution is ensured to be in thread 0
    const unsigned thread_id = 0;
    RuntimeThread* rtThread = sim::runtime::bridge.getRuntimeThread(thread_id);

    sim::runtime::bridge.getThreadState((reinterpret_cast<RuntimeThread*>(rtThread))->getId())->setToHalted();

    rtThread->restoreSimState();
    assert(0);  // Must not return from yield after halting the CPU
}

void
RuntimeThread::saveRTState(void * ctx, void * arg, intptr_t* state) {
    RuntimeThread* rtth = reinterpret_cast<RuntimeThread*>(ctx);
    rtth->_thread->_state = state;
}

void
RuntimeThread::saveSimState(void * ctx, void * arg, intptr_t* state) {
    assert(ctx != NULL);
    assert(state != NULL);
    RuntimeThread* rtth = reinterpret_cast<RuntimeThread*>(ctx);
    if (rtth->_started) {
        rtth->_thread->_simState = state;
    } else {
        rtth->_masterState = state;
        rtth->_masterStateSaved = true;
    }
}

void
RuntimeThread::doNothing(void * oldArg, void * newArg, intptr_t* state)
{
}

void
RuntimeThread::restoreSimState()
{
    assert(nanos::myThread != NULL);
    assert(_thread != NULL);

    _thread->_myThread = nanos::myThread;
    void* state2restore;

    if (_masterStateSaved) {
       state2restore = _masterState;
       _masterStateSaved = false;
    } else {
        assert(_thread->_simState != NULL);
        state2restore = _thread->_simState;
    }

    ::switchStacks(this,
                   0,
                   state2restore,
                   reinterpret_cast<void*>(saveRTState));

    assert(_thread->_myThread != NULL);
    nanos::myThread = _thread->_myThread;
}

void
RuntimeThread::restoreRTState()
{
    Log::debug2() << "RuntimeThread::restoreRTState(): tid=" << _id << ' '
                  << _thread->_myThread << ' ' << _thread->getId();
    assert(_thread->_state != NULL);
    ::switchStacks(this,
                   0,
                   _thread->_state,
                   reinterpret_cast<void*>(&saveSimState));
}

}  // namespace runtime
}  // namespace sim
