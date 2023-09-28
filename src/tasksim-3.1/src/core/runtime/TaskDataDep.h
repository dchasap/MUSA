/*
 * Copyright - Barcelona Supercomputing Center.
 * All rights reserved.
 *
 * Redistribution in source and binary forms, with or without
 * modification, is not permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef RUNTIME_TASKDATADEP_H_
#define RUNTIME_TASKDATADEP_H_

#include <list>
#include <map>
#include "core/engine/types.h"

namespace sim {
namespace runtime {

typedef enum {
    NONE = 0, IN = 1, OUT = 2, INOUT = 4, ANY_IN_OUT = 7, NON_IN_OUT = 8, ANY = 0x0f
} task_dep_t; /* Don't change values - support for bitwise AND */

class TaskDepConv {
public:
    static unsigned char pack(const task_dep_t);
    static task_dep_t unpack(const unsigned char);
};

const static std::map<std::string, task_dep_t> string_to_task_dep ({
    {"NONE",  task_dep_t::NONE},
    {"IN",    task_dep_t::IN},
    {"OUT",   task_dep_t::OUT},
    {"INOUT", task_dep_t::INOUT},
    {"ANY_IN_OUT",  task_dep_t::ANY_IN_OUT},
    {"NON_IN_OUT",  task_dep_t::NON_IN_OUT},
    {"ANY",   task_dep_t::ANY},
});
static const std::unordered_map<task_dep_t, std::string> task_dep_to_string ({
    {task_dep_t::NONE,  "NONE"},
    {task_dep_t::IN,    "IN"},
    {task_dep_t::OUT,   "OUT"},
    {task_dep_t::INOUT, "INOUT"},
    {task_dep_t::ANY_IN_OUT, "ANY_IN_OUT"},
    {task_dep_t::NON_IN_OUT, "NON_IN_OUT"},
    {task_dep_t::ANY,   "ANY"},
});
static const std::map<task_dep_t, std::size_t> task_dep_to_index ({
    {task_dep_t::IN,         0},
    {task_dep_t::OUT,        1},
    {task_dep_t::INOUT,      2},
    {task_dep_t::NON_IN_OUT, 3},
});

struct task_dep_entry_t {
    engine::addr_t start_addr_;
    engine::addr_t end_addr_;
    task_dep_t type_;
    std::size_t task_id_; /* id of the task to which this info belongs */
    bool concurrent_;

    task_dep_entry_t(engine::addr_t, engine::addr_t, task_dep_t, std::size_t, bool concurrent);
    task_dep_entry_t();
};

struct TaskDeps {
    typedef std::list<task_dep_entry_t> dep_list_t;
    dep_list_t entries_;
    task_dep_entry_t current_; /* used to store task inout info for the task parameter that is currently being set by the runtime */

    engine::cycles_t current_task_start_time_;
    typedef enum { RDY_FOR_TASK_TYPE, RDY_FOR_START_ADDR, RDY_FOR_SIZE, RDY_FOR_TYPE } TempTaskDepState;
    TempTaskDepState current_task_dep_state_;

    TaskDeps();

    void insert(); /* insert the current_ into the entries */
    void clear();
    bool is_task_dep(engine::addr_t, task_dep_t) const;
    bool is_concurrent(engine::addr_t) const;
    task_dep_t get_task_dep_type(engine::addr_t) const;
    std::size_t get_current_task_type() const;
    const dep_list_t::const_iterator get_concurrent_variable() const;

    static bool equal(task_dep_t, task_dep_t);
    void print() const;
};

}  // namespace engine
}  // namespace sim

#include "TaskDataDep-impl.h"

#endif /* RUNTIME_TASKDATADEP_H_ */
