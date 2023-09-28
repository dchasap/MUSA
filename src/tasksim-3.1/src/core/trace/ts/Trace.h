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


#ifndef CORE_TRACE_TS_TRACE_H_
#define CORE_TRACE_TS_TRACE_H_

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "nextsim/core/trace/FileTrace.h"
#include "nextsim/core/trace/CompressedFileTrace.h"
#include "nextsim/core/trace/TraceReader.h"
#include "nextsim/core/trace/TraceWriter.h"
#include "nextsim/core/trace/VectorEvent.h"
#include "nextsim/core/trace/MapEvent.h"
#include "nextsim/core/trace/types.h"
#include "nanos.h"
#include "types.h"
// Temporary untill rle types are included in common.
#include "nextsim/core/trace/rle/types.h"
#include "nextsim/core/trace/ts/types.h"
#include "nextsim/core/trace/ompss/types.h"


namespace sim {
namespace trace {
namespace ts {

/**
 * Trace defines a set of streams to manage an OmpSs/TaskSim event trace
 * The template parameter TraceInternal is used for the internal representation
 *  of task subtraces: whether they are instruction or memory access subtraces
 * Note: the terms WD and task are used interchangeably
 */
template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
class Trace : private TraceInternal<StreamType, Store> {
protected:
    static const unsigned Name_Stream_ = 0;     /**< stream holding the names of WD functions */
    static const unsigned Subtrace_Stream_ = 1; /**< stream holding the information of the other streams */
    static const unsigned Wd_Info_Stream_ = 2;  /**< stream holding the information relative to WDs */
    static const unsigned Task_Stream_ = 3;     /**< stream holding the trace events */
    static const unsigned Bbl_Dict_Stream_ = 4; /**< stream holding basic block dictionary */
    static const unsigned Phase_Stream_ = 5;    /**< stream holding phase informations of pin instrumented run */
    static const unsigned User_Code_Data_Stream_ = 6;  /**< stream holding user code stats per phase  */
    static const unsigned Wd_Replay_Stream_ = 7;  /**< stream holding user code stats per phase  */
    static const unsigned Reserved_1_ = 8;  /**< stream holding user code stats per phase  */
    static const unsigned Reserved_2_ = 9;  /**< stream holding user code stats per phase  */
    static const unsigned Reserved_3_ = 10;  /**< stream holding user code stats per phase  */
    static const unsigned Reserved_4_ = 11;  /**< stream holding user code stats per phase  */
    static const unsigned Reserved_5_ = 12;  /**< stream holding user code stats per phase  */
    static const std::string TS_TRACE_COMPATIBILITY_MODE;
    int next_stream_;
    unsigned subtrace_count_;
    using TraceInternal<StreamType, Store>::store_;

    typedef TraceReader<StreamType, Store> Reader;
    typedef TraceWriter<StreamType, Store> Writer;
    typedef TraceInternal<StreamType, Store> Internal;

    typedef VectorEvent<ts::name_t> name_stream_t;
    typedef MapEvent<unsigned, ts::subtrace_t, &ts::subtrace_t::wd_id> subtrace_stream_t;
    typedef MapEvent<unsigned, ts::wd_info_event_t, &ts::wd_info_event_t::wd_id_> wd_info_stream_t;
    typedef MapEvent<unsigned, ts::task_t, &ts::task_t::wd_id> task_stream_t;
    typedef VectorEvent<bbl_t> bbl_dict_stream_t;
    typedef Event<sim::trace::ompss::event_t> phase_stream_t;
    typedef MapEvent<unsigned, sim::trace::user_code_data_t,
            &sim::trace::user_code_data_t::wd_id_> user_code_data_stream_t;
    typedef Event<replay_event_t> wd_replay_stream_t;
    typedef VectorEvent<replay_event_t> void_stream_t;

    typedef std::map<unsigned, int> subtrace_map_t;
    subtrace_map_t subtrace_map_;
    std::map<unsigned, int> count_phase_ends_;
    void build_subtrace_map();

public:
    explicit Trace(const char *name, bool check_backwards_compatibility = false);
    ~Trace();

    /** adds the name of a WD function using its ID */
    void add_task_name(unsigned, const std::string &);
    /** queries whether a WD has a function name using its ID */
    bool has_task_name(unsigned) const;
    /** retrieves the WD function name using the WD ID */
    const char *get_task_name(unsigned) const;
    /** retrieves the total number of WD function names */
    unsigned get_number_of_task_names() const;
    /** retrieves the number of WDs for a given function name */
    unsigned get_number_of_tasks(unsigned) const;
    /** retrieves the total number of WDs */
    unsigned get_total_number_of_tasks() const;

    /** adds the information relative to a WD using its ID */
    void add_wd_info(unsigned, const std::vector<ts::dep_t> &);
    /** adds the information relative to a synchronization dependency */
    void add_wait_on_dependencies(unsigned wd_id, const std::vector<ts::dep_t> &);
    /** adds the information relative to a synchronization dependency */
    void add_wait_on_dependencies(ts::subtrace_t &, const std::vector<ts::dep_t> &);

    /** adds a new task using its WD id */
    unsigned add_task(unsigned);
    /** retrieves the stream ID for a given WD */
    unsigned get_task_stream(unsigned) const;

    /** adds a new task using its WD id */
    bool add_phase_event(const ompss::event_t &);
    /** function to retrieve events sequentially (returns whether it succeeded or the end of stream was found) */
    bool get_next_phase_event(ompss::event_t &) const;

    /** adds a new event to a WD */
    template<typename EventType>
    bool add_task_event(unsigned, ts::type_t, const EventType &);
    /** retrieves OmpSs events sequentially from a given WD id */
    bool get_task_event(unsigned, ts::event_t &);

