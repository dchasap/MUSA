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


#ifndef CORE_TRACE_TS_TYPES_H_
#define CORE_TRACE_TS_TYPES_H_

#include "core/trace/common/bbl.h"
#include "core/trace/common/mem.h"
#include "core/trace/common/mitem.h"
#include "core/trace/NameEvent.h"


namespace sim {
namespace trace {
namespace ts {


typedef NameEvent name_t;

struct subtrace_t {
    unsigned int wd_id;
    unsigned int name_id;
    int bbl_dict_id;
    int mem_trace_id;
    int bbl_trace_id;
    int wait_trace_id;
    uint64_t n_instr;
    int mem_trace_chunks;
    void *stack;

    subtrace_t() :
        bbl_dict_id(-1),
        mem_trace_id(-1),
        bbl_trace_id(-1),
        wait_trace_id(-1),
        n_instr(0),
        mem_trace_chunks(0) {}
};
std::ostream &operator<<(std::ostream &, const subtrace_t &);
std::istream &operator>>(std::istream &, subtrace_t &);

struct task_t {
    unsigned int wd_id;
    unsigned int stream_id;
};

std::ostream &operator>>(std::ostream &, const task_t &);
std::istream &operator<<(std::istream &, task_t &);

typedef enum {
    PHASE = 1, CREATE_TASK = 15, TASK_WAIT = 16, WAIT_ON = 17, TASK_NAME = 20, SET_LOCK = 21, UNSET_LOCK = 22
} type_t;

typedef enum : int {
    NO_SUBTRACE = -1,
    EMPTY_SUBTRACE = -2
} subtrace_id_t;

struct phase_t {
    unsigned int burst_id;
    int subtrace_id;
    uint64_t duration;
};

struct create_task_t {
    unsigned int wd_id;
};

struct task_wait_t {
};

struct wait_on_t {
    int subtrace_id;
};

struct lock_t {
    uint64_t address;
};

struct event_t {
    unsigned type;
    union {
        phase_t phase;
        create_task_t create_task;
        task_wait_t task_wait;
        wait_on_t wait_on;
        lock_t lock;
    } u;

    event_t();
};

std::ostream &operator<<(std::ostream &, const event_t &);
std::istream &operator>>(std::istream &, event_t &);

struct replay_event_t {
    unsigned wd_id;
    replay_event_t();
    explicit replay_event_t(unsigned id);
};

std::ostream &operator<<(std::ostream &, const replay_event_t &);
std::istream &operator>>(std::istream &, replay_event_t &);

}  // namespace ts
}  // namespace trace
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_TRACE_TS_TYPES_H_
