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


#ifndef CORE_TRACE_OMPSS_TYPES_IMPL_H_
#define CORE_TRACE_OMPSS_TYPES_IMPL_H_

#include <string>
#include <vector>

namespace sim {
namespace trace {
namespace ompss {


inline nanos_phase_map_t::nanos_phase_map_t()
{
    // phase_names_ maps function names (except initial nanos_ and eventual final _) to their phase number
    // (as defined in Instrumentation's "api" dictionary) grab functions and corresponding "api" instrumentation keys
    // with for example: grep -rB2 'InstrumentStateAndBurst \+[a-zA-Z]\+ *( *"api" *,' <nanox>/src/
    // API key <-> phase number shamelessly imported from InstrumentationDictionary in
    // <nanox>/src/core/instrumentation.hpp

    // to ignore a phase completely (anciently "skip_list"),
    // do not add them to phase_names_ nor trace_phase_, e.g. current_wd and in_final
    // to not trace a function (anciently "whitelist"), set corresponding trace_phase_ to false

    phase_names_["find_slicer"] = 1;
    phase_names_["wg_wait_completion"] = 2;
    phase_names_["create_bool_sync_cond"] = 3;
    phase_names_["create_int_sync_cond"] = 3;
    phase_names_["sync_cond_wait"] = 4;
    phase_names_["sync_cond_signal"] = 5;
    phase_names_["destroy_sync_cond"] = 6;
    phase_names_["wait_on"] = 7;
    phase_names_["init_lock"] = 8;
    phase_names_["set_lock"] = 9;
    phase_names_["unset_lock"] = 10;
    phase_names_["try_lock"] = 11;
    phase_names_["destroy_lock"] = 12;
    phase_names_["single_guard"] = 13;
    phase_names_["team_barrier"] = 14;
    // phase_names_["current_wd"] = 15;
    phase_names_["get_wd_id"] = 16;
    phase_names_["create_wd_compact"] = 17;
    phase_names_["create_sliced_wd"] = 17;
    phase_names_["submit"] = 18;
    phase_names_["create_wd_and_run"] = 19;
    phase_names_["set_internal_wd_data"] = 20;
    phase_names_["get_internal_wd_data"] = 21;
    phase_names_["yield"] = 22;
    phase_names_["create_team"] = 23;
    phase_names_["enter_team"] = 24;
    phase_names_["leave_team"] = 25;
    phase_names_["end_team"] = 26;
    // phase_names_["get_addr"] = 27;
    phase_names_["copy_value"] = 28;
    phase_names_["omp_barrier"] = 29;
    phase_names_["get_starring_threads"] = 30;
    phase_names_["get_supporting_threads"] = 31;
    phase_names_["omp_find_worksharing"] = 32;
    phase_names_["omp_get_schedule"] = 33;
    phase_names_["malloc"] = 34;
    phase_names_["free"] = 35;
    phase_names_["get_num_ready_tasks"] = 36;
    phase_names_["get_num_total_tasks"] = 37;
    phase_names_["get_num_nonready_tasks"] = 38;
    phase_names_["get_num_blocked_tasks"] = 39;
    phase_names_["get_num_running_tasks"] = 40;
    phase_names_["dependence_pendant_writes"] = 41;
    // phase_names_["in_final"] = 42;
    phase_names_["set_final"] = 43;
    phase_names_["dependence_release_all"] = 44;
    phase_names_["set_translate_function"] = 45;

    trace_phase_[phase_names_["find_slicer"]] = false;
    trace_phase_[phase_names_["wg_wait_completion"]] = false;
    trace_phase_[phase_names_["create_bool_sync_cond"]] = false;  // *_create_sync_cond
    trace_phase_[phase_names_["sync_cond_wait"]] = false;
    trace_phase_[phase_names_["sync_cond_signal"]] = false;
    trace_phase_[phase_names_["destroy_sync_cond"]] = false;
    trace_phase_[phase_names_["wait_on"]] = false;
    trace_phase_[phase_names_["init_lock"]] = false;
    trace_phase_[phase_names_["set_lock"]] = false;
    trace_phase_[phase_names_["unset_lock"]] = false;
    trace_phase_[phase_names_["try_lock"]] = false;
    trace_phase_[phase_names_["destroy_lock"]] = false;
    trace_phase_[phase_names_["single_guard"]] = false;
    trace_phase_[phase_names_["team_barrier"]] = false;
    // trace_phase_[phase_names_["current_wd"]] = false;
    trace_phase_[phase_names_["get_wd_id"]] = false;
    trace_phase_[phase_names_["create_wd_compact"]] = false;  // *_create_wd
    trace_phase_[phase_names_["submit"]] = false;
    trace_phase_[phase_names_["create_wd_and_run"]] = false;
    trace_phase_[phase_names_["set_internal_wd_data"]] = false;
    trace_phase_[phase_names_["get_internal_wd_data"]] = false;
    trace_phase_[phase_names_["yield"]] = false;
    trace_phase_[phase_names_["create_team"]] = false;
    trace_phase_[phase_names_["enter_team"]] = false;
    trace_phase_[phase_names_["leave_team"]] = false;
    trace_phase_[phase_names_["end_team"]] = false;
    // trace_phase_[phase_names_["get_addr"]] = false;
    trace_phase_[phase_names_["copy_value"]] = false;
    trace_phase_[phase_names_["omp_barrier"]] = false;
    trace_phase_[phase_names_["get_starring_threads"]] = false;
    trace_phase_[phase_names_["get_supporting_threads"]] = false;
    trace_phase_[phase_names_["omp_find_worksharing"]] = false;
    trace_phase_[phase_names_["omp_get_schedule"]] = false;
    trace_phase_[phase_names_["malloc"]] = false;
    trace_phase_[phase_names_["free"]] = false;
    trace_phase_[phase_names_["get_num_ready_tasks"]] = false;
    trace_phase_[phase_names_["get_num_total_tasks"]] = false;
    trace_phase_[phase_names_["get_num_nonready_tasks"]] = false;
    trace_phase_[phase_names_["get_num_blocked_tasks"]] = false;
    trace_phase_[phase_names_["get_num_running_tasks"]] = false;
    trace_phase_[phase_names_["dependence_pendant_writes"]] = false;
    // trace_phase_[phase_names_["in_final"]] = false;
    trace_phase_[phase_names_["set_final"]] = false;
    trace_phase_[phase_names_["dependence_release_all"]] = false;
    trace_phase_[phase_names_["set_translate_function"]] = false;

    // OpenMP functions (added on-demand)
    // phase_names_["omp_init_lock"] =  OPENMP_LOCKS_PHASE;
    // phase_names_["omp_set_lock"] =  OPENMP_LOCKS_PHASE;
    // phase_names_["omp_unset_lock"] =  OPENMP_LOCKS_PHASE;
    // phase_names_["omp_destroy_lock"] =  OPENMP_LOCKS_PHASE;
    // phase_names_["omp_get_max_threads"] =  OPENMP_THREADS_PHASE;
    // phase_names_["omp_get_num_threads"] =  OPENMP_THREADS_PHASE;

    trace_phase_[WD_ID_PHASE] = false;
    trace_phase_[USER_CODE_PHASE] = true;
    // trace_phase_[OPENMP_LOCKS_PHASE] = false;
    // trace_phase_[OPENMP_THREADS_PHASE] = false;
};

inline bool nanos_phase_map_t::is_phase(std::string function_name) const {
    return phase_names_.find(function_name) != phase_names_.end();
}

inline unsigned nanos_phase_map_t::operator[](std::string function_name) const {
    return phase_names_.at(function_name);
}

inline bool nanos_phase_map_t::trace(unsigned phase) const {
    return (trace_phase_.find(phase) != trace_phase_.end()) and trace_phase_.at(phase);
}

inline
event_t::event_t() {}

inline
event_t::event_t(unsigned int wd_id, type_t type, unsigned value, uint64_t time) :
        wd_id_(wd_id), type_(type), value_(value), time_(time)
{}
inline
std::ostream &operator<<(std::ostream &os, const event_t &event)
{
    unsigned type = event.type_;
    os << event.wd_id_ << ":" << type << ":" << event.value_
       << ":" << event.time_;
    return os;
}

inline
std::istream &operator>>(std::istream &is, event_t &event)
{
    char sep;
    unsigned type;
    is >> event.wd_id_;
    is >> sep;
    assert(sep == ':');

    is >> type;
    event.type_ = static_cast<type_t>(type);
    is >> sep;
    assert(sep == ':');

    is >> event.value_;
    is >> sep;
    assert(sep == ':');

    is >> event.time_;
    return is;
}


inline
dep_t::dep_t() :
        wd_id_(0),
        dep_()
{
}

inline
dep_t::dep_t(unsigned id, const ts::dep_t& dep) :
        wd_id_(id),
        dep_(dep)
{
}

inline
void dep_t::write(std::ostream &os) const {
    os.write(reinterpret_cast<const char *>(&wd_id_), sizeof(wd_id_));
    dep_.write(os);
}

inline
void dep_t::read(std::istream &is) {
    is.read(reinterpret_cast<char *>(&wd_id_), sizeof(wd_id_));
    dep_.read(is);
}

inline
std::ostream &operator<<(std::ostream &os, const dep_t &dep) {
    os << dep.wd_id_ << ":" << dep.dep_;
    return os;
}

inline
std::istream &operator>>(std::istream &is, dep_t &dep) {
    char sep;
    is >> dep.wd_id_;
    is >> sep; assert(sep == ':');
    is >> dep.dep_;
    return is;
}


inline wd_info_t::wd_info_t() :
        wd_id_(0),
        phase_st_time_(0),
        active_(false),
        deps_(),
        phase_stack_()
{
}

inline wd_info_t::wd_info_t(unsigned id) :
        wd_id_(id),
        phase_st_time_(0),
        active_(false),
        deps_(),
        phase_stack_()
{
}


inline
void wd_info_t::write(std::ostream &os) const
{
    std::vector<ts::dep_t>::size_type num_deps = deps_.size();
    os.write(reinterpret_cast<const char *>(&wd_id_), sizeof(wd_id_));
    os.write(reinterpret_cast<const char *>(&num_deps), sizeof(num_deps));
    std::vector<ts::dep_t>::const_iterator i;
    for (i = deps_.begin(); i != deps_.end(); ++i) {
        i->write(os);
    }
}


inline
void wd_info_t::read(std::istream &is)
{
    std::vector<ts::dep_t>::size_type num_deps = 0;
    is.read(reinterpret_cast<char *>(&wd_id_), sizeof(wd_id_));
    is.read(reinterpret_cast<char *>(&num_deps), sizeof(num_deps));
    deps_.resize(num_deps);
    std::vector<ts::dep_t>::iterator i;
    for (i = deps_.begin(); i != deps_.end(); ++i) {
        i->read(is);
    }
}


inline
std::ostream &operator<<(std::ostream &os, const wd_info_t &wd)
{
    os << wd.wd_id_ << ":" << wd.deps_.size();
    std::vector<ts::dep_t>::const_iterator i;
    for (i = wd.deps_.begin(); i != wd.deps_.end(); ++i) {
        os << ":" << (*i);
    }
    return os;
}

inline
std::istream &operator>>(std::istream &is, wd_info_t &wd) {
    char sep;
    is >> wd.wd_id_;
    is >> sep; assert(sep == ':');
    std::vector<ts::dep_t>::size_type num_deps = 0;
    is >> num_deps;
    wd.deps_.resize(num_deps);
    for (unsigned i = 0; i < num_deps; i++) {
        is >> sep; assert(sep == ':');
        is >> wd.deps_[i];
    }
    return is;
}

}  // namespace ompss
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_OMPSS_TYPES_IMPL_H_
