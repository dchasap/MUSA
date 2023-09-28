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


#ifndef CORE_ENGINE_SIMULATOR_IMPL_H_
#define CORE_ENGINE_SIMULATOR_IMPL_H_

#include <cassert>

#include "core/stats/Stat.h"
#include "Module.h"

namespace sim {
namespace engine {

inline
unsigned Simulator::register_module(Module &module)
{
    module_vector_.push_back(&module);
    return module_vector_.size() - 1;
}

inline
void Simulator::wake_up_module(Module &module)
{
    assert(module.get_id() < module_vector_.size());
    assert(module_vector_[module.get_id()] == &module);

    /* If writing at the end of the cycle, the module will be waken up the next cycle */
    if (phase_ == High) {
        if (module.get_next_activity() == current_cycle_) {
            module.set_execute_next_cycle(true);
        } else {
            module.set_module_activity(1);
        }
    } else {
        module.set_module_activity(0);
    }
}

inline
void Simulator::add_module_call(Module &module, engine::cycles_t delay)
{
    assert(module.get_id() < module_vector_.size());
    assert(module_vector_[module.get_id()] == &module);

    engine::cycles_t target_cycle = current_cycle_ + delay;

    /* Check if the list is empty or last element of the list is younger than new call */
    if (active_module_list_.empty() || active_module_list_.back().first < target_cycle) {
        active_module_list_.emplace_back(std::make_pair(target_cycle, module.get_id()));
        return;
    }

    /* TODO: most likely it is worthy to traverse the list backwards */
    active_module_list_t::iterator i;
    for (i = active_module_list_.begin(); i != active_module_list_.end(); ++i) {
        if (i->first < target_cycle) continue;
        active_module_list_.emplace(i, std::make_pair(target_cycle, module.get_id()));
        return;
    }

    assert(0);
}

inline
void Simulator::remove_module_call(Module &module)
{
    assert(module.get_id() < module_vector_.size());
    assert(module_vector_[module.get_id()] == &module);

    unsigned mod_id = module.get_id();
    active_module_list_t::iterator i;
    for (i = active_module_list_.begin(); i != active_module_list_.end(); ++i) {
        if (i->second == mod_id) {
            active_module_list_.erase(i);
            return;
        }
    }
    assert(0);
}

inline
unsigned Simulator::register_stat(stats::Stat &stat)
{
    stat_vector_.push_back(&stat);
    return stat_vector_.size() - 1;
}


inline
cycles_t Simulator::get_clock() const
{
    return current_cycle_;
}

inline
void Simulator::testing_only_add_cycles(sim::engine::cycles_t plus)
{
    current_cycle_ += plus;
}

inline
const Config &Simulator::get_config() const
{
    return config_;
}

inline
void Simulator::finish() {
    end_ = true;
}

inline
utils::instrumentation::SelectedInstrumentation &Simulator::get_instrumentation()
{
    return instrumentation_;
}

inline void Simulator::set_abnormal_termination_function(std::function<void()> f)
{
    abnormal_termination_function = f;
}

}  // namespace engine
}  // namespace sim

#endif  // CORE_ENGINE_SIMULATOR_IMPL_H_
