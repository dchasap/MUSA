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


#ifndef CORE_MODULES_MEMORY_PERFECT_H_
#define CORE_MODULES_MEMORY_PERFECT_H_

#include <list>
#include <deque>
#include <string>
#include <vector>

#include "core/engine/types.h"
#include "core/engine/IModule.h"
#include "core/modules/memory/types.h"
#include "core/utils/TimeList.h"
#include "core/stats/VulnerabilityStat.h"

namespace sim {

namespace engine {
class Simulator;
class Config;
}

namespace memory {

template<typename Message, template<typename> class Interconnect>
class Perfect : public engine::IModule<Message> {
public:
    typedef typename Interconnect<Message>::Input Input;
    /** Perfect DRAM constructor: */
    Perfect(sim::engine::Simulator &sim, engine::Config config, unsigned id  = 0);
    /** Perfect DRAM destructor: */
    ~Perfect();
    /** Module reset function implementation. */
    bool reset();
    /** Module start function (send ACKs). */
    void start();
    /** Module end function (process Requests). */
    void end();
    /** Returns the module name. */
    const std::string get_name() const;
    /** Inspects incoming Requests (always accept). */
    bool inspect(const typename Message::Request &request);
    /** Inspects incoming Requests (always accept, but not used). */
    bool inspect(const typename Message::Ack &ack);
    /** Prints the hardware counters associated to the module. */
    void print_hardware_counters(sim::engine::cycles_t time, bool propagate = false, unsigned overwrite_cpu_id = 0);
    /** Returns the number of input ports. */
    unsigned get_num_input_ports() const;
    /** Returns the index among all Modules of the same type. */
    unsigned get_id() const;
    /** Prints current Module status (deadlock detection). */
    void print_status() const override;

    /** Input connection. */
    Input in_;

private:
    /** Calculates the next cycle this RamulatorController can perform any pending action,
      * and adds it to the Simulator ready queue. */
    void set_next_cycle();

    typedef sim::utils::TimeList<typename Message::Request> pending_queue_t;
    /** List of Request being processed and their due cycle. */
    pending_queue_t pending_;

    /** Requests not yet processed. */
    std::list<typename Message::Request> input_buffer_;
    /** Maximum size of the pending Requests buffer. */
    size_t max_input_buffer_size_;

    /** Request latency [cycles]. */
    sim::engine::cycles_t latency_;
    /** Maximum number of Requests ot be processed concurrently. */
    unsigned max_concurrent_;
    /** Index among all Modules of the same type.*/
    unsigned id_;

    /** Perfect DRAM statistics. */
    RAMStats stats_;

    /** Last read input port (useful for the current multiport implementation. */
    unsigned last_read_port_;
    /** Last written output port (useful for the current multiport implementation. */
    unsigned last_written_port_;

    /** Indicates wether there are Requests blocked (limit reached). */
    bool blocked_;
    /** Indicates the last cycles where there were no blocked Requests. */
    sim::engine::cycles_t last_nonblocked_cycle_;
};

}  // namespace memory
}  // namespace sim

#include "Perfect-impl.h"

#endif  // CORE_MODULES_MEMORY_PERFECT_H_
