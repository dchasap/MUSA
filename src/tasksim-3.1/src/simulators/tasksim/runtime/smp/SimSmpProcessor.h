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

#ifndef SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPPROCESSOR_H_
#define SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPPROCESSOR_H_


#include "config.hpp"
#include "memtracker.hpp"
#include "processingelement.hpp"
#include "arch/smp/smpprocessor.hpp"

namespace sim {
namespace runtime {
class ThreadState;
}  // namespace runtime
}  // namespace sim

namespace nanos {
namespace ext {

// class SimSmpProcessor : public PE
class SimSmpProcessor : public SMPProcessor
{
private:
    /** Pointer to CPU state held by simulator runtime class */
    sim::runtime::ThreadState* thread_state_;

    // config variables
    static bool _useUserThreads;

    // disable copy constructor and assignment operator
    SimSmpProcessor(const SimSmpProcessor &pe);
    const SimSmpProcessor & operator= (const SimSmpProcessor &pe);

public:
    /** Constructor */
    SimSmpProcessor(int binding_id, int memId, bool active, unsigned numaNode, unsigned socket);

    /** Destructor */
    virtual ~SimSmpProcessor() {}

    virtual WD & getWorkerWD() const;
    virtual WD & getMasterWD() const;
    virtual BaseThread & createThread(WorkDescriptor &wd);

    inline sim::runtime::ThreadState* getThreadState() { return thread_state_; }

    // capability query functions
    virtual bool supportsUserLevelThreads() const;
    virtual bool hasSeparatedMemorySpace() const;

    virtual void copyDataIn(WorkDescriptor& wd);
    virtual void copyDataOut(WorkDescriptor& wd);

    // To make it compatible:

    virtual nanos::WorkDescriptor& getMultiWorkerWD() const {
        return getWorkerWD();
    }
    virtual nanos::BaseThread& createThread(nanos::WorkDescriptor &wd, nanos::ext::SMPMultiThread *smp) {
        return createThread(wd);
    }
    virtual nanos::BaseThread& createMultiThread(nanos::WorkDescriptor &wd, unsigned int, nanos::ProcessingElement**) {
        return createThread(wd);
    }
};

}  // namespace ext
}  // namespace nanos

#endif  // SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPPROCESSOR_H_
