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

#include "SimSmpProcessor.h"

#include <iostream>

#include "simulators/tasksim/runtime/smp/SimSmpDD.h"
#include "simulators/tasksim/runtime/ThreadState.h"
#include "simulators/tasksim/runtime/NanoxBridge.h"
#include "smpdd.hpp"
#include "schedule.hpp"
#include "debug.hpp"
#include "config.hpp"
#include "cpuset.hpp"

namespace nanos {
namespace ext {

bool SimSmpProcessor::_useUserThreads = true;

SimSmpProcessor::SimSmpProcessor(int binding_id, int memId, bool active, unsigned numaNode, unsigned socket) :
    SMPProcessor(binding_id, CpuSet(binding_id), memId, active, numaNode, socket),
    thread_state_(sim::runtime::bridge.getThreadState(binding_id))
{
}

WorkDescriptor & SimSmpProcessor::getWorkerWD() const
{
    SimSmpDD * dd = new SimSmpDD((SMPDD::work_fct)Scheduler::workerLoop);
    WD *wd = new WD(dd);
    return *wd;
}

WorkDescriptor & SimSmpProcessor::getMasterWD() const
{
    SimSmpDD *test = new SimSmpDD();
    WD * wd = new WD( test );
    // WD * wd = new WD( new SimSmpDD() );
    return *wd;
}

BaseThread & SimSmpProcessor::createThread(WorkDescriptor &helper)
{
    ensure(helper.canRunIn(SMP), "Incompatible worker thread");
    SimSmpThread &th = *new SimSmpThread(helper, this, this);

    // sim::runtime::bridge.setRuntimeThread(getId(),&th); //arico DEBUG: std::cout << "setRuntimeThread\n";
    // TG std::cout << "SimSmpProcessor::createThread(), tid=" << th.getId() << std::endl;
    sim::runtime::bridge.setRuntimeThread(th.getId(), &th);

    return th;
}

bool SimSmpProcessor::supportsUserLevelThreads() const
{
    return true;
}

bool SimSmpProcessor::hasSeparatedMemorySpace() const
{
    return false;
}

void SimSmpProcessor::copyDataIn(WorkDescriptor& wd)
{
    CopyData* copies = wd.getCopies();

    unsigned int inputDeps = 0;
    unsigned int sizeBytes = 0;
/*
    std::cout << Sim::cycle << ": Cpu " << _cpuModule->getTaskCpu()->cpu().tid()
            << " beginning to copyDataIn from Task " << wd.getId() << std::endl;
*/
    for (unsigned int i = 0; i < wd.getNumCopies(); i++) {
        CopyData & cd = copies[i];
        if (cd.isInput()) {
            inputDeps++;
            sizeBytes += cd.getSize();
        }
    }

// TODO(TG): this needs to be implemented in the simulator runtime module. For now we don't do this.
//    unsigned int cache_level = _cpuModule->getTaskCpu()->numprefetched(inputDeps,sizeBytes);
//
//    for(unsigned int i = 0; i < wd.getNumCopies(); i++)
//    {
//        CopyData & cd = copies[i];
//        if(cd.isInput() && (cd.getSize() > 0))
//        {
////            std::cout << "Input copy " << i << ", size = " << cd.getSize() << std::endl;
////            std::cout << "----------copyDataIn (prefetch)  data from wd " << wd.getId() << std::endl;
//            _cpuModule->getTaskCpu()->prefetchData(cd.getAddress(), cd.getSize(),cache_level);
//        }
//    }
}

void SimSmpProcessor::copyDataOut(WorkDescriptor& wd)
{
// TODO(TG): this needs to be implemented in the simulator runtime module. For now we don't do this.
//    CopyData* copies = wd.getCopies();
//    for(unsigned int i = 0; i < wd.getNumCopies(); i++)
//    {
//        CopyData & cd = copies[i];
//        if(cd.isOutput())
//        {
//            _cpuModule->getTaskCpu()->flushData(cd.getAddress(), cd.getSize());
//        }
//    }
}

}  // namespace ext
}  // namespace nanos
