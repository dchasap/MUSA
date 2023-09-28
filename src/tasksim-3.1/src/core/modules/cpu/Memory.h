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


#ifndef CORE_MODULES_CPU_MEMORY_H_
#define CORE_MODULES_CPU_MEMORY_H_

extern "C" {
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
}

#include <unordered_map>
#include <string>

#include "core/engine/Config.h"
#include "core/engine/DModule.h"
#include "core/engine/EventBuffer.h"
#include "core/engine/Module.h"
#include "core/modules/interconnect/Direct.h"
#include "core/stats/Histogram.h"
#include "core/stats/VulnerabilityStat.h"
#include "core/trace/mem/types.h"


namespace sim {
namespace cpu {

template<typename TimingModel, typename MemoryAccess>
class Memory : public sim::engine::DModule {
public:
    typedef sim::engine::EventBuffer<sim::trace::mitem_t> mem_buffer_t;

private:
    TimingModel model_;

    /** Reference to simulator */
    sim::engine::Simulator &sim_;

    unsigned cpu_id_;

    void send_mem_request();
    void get_mem_response();

    mem_buffer_t *mem_in_;

    bool past_activity_;

    stats::Histogram load_size_hist_;
    stats::Histogram store_size_hist_;
    std::unique_ptr<sim::stats::VulnerabilityStat> vulnerability_;

    uint64_t current_wd_id_;

public:
    Memory(engine::Simulator &sim, const engine::Config &config, unsigned cpu_id = 0);
    ~Memory();

    void start();
    void end();
    void print_hardware_counters(engine::cycles_t time, bool propagate = false, unsigned overwrite_cpu_id = 0);
    bool reset() { return true; }
    void context_switch(engine::addr_t);

    /* Interface to memory hierarchy */
    typename interconnect::Direct<MemoryAccess>::Output dl1_;

    // Copied from Burst CPU model
    friend void operator>>(mem_buffer_t &e, Memory<TimingModel, MemoryAccess> &c) {
        c.mem_in_ = &e;
        e.set_remote_module(&c);
    }

    bool drain() override;

    void drain_resume() override;

    uint64_t get_ins_current_phase() { return model_.get_ins_current_phase(); }

    const unsigned get_id() const;
    const std::string get_name() const;

    virtual bool cpu_activity_poll() override;

    virtual void print_status() const override;

    // TODO(who?): This will be done in ompss (simulator) runtime class
    // Methods for trace reading
    // void switch_to_subtrace(int subt) { reader_.set_subtrace(subt); }
    void set_current_wd_id(uint64_t);
};

}  // namespace cpu
}  // namespace sim

#include "Memory-impl.h"

#endif  // CORE_MODULES_CPU_MEMORY_H_
