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


#ifndef CORE_MODULES_MEMORY_DRAM_H_
#define CORE_MODULES_MEMORY_DRAM_H_

#include <list>
#include <queue>
#include <string>
#include <vector>

#include "core/engine/IModule.h"
#include "core/engine/types.h"
#include "core/logic/memory/DRAM.h"
#include "core/logic/memory/MMU.h"
#include "core/modules/memory/types.h"
#include "core/utils/TimeList.h"
#include "core/stats/TimeAverage.h"

namespace sim {

namespace engine {
class Simulator;
class Config;
}

namespace memory {

class DRAMStats : public RAMStats {
    public:
    /** Generic DRAM stats constructor. */
    template<typename OwnerType>
    explicit DRAMStats(const OwnerType &parent);
    /** Open event counter (DRAM module). */
    sim::stats::Counter open;
    /** Precharge event counter (DRAM module). */
    sim::stats::Counter precharge;
};

template<typename Message, template<typename> class Interconnect>
class DRAM : public engine::IModule<Message> {
public:
    struct Config {
        explicit Config(const engine::Config &);

        float frequency_divider_;
        unsigned cas_;
        unsigned ras_;
        unsigned precharge_;

        logic::memory::DRAM::range_t column_range_;
        logic::memory::DRAM::range_t row_range_;
        logic::memory::DRAM::range_t bank_range_;
        logic::memory::DRAM::range_t rank_range_;

        sim::engine::addr_t addr_range_start_;
        sim::engine::addr_t addr_range_end_;
    };

protected:
    Config config_;

    std::size_t input_buffer_size_;
    std::list<typename Message::Request>  input_buffer_;
    utils::TimeList<typename Message::Ack> output_buffer_;

    sim::logic::memory::DRAM dram_;

    DRAMStats stats_;

    sim::logic::memory::MMU* mmu_;
    unsigned id_;

public:
    DRAM(sim::engine::Simulator &simulator, sim::engine::Config config, sim::logic::memory::MMU* = NULL,
            unsigned id = 0);
    ~DRAM();

    bool reset();
    void start();
    void end();
    const std::string get_name() const;

    bool inspect(const typename Message::Request &);
    bool inspect(const typename Message::Ack &);

    void print_hardware_counters(sim::engine::cycles_t time, bool propagate = false, unsigned overwrite_cpu_id = 0);
    typedef typename Interconnect<Message>::Input Input;
    Input in_;
    unsigned get_id() const;
    virtual void print_status() const override;
};

}  // namespace memory
}  // namespace sim

#include "DRAM-impl.h"

#endif  // CORE_MODULES_MEMORY_DRAM_H_
