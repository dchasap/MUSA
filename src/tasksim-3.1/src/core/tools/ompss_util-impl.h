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

#ifndef CORE_TOOLS_OMPSS_UTIL_IMPL_H_
#define CORE_TOOLS_OMPSS_UTIL_IMPL_H_

#include <set>
#include <list>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/trace/ompss/Trace.h"
#include "core/trace/ts/Trace.h"
#include "core/utils/Log.h"

namespace sim {
namespace tools {

using sim::utils::Log;

template<template<typename> class TSType ,
         template <template <typename> class, typename> class InternalFormat>
inline void process_event_for_wd_id(sim::trace::ompss::event_t &event,
        sim::trace::ts::FileTrace<TSType, InternalFormat> &ts_trace,
        wait_on_map_t &wait_on_deps,
        std::unordered_map<unsigned, int> &subtrace_phase_count,
        const wd_name_map_t &wd_name_map,
        const sim::trace::ompss::nanos_phase_map_t &nanos_phase_map,
        bool nopin)
{
    int subtrace_id = 0;
    unsigned wd_id = event.wd_id_;

    if (wd_name_map.find(event.wd_id_) == wd_name_map.end()) {
        std::cout << "Could not find wd_name_map entry for wd_id " << event.wd_id_ << std::endl;
    }
    int16_t name_id = wd_name_map.at(event.wd_id_);
    switch (event.type_) {
        case sim::trace::ompss::PHASE_EVENT:
            if (nanos_phase_map.trace_phase_.find(event.value_) == nanos_phase_map.trace_phase_.end()) {
                break;
            }
            if ((nopin == false) and nanos_phase_map.trace_phase_.at(event.value_)) {
                subtrace_id = ts_trace.get_task_trace_id_by_wd_id(wd_id);
                unsigned phase_number = subtrace_phase_count[subtrace_id];
                const sim::trace::phase_data_t &phase_data = ts_trace.get_user_code_data(wd_id).data[phase_number];
                subtrace_phase_count[subtrace_id]++;
                if (phase_data.n_instrs == 0) {
                    // This user code phase has no associated instructions.
                    subtrace_id = sim::trace::ts::EMPTY_SUBTRACE;
                }
            } else {
                subtrace_id = sim::trace::ts::NO_SUBTRACE;
            }
            ts_trace.add_task_event(wd_id, sim::trace::ts::PHASE,
                    sim::trace::ts::phase_t({static_cast<unsigned>(event.value_), subtrace_id, event.time_}));
            break;
        case sim::trace::ompss::CREATE_TASK_EVENT:
            ts_trace.add_task_event(wd_id, sim::trace::ts::CREATE_TASK,
                                    sim::trace::ts::create_task_t({static_cast<unsigned>(event.value_)}));
            break;
        case sim::trace::ompss::WAIT_GROUP_EVENT:
            ts_trace.add_task_event(wd_id, sim::trace::ts::TASK_WAIT, sim::trace::ts::task_wait_t({}));
            break;
        case sim::trace::ompss::WAIT_ON_EVENT:
            if (nopin) {
                // Add trace (if it already exists, it returns it)
                sim::trace::ts::subtrace_t subtrace = ts_trace.add_task_trace(wd_id, name_id, NULL);
                // Add the wait on dependencies
                std::vector<sim::trace::ts::dep_t> deps;
                for (size_t i = 0; i < event.value_; i++) {
                    deps.push_back(wait_on_deps[wd_id].front());
                    wait_on_deps[wd_id].pop();
                }
                ts_trace.add_wait_on_dependencies(subtrace, deps);
            }
            subtrace_id = ts_trace.get_task_trace_id_by_wd_id(wd_id);
            ts_trace.add_task_event(wd_id, sim::trace::ts::WAIT_ON, sim::trace::ts::wait_on_t({subtrace_id}));
            break;
        case sim::trace::ompss::SET_LOCK:
            ts_trace.add_task_event(wd_id, sim::trace::ts::SET_LOCK, sim::trace::ts::lock_t({event.value_}));
            break;
        case sim::trace::ompss::UNSET_LOCK:
            ts_trace.add_task_event(wd_id, sim::trace::ts::UNSET_LOCK, sim::trace::ts::lock_t({event.value_}));
            break;
        default:
            std::abort();
    }
}

template<template<typename> class TSType ,
         template <template <typename> class, typename> class InternalFormat>
inline void check_phases(sim::trace::ts::FileTrace<TSType, InternalFormat> &ts_trace,
        std::unordered_map<unsigned, std::list<sim::trace::ompss::event_t>> &ompss_events_matrix,
        const sim::trace::ompss::nanos_phase_map_t &nanos_phase_map)
{
    // this function will go through the phases of ts_trace once and through ompss_events wd_count+1 number of times
    std::cout << "[OMPSS] Checking phases." << std::endl;
    // We sort the phases by wd id:
    std::unordered_map<unsigned, unsigned> event_count;
    std::unordered_map<unsigned, std::list<sim::trace::ompss::event_t>::iterator> event_pos;
    for (auto &wd_eventlist_pair: ompss_events_matrix) {
        event_count.insert(std::make_pair(wd_eventlist_pair.first, 0));
        event_pos.insert(std::make_pair(wd_eventlist_pair.first, wd_eventlist_pair.second.begin()));
    }

    std::unordered_map<unsigned, unsigned> wd_map;
    std::set<unsigned> assigned_wd;
    // We initialize the Main Task WD (is always the same)
    wd_map[1] = 1;
    assigned_wd.insert(1);
    sim::trace::ompss::event_t ts_e;
    int count = 0;
    while (ts_trace.get_next_phase_event(ts_e)) {
        Log::debug() << "Processing memory trace event " << ts_e;
        count++;
        unsigned ts_wd_id = ts_e.wd_id_;
        if (wd_map.find(ts_e.wd_id_) == wd_map.end()) {
            // With MUSA we do not have the master aliases on th map
            wd_map[ts_wd_id] = ts_wd_id;
        }

        unsigned ompss_wd_id = wd_map.at(ts_wd_id);
        if (event_count[ompss_wd_id]) {
            const sim::trace::ompss::event_t &past_ompss_event = *std::prev(event_pos[ompss_wd_id]);
            if (past_ompss_event.type_ == ts_e.type_ and past_ompss_event.value_ == ts_e.value_ and
                    ts_e.type_ == sim::trace::ompss::PHASE_EVENT and
                    (ts_e.value_ == nanos_phase_map["wait_on"] or
                     ts_e.value_ == nanos_phase_map["wg_wait_completion"])) {
                // Ignore this event for matching purposes.
                continue;
            }
        }

        // We get the next event on the ompss trace for that wd:
        if (event_count[ompss_wd_id] >= ompss_events_matrix[ompss_wd_id].size()) {
            continue;
            std::cerr << "[OMPSS ERROR] Work Descriptor " << ompss_wd_id << " is asking for its "
                      << 1+event_count[ompss_wd_id] << " event in Tasksim trace, but only has "
                      <<  ompss_events_matrix[ompss_wd_id].size() << " in the OmpSs trace" << std::endl;
            std::cerr << "OmpSs number of tasks: " << ompss_events_matrix.size() << std::endl;
            for (auto &wd_eventlist_pair: ompss_events_matrix) {
                std::cerr << " [wd " << wd_eventlist_pair.first << "] " << wd_eventlist_pair.second.size();
            }
            std::cerr << std::endl;

            std::cerr << "OmpSs event list:" << std::endl;
            unsigned int i = 0;
            for (auto &event: ompss_events_matrix[ompss_wd_id]) {
                std::cerr << i++ << " " << event << std::endl;
            }
            std::cerr << "Offending event " << ts_e << std::endl;
            while (ts_trace.get_next_phase_event(ts_e)) {
                if (ts_e.wd_id_ == ts_wd_id) {
                  std::cerr << " / " << ts_e << std::endl;
                }
            }

            std::cerr << "WD_ID map:" << std::endl;
            for (auto it = wd_map.begin(); it != wd_map.end(); it++) {
                std::cerr << it->first << " " << it->second << std::endl;
            }
            return;
        }
        const sim::trace::ompss::event_t &next_ompss_event = *event_pos[ompss_wd_id];
        Log::debug() << "Equivalent Nanox trace event " << next_ompss_event;
        // For MUSA MPI+OpenMP
        if (next_ompss_event.type_ != ts_e.type_ or next_ompss_event.value_ != ts_e.value_) {
            if (ts_e.type_ == sim::trace::ompss::PHASE_EVENT) {
                if (ts_e.value_ == 2 or ts_e.value_ == 18) {
                  // Either submit task or wait completion.
                  continue;
                }
            }
        }

        ++event_pos[ompss_wd_id];
        event_count[ompss_wd_id]++;

        // For locks, we trust the address of the ompss execution:
        if (next_ompss_event.type_ == ts_e.type_ and
                (next_ompss_event.type_ == sim::trace::ompss::SET_LOCK or
                 next_ompss_event.type_ == sim::trace::ompss::UNSET_LOCK)) {
            ts_e.value_ = next_ompss_event.value_;
        }

        // We check that they are the same event.
        if (next_ompss_event.type_ != ts_e.type_ or next_ompss_event.value_ != ts_e.value_) {
            // could be warning if phases can be sorted out by removing non-traced ones
            std::cerr << "[OMPSS ERROR] Events mismatch for event " << event_count[ompss_wd_id]
                      << " in Work Descriptor " << ompss_wd_id << ": " << ompss_wd_id << ":"
                      << next_ompss_event.type_ << ":" << next_ompss_event.value_ << " in OmpSs trace, but "
                      << ts_wd_id << ":" << ts_e.type_ << ":" << ts_e.value_
                      << " in the instrumentation trace" << std::endl;
            std::cerr << "OmpSs " << ompss_wd_id << " is mapped to DynamoRIO's " << ts_wd_id << std::endl;
            for (auto event_it = ompss_events_matrix[ompss_wd_id].begin();
                      event_it != std::prev(event_pos[ompss_wd_id]); ++event_it) {
                std::cerr << " - " << *event_it << std::endl;
            }
            std::cerr << "++ " << *std::prev(event_pos[ompss_wd_id]) << std::endl;
            for (auto event_it = event_pos[ompss_wd_id];
                      event_it != ompss_events_matrix[ompss_wd_id].end(); ++event_it) {
                std::cerr << " - " << *event_it << std::endl;
            }
            std::cerr << "Instrumentation events:" << std::endl;
            while (ts_trace.get_next_phase_event(ts_e)) {
                event_count[ompss_wd_id]++;
                if (ts_e.wd_id_ == ts_wd_id) {
                    std::cerr << " / " << ts_e << std::endl;
                } else {
                    std::cerr << " - " << ts_e << std::endl;
                }
            }
            return;
        }
        // If it is a task create event, we update the wd_map:
        if (ts_e.type_ == sim::trace::ompss::type_t::CREATE_TASK_EVENT) {
            // We check that we do not repeat map.
            assert(wd_map.find(ts_e.value_) == wd_map.end());
            assert(assigned_wd.find(next_ompss_event.value_) == assigned_wd.end());
            assigned_wd.insert(next_ompss_event.value_);
            wd_map[ts_e.value_] = next_ompss_event.value_;
        }
    }

    assert(event_count[0] == 0);
    for (auto &wd_eventlist_pair: ompss_events_matrix) {
        if (event_count[wd_eventlist_pair.first] != wd_eventlist_pair.second.size()) {
            std::cerr << "[OMPSS ERROR] Work Descriptor " << wd_eventlist_pair.first << " has only " << event_count[wd_eventlist_pair.first]
                      << " events in TaskSim trace, but more in OmpSs trace (" << wd_eventlist_pair.second.size()
                      << ")" << std::endl;
            for (auto &event: wd_eventlist_pair.second) {
                std::cerr << event << std::endl;
            }
            return;
        }
    }
    std::cout << "[OMPSS] OmpSs and Memory traces correlate properly." << std::endl;
}

template <template <typename> class OmpSsFormat>
void load_ompsstrace_events(const sim::trace::ompss::FileTrace<OmpSsFormat> &ompss_reader,
    std::unordered_map<unsigned, std::list<sim::trace::ompss::event_t>> &ompss_events_matrix,
    const sim::trace::ompss::nanos_phase_map_t &nanos_phase_map, wd_name_map_t &wd_name_map)
{
    std::unordered_map<unsigned, std::pair<bool, std::list<sim::trace::ompss::event_t>::reverse_iterator>> split_events;
    sim::trace::ompss::event_t event;
    unsigned prev_event_wd = 0, wd;

    for (; ompss_reader.get_next_event(event); prev_event_wd = wd) {
        wd = event.wd_id_;
        std::list<sim::trace::ompss::event_t> &wd_events = ompss_events_matrix[wd];
        // Special case: task names are not really events, they go to the name map instead of the wd's events trace.
        if (event.type_ == sim::trace::ompss::TASK_NAME) {
            wd_name_map.insert(std::make_pair(wd, event.value_));
            continue;
        }

        // Special case: Phase type has been disabled, so we ignore it by adding this phases' time to the last phase
        // in this wd, and marking it as split.
        if (event.type_ == sim::trace::ompss::PHASE_EVENT and
                nanos_phase_map.trace_phase_.find(event.value_) == nanos_phase_map.trace_phase_.end()) {
            for (auto it = wd_events.rbegin(); it != wd_events.rend(); ++it) {
                if (it->type_ == sim::trace::ompss::PHASE_EVENT) {
                    split_events[wd] = std::make_pair(true, it);
                    it->time_ += event.time_;
                    break;
                }
            }
            continue;
        }

        // Common case: A valid event to add to this WD's trace.

        // Check if it's the second half of a split phase. Split phases are either a very short phase with an event in
        // the middle, or a single phase that had another WD insert events or phases in the middle.
        // In both cases: report one phase (with length of both half-phases), add the event afterwards if there is one.

        /** Whether an event has split a phase */
        bool prev_event_split_phase = false;
        /** Reverse iterators pointing respectively to the split phase and the last event in this WD */
        auto split_phase = wd_events.rend(), last_wd_event = wd_events.rbegin();

        if (event.type_ == sim::trace::ompss::PHASE_EVENT and split_events.find(wd) != split_events.end()) {
            std::tie(prev_event_split_phase, split_phase) = split_events.at(wd);
        }
        if (prev_event_split_phase and event.type_ == split_phase->type_ and event.value_ == split_phase->value_) {
            // Fonund split by event: add time, don't add second half of phase
            assert(event.wd_id_ == split_phase->wd_id_);
            split_phase->time_ += event.time_;
            continue;
        } else if (prev_event_wd != wd and last_wd_event != wd_events.rend() and
                last_wd_event->type_ == event.type_ and last_wd_event->value_ == event.value_) {
            // Have found a long gone split phase (by a different WD) for no reason at all.
            last_wd_event->time_ += event.time_;
            continue;
        } else {
            wd_events.push_back(event);
            last_wd_event = wd_events.rbegin();
        }

        // check whether we added an event that split a phase.
        if (wd_events.size() < 2) {
            continue;
        }

        // split candidate is one-before-last, *last_wd_event is the event potentially splitting the phase in two.
        auto split_candidate = std::next(wd_events.rbegin());

        // phases that our events could have interrupted
        if (split_candidate->type_ != sim::trace::ompss::PHASE_EVENT) {
            prev_event_split_phase = false;
        } else if (last_wd_event->type_ == sim::trace::ompss::CREATE_TASK_EVENT) {
            prev_event_split_phase = (split_candidate->value_ == nanos_phase_map["submit"]);
        } else if (last_wd_event->type_ == sim::trace::ompss::WAIT_ON_EVENT) {
            prev_event_split_phase = (split_candidate->value_ == nanos_phase_map["wait_on"]);
        } else if (last_wd_event->type_ == sim::trace::ompss::WAIT_GROUP_EVENT) {
            prev_event_split_phase = (split_candidate->value_ == nanos_phase_map["wg_wait_completion"]);
        } else if (last_wd_event->type_ == sim::trace::ompss::SET_LOCK) {
            prev_event_split_phase = (split_candidate->value_ == nanos_phase_map["set_lock"]);
        } else if (last_wd_event->type_ == sim::trace::ompss::UNSET_LOCK) {
            prev_event_split_phase = (split_candidate->value_ == nanos_phase_map["unset_lock"]);
        } else {
            prev_event_split_phase = false;
        }

        // insert if phase if split, update if there already was info
        if (prev_event_split_phase or split_events.find(wd) != split_events.end()) {
            split_events[wd] = std::make_pair(prev_event_split_phase, split_candidate);
        }
    }
}


template <template <typename> class OmpSsFormat,
          template <typename> class TSFormat,
          template <template <typename> class, typename> class InternalFormat>
void merge_traces(const char* ompss_file_name, const char* ts_file_name)
{
    std::cout << "[MERGE] OMPSS: [" << ompss_file_name
              << "] and DETAILED: [" << ts_file_name << "]" << std::endl;
    // Open the temporary and final traces
    const sim::trace::ompss::FileTrace<OmpSsFormat> ompss_reader(ompss_file_name);
    sim::trace::ts::FileTrace<TSFormat, InternalFormat> ts_trace(ts_file_name);

    sim::trace::ompss::nanos_phase_map_t nanos_phase_map;
    wd_name_map_t wd_name_map;
    // Add main task name id and count, FIXME: add it in nanox plugin?

    std::cout << "[MERGE] Reading events" << std::endl;

    std::unordered_map<unsigned, std::list<sim::trace::ompss::event_t>> ompss_events_matrix;
    load_ompsstrace_events(ompss_reader, ompss_events_matrix, nanos_phase_map, wd_name_map);

    std::set<unsigned> wd_ids;
    size_t event_count = 0;
    for (auto &wd_eventlist_pair: ompss_events_matrix) {
        wd_ids.insert(wd_eventlist_pair.first);
        event_count += wd_eventlist_pair.second.size();
    }

    std::cout << "[MERGE] Read " << event_count << " events" << std::endl;

    bool nopin = ts_trace.get_total_number_of_tasks() == 0;
    wait_on_map_t wait_on_deps;
    // If ts_trace is empty (Pin was not executed), it should add wd_info and deps
    if (nopin) {
        std::cout << "[MERGE] Detailed trace not present." << std::endl;

        for (auto &wd_id_name_pair: wd_name_map) {
            ts_trace.add_task_trace(wd_id_name_pair.first, wd_id_name_pair.second, NULL);
        }

        // Process WD info
        assert(ompss_reader.get_number_of_wd_info() == wd_ids.size());
        std::cout << "[MERGE] Adding " << ompss_reader.get_number_of_wd_info()
                  << " work descriptors" << std::endl;
        for (auto it = wd_ids.begin(); it != wd_ids.end(); it++) {
            ts_trace.add_wd_info(*it, { std::vector<sim::trace::ts::dep_t>(ompss_reader.get_wd_info(*it).deps_) });
        }

        // Process wait on dependencies
        sim::trace::ompss::dep_t dep;
        unsigned dep_count = 0;
        while (ompss_reader.get_dep(dep)) {
            wait_on_map_t::iterator it = wait_on_deps.find(dep.wd_id_);
            if (it == wait_on_deps.end()) {
                // If the entry does not exist, we add it
                auto ret = wait_on_deps.insert(wait_on_map_t::value_type(dep.wd_id_, wait_on_map_t::mapped_type()));
                assert(ret.second);
                it = ret.first;
            }
            it->second.push(dep.dep_);
            ++dep_count;
        }
        if (dep_count > 0) {
            std::cout << "[MERGE] Processed " << dep_count << " wait_on dependencies" << std::endl;
        }
    } else {
        std::cout << "[MERGE] Merging Instrumentation trace." << std::endl;
        sim::tools::check_phases(ts_trace, ompss_events_matrix, nanos_phase_map);
    }

    std::cout << "[MERGE] Processing events" << std::endl;
    std::unordered_map<unsigned, int> subtrace_phase_count;

    event_count = 0;
    for (auto &wd_eventlist_pair : ompss_events_matrix) {
        for (auto &event: wd_eventlist_pair.second) {
            sim::tools::process_event_for_wd_id(event, ts_trace, wait_on_deps, subtrace_phase_count,
                    wd_name_map, nanos_phase_map, nopin);
            event_count++;
        }
    }

    std::cout << "[MERGE] Processed " << event_count << " events" << std::endl;

    if (not nopin) {
        for (unsigned i = 0; i < ts_trace.get_number_of_task_traces(); i++) {
            if (ts_trace.get_task_trace_by_id(i).mem_trace_chunks != subtrace_phase_count[i]) {
                std::cerr << "[MERGE][ERROR] Subtrace " << i << " has "
                          << ts_trace.get_task_trace_by_id(i).mem_trace_chunks
                          << " memory chunks but " << subtrace_phase_count[i]
                          << " phases point to it" << std::endl;
                std::cerr << "Subtrace info : " << ts_trace.get_task_trace_by_id(i)
                          << "." << std::endl;
            }
        }
    }

    // Add task names
    for (unsigned i = 0; i < ompss_reader.get_number_of_task_name(); i++) {
        if (ts_trace.has_task_name(i)) continue;
        ts_trace.add_task_name(i, ompss_reader.get_task_name(i));
    }

    std::cout << "[MERGE] Trace has " << ts_trace.get_number_of_task_names()
              << " task types" << std::endl;
    std::cout << "[MERGE] Trace has " << wd_ids.size() << " task instances" << std::endl;
    std::cout << "[MERGE] Merge done" << std::endl;
}

inline void rename_trace_files(const std::string &out_name, const std::string &final_name, const std::string path_prefix)
{
    if (out_name == final_name) {
        std::cout << "[DEBUG] Intermediate and final names are the same, no need to rename." << std::endl;
        return;
    }
    std::cout << "[MERGE] RENAMING " << std::endl
              << "[MERGE] out_name:   ["  << out_name   << "]" << std::endl
              << "[MERGE] final_name: ["  << final_name << "]" << std::endl;

    const std::vector<std::string> replace_sufix({".streaminfo", ".default", ".mem", ".phases", ".bbl", ".dict",".phase_data"});
    std::vector<bool> replaced(replace_sufix.size(), false);
    replaced[1] = true;

    std::string replace_full_stream_file     = path_prefix + "/" + out_name + ".streaminfo";
    std::string replace_full_mpi_phases_file = path_prefix + "/" + final_name + ".mpiphases";
    std::string new_full_stream_file      = path_prefix + "/" + final_name + ".streaminfo";
    std::string new_full_mpi_phases_file  = path_prefix + "/" + final_name + ".mpiphases";

    // UPDATE content from streaminfo file
    // substitution of old filename references.
    std::ifstream file_to_update(replace_full_stream_file);
    std::ofstream file_to_create(new_full_stream_file);
    std::string line;

    for (unsigned i = 0; i < replace_sufix.size(); i++) {
        Log::debug() << "[DEBUG] Searching for " << (out_name + replace_sufix[i])
                     << " to replace with " << (final_name + replace_sufix[i]);
    }
    while (std::getline(file_to_update, line)) {
        Log::debug() << "[DEBUG] Analyzing line " << line;
        for (unsigned i = 0; i < replace_sufix.size(); i++) {
            const std::string &sufix = replace_sufix[i];
            const std::string target = out_name + sufix;
            const std::string replacement = final_name + sufix;
            while (line.find(target) != std::string::npos) {
                line.replace(line.find(target), target.length(), replacement);
                replaced[i] = true;
            }
        }
        file_to_create << line << std::endl;
    }

    file_to_update.close();
    file_to_create.close();

    // Remove old version of stream file
    remove(replace_full_stream_file.c_str());
    // Perform the renaming on the detected files.
    for (unsigned i = 1; i < replaced.size(); i++) {
        if (replaced[i]) {
            const std::string old_name(path_prefix + "/" + out_name + replace_sufix[i] + ".trace");
            const std::string new_name(path_prefix + "/" + final_name + replace_sufix[i] + ".trace");
            std::cerr << "[DEBUG] Renaming " << old_name << " to " << new_name << std::endl;
            if (rename(old_name.c_str(), new_name.c_str()) != 0) {
                std::cerr << "Error renaming " << replace_sufix[i] << ".trace file" << std::endl;
                exit(-1);
            }
        }
    }
    // move mpiphases
    if (rename(replace_full_mpi_phases_file.c_str(), new_full_mpi_phases_file.c_str()) != 0) {
        std::cerr << "Error moving mpiphases from "
                  << replace_full_mpi_phases_file << " to "
                  << new_full_mpi_phases_file << std::endl;
        // exit(-1);
    }
    return;
}


}  // namespace tools
}  // namespace sim

#endif  // CORE_TOOLS_OMPSS_UTIL_IMPL_H_
