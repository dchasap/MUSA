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

#ifndef SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPPLUGIN_H_
#define SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPPLUGIN_H_


#include <atomic>

#include "support/smpbaseplugin_decl.hpp"
#include "arch/smp/smpplugin_decl.hpp"
#include "core/engine/Config.h"
#include "simulators/tasksim/runtime/SimBaseThread.h"



namespace nanos {
namespace ext {

class SimSmpPlugin : public SMPPlugin
{
public:
    SimSmpPlugin();
    explicit SimSmpPlugin(sim::engine::Config& config);

    virtual void set_config_values(const sim::engine::Config& config);

    virtual void init();
};


}  // namespace ext
}  // namespace nanos

#include "simulators/tasksim/runtime/smp/SimSmpPlugin-impl.h"

#endif  // SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPPLUGIN_H_
