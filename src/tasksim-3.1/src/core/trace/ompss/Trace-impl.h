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


#ifndef CORE_TRACE_OMPSS_TRACE_IMPL_H_
#define CORE_TRACE_OMPSS_TRACE_IMPL_H_

#include <string>
#include "nextsim/core/utils/macros.h"


namespace sim {
namespace trace {
namespace ompss {

template<template <typename> class StreamType, typename Store>
inline
Trace<StreamType, Store>::Trace(const char *name, bool read_mode) :
        trace::Trace<StreamType, Store>(store_),
        TraceReader<StreamType, Store>(store_),
        TraceWriter<StreamType, Store>(store_),
        store_(name)
{
    this->template register_event_stream<Event<ompss::event_t> >(Event_Stream_);
    this->template register_event_stream<name_stream_t>(Name_Stream_);
    if (not read_mode) {
        this->template register_event_stream<wd_info_stream_t>(Wd_Info_Stream_, "wd_info");
    }
    this->template register_event_stream<dep_stream_t>(Dep_Stream_);
}

template<template <typename> class StreamType, typename Store>
inline
Trace<StreamType, Store>::~Trace()
{
    this->flush_event_streams();
}


template<template <typename> class StreamType, typename Store>
inline
bool Trace<StreamType, Store>::add_event(const ompss::event_t &e)
{
    return Writer::template stream< Event<ompss::event_t> >(Event_Stream_).append(e);
}


template<template <typename> class StreamType, typename Store>
inline
bool Trace<StreamType, Store>::get_next_event(ompss::event_t &e) const
{
    return Reader::template stream< Event<ompss::event_t> >(Event_Stream_).get_next(e);
}


template<template <typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::set_next_event(unsigned event_id) const
{
    bool ret = Reader::template stream< Event<ompss::event_t> >(Event_Stream_).seek(event_id);
    assert(ret == true);
    _unused(ret);
}

template<template <typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::reset_event() const
{
    Reader::template stream< Event<ompss::event_t> >(Event_Stream_).reset();
}

template<template <typename> class StreamType, typename Store>
inline
bool Trace<StreamType, Store>::add_task_name(const std::string &label, unsigned &new_id)
{
    std::string new_label = label.substr(0, label.find_first_of('@'));
    if (Reader::template stream<name_stream_t>(Name_Stream_).has_index(new_label)) {
        new_id = Reader::template stream<name_stream_t>(Name_Stream_).get_index_id(new_label);
        return false;
    }

    new_id = Reader::template stream<name_stream_t>(Name_Stream_).size();
    Writer::template stream<name_stream_t>(Name_Stream_).append(ompss::name_t(new_id, new_label.c_str()));
    return true;
}


template<template <typename> class StreamType, typename Store>
inline
bool Trace<StreamType, Store>::has_task_name(const std::string &name) const
{
    return Reader::template stream<name_stream_t>(Name_Stream_).has_index(name);
}


template<template <typename> class StreamType, typename Store>
inline
unsigned Trace<StreamType, Store>::get_task_id(const std::string &name) const
{
    return Reader::template stream<name_stream_t>(Name_Stream_).get_index_id(name);
}


template<template <typename> class StreamType, typename Store>
inline
const std::string Trace<StreamType, Store>::get_task_name(unsigned id) const
{
    return Reader::template stream<name_stream_t>(Name_Stream_).get(id).get_name();
}


template<template <typename> class StreamType, typename Store>
inline
unsigned Trace<StreamType, Store>::get_number_of_task_name() const
{
    return Reader::template stream<name_stream_t>(Name_Stream_).size();
}


template<template <typename> class StreamType, typename Store>
inline
ompss::wd_info_t &Trace<StreamType, Store>::add_wd_info(unsigned id)
{

  if (Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).has_index(id)) {
      unsigned index = Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).get_index_id(id);
      return Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).get(index);
  }

  //Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).append(ompss::wd_info_t(id));
  ompss::wd_info_t my_info = ompss::wd_info_t(id);
  Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).append(my_info);

  unsigned index = Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).get_index_id(id);

ompss::wd_info_t &temp = Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).get(index);

  return temp;

//  return Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).get(index);
}


template<template <typename> class StreamType, typename Store>
inline
const ompss::wd_info_t &Trace<StreamType, Store>::get_wd_info(unsigned id) const
{
    assert(Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).has_index(id));
    return Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).get_index(id);
}


template<template <typename> class StreamType, typename Store>
inline
ompss::wd_info_t &Trace<StreamType, Store>::get_wd_info(unsigned id)
{
    assert(Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).has_index(id));
    unsigned index = Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).get_index_id(id);
    return Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).get(index);
}

template<template <typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::flush_wd_contents()
{
    Writer::template stream<wd_info_stream_t>(Wd_Info_Stream_).flush_contents();
}

template<template <typename> class StreamType, typename Store>
inline
unsigned Trace<StreamType, Store>::get_number_of_wd_info() const
{
    return Reader::template stream<wd_info_stream_t>(Wd_Info_Stream_).size();
}


template<template <typename> class StreamType, typename Store>
inline
void Trace<StreamType, Store>::add_dep(const ompss::dep_t &dep)
{
    Writer::template stream< Event<ompss::dep_t> >(Dep_Stream_).append(dep);
}


template<template <typename> class StreamType, typename Store>
inline
bool Trace<StreamType, Store>::get_dep(ompss::dep_t &dep) const
{
    return Reader::template stream<dep_stream_t>(Dep_Stream_).get_next(dep);
}

}  // namespace ompss
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_OMPSS_TRACE_IMPL_H_
