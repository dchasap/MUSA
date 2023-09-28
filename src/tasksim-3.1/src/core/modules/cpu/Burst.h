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


#ifndef CORE_MODULES_CPU_BURST_H_
#define CORE_MODULES_CPU_BURST_H_

#include <string>

#include "core/engine/InPort.h"
#include "core/engine/DModule.h"
#include "core/engine/types.h"
#include "core/engine/EventBuffer.h"
#include "core/stats/Counter.h"
#include "core/stats/CounterMap.h"
#include "core/trace/ts/types.h"

namespace sim {
namespace cpu {

class Burst : public engine::DModule {
public:
    typedef engine::EventBuffer<trace::ts::phase_t> burst_buffer_t;

private:
    engine::cycles_t busy_timer_;

    /** System wide unique id of this CPU */
    unsigned cpu_id_;
    /** Frequency of this CPU */
    unsigned tracing_freq_mhz_;
    float perf_ratio_;

    /** Statistics */
    stats::CounterMap<unsigned> burst_count_;
    stats::CounterMap<unsigned> burst_cycles_;

    burst_buffer_t *burst_in_;

    /** Boolean that indicates if the module has been active since last polling. */
    bool past_activity_;
    /** Stores the current phase identifier. Allows running/idle evaluation. */
    unsigned current_phase_id_;
public:
    Burst(engine::Simulator &, const engine::Config &, unsigned cpu_id_ = 0);
    ~Burst();

    bool reset();
    void start();
    void end();

    bool drain();
    void drain_resume();

    const unsigned get_id() const;
    const std::string get_name() const;

    virtual bool cpu_activity_poll() override;

    virtual void print_status() const override;

    friend void operator>>(burst_buffer_t &e, Burst &c) {
        c.burst_in_ = &e;
        e.set_remote_module(&c);
    }
};

}  // namespace cpu
}  // namespace sim

#endif   // CORE_MODULES_CPU_BURST_H_
