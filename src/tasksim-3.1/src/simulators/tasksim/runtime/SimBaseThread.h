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

#ifndef SIMULATORS_TASKSIM_RUNTIME_SIMBASETHREAD_H_
#define SIMULATORS_TASKSIM_RUNTIME_SIMBASETHREAD_H_

#include "basethread.hpp"
#include "smp/SimSmpProcessor.h"
#include "arch/smp/smpthread.hpp"

namespace sim {
namespace runtime {

class RuntimeThread;

}  // namespace runtime
}  // namespace sim

namespace nanos {
namespace ext {

/**
* Base class for threads that represent simulated processors or accelerators.
*
* Needs to be extended for each new simulated device that gets exposed to NANOS++.
*
*/
class SimBaseThread : public SMPThread
{
    friend class sim::runtime::RuntimeThread;

protected:
    BaseThread * _myThread;

    void *   _stack;
    void *   _state;
    intptr_t *   _simState;

private:
    // Disable copy and assignment
    SimBaseThread(const SimBaseThread &th);
    const SimBaseThread & operator= (const SimBaseThread &th);

    void* getState() { return _state; }
    bool      hasStack() { return _stack != NULL; }

public:
    SimBaseThread(WD &wd, PE *pe, SimSmpProcessor *cpu) : SMPThread(wd, pe, dynamic_cast<SMPProcessor*>(cpu)) {}
};

}  // namespace ext
}  // namespace nanos

#endif  // SIMULATORS_TASKSIM_RUNTIME_SIMBASETHREAD_H_
