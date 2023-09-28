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


#ifndef CORE_MODULES_MEMORY_DRAMSIM2CONTROLLER_H_
#define CORE_MODULES_MEMORY_DRAMSIM2CONTROLLER_H_

#include <list>
#include <map>
#include <string>
#include <queue>
#include <vector>

#include "core/modules/memory/Dramsim2ControllerDefs.h"
// DRAMSim2 headers:
#include "MultiChannelMemorySystem.h"
#include "MemorySystem.h"


#include "core/engine/Config.h"
#include "core/engine/IModule.h"
#include "core/engine/Simulator.h"
#include "core/engine/types.h"
#include "core/modules/memory/types.h"
#include "core/utils/TimeList.h"
#include "core/stats/TimeAverage.h"

namespace sim {
namespace memory {

template<typename Message, template<typename> class Interconnect>
class Dramsim2Controller : public engine::IModule<Message> {
public:
    struct Dramsim2Config {
        explicit Dramsim2Config(const engine::Config &);
        ~Dramsim2Config();
        static std::string lock_config_file(const std::string &tmpdir,
                                            const std::string &prefix,
                                            const std::string &sufix);
        std::string path_;      // Prefix path to those configuration files: /tmp/ ?
        std::string dram_ini_;  // DRAMSim2 DRAM module configuration file.
        std::string system_ini_;  // DRAMSim2 System configuration file.
        std::string trace_file_;  // String added to the log file? (verify)
        size_t megs_of_ram_;
    };

protected:
    Dramsim2Config config_;
    std::list<typename Message::Request>  input_buffer_;
    std::size_t input_buffer_size_;
    std::map<engine::addr_t, utils::TimeList<typename Message::Request>> request_buffer_;
    std::list<typename Message::Ack> output_buffer_;
    DRAMSim::MultiChannelMemorySystem dram_;
    sim::engine::cycles_t dram_cycle_;

    RAMStats stats_;
    logic::memory::MMU* mmu_;
    unsigned id_;
    void update_dram_cycle();
    void advance_dram_cycle();

public:
    Dramsim2Controller(engine::Simulator &, const engine::Config &, logic::memory::MMU* = NULL, unsigned id = 0);
    ~Dramsim2Controller();

    bool reset();
    void start();
    void end();

    bool inspect(const typename Message::Request &);
    bool inspect(const typename Message::Ack &);
    typedef typename Interconnect<Message>::Input Input;
    Input in_;

    void read_complete(unsigned id, uint64_t addr, uint64_t dramsim_cycle);
    void write_complete(unsigned id, uint64_t addr, uint64_t dramsim_cycle);
    void return_request(unsigned id, uint64_t addr, uint64_t dramsim_cycle);
    void power_callback(unsigned id, uint64_t addr, uint64_t dramsim_cycle);

    void print_hardware_counters(sim::engine::cycles_t time, bool propagate = false, unsigned overwrite_cpu_id = 0);
    const std::string get_name() const;
    unsigned get_id() const;
    virtual void print_status() const override;
};

}  // namespace memory
}  // namespace sim

#include "Dramsim2Controller-impl.h"

#endif  // CORE_MODULES_MEMORY_DRAMSIM2CONTROLLER_H_
