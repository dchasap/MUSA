/*
 * Copyright (c) 2016, Barcelona Supercomputing Center
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


#ifndef CORE_MODULES_MEMORY_RAMULATORCONTROLLER_H_
#define CORE_MODULES_MEMORY_RAMULATORCONTROLLER_H_

#include <list>
#include <map>
#include <string>
#include <queue>
#include <vector>


// Ramulator headers:
#include "src/Config.h"
#include "src/Request.h"
#include "src/MemoryFactory.h"
#include "src/Memory.h"
#include "src/DDR3.h"
#include "src/DDR4.h"
#include "src/LPDDR3.h"
#include "src/LPDDR4.h"
#include "src/GDDR5.h"
#include "src/WideIO.h"
#include "src/WideIO2.h"
#include "src/HBM.h"
#include "src/SALP.h"

#include "core/modules/memory/RamulatorControllerDefs.h"
#include "core/modules/memory/types.h"
#include "core/engine/Config.h"
#include "core/engine/IModule.h"
#include "core/engine/Simulator.h"
#include "core/engine/types.h"
#include "core/utils/TimeList.h"

namespace sim {
namespace memory {


static const std::unordered_map<std::string, std::function<ramulator::MemoryBase *(const ramulator::Config&, int)>>
    name_to_func({
        {"DDR3", &ramulator::MemoryFactory<ramulator::DDR3>::create},
        {"DDR4", &ramulator::MemoryFactory<ramulator::DDR4>::create},
        {"LPDDR3", &ramulator::MemoryFactory<ramulator::LPDDR3>::create},
        {"LPDDR4", &ramulator::MemoryFactory<ramulator::LPDDR4>::create},
        {"GDDR5", &ramulator::MemoryFactory<ramulator::GDDR5>::create},
        {"WideIO", &ramulator::MemoryFactory<ramulator::WideIO>::create},
        {"WideIO2", &ramulator::MemoryFactory<ramulator::WideIO2>::create},
        {"HBM", &ramulator::MemoryFactory<ramulator::HBM>::create},
        {"SALP-1", &ramulator::MemoryFactory<ramulator::SALP>::create},
        {"SALP-2", &ramulator::MemoryFactory<ramulator::SALP>::create},
        {"SALP-MASA", &ramulator::MemoryFactory<ramulator::SALP>::create}
});


template<typename Message, template<typename> class Interconnect>
class RamulatorController : public engine::IModule<Message> {
public:
    struct RamulatorConfig {
        explicit RamulatorConfig(const engine::Config &config);
        ~RamulatorConfig();
        static std::string lock_config_file(const std::string &prefix,
                                            const std::string &suffix);
        std::string ramulator_ini_;  // Ramulator DRAM module configuration file
    };

protected:
    /** RamulatorController configuration parameters: */
    RamulatorConfig config_;
    /** Ramulator configuration parameters: */
    ramulator::Config ramulator_config_;
    /** Requests input buffer. */
    std::vector<typename Message::Request>  input_buffer_;
    /** Maximum input buffer size. */
    std::size_t input_buffer_size_;
    /** Buffer containing the requests Ramulator is currently processing. */
    std::map<sim::engine::addr_t, sim::utils::TimeList<typename Message::Request>> request_buffer_;
    /** Ack output buffer. */
    std::list<typename Message::Ack> output_buffer_;
    /** Number of simulated system cpus. */
    int num_cpus_;
    /** Unique pointer to the simulated memory system. */
    std::unique_ptr<ramulator::MemoryBase> mem_;
    /** Read request callback function. */
    std::function<void(ramulator::Request&)> read_cb_func;
    /** Write request callback function. */
    std::function<void(ramulator::Request&)> write_cb_func;
    /** Current cycle in the Ramulator simulated DRAM. */
    sim::engine::cycles_t dram_cycle_;
    /** Ramulator statistics. */
    RAMStats stats_;
    /** Associated Memory Management Unit. */
    sim::logic::memory::MMU* mmu_;
    /** Index in all simulated Ramulator memory systems. */
    unsigned id_;
    /** Number of read requests being processed inside Ramulator. */
    unsigned pending_reads_;
    /** Number of write requests being processed inside Ramulator. */
    unsigned pending_writes_;
    /** Indicates wether there are Requests blocked from Ramulator's memory. */
    bool blocked_;
    /** Indicates the last cycles where there were no blocked Requests. */
    sim::engine::cycles_t last_nonblocked_cycle_;
    /** Ratio of the cpu frequency over the memory bus frequency. */
    float frequency_ratio_;

    /** Updates Ramulator current clock until it coincides with the Simulation clcok. */
    void update_dram_cycle();
    /** Updates Ramulator clock by one cycle. */
    void advance_dram_cycle();

public:
    RamulatorController(sim::engine::Simulator &simulator, const sim::engine::Config &config,
                        const sim::engine::Config &config_L3, unsigned int tcpus,
                        logic::memory::MMU* = NULL, unsigned id = 0);
    ~RamulatorController();

    bool reset();
    void start();
    void end();

    bool inspect(const typename Message::Request &request);
    bool inspect(const typename Message::Ack &ack);

    typedef typename Interconnect<Message>::Input Input;
    Input in_;
    unsigned last_read_port_;
    unsigned last_written_port_;

    /** Callback function for Ramulator on read request completion. */
    void read_complete(ramulator::Request& request);
    /** Callback function for Ramulator on write request completion. */
    void write_complete(ramulator::Request& request);
    /** Generic function for request completion. */
    void return_request(ramulator::Request& request);
    /** Stub for Ramulator's power_callback. */
    void power_callback(unsigned id, uint64_t addr, uint64_t ramulator_cycle);

    /** Print Cache module hardware counters to the Paraver trace. */
    void print_hardware_counters(sim::engine::cycles_t time, bool propagate = false, unsigned overwrite_cpu_id = 0);
    /** Returns this RamulatorController full name. */
    const std::string get_name() const;
    /** Returns the module index amongst RamulatorControllers. */
    unsigned get_id() const;
    /** Prints the current status of the Cache contents (debug purposes). */
    void print_status() const override;

protected:
    /** Calculates the next cycle this RamulatorController can perform any pending action,
      * and adds it to the Simulator ready queue. */
    void set_next_cycle();
};

}  // namespace memory
}  // namespace sim

#include "RamulatorController-impl.h"

#endif  // CORE_MODULES_MEMORY_RAMULATORCONTROLLER_H_
