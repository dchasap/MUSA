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

#include "SimSmpThread.h"

#include <sched.h>
#include <iostream>

#include "simulators/tasksim/runtime/smp/SimSmpProcessor.h"
#include "simulators/tasksim/runtime/smp/SimSmpDD.h"
#include "simulators/tasksim/runtime/NanoxBridge.h"
#include "simulators/tasksim/runtime/OmpSs.h"
#include "simulators/tasksim/runtime/RuntimeThread.h"
#include "core/stats/TimeStatistics.h"
#include "smpdd.hpp"
#include "schedule.hpp"
#include "debug.hpp"
#include "system.hpp"
#include "smp_ult.hpp"

#include "core/utils/Log.h"


namespace nanos {
namespace ext {

using sim::utils::Log;

int SimSmpThread::_stackSize = 10240;

static void bootthread(WD &arg)
{
    SimSmpThread *self = reinterpret_cast<SimSmpThread*>(&arg);

    self->run();
}

static void finishThread(SimSmpThread * th)
{
}

SimSmpThread::SimSmpThread(WD &w, PE *pe, SimSmpProcessor *cpu) :
        SimBaseThread(w, pe, cpu)
{
}

SimSmpThread::~SimSmpThread() throw()
{
}

int SimSmpThread::getStackSize() {
    return _stackSize;
}

void SimSmpThread::start()
{
    initStack();

    sim::runtime::bridge.startRuntimeThread(getId());
}

void SimSmpThread::runDependent()
{
    WD &work = getThreadWD();
    setCurrentWD(work);

    SMPDD &dd = (SMPDD&) work.activateDevice(getSMPDevice());

    dd.getWorkFct()(work.getData());
}

void SimSmpThread::join()
{
    BaseThread * current_thread = nanos::myThread;
    nanos::myThread = this;
    reinterpret_cast<SimSmpProcessor*>(runningOn())->getThreadState()->setToHalted();
    BaseThread::finish();
    joined();
    nanos::myThread = current_thread;
}

void SimSmpThread::initializeDependent()
{
}

void SimSmpThread::bind()
{
    // No need to do anything, threads are bound by default (no OS scheduling modeling)
}

void SimSmpThread::yield()
{
    dynamic_cast<SimSmpProcessor*>(runningOn())->getThreadState()->setToIdle();
    sim::runtime::bridge.getRuntimeThread(getId())->restoreSimState();
    dynamic_cast<SimSmpProcessor*>(runningOn())->getThreadState()->setToRunning();
}

void SimSmpThread::idle(bool debug)
{
    dynamic_cast<SimSmpProcessor*>(runningOn())->getThreadState()->setToIdle();
    sim::runtime::bridge.getRuntimeThread(getId())->restoreSimState();
    dynamic_cast<SimSmpProcessor*>(runningOn())->getThreadState()->setToRunning();
}

// This is executed in between switching stacks
void SimSmpThread::switchHelperDependent(WD *oldWD, WD *newWD, void *oldState)
{
    SimSmpDD & dd = dynamic_cast<SimSmpDD&>(oldWD->getActiveDevice());
    dd.setState(reinterpret_cast<intptr_t*>(oldState));
}


void SimSmpThread::exitHelperDependent(WD* oldWD, WD* newWD, void *arg)
{
}

bool SimSmpThread::inlineWorkDependent(WD &wd)
{
    Log::debug() << "(" << myThread->getId() << "): inlineWork task "
                 << ((SimSmpDD&)wd.getActiveDevice()).getTaskId();
    wd.start(WD::IsNotAUserLevelThread);
    unsigned int task_id = ((SimSmpDD&)getCurrentWD()->getActiveDevice()).getTaskId();
    work(task_id);
    return true;
}

void SimSmpThread::switchTo(WD *wd, SchedulerHelper *helper)
{
    Log::debug() << "(" << myThread->getId() << "): switchTo task "
                 << ((SimSmpDD&)wd->getActiveDevice()).getTaskId();
    // WD must have an active SMP Device when it gets here
    ensure(wd->hasActiveDevice(), "WD has no active SMP device");
    SimSmpDD &dd = (SimSmpDD&)wd->getActiveDevice();
    ensure(dd.hasStack(), "DD has no stack for ULT");

    ::switchStacks(
            reinterpret_cast<void*>(getCurrentWD()),
            reinterpret_cast<void*>(wd),
            reinterpret_cast<void*>(dd.getState()),
            reinterpret_cast<void*>(helper));
}

void SimSmpThread::exitTo(WD *wd,   SchedulerHelper *helper)
{
    Log::debug() << "(" << myThread->getId() << "): exitTo task "
                 << ((SimSmpDD&)wd->getActiveDevice()).getTaskId();
    // wd MUST have an active SMP Device when it gets here
    ensure(wd->hasActiveDevice(), "WD has no active SMP device");
    SimSmpDD &dd = (SimSmpDD&)wd->getActiveDevice();
    ensure(dd.hasStack(), "DD has no stack for ULT");

    // TODO(???): optimize... we don't really need to save a context in this case
    ::switchStacks(
            reinterpret_cast<void*>(getCurrentWD()),
            reinterpret_cast<void*>(wd),
            reinterpret_cast<void*>(dd.getState()),
            reinterpret_cast<void*>(helper));
}

void
SimSmpThread::initStack() {
    _stack = new intptr_t[_stackSize];
    _state = ::initContext(_stack, _stackSize, (void(*)(WD&)) &bootthread,
            reinterpret_cast<WD*>(this), reinterpret_cast<void*>(&finishThread), this);
}

void
SimSmpThread::work(unsigned int task_id)
{
    SimSmpThread* tsth = reinterpret_cast<SimSmpThread*>(myThread);

    bool stillHasEvents = false;

    Log::debug() << "(" << tsth->getId() << "): start task " << task_id;
    sim::runtime::bridge.getSimRuntime()->task_start(task_id, tsth->getId());
    do {
        stillHasEvents = sim::runtime::bridge.getSimRuntime()->process_next_event(task_id, tsth->getId());
        tsth = reinterpret_cast<SimSmpThread*>(myThread);

        if (stillHasEvents) {
            sim::runtime::bridge.getRuntimeThread(tsth->getId())->restoreSimState();
        }
    } while (stillHasEvents);
    Log::debug() << "(" << tsth->getId() << "): finished task " << task_id;
    sim::runtime::bridge.getSimRuntime()->task_end(task_id, tsth->getId());
}

void
SimSmpThread::set_lock(std::unordered_map<unsigned, unsigned> &locks, unsigned address)
{
    SimSmpThread* tsth = reinterpret_cast<SimSmpThread*>(myThread);
    while (locks[address] != 0) {
        reinterpret_cast<SimSmpProcessor*>(runningOn())->getThreadState()->setToLockWait();
        sim::runtime::bridge.getRuntimeThread(getId())->restoreSimState();
        reinterpret_cast<SimSmpProcessor*>(runningOn())->getThreadState()->setToRunning();
    }
    locks[address] = tsth->getId() + 1;
}

void
SimSmpThread::unset_lock(std::unordered_map<unsigned, unsigned> &locks, unsigned address)
{
    SimSmpThread* tsth = reinterpret_cast<SimSmpThread*>(myThread);
    assert(locks.at(address) == static_cast<unsigned>(tsth->getId() + 1));
    _unused(tsth);
    locks.erase(address);
}

void
SimSmpThread::schedule_delay()
{
    reinterpret_cast<SimSmpProcessor*>(runningOn())->getThreadState()->thread_migration_delay();
    sim::runtime::bridge.getRuntimeThread(getId())->restoreSimState();
    reinterpret_cast<SimSmpProcessor*>(runningOn())->getThreadState()->setToRunning();
}

}  // namespace ext
}  // namespace nanos