    /** retrieves the dependencies of a 'wait on' synchronization */
    bool get_wait_on_dependencies(const ts::subtrace_t &, std::vector<ts::dep_t> &);
    /** retrieves the dependencies of a given WD */
    const std::vector<ts::dep_t> &get_task_dependencies(const ts::create_task_t &) const;

    /** retrieves the number of available subtraces */
    unsigned get_number_of_task_traces();
    /** adds a new task subtrace */
    const ts::subtrace_t &add_task_trace(unsigned, unsigned, void * stack = NULL);
    /** adds a new detailed (e.g. memory trace) for a task subtrace */
    const ts::subtrace_t &add_detailed_trace(unsigned, unsigned);
    /** sets its stack pointer to a task subtrace */
    void set_task_stack_ptr(unsigned, unsigned, void *);
    /** sets the number of instructions in a task. */
    void set_task_instructions(unsigned wd_id, uint64_t n_instr);
    /** gets the number of instructions of a task */
    uint64_t get_task_instructions(unsigned wd_id);
    /** retrieves a task subtrace using its ID */
    const ts::subtrace_t &get_task_trace_by_id(unsigned) const;
    /** retrieves a task subtrace using its function name and call ID (ID for a given call to a WD function name) */
    const ts::subtrace_t &get_task_trace_by_wd_id(unsigned) const;
    /** retrieves a task ID using its function name and call ID (ID for a given call to a WD function name) */
    unsigned get_task_trace_id_by_wd_id(unsigned) const;
    /** Retrieves the Work Descriptor of the next executed computational phase. */
    bool get_replay_wd_id(unsigned &wd_id);

    /** adds a new basic block */
    void add_bbl(const bbl_t &);
    /** adds the execution instance of a basic block using its ID in a given task using its subtrace */
    void add_bbl_trace(const ts::subtrace_t &, const bbl_id_t &);
    /** retrieves basic blocks sequentially from a given task using its subtrace */
    const bbl_t &get_next_bbl(const ts::subtrace_t &);
    /** retrieves the basic block with the index requested */
    const bbl_t &get_bbl(const sim::trace::bbl_id_t id) const;

    /** adds a new memory access in a given task using its subtrace */
    void add_mem_trace(const ts::subtrace_t &, const mem_t &);
    /** adds a new memory access trace item in a given task using its subtrace */
    void add_mem_item_trace(const ts::subtrace_t &, const mitem_t&);
    /** retrieves memory accesses sequentially from a given task using its subtrace */
    addr_t get_next_mem(const ts::subtrace_t &) const;
    /** retrieves memory access trace items sequentially from a given task using its subtrace */
    mitem_t get_next_mem_item(const subtrace_t &subtrace) const;

    /** queries whether a given task burst has an instruction trace */
    bool has_burst_instr_trace(const ts::phase_t &) const;
    /** queries whether a given task burst has a memory access trace */
    bool has_burst_mem_trace(const ts::phase_t &) const;

    /** Adds a phase_data_t to the vector of the associated wd.*/
    void add_phase_data(const ts::subtrace_t &, const sim::trace::phase_data_t &data);
    /** Get the user_code_data_t for the associated wd.*/
    const sim::trace::user_code_data_t& get_user_code_data(unsigned wd_id);
    /** TESTING **/
    void flush_wd_contents();
};

template<template <typename> class StreamType, template <template <typename> class,
         typename Store> class TraceInternal, typename Store>
const std::string Trace<StreamType, TraceInternal, Store>::TS_TRACE_COMPATIBILITY_MODE("TS_TRACE_COMPATIBILITY_MODE")
;
/**
 * Subclass of Trace that uses a file stream as storage
 */
template<template <typename> class StreamType,
         template<template <typename> class, typename> class TraceInternal,
         typename SelectedFileType = trace::SelectedFileTrace<StreamType, std::fstream>>
using FileTrace =  Trace<StreamType, TraceInternal, SelectedFileType>;

/*
template<template<typename> class StreamType,
         template<template <typename> class, typename> class TraceInternal,
         typename Storage = std::fstream,
         typename SelectedFileType = trace::SelectedFileTrace<StreamType, Storage>>
class FileTrace : public Trace<StreamType, TraceInternal, SelectedFileType> {
public:
    explicit FileTrace(const char *file_name) :
        trace::Trace<StreamType, SelectedFileType>(this->store_),
        Trace<StreamType, TraceInternal, SelectedFileType>(file_name, false)

    {};
};
*/
/**
 * Subclass of Trace that uses an input file stream as storage (read-only)
 */
template<template <typename> class StreamType,
         template<template <typename> class, typename> class TraceInternal,
         typename SelectedFileType = trace::SelectedFileTrace<StreamType, std::ifstream>>
using FileTraceReader =  Trace<StreamType, TraceInternal, SelectedFileType>;
/*
template<template <typename> class StreamType,
         template<template <typename> class, typename> class TraceInternal,
         typename Storage = std::ifstream,
         typename SelectedFileType = trace::SelectedFileTrace<StreamType, Storage>>
class FileTraceReader : public Trace<StreamType, TraceInternal, SelectedFileType> {
public:
    explicit FileTraceReader(const char *file_name) :
        trace::Trace<StreamType, SelectedFileType>(this->store_),
        Trace<StreamType, TraceInternal, SelectedFileType>(file_name, true)

    {};
};
*/
}  // namespace ts
}  // namespace trace
}  // namespace sim

#include "Trace-impl.h"

#endif  // CORE_TRACE_TS_TRACE_H_
