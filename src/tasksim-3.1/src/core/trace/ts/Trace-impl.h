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


#ifndef CORE_TRACE_TS_TRACE_IMPL_H_
#define CORE_TRACE_TS_TRACE_IMPL_H_

#include <cassert>
#include <string>
#include <vector>
#include "types.h"
#include "nextsim/core/utils/Log.h"
#include "nextsim/core/utils/macros.h"

namespace sim {
namespace trace {
namespace ts {

using sim::utils::Log;

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline Trace<StreamType, TraceInternal, Store>::Trace(const char *name, bool check_backwards_compatibility) :
        sim::trace::Trace<StreamType, Store>(this->store_),
        TraceInternal<StreamType, Store>(name),
        subtrace_count_(0)
{
    this->template register_event_stream<name_stream_t>(Name_Stream_);
    this->template register_event_stream<subtrace_stream_t>(Subtrace_Stream_);
    this->template register_event_stream<wd_info_stream_t>(Wd_Info_Stream_);
    this->template register_event_stream<task_stream_t>(Task_Stream_);
    this->template register_event_stream<bbl_dict_stream_t>(Bbl_Dict_Stream_, "dict");
    this->template register_event_stream<phase_stream_t>(Phase_Stream_, "phases");
    if (check_backwards_compatibility and
            std::getenv(Trace::TS_TRACE_COMPATIBILITY_MODE.c_str()) != NULL and
            std::stoi(std::getenv(Trace::TS_TRACE_COMPATIBILITY_MODE.c_str())) == 1) {
        std::cout << "[DEBUG] Using old trace format compatibility mode." << std::endl;
    } else {
        this->template register_event_stream<user_code_data_stream_t>(User_Code_Data_Stream_, "phase_data");
    }

    this->template register_event_stream<wd_replay_stream_t>(Wd_Replay_Stream_, "wd_replay");
    this->template register_event_stream<void_stream_t>(Reserved_1_, "void");
    this->template register_event_stream<void_stream_t>(Reserved_2_, "void");
    this->template register_event_stream<void_stream_t>(Reserved_3_, "void");
    this->template register_event_stream<void_stream_t>(Reserved_4_, "void");
    this->template register_event_stream<void_stream_t>(Reserved_5_, "void");

    build_subtrace_map();
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline Trace<StreamType, TraceInternal, Store>::~Trace()
{
    for (unsigned i = 0; i < Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).size(); i++) {
        const ts::subtrace_t &sub = Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(i);
        assert(sub.mem_trace_chunks == count_phase_ends_[i]);
        _unused(sub);
    }
    this->flush_event_streams();
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline void Trace<StreamType, TraceInternal, Store>::build_subtrace_map()
{
    // std::cout << "[DEBUG] Subtrace map." << std::endl;
    for (unsigned i = 0; i < Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).size(); i++) {
        const ts::subtrace_t &sub = Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(i);
        count_phase_ends_[i] = sub.mem_trace_chunks;
        subtrace_map_[sub.wd_id] = i;
        // std::cout << "[DEBUG] " << i << " - WD " << sub.wd_id << " subtrace " << sub << std::endl;
        /* Register BBL dictionary, trace and memory trace if exist */
        if (sub.bbl_dict_id >= 0 && sub.bbl_dict_id != static_cast<int>(Bbl_Dict_Stream_))
             this->template register_event_stream<bbl_dict_stream_t>(sub.bbl_dict_id, "dict");
        if (sub.bbl_trace_id >= 0)
            this->template register_event_stream<Event<bbl_id_t>>(sub.bbl_trace_id, "bbl");
        if (sub.mem_trace_id >= 0)
            this->template register_event_stream<Event<mem_t>>(sub.mem_trace_id, "mem");
        if (sub.wait_trace_id >= 0)
            this->template register_event_stream<Event<ts::wait_info_t>>(sub.wait_trace_id, "deps");
    }
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_task_name(unsigned name_id, const std::string &name)
{
    Writer::template stream<name_stream_t>(Name_Stream_).append(ts::name_t(name_id, name.c_str()));
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
bool Trace<StreamType, TraceInternal, Store>::has_task_name(unsigned name_id) const
{
    if (name_id >= Reader::template stream<name_stream_t>(Name_Stream_).size()) return false;
    if (Reader::template stream<name_stream_t>(Name_Stream_).get(name_id).get_name_length() == 0) return false;
    return true;
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const char *Trace<StreamType, TraceInternal, Store>::get_task_name(unsigned name_id) const
{
    return Reader::template stream<name_stream_t>(Name_Stream_).get(name_id).get_name();
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
unsigned Trace<StreamType, TraceInternal, Store>::get_number_of_task_names() const
{
    return Reader::template stream<name_stream_t>(Name_Stream_).size();
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
unsigned Trace<StreamType, TraceInternal, Store>::get_total_number_of_tasks() const
{
    return subtrace_map_.size();
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_wd_info(unsigned wd_id, const std::vector<ts::dep_t> &wd_info)
{
    assert(not Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).has_index(wd_id));
    Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).append({wd_id, {wd_info}});
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_wait_on_dependencies(unsigned wd_id,
                                                                       const std::vector<ts::dep_t> &deps)
{
    assert(subtrace_map_.find(wd_id) != subtrace_map_.end());
    subtrace_t &sub = Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_.at(wd_id));
    if (sub.wait_trace_id < 0) {
        unsigned wait_trace_id = this->get_number_of_streams();
        Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).
                get(subtrace_map_.at(wd_id)).wait_trace_id = wait_trace_id;
        this->template register_event_stream<Event<ts::wait_info_t>>(wait_trace_id, "deps");
        sub.wait_trace_id = wait_trace_id;
    }
    Writer::template stream<Event<ts::wait_info_t>>(sub.wait_trace_id).append({deps});
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_wait_on_dependencies(ts::subtrace_t &subtrace,
                                                                       const std::vector<ts::dep_t> &deps)
{
    if (subtrace.wait_trace_id < 0) {
        assert(subtrace_map_.find(subtrace.wd_id) != subtrace_map_.end());
        unsigned wait_trace_id = this->get_number_of_streams();
        Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).
                get(subtrace_map_.at(subtrace.wd_id)).wait_trace_id = wait_trace_id;
        this->template register_event_stream<Event<ts::wait_info_t>>(wait_trace_id, "deps");
        subtrace.wait_trace_id = wait_trace_id;
    }
    Writer::template stream<Event<ts::wait_info_t>>(subtrace.wait_trace_id).append({deps});
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
unsigned Trace<StreamType, TraceInternal, Store>::add_task(unsigned wd_id)
{
    Log::debug2() << "Adding new task " << wd_id;
    if (Reader::template stream<task_stream_t>(Task_Stream_).has_index(wd_id)) {
        return Reader::template stream<task_stream_t>(Task_Stream_).get_index(wd_id).stream_id;
    }
    unsigned stream_id = this->get_number_of_streams();

    ts::task_t task = {wd_id, stream_id};
    Writer::template stream<task_stream_t>(Task_Stream_).append(task);
    this->template register_event_stream<Event<ts::event_t>>(stream_id);
    return stream_id;
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
unsigned Trace<StreamType, TraceInternal, Store>::get_task_stream(unsigned wd_id) const
{
    assert(Reader::template stream<task_stream_t>(Task_Stream_).has_index(wd_id));
    unsigned ret = Reader::template stream<task_stream_t>(Task_Stream_).get_index(wd_id).stream_id;
    assert(ret > Task_Stream_);
    return ret;
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
bool Trace<StreamType, TraceInternal, Store>::add_phase_event(const ompss::event_t &e)
{
    return Writer::template stream<phase_stream_t>(Phase_Stream_).append(e);
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
bool Trace<StreamType, TraceInternal, Store>::get_next_phase_event(ompss::event_t &e) const
{
    return Reader::template stream<phase_stream_t>(Phase_Stream_).get_next(e);
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
template<typename EventType>
inline
bool Trace<StreamType, TraceInternal, Store>::add_task_event(unsigned wd_id, ts::type_t type,
                                                             const EventType &sub_event)
{
    unsigned stream;
    if (not Reader::template stream<task_stream_t>(Task_Stream_).has_index(wd_id)) {
        /* We have not created the task stream yet */
        stream = add_task(wd_id);
    } else {
        assert(Reader::template stream<task_stream_t>(Task_Stream_).has_index(wd_id));
        stream = Reader::template stream<task_stream_t>(Task_Stream_).get_index(wd_id).stream_id;
    }

    ts::event_t event;
    event.type = type;
    EventType *field = reinterpret_cast<EventType *>(&event.u);
    *field = sub_event;
    return Writer::template stream<Event<ts::event_t>>(stream).append(event);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
bool Trace<StreamType, TraceInternal, Store>::get_task_event(unsigned wd_id, ts::event_t &e)
{
    unsigned stream;
    if (not Reader::template stream<task_stream_t>(Task_Stream_).has_index(wd_id)) {
        std::cout << "[ERROR] Canot find Task Stream id for WD " << wd_id << std::endl;
        std::cout << "Task Stream size " << Reader::template stream<task_stream_t>(Task_Stream_).get_size() << std::endl;
        Reader::template stream<task_stream_t>(Task_Stream_).print_contents();
        assert(0);
        /* We have not created the task stream yet */
        stream = add_task(wd_id);
    } else {
        assert(Reader::template stream<task_stream_t>(Task_Stream_).has_index(wd_id));
        stream = Reader::template stream<task_stream_t>(Task_Stream_).get_index(wd_id).stream_id;
    }
    if (this->event_caches_.size() <= stream or this->event_caches_[stream] == NULL) {
        this->template register_event_stream< Event<ts::event_t> >(stream);
    }
    return Reader::template stream< Event<ts::event_t> >(stream).get_next(e);
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
bool Trace<StreamType, TraceInternal, Store>::get_wait_on_dependencies(const ts::subtrace_t &subtrace,
                                                                       std::vector<ts::dep_t> &deps)
{
    deps.clear();
    if (subtrace.wait_trace_id < 0) {
        return false;
    }
    unsigned stream = subtrace.wait_trace_id;
    if (this->event_caches_.size() <= stream or this->event_caches_[stream] == NULL) {
        this->template register_event_stream< Event<ts::wait_info_t> >(stream);
    }
    ts::wait_info_t wait_info;
    Reader::template stream<Event<ts::wait_info_t>>(stream).get_next(wait_info);
    deps = wait_info.deps_;
    return true;
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const std::vector<ts::dep_t>&
Trace<StreamType, TraceInternal, Store>::get_task_dependencies(const ts::create_task_t &tc) const
{
    assert(Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).has_index(tc.wd_id));
    return Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).get_index(tc.wd_id).info_.deps_;
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_bbl(const bbl_t &bbl)
{
    Internal::add_bbl(bbl, Bbl_Dict_Stream_);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
unsigned Trace<StreamType, TraceInternal, Store>::get_number_of_task_traces()
{
    return Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).size();
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::set_task_stack_ptr(unsigned wd_id, unsigned name_id, void* stack_ptr)
{
    /* Check if the subtrace already exists */
    if (subtrace_map_.find(wd_id) != subtrace_map_.end()) {
        ts::subtrace_t& subtrace =
                Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_[wd_id]);
        // assert(subtrace.stack == NULL);
        subtrace.stack = stack_ptr;
    } else {
        add_task_trace(wd_id, name_id, stack_ptr);
    }
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::set_task_instructions(unsigned wd_id, uint64_t n_instr)
{
    /* Check if the subtrace already exists */
    assert(subtrace_map_.find(wd_id) != subtrace_map_.end());

    ts::subtrace_t& subtrace =
            Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_.at(wd_id));
    subtrace.n_instr = n_instr;
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
uint64_t Trace<StreamType, TraceInternal, Store>::get_task_instructions(unsigned wd_id)
{
    /* Check if the subtrace already exists */
    assert(subtrace_map_.find(wd_id) != subtrace_map_.end());

    const ts::subtrace_t& subtrace =
            Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_.at(wd_id));
    return subtrace.n_instr;
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const ts::subtrace_t &Trace<StreamType, TraceInternal, Store>::add_task_trace(unsigned wd_id, unsigned name_id,
        void *stack)
{
    /* Check if the subtrace already exists */
    if (subtrace_map_.find(wd_id) != subtrace_map_.end()) {
        std::cout << "Calling add task trace to an already existing WD." << std::endl;
        return Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_[wd_id]);
    }
    // assert(subtrace_map_.find(wd_id) == subtrace_map_.end());

    /* Add subtrace */
    ts::subtrace_t subtrace;
    subtrace.wd_id = wd_id;
    subtrace.name_id = name_id;
    subtrace.bbl_dict_id = Bbl_Dict_Stream_;
    subtrace.mem_trace_id = -1;
    subtrace.bbl_trace_id = -1;
    subtrace.wait_trace_id = -1; /* We assume that there is no wait stream */
    subtrace.n_instr = 0;
    subtrace.mem_trace_chunks = 0;
    subtrace.stack = stack;
    /* Update subtrace map */
    subtrace_map_[wd_id] = Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).size();
    Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).append(subtrace);

    return Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_[wd_id]);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const ts::subtrace_t &Trace<StreamType, TraceInternal, Store>::add_detailed_trace(unsigned wd_id, unsigned name_id)
{
    /* Check if the subtrace already exists */
    if (subtrace_map_.find(wd_id) == subtrace_map_.end()) {
        add_task_trace(wd_id, name_id, nullptr);
    }
    ts::subtrace_t& subtrace = Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_[wd_id]);
    subtrace.mem_trace_chunks++;
    // Store the phase WD_ID in the replay stream:
    Writer::template stream<wd_replay_stream_t>(Wd_Replay_Stream_).append(replay_event_t(wd_id));

    return Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_[wd_id]);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const ts::subtrace_t &Trace<StreamType, TraceInternal, Store>::get_task_trace_by_id(unsigned id) const
{
    return Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(id);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const ts::subtrace_t &Trace<StreamType, TraceInternal, Store>::get_task_trace_by_wd_id(unsigned wd_id) const
{
    assert(subtrace_map_.find(wd_id) != subtrace_map_.end());
    return Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_.at(wd_id));
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
unsigned Trace<StreamType, TraceInternal, Store>::get_task_trace_id_by_wd_id(unsigned wd_id) const
{
    if (subtrace_map_.find(wd_id) == subtrace_map_.end()) {
        std::cout << "[ERROR] Could not find subtrace for wd " << wd_id << std::endl;
        for (const auto &elem : subtrace_map_) {
            std::cout << "WD " << elem.first << " position " << elem.second << std::endl;
        }
        return -1;
    } else {
        return subtrace_map_.at(wd_id);
    }
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
bool Trace<StreamType, TraceInternal, Store>::get_replay_wd_id(unsigned &wd_id)
{
    // Read the Work Descriptor Identifier for the computation phase  in the replay stream:
    replay_event_t replay_event;
    bool ret = Reader::template stream<wd_replay_stream_t>(Wd_Replay_Stream_).get_next(replay_event);
    wd_id = replay_event.wd_id;
    return ret;
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_bbl_trace(const ts::subtrace_t &subtrace, const bbl_id_t &bbl_id)
{
    subtrace_t &s = Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_.at(subtrace.wd_id));
    if (s.bbl_trace_id == -1) {
        s.bbl_trace_id = this->get_number_of_streams();
        this->template register_event_stream<Event<bbl_id_t>>(s.bbl_trace_id, "bbl");
    }
    Internal::add_bbl_trace(bbl_id, s.bbl_trace_id);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_mem_trace(const ts::subtrace_t &subtrace, const mem_t &addr)
{
    subtrace_t &s = Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).get(subtrace_map_.at(subtrace.wd_id));
    if (s.mem_trace_id == -1) {
        s.mem_trace_id = this->get_number_of_streams();
        this->template register_event_stream<Event<mem_t>>(s.mem_trace_id, "mem");
    }
    Internal::add_mem_trace(addr, s.mem_trace_id);
    if (addr.addr_ == 0) {
        assert(subtrace_map_.find(subtrace.wd_id) != subtrace_map_.end());
        count_phase_ends_[subtrace_map_.at(subtrace.wd_id)]++;
    }
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_mem_item_trace(const ts::subtrace_t &subtrace, const mitem_t &item)
{
    Internal::add_mem_item_trace(item, subtrace.mem_trace_id);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
bool Trace<StreamType, TraceInternal, Store>::has_burst_instr_trace(const ts::phase_t &p) const
{
    if (p.subtrace_id < 0) {
        return false;
    }
    if (Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(p.subtrace_id).bbl_trace_id == 0) {
        return false;
    }
    return true;
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
bool Trace<StreamType, TraceInternal, Store>::has_burst_mem_trace(const ts::phase_t &p) const
{
    if (p.subtrace_id < 0) {
        return false;
    }
    if (Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(p.subtrace_id).mem_trace_id == 0) {
        return false;
    }
    return true;
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const bbl_t &Trace<StreamType, TraceInternal, Store>::get_next_bbl(const ts::subtrace_t &subtrace)
{
    return Internal::get_next_bbl(subtrace.bbl_trace_id, subtrace.bbl_dict_id);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const bbl_t &Trace<StreamType, TraceInternal, Store>::get_bbl(const sim::trace::bbl_id_t id) const
{
    return Internal::get_bbl(id, Bbl_Dict_Stream_);
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
addr_t Trace<StreamType, TraceInternal, Store>::get_next_mem(const subtrace_t &subtrace) const
{
    return Internal::get_next_mem(subtrace.mem_trace_id);
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
mitem_t Trace<StreamType, TraceInternal, Store>::get_next_mem_item(const subtrace_t &subtrace) const
{
    return Internal::get_next_mem_item(subtrace.mem_trace_id);
}


template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::add_phase_data(const subtrace_t &subtrace, const sim::trace::phase_data_t &phase_data)
{
    unsigned wd_id = subtrace.wd_id;
    if (Reader::template stream<user_code_data_stream_t>(User_Code_Data_Stream_).has_index(wd_id)) {
        // We already had some data.
        Writer::template stream<user_code_data_stream_t>(User_Code_Data_Stream_).get_reference(wd_id).data.push_back(phase_data);
    } else {
        // We need to create a new one.
        Writer::template stream<user_code_data_stream_t>(User_Code_Data_Stream_).append(sim::trace::user_code_data_t(wd_id, phase_data));
    }
}

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
const sim::trace::user_code_data_t& Trace<StreamType, TraceInternal, Store>::get_user_code_data(unsigned wd_id)
{
    assert(Reader::template stream<user_code_data_stream_t>(User_Code_Data_Stream_).has_index(wd_id));
    return Reader::template stream<user_code_data_stream_t>(User_Code_Data_Stream_).get_index(wd_id);
}

template<template <typename> class StreamType, template<template <typename> class,
         typename Store> class TraceInternal, typename Store>
inline
void Trace<StreamType, TraceInternal, Store>::flush_wd_contents()
{
    for (unsigned i = 0; i < Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).size(); i++) {
        const ts::subtrace_t &subt = Reader::template stream<subtrace_stream_t>(Subtrace_Stream_).get(i);
        if (Reader::template stream<task_stream_t>(Task_Stream_).has_index(subt.wd_id)) {
            unsigned stream_id = Reader::template stream<task_stream_t>(Task_Stream_).get_index(subt.wd_id).stream_id;
            this->flush_event_stream(stream_id);
            store_.clear_stream(stream_id);
        }
        if (subt.mem_trace_id != -1) {
            this->flush_event_stream(subt.mem_trace_id);
            store_.clear_stream(subt.mem_trace_id);
        }
        if (subt.bbl_trace_id != -1) {
            this->flush_event_stream(subt.bbl_trace_id);
            store_.clear_stream(subt.bbl_trace_id);
        }
        if (subt.wait_trace_id != -1) {
            this->flush_event_stream(subt.wait_trace_id);
            store_.clear_stream(subt.wait_trace_id);
        }
    }
    Writer::template stream<subtrace_stream_t>(Subtrace_Stream_).flush_contents();
    count_phase_ends_.clear();
    subtrace_map_.clear();
    Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).flush_contents();
    Writer::template stream<user_code_data_stream_t>(User_Code_Data_Stream_).flush_contents();
}

}  // namespace ts
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_TS_TRACE_IMPL_H_
