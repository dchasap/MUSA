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


#ifndef CORE_UTILS_INSTRUMENTATION_TYPES_H_
#define CORE_UTILS_INSTRUMENTATION_TYPES_H_

#include <string>
#include <memory>
#include "core/engine/types.h"
#include "ParaverDefs.h"
#include "MitosDefs.h"

namespace sim {
namespace utils {
namespace instrumentation {

enum instrumentation_masks {CPU_ID = 1, MEM_ID = 2, BUS_ID = 4};

struct State {
    engine::cycles_t end_time_stamp_;
    uint64_t value_;
};

struct Comm {
    engine::cycles_t send_start_time_stamp_;
    engine::cycles_t recv_invocation_time_stamp_;
    engine::cycles_t recv_start_time_stamp_;
    unsigned recv_cpu_id_;
    unsigned recv_thread_id_;
    uint64_t byte_size_;
    uint64_t tag_;
};

struct Event {
    uint64_t type_;
    uint64_t value_;
};

struct PreciseMemoryEvent {
		uint32_t op_;
		uint64_t ip_;
		uint64_t addr_;
		uint64_t latency_;
		uint64_t level_;
		uint64_t access_type_;	
};

enum RecordType { STATE_RECORD = 1, EVENT_RECORD = 2, COMM_RECORD = 3};

struct InstrumentationItem {
    unsigned cpu_id_;
    unsigned thread_id_;
    engine::cycles_t time_stamp_;
    RecordType record_type_;
    union Item {
        State state_;
        Comm comm_;
        Event event_;
				PreciseMemoryEvent pmem_event_;
        Item(State s) : state_(s) { }
        Item(Comm c) : comm_(c) { }
        Item(Event e) : event_(e) { }
				Item(PreciseMemoryEvent m) : pmem_event_(m) { }
        Item() { }
    } u;

    /* We might need it for item sorting.
     * It implements Paraver's expected input order.
    bool operator<( const InstrumentationItem &b)
    {
        if (this->time_stamp_ < b.time_stamp_) {
            return true;
        }
        else if (this->time_stamp_ == b.time_stamp_ and this->record_type_ < b.record_type_)
        {
            return true;
        }
        return false;
    }*/
};

std::ostream &operator<<(std::ostream &os, const InstrumentationItem &item);
std::istream &operator>>(std::istream &is, InstrumentationItem &item);

struct InstrumentationBuffer {
    std::shared_ptr<InstrumentationItem> items_;
    unsigned int max_elements_;
    unsigned int current_elements_;
    InstrumentationBuffer(): items_(NULL), max_elements_(0), current_elements_(0) { }
    // Necessary on C++11, corrected in C++14:
    // The lambda expression is necessary to call delete[] instead of delete:
    explicit InstrumentationBuffer(unsigned max_elements):
        items_(std::shared_ptr<InstrumentationItem>(
                new InstrumentationItem[ max_elements ], []( InstrumentationItem *p ) { delete[] p; })),
        max_elements_(max_elements),
        current_elements_(0) { }
};

#if 0
class InstrumentationInfo
{
//  private:
    /* The following fields are needed for Paraver tracing in order to create
     * the trace file header */
    /*unsigned num_applications_;
    unsigned num_processes_in_application_;
    unsigned num_nodes_;
    unsigned num_cores_per_node_;*/

    /** Number of components instrumented.
     *  This includes all software (threads) and
     *  hardware (caches, interconnects, memory)
     *  components in the system
     */
public:
    unsigned num_components_;
    std::string base_name_;
    std::string pcf_filename_;
};
#endif

typedef std::unordered_map< int, InstrumentationItem> CurrentStateHashmap;
static const int DEFAULT_BUFFER_SIZE = 512000;

/** Sampling policy:
 *  PERIODIC: hardware counters are periodically printed to the trace.
 *  PER_TASK: hardware counters are printer to the trace at the start and end of every task.
 *  DISABLED: Instrumentation trace will not be generated.
 */
enum class sampling_policy_t{
    PERIODIC = 0,
    PER_TASK = 1,
    DISABLED = 2
};

static const std::unordered_map<std::string, sampling_policy_t> sampling_modes({
        {"PERIODIC", sampling_policy_t::PERIODIC},
        {"PER_TASK", sampling_policy_t::PER_TASK},
        {"DISABLED", sampling_policy_t::DISABLED}
});

/** Counter distribution:
 *  PER_MODULE: Each simulated Module will have a pseudo-cpu in the isntrumentation trace.
 *  PER_TASK: Only CPUs will be present in the instrumentation file, the rest of hardware counters
 *            will be distributed to the appropiate cpu.
 */
enum class counter_distribution_t{
    PER_MODULE = 1,
    PER_TASK = 2
};

static const std::unordered_map<std::string, counter_distribution_t> distribution_modes({
    {"PER_MODULE", counter_distribution_t::PER_MODULE},
    {"PER_TASK", counter_distribution_t::PER_TASK}
});

}  // namespace instrumentation
}  // namespace utils
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_UTILS_INSTRUMENTATION_TYPES_H_
