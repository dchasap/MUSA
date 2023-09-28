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


#ifndef CORE_MODULES_MEMORY_CACHE_H_
#define CORE_MODULES_MEMORY_CACHE_H_

#include <string>
#include <utility>
#include <vector>

#include "core/engine/IModule.h"
#include "core/engine/types.h"
#include "core/logic/coherence/single/Directory.h"
#include "core/stats/Average.h"
#include "core/stats/Counter.h"
#include "core/stats/CounterMap.h"
#include "core/stats/VulnerabilityStat.h"
#include "core/utils/TimeList.h"
#include "CacheStats.h"

namespace sim {
namespace memory {

template<typename Message,
         template<typename> class CacheType,
         template<typename> class Upper,
         template<typename> class Lower = Upper>
class Cache : public engine::IModule<Message> {
protected:
    typedef typename Upper<Message>::Input Input;
    typedef typename Lower<Message>::Output Output;
    sim::logic::coherence::single::Directory<Message> &directory_;
    unsigned cache_id_;
    unsigned level_;
    CacheStats stats_;
    CacheType<Message> cache_;
    bool write_request_failed_;

    std::unique_ptr<sim::stats::VulnerabilityStat> vulnerability_;

    typedef std::pair<unsigned, typename Message::Request> buffer_entry_t;
    utils::TimeList<buffer_entry_t> input_buffer_;

    engine::cycles_t latency_;
    engine::cycles_t stall_start_cycle_;

public:
    /** Constructor: */
    Cache(sim::engine::Simulator &simulator, sim::logic::coherence::single::Directory<Message> &directory,
            sim::engine::Config cache_config, unsigned id);
    /** Destructor: */
    ~Cache();
    /** Module reset function: */
    bool reset();
    /** Start of cycle for the Cache module (1 time per cycle. */
    void start();
    /** End of cycle (may be called multiple times per cycle). */
    void end();
    /** Print Cache module hardware counters to the Paraver trace. */
    void print_hardware_counters(engine::cycles_t time, bool propagate = false,
            unsigned overwrite_cpu_id = 0);


    /** Inspects a request to check if we can accept it (always true for Caches). */
    bool inspect(const typename Message::Request &request);
    /** Inspects an akc to check if we can accept it (matches a pending request). */
    bool inspect(const typename Message::Ack &ack);

    /** Returns the module index amongst the Caches of the same level. */
    const unsigned get_id() const;
    /** Returns the level of the cache in the hierarchy. */
    const unsigned get_level() const;
    /** Returns this Cache full name (with level). */
    const std::string get_name() const;

    /** Prints the current status of the Cache contents (debug purposes). */
    void print_status() const override;
    /** Input Inteconection vector from previous memory levels. */
    std::vector<Input> in_;
    /** Output connection to the next memory level. */
    Output out_;

protected:
    /** Calculates the next cycle this Cache can perform any pending action,
      * and adds it to the Simulator ready queue. */
    void set_next_cycle();
};

}  // namespace memory
}  // namespace sim

#include "Cache-impl.h"

#endif  // CORE_MODULES_MEMORY_CACHE_H_
