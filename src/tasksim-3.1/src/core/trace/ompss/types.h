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


#ifndef CORE_TRACE_OMPSS_TYPES_H_
#define CORE_TRACE_OMPSS_TYPES_H_

#if defined(_NANOS_INTERNAL)
#   include <nanos-int.h>
#else
#   include <nanox/nanos-int.h>
#endif

#include <map>
#include <stack>
#include <string>
#include <vector>
#include "nextsim/core/trace/NameEvent.h"
#include "nextsim/core/trace/ts/nanos.h"
#include "nextsim/core/utils/ReusableObject.h"

namespace sim {
namespace trace {
namespace ompss {

/** types for an OmpSs trace event */
typedef enum {
    PHASE_EVENT = 1, CREATE_TASK_EVENT = 15, WAIT_GROUP_EVENT = 16, WAIT_ON_EVENT = 17, TASK_NAME = 20, SET_LOCK = 21, UNSET_LOCK = 22
} type_t;

/** types for an OmpSs phases */
/** It expands uppon Nanos++ API indexes for phases */
typedef enum {
    WD_ID_PHASE = 10050,
    USER_CODE_PHASE = 10051,
    OPENMP_LOCKS_PHASE = 10052,
    OPENMP_THREADS_PHASE = 10053,
    DEFAULT_PHASE_ID = 0
} phase_id_t;

// Enum values changed to 100xx from xx on Feb2018 voiding backwards compatibility of traces.
// max phase existing in InstrumentationDictionary (<nanox>/src/core/instrumentation_decl.hpp) is now 50

struct nanos_phase_map_t {
    std::map<std::string, unsigned> phase_names_;
    std::map<unsigned, bool> trace_phase_;

    inline nanos_phase_map_t();
    inline bool is_phase(std::string) const;
    inline unsigned operator[](std::string) const;
    inline bool trace(unsigned) const;
};
/** struct representing an OmpSs trace event */
struct event_t {
    unsigned int wd_id_;
    type_t type_;
    unsigned value_;
    uint64_t time_;

    inline event_t();
    inline event_t(unsigned int wd_id, type_t type, unsigned value = 0, uint64_t time = 0);
};
/** function to write a event_t to an output stream */
std::ostream &operator<<(std::ostream &, const event_t &);
/** function to read a event_t from an input stream */
std::istream &operator>>(std::istream &, event_t &);

/** type of an OmpSs trace WD function name */
typedef NameEvent name_t;
/** type of an OmpSs trace synchronization dependency */
struct dep_t {
    unsigned wd_id_;
    ts::dep_t dep_;

    dep_t();
    dep_t(unsigned, const ts::dep_t&);
    void write(std::ostream &) const;
    void read(std::istream &);
};

/** function to write a dep_t to an output stream */
std::ostream &operator<<(std::ostream &, const dep_t &);
/** function to read a dep_t from an input stream */
std::istream &operator>>(std::istream &, dep_t &);

/** struct representing the information relative to an OmpSs trace WD */
struct wd_info_t : public utils::ReusableObject<wd_info_t> {
    unsigned wd_id_;
    uint64_t phase_st_time_;
    bool active_;
    std::vector<ts::dep_t> deps_;
    std::stack<unsigned int> phase_stack_;

    wd_info_t();
    explicit wd_info_t(unsigned id);
    void write(std::ostream &) const;
    void read(std::istream &);
};

/** function to write a wd_info_t to an output stream */
std::ostream &operator<<(std::ostream &, const wd_info_t &);
/** function to read a wd_info_t from an input stream */
std::istream &operator>>(std::istream &, wd_info_t &);

}  // namespace ompss
}  // namespace trace
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_TRACE_OMPSS_TYPES_H_
