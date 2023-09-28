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

#ifndef SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPTHREAD_H_
#define SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPTHREAD_H_

#include "simulators/tasksim/runtime/SimBaseThread.h"

namespace nanos {
namespace ext {

class SimSmpThread : public SimBaseThread
{
    friend class SimSmpProcessor;
    friend class sim::runtime::RuntimeThread;

private:
    static int _stackSize;

    // Disable copy constructor and assignment operator
    SimSmpThread(const SimSmpThread &th);
    const SimSmpThread & operator=(const SimSmpThread &th);

    // TODO(???): This could go to the base thread as well (with stacksize)
    void initStack();

public:
    static void work(unsigned int task_id);

public:
    /** Constructor */
    SimSmpThread(WD &w, PE *pe, SimSmpProcessor *cpu);

    /** Destructor */
    virtual ~SimSmpThread() throw();

    static int getStackSize();

    virtual void start();
    /** Called at simulation end to join all threads */
    virtual void join();
    virtual void initializeDependent();
    virtual void runDependent();

    virtual bool inlineWorkDependent(WD &work);
    virtual void switchTo(WD *work, SchedulerHelper *helper);
    virtual void exitTo(WD *work, SchedulerHelper *helper);

    virtual void switchHelperDependent(WD* oldWD, WD* newWD, void *arg);
    virtual void exitHelperDependent(WD* oldWD, WD* newWD, void *arg);

    virtual void outlineWorkDependent(WD &work) {}
    virtual void preOutlineWorkDependent(WD &work) {}

    virtual nanos::BaseThread* getNextThread() { return this;}
    virtual void switchToNextThread() {}
    virtual bool isCluster() { return true;}

    virtual void idle(bool debug = false);
    /**
     * Bind thread to a processing element. For now this does nothing, i.e.
     * threads are bound statically at instantiation time.
     * */
    virtual void bind();

    /** \brief SMP specific yield implementation
     */
    virtual void yield();

    void set_lock(std::unordered_map<unsigned, unsigned> &locks, unsigned address);
    void unset_lock(std::unordered_map<unsigned, unsigned> &locks, unsigned address);
    void schedule_delay();

    virtual void sleep() { }
    virtual void wakeup() { }
    virtual void wait() { }

    virtual void lock() { }
    virtual void unlock() { }
};


}  // namespace ext
}  // namespace nanos

#endif  // SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPTHREAD_H_
