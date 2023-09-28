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


#ifndef CORE_ENGINE_MODULE_IMPL_H_
#define CORE_ENGINE_MODULE_IMPL_H_

#include "Simulator.h"
#include "core/stats/Stat.h"
#include "core/utils/Log.h"

namespace sim {
namespace engine {

using sim::utils::Log;

inline
Module::Module(Simulator &simulator, ModuleType module_type) :
        id_(simulator.register_module(*this)),
        next_activity_(NEVER),
        execute_next_cycle_(false),
        simulator_(simulator),
        module_type_(module_type)
{
}

inline
Module::~Module()
{
}


inline
const unsigned Module::get_id() const
{
    return id_;
}

inline
const unsigned Module::get_module_id() const
{
    return id_;
}

inline
ModuleType Module::get_module_type() const
{
    return module_type_;
}

inline
Simulator &Module::get_simulator() const
{
    return simulator_;
}


inline
void Module::set_module_activity(engine::cycles_t delay)
{
    Log::debug2() << simulator_.get_clock() << ":set_module_activity module " << id_ << " delay " << delay;
    if ((delay != 0) && (this->get_execute_next_cycle())) {
        this->set_execute_next_cycle(false);
        delay = 1;
    }

    if ((next_activity_ == NEVER) && (delay == NEVER)) {
        return;
    }

    if ((next_activity_ != NEVER) && (delay != NEVER) &&
            (next_activity_ == simulator_.get_clock() + delay)) {
        return;
    }

    if (next_activity_ != NEVER) {
        simulator_.remove_module_call(*this);
    }

    if (delay != NEVER) {
        simulator_.add_module_call(*this, delay);
        next_activity_ = simulator_.get_clock() + delay;
    } else {
        next_activity_ = NEVER;
    }
}


inline
engine::cycles_t Module::get_next_activity()
{
    return next_activity_;
}


inline
bool Module::get_execute_next_cycle()
{
    return execute_next_cycle_;
}


inline
void Module::set_execute_next_cycle(bool execute_next_cycle)
{
    execute_next_cycle_ = execute_next_cycle;
}

inline
void Module::notify_wakeup(void *caller_ptr)
{
}

inline
void Module::print_hardware_counters(engine::cycles_t time, bool propagate, unsigned overwrite_cpu_id)
{
}

inline
const unsigned int Module::get_instrumentation_id() const
{
    return instrumentation_id_;
}

inline
bool Module::cpu_activity_poll()
{
    return false;
}

inline
void Module::print_status() const
{
}

}  // namespace engine
}  // namespace sim

#endif  // CORE_ENGINE_MODULE_IMPL_H_
