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

#include "Burst.h"

#include <string>

#include "core/engine/Config.h"
#include "core/utils/Log.h"

namespace sim {
namespace cpu {

using sim::utils::Log;

Burst::Burst(engine::Simulator &sim, const engine::Config &c, unsigned cpu_id) :
        engine::DModule(sim, engine::ModuleType::CPU),
        busy_timer_(0),
        cpu_id_(cpu_id),
        tracing_freq_mhz_(sim.get_config().get_value<unsigned>("tracing_cpu_freq")),
        perf_ratio_(c.get_value<float>("perf_ratio")),
        burst_count_(*this, "burst_count"),
        burst_cycles_(*this, "burst_cycles"),
        burst_in_(NULL),
        past_activity_(false)
{
    instrumentation_id_ = sim.get_instrumentation().add_module(*this);
}

Burst::~Burst()
{
    if (busy_timer_ > get_simulator().get_clock()) {
        // We remove excessive idle that we will not really simulate.
        burst_cycles_[0] -= (busy_timer_ - get_simulator().get_clock());
    }
}


bool Burst::reset()
{
    busy_timer_ = 0;
    return true;
}

void Burst::start()
{
}


void Burst::end()
{
    assert(burst_in_ != NULL);
    if (busy_timer_ <= get_simulator().get_clock()) {
        trace::ts::phase_t phase;
        if (burst_in_->read(phase)) {
            // past_activity_ = true;
            Log::debug4() << "(" << cpu_id_ << "):" << get_simulator().get_clock()
                          << ": phase: " << phase.burst_id << ":" << phase.duration;
            /** Compute cycles from duration in ns and frequency in MHz **/
            engine::cycles_t phase_cycles = phase.duration * tracing_freq_mhz_ / 1e3 / perf_ratio_;
            if (phase_cycles < 1) {
                phase_cycles = 1;
            }
            busy_timer_ = get_simulator().get_clock() + phase_cycles;
            current_phase_id_ = phase.burst_id;
            if (current_phase_id_ != 0) {
                past_activity_ = true;
            }
            set_module_activity(phase_cycles);

            /** Update stats **/
            burst_count_[phase.burst_id] += 1;
            burst_cycles_[phase.burst_id] += phase_cycles;
        } else {
            set_module_activity(engine::NEVER);
        }
    } else {
        set_module_activity(busy_timer_ - get_simulator().get_clock());
    }
}

const unsigned Burst::get_id() const
{
    return cpu_id_;
}

const std::string Burst::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

bool Burst::drain()
{
    drain_state_ = engine::drain_state_t::DRAINED;
    return true;
}

void Burst::drain_resume()
{
    if (drain_state_ == engine::drain_state_t::DRAINED) {
        drain_state_ = engine::drain_state_t::RUNNING;
        set_module_activity(1);
    }
}

bool Burst::cpu_activity_poll()
{
    if (busy_timer_ > get_simulator().get_clock() && current_phase_id_ != 0) {
        // We are currently active:
        return true;
    }
    if (past_activity_ == true) {
        past_activity_ = false;
        return true;
    } else {
        return false;
    }
}

void Burst::print_status() const
{
    if (busy_timer_ > get_simulator().get_clock()) {
        Log::warning() << "[STATUS] Burst cpu " << get_id() << " next activity will be at cycle "
                       << busy_timer_ << (past_activity_ ? " with past activity " : " without past activity.");

    } else {
        Log::warning() << "[STATUS] Burst cpu " << get_id() << " last activity was at cycle "
                       << busy_timer_;
    }
}

}  // namespace cpu
}  // namespace sim
