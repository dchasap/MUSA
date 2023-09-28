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

#ifndef RUNTIME_TASKDATADEP_IMPL_H_
#define RUNTIME_TASKDATADEP_IMPL_H_

#include <cassert>

namespace sim {
namespace runtime {

inline
bool TaskDeps::equal(task_dep_t dep1, task_dep_t dep2)
{
    return ((static_cast<uint8_t>(dep1) & static_cast<uint8_t>(dep2)) != 0);
}

inline
unsigned char TaskDepConv::pack(const task_dep_t dep)
{
    switch (dep) {
        case task_dep_t::IN:
            return 1;
        case task_dep_t::OUT:
            return 2;
        case task_dep_t::INOUT:
            return 3;
        case task_dep_t::NON_IN_OUT:
            return 0;
        default: assert(0);
            return (unsigned char)-1;
    }
}


inline
task_dep_t TaskDepConv::unpack(const unsigned char dep)
{
    switch (dep) {
        case 0:
            return task_dep_t::NON_IN_OUT;
        case 1:
            return task_dep_t::IN;
        case 2:
            return task_dep_t::OUT;
        case 3:
            return task_dep_t::INOUT;
        default:
            assert(0);
            return task_dep_t::NONE;
    }
}

inline
task_dep_entry_t::task_dep_entry_t(engine::addr_t addr, engine::addr_t size, task_dep_t type, std::size_t task_id,
                                     bool concurrent) :
    start_addr_(addr),
    end_addr_(addr + size - 1),
    type_(type),
    task_id_(task_id),
    concurrent_(concurrent)
{
}


inline
task_dep_entry_t::task_dep_entry_t() :
    start_addr_(0),
    end_addr_(0),
    type_(task_dep_t::NONE)
{}


inline
TaskDeps::TaskDeps() :
    entries_(std::list<task_dep_entry_t>()),
    current_(task_dep_entry_t()),
    current_task_dep_state_(TempTaskDepState::RDY_FOR_TASK_TYPE)
{}


inline
void TaskDeps::insert()
{
    assert(current_.type_ != task_dep_t::NONE);
    entries_.push_back(current_);
}


inline
void TaskDeps::clear()
{
    entries_.clear();
}


inline
bool TaskDeps::is_task_dep(engine::addr_t addr, task_dep_t type) const
{
    return (get_task_dep_type(addr) & type) != 0;
}


inline
bool TaskDeps::is_concurrent(engine::addr_t addr) const
{
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        if ((*it).start_addr_ <= addr and (*it).end_addr_ >= addr and (*it).concurrent_) {
            return true;
        }
    }
    return false;
}


inline
task_dep_t TaskDeps::get_task_dep_type(engine::addr_t addr) const
{
    /* TODO: what if address belongs to more inputs */
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        if (((*it).start_addr_ <= addr) and ((*it).end_addr_ >= addr)) {
            return (*it).type_;
        }
    }
    return task_dep_t::NON_IN_OUT;
}

inline
std::size_t TaskDeps::get_current_task_type() const
{
    return current_.task_id_;
}


inline
const TaskDeps::dep_list_t::const_iterator TaskDeps::get_concurrent_variable() const
{
    for (dep_list_t::const_iterator it = entries_.begin(); it != entries_.end(); ++it) {
        if (it->concurrent_) {
            return it;
        }
    }
    return entries_.end();
}

}  // namespace runtime
}  // namespace sim

#endif /* RUNTIME_TASKDATADEP_IMPL_H_ */
