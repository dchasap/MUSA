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


#ifndef CORE_ENGINE_MODULE_H_
#define CORE_ENGINE_MODULE_H_

#include <string>
#include "types.h"

namespace sim {
namespace engine {

class Simulator;

class Module {
private:
    /** Module ID */
    const unsigned id_;

    /** Cycle in which a module is scheduled to execute */
    cycles_t next_activity_;

    /** If a module is waken during end half-cycle it needs
     * to be executed in the next cycle, and still execute in
     * current cycle (if it is scheduled to)
     */
    bool execute_next_cycle_;

    /** Simulator associated to the Module */
    Simulator &simulator_;

    /** Returns the cycle in which a module is scheduled to execute
     * \return Next activity
     */
    engine::cycles_t get_next_activity();

    /** Returns if a module needs to execute in the next cycle
     * \return execute_next_cycle
     */
    bool get_execute_next_cycle();

    /** Schedules a module to execute in the next cycle
     * (used only when a module is waken up during end half-cycle
     * \param Execute in next cycle
     */
    void set_execute_next_cycle(bool execute_next_cycle);

    /** Simulator is a friend class to avoid making the about methods and
     * set_module_activity public and accessible to any other class. This
     * is an exception, and only applies because Simulator is part of the
     * engine. In any case, Simulator must still only use the above method
     * and never access individual members directly.
     */
    friend class Simulator;

    /** Module type */
    const ModuleType module_type_;

protected:
    /** Requests a delayed call to the simulation engine
     * \param delay Number of cycles when the delayed call happens
     */
    void set_module_activity(engine::cycles_t delay);

    /** Instrumentation id */
    unsigned int instrumentation_id_;

public:
    explicit Module(Simulator &simulator, ModuleType module_type = UNKNOWN);
    virtual ~Module();

    /** Returns the module ID
     * \return Module ID
     */
    const unsigned get_id() const;
    const unsigned get_module_id() const;

    virtual const std::string get_name() const = 0;

    /** Returns the module type
     * \return Module Type
     */
    ModuleType get_module_type() const;

    /** Return the simulator associated to the module
     * \return Reference to the associated simulator
     */
    Simulator &get_simulator() const;

    virtual void notify_wakeup(void *caller_ptr);

    /** Resets the module
     \return True if the module should be executed the first cycle
     */
    virtual bool reset() = 0;

    /** Notifies the module that a new cycle has started
     */
    virtual void start() = 0;

    /** Notifies the module that a new cycle is about to end
     * \return Number of cycles when the module will have activity
     */
    virtual void end() = 0;

    /* Prints the hardward counters associated with the module */
    virtual void print_hardware_counters(engine::cycles_t time, bool propagate = false, unsigned overwrite_cpu_id = 0);

    /* Returns the instrumentation id assigned by the Instrumentation module. */
    const unsigned int get_instrumentation_id() const;

    /** Returns wether the module has executed instructions/phases (false for non-cpus). */
    virtual bool cpu_activity_poll();

    /** Prints status information. */
    virtual void print_status() const;
};

}  // namespace engine
}  // namespace sim

#include "Module-impl.h"

#endif  // CORE_ENGINE_MODULE_H_
