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


#ifndef CORE_ENGINE_SIMULATOR_H_
#define CORE_ENGINE_SIMULATOR_H_

#include <functional>
#include <list>
#include <utility>
#include <vector>
#include <functional>

#include "core/utils/instrumentation/Instrumentation.h"
#include "types.h"

namespace sim {
namespace stats {
class Stat;
}
}

namespace sim {
namespace engine {

class Config;
class Module;

class Simulator {
private:
    /** Vector of modules registered in the simulator */
    std::vector<Module *> module_vector_;

    /** Vector of stats registered in the simulator */
    std::vector<stats::Stat *> stat_vector_;

    typedef std::list<std::pair<cycles_t, unsigned>> active_module_list_t;

    /** List of modules that are active */
    active_module_list_t active_module_list_;

    /** Clock */
    cycles_t current_cycle_;

    /** Clock phase */
    enum {
        Low, High
    } phase_;

    /** Hardware sampling clocks */
    cycles_t next_hardware_sampling_;
    // No longer const as it must be assigned after instrumentation initialization
    cycles_t hardware_sampling_interval_;

    /** Number of cycles of the deadlock detection interval. */
    cycles_t deadlock_detection_interval_;
    /** Cycle after which we will watch for deadlocks. */
    cycles_t next_deadlock_detection_cycle_;

    /** Simulator configuration */
    const Config &config_;

    /** Simulation end, set when finished */
    bool end_;

    /** Instrumentation */
    utils::instrumentation::SelectedInstrumentation instrumentation_;

    /** Pointer to simulator instance. Set in the constructor. Only valid if
        there is only one simulator in the system, which is always the case */
    static Simulator* instance_;

    /** Register a new module in the simulator
     * \param Module to be registered
     */
    unsigned register_module(Module &);

    /** Wake up a module
     * \param Module to be woken up
     */
    void wake_up_module(Module &);

    /** Adds a request to call a module
     * \param Module to be called
     * \param Delay in cycles to call the module
     */
    void add_module_call(Module &, engine::cycles_t);

    /** Removes a request to call a module
     * \param Module to be called
     * \param Delay in cycles to call the module
     */
    void remove_module_call(Module &);

    /** InPort, OutPort and Module are friend classes to avoid making the
     * above methods public and accessible to any other class. This
     * is an exception, and only applies because InPort, OutPort and Module
     * are part of the engine. In any case, InPort, OutPort and Module
     * must still only use the above method and never access individual
     * members directly.
     */
    template <typename T> friend class InPort;
    template <typename T> friend class OutPort;
    friend class Module;

    /** We can register a void function without parameters to be called
      * In case of SIGABORT. */
    std::function<void()> abnormal_termination_function;

    /** Checks for lack of activity in the modules and writes debug information. */
    void check_deadlock();

    /** Prints the hardware counters for all modules if the sampling interval has elapsed. */
    void hardware_counters_sampling();

public:
    explicit Simulator(Config &);
    virtual ~Simulator();

    /** Execute the simulation */
    void run();

    /** Get the current time */
    cycles_t get_clock() const;

    /* Only added for testing: */
    void testing_only_add_cycles(sim::engine::cycles_t plus);

    /** Get the simulator configuration */
    const Config & get_config() const;

    /** Simulation finished */
    void finish();

    /** Get Instrumentor */
    utils::instrumentation::SelectedInstrumentation& get_instrumentation();

    /** Set abnormal termination function */
    void set_abnormal_termination_function(std::function<void()> f);

    /** Method to get pointer to Simulator instance. There should be only one
        simulator instance in a simulation */
    static Simulator* get_instance();

    /** Register a new stat in the simulator
     * \param Module to be registered
     */
    unsigned register_stat(sim::stats::Stat &stat);

    // Extrapolate stats from given to other user code id
    void extrapolate_stats(uint64_t from, uint64_t to);
};

/** Sigterm handling signal */
void handle_sigterm(int param);

}  // namespace engine
}  // namespace sim

#include "Simulator-impl.h"

#endif  // CORE_ENGINE_SIMULATOR_H_
