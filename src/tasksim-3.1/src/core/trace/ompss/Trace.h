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


#ifndef CORE_TRACE_OMPSS_TRACE_H_
#define CORE_TRACE_OMPSS_TRACE_H_

#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

#include "nextsim/core/trace/FileTrace.h"
#include "nextsim/core/trace/MapEvent.h"
#include "nextsim/core/trace/TraceReader.h"
#include "nextsim/core/trace/TraceWriter.h"
#include "nextsim/core/trace/VectorEvent.h"
#include "nextsim/core/trace/ompss/types.h"


namespace sim {
namespace trace {
namespace ompss {

/**
 * Trace handles a set of streams to read and write events in an OmpSs trace
 */
template<template <typename> class StreamType, typename Store =  trace::FileTrace<StreamType, std::fstream>>
class Trace :
    private TraceReader<StreamType, Store> ,
    private TraceWriter<StreamType, Store> {
protected:
    typedef TraceReader<StreamType, Store> Reader;
    typedef TraceWriter<StreamType, Store> Writer;

    static const int Event_Stream_ = 0;   /**< stream holding events (e.g., burst, sync) */
    static const int Name_Stream_ = 1;    /**< stream holding the name of WD functions */
    static const int Wd_Info_Stream_ = 2; /**< stream holding information to create a WD */
    static const int Dep_Stream_ = 3;     /**< stream holding information about sync. dependencies (e.g., wait on) */

    Store store_;
    typedef MapEvent<std::string, ompss::name_t, &ompss::name_t::name_> name_stream_t;
    typedef MapEvent<unsigned, ompss::wd_info_t, &ompss::wd_info_t::wd_id_> wd_info_stream_t;
    typedef VectorEvent<ompss::dep_t> dep_stream_t;

public:
    explicit Trace(const char *filename, bool read_mode = false);
    virtual ~Trace();

    /** function to add an event */
    bool add_event(const ompss::event_t &);
    /** function to retrieve events sequentially (returns whether it succeeded or the end of stream was found) */
    bool get_next_event(ompss::event_t &) const;
    /** function to set the stream reading point to a specific event (using its ID) */
    void set_next_event(unsigned) const;
    /** function to reset the event stream */
    void reset_event() const;

    /** function to add a name of a WD function */
    bool add_task_name(const std::string &, unsigned &);
    /** function to query whether a WD has a name function */
    bool has_task_name(const std::string &) const;
    /** function to retrieve a task ID using its function name */
    unsigned get_task_id(const std::string &) const;
    /** function to retrieve the function name of a WD */
    const std::string get_task_name(unsigned) const;
    /** function to retrieve the total number of function names */
    unsigned get_number_of_task_name() const;

    /** function to add the information relative to a WD */
    ompss::wd_info_t &add_wd_info(unsigned);
    /** function to retrieve the information relative to a WD using its ID (cannot modify the WD information) */
    const ompss::wd_info_t &get_wd_info(unsigned) const;
    /** function to retrieve the information relative to a WD using its ID (can modify the WD information) */
    ompss::wd_info_t &get_wd_info(unsigned);
    /** function to retrieve the total number of WD information */
    unsigned get_number_of_wd_info() const;
    /** TESTING **/
    void flush_wd_contents();
    /** function to add a synchronization dependency */
    void add_dep(const ompss::dep_t &);
    /** function to retrieve a synchronization dependency */
    bool get_dep(ompss::dep_t &) const;
};

/**
 * Subclass of Trace that uses a file stream as storage
 */
template<template<typename> class StreamType>
using FileTrace = sim::trace::ompss::Trace<StreamType, sim::trace::FileTrace<StreamType, std::fstream>>;

}  // namespace ompss
}  // namespace trace
}  // namespace sim

#include "Trace-impl.h"

#endif  // CORE_TRACE_OMPSS_TRACE_H_
