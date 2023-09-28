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


#include "NanoxBridge.h"

#include <iostream>
#include <vector>

#include "basethread_decl.hpp"
#include "system.hpp"

#include "simulators/tasksim/runtime/smp/SimSmpProcessor.h"
#include "NanoxAPI.h"
#include "RuntimeThread.h"

#include "core/utils/Log.h"

namespace sim {
namespace runtime {

NanoxBridge bridge;

}  // namespace runtime
}  // namespace sim

namespace nanos {
namespace ext {

PE* tasksimProcessorFactory(int pid, int uid)
{
    // TG std::cout << "tasksimProcessorFactory() pid=" << pid << ", uid=" << uid << std::endl;
    return new SimSmpProcessor(0, uid, false, 0, 0);
}

}  // namespace ext
}  // namespace nanos

namespace sim {
namespace runtime {

using sim::utils::Log;

void
NanoxBridge::init(const sim::engine::Config& config)
{
    assert(thread_states_ != NULL && !_init && !_fini);  // Cpus must be set before initilization

    // We do not force the master to be tied to thread 0 because it generates abnormal behavior.
    nanos::sys.setUntieMaster(true);
    nanos::sys.setSynchronizedStart(false);

    nanos_plugin_.set_config_values(config);
    nanos_plugin_.init();
    nanos::sys.setHostFactory(nanos::ext::tasksimProcessorFactory);
    nanos::sys.start();

    _init = true;
}

void
NanoxBridge::fini()
{
    // TG std::cout << "NanoxBridge::fini()" << std::endl;
    assert(_init && !_fini);
    nanos::sys.finish();  // the WD will switch to CPU[0] if it was not executing there
    _fini = true;
}

void
NanoxBridge::createTask(unsigned int task_id, const std::vector<trace::ts::dep_t> &deps, bool copy_deps)
{
    assert(_init && !_fini);
    sim::runtime::NanoxAPI::createTask(task_id, deps, copy_deps);
}

void
NanoxBridge::waitForTaskGroup(unsigned int task_group)
{
    assert(_init && !_fini);
    // arico DEBUG: std::cout << Sim::cycle << ": waiting for task group" << ((WD*)nanos_current_wd())->getId() << "\n";
    sim::runtime::NanoxAPI::waitForTaskGroup(task_group);
}

void
NanoxBridge::waitOn(const std::vector<trace::ts::dep_t> &deps)
{
    assert(_init && !_fini);
    assert(not deps.empty());
    sim::runtime::NanoxAPI::waitOn(deps);
}

void
NanoxBridge::initThreads(std::vector<sim::runtime::ThreadState*> *thread_states)
{
    assert(!_init && !_fini);
    thread_states_ = thread_states;

    // Copy pointers to runtime threads from CPUStates residing in SimRuntime
    for (std::vector<sim::runtime::ThreadState*>::iterator it = thread_states_->begin();
        it != thread_states_->end(); ++it) {
        // TODO(XXX): for now we support only one thread per CPU
        assert((*it)->getThread() != static_cast<RuntimeThread*>(0));
        // TG std::cout << "NanoxBridge::initCpus(): initializing RTthread "
        // TG         << (*it)->getThread(0) << std::endl;
        runtime_threads_.push_back((*it)->getThread());
    }
    // TG std::cout << "Number of runtime threads: " << runtime_threads_.size() << std::endl;
}

void NanoxBridge::setOmpSs(sim::runtime::OmpSs* sim_runtime)
{
    sim_runtime_ =  sim_runtime;
}

sim::runtime::OmpSs* NanoxBridge::getSimRuntime()
{
    return sim_runtime_;
}

sim::runtime::ThreadState* NanoxBridge::getThreadState(unsigned int i) const
{
    assert(i < thread_states_->size());
    return (*thread_states_)[i];
}

void NanoxBridge::startRuntimeThread(int i)
{
    // TG std::cout << "Starting runtime thread " << i << std::endl;
    assert(runtime_threads_[i] != NULL);
    runtime_threads_[i]->start();
}

void NanoxBridge::setRuntimeThread(unsigned int i, nanos::BaseThread* tsth)
{
    // TG std::cout << "NanoxBridge::setRuntimeThread(): i=" << i << ", SimSmpThread=" << tsth << std::endl;
    // TG std::cout << "Test1" << std::endl;
    assert(i < runtime_threads_.size());
    assert(runtime_threads_[i] != NULL);
    runtime_threads_[i]->setRuntimeThread(reinterpret_cast<void*>(tsth));
    // TG std::cout << "Test5" << std::endl;
}

RuntimeThread* NanoxBridge::getRuntimeThread(unsigned int i)
{
    Log::debug2() << "NanoxBridge::getRuntimeThread(), i=" << i;
    assert(i < runtime_threads_.size());
    assert(runtime_threads_[i] != NULL);
    return runtime_threads_[i];
}

void NanoxBridge::setMainTask(unsigned id)
{
    RuntimeThread::MAIN_TASK_ID = id;
}


}  // namespace runtime
}  // namespace sim
