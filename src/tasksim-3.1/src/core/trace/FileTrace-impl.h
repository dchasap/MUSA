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


#ifndef CORE_TRACE_FILETRACE_IMPL_H_
#define CORE_TRACE_FILETRACE_IMPL_H_

#include <err.h>
#include <cassert>
#include <cstring>
#include <vector>
#include <string>

#include "nextsim/core/utils/Log.h"
#include "TextEventStream.h"
#include "BinaryEventStream.h"


namespace sim {
namespace trace {

using sim::utils::Log;


template<template<typename> class StreamType, typename T>
inline
FileTrace<StreamType, T>::file_t::file_t(T *file, int id) :
    active_id_(id), file_(file)
{
}


template<template<typename> class StreamType, typename T>
inline
FileTrace<StreamType, T>::pos_t::pos_t(uint64_t first, uint64_t last) :
        first_(first),
        last_(last)
{
}

template<template<typename> class StreamType, typename T>
inline
FileTrace<StreamType, T>::stream_t::stream_t(unsigned id,
        file_map_t &input_map, file_map_t &output_map) :
        id_(id),
        needs_update_(false),
        file_pos_(0),
        stream_pos_(offset_vector_.end()),
        input_(input_map.end()),
        output_(output_map.end())
{
}


template<template<typename> class StreamType, typename T>
inline
FileTrace<StreamType, T>::stream_t::stream_t(const stream_t &s) :
        id_(s.id_),
        file_(s.file_),
        offset_vector_(s.offset_vector_),
        needs_update_(s.needs_update_),
        file_pos_(s.file_pos_),
        stream_pos_(offset_vector_.begin()),
        input_(s.input_),
        output_(s.output_)
{
}

template<template<typename> class StreamType, typename T>
inline
void FileTrace<StreamType, T>::stream_t::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char *>(&id_), sizeof(id_));
    unsigned int name_len = file_.length();
    os.write(reinterpret_cast<const char *>(&name_len), sizeof(name_len));
    os.write(file_.c_str(), name_len);
    unsigned int offset_list_len = offset_vector_.size();
    os.write(reinterpret_cast<const char *>(&offset_list_len), sizeof(offset_list_len));
    os.write(reinterpret_cast<const char *>(&offset_vector_[0]), offset_list_len * sizeof(pos_t));
}

template<template<typename> class StreamType, typename T>
inline
void FileTrace<StreamType, T>::stream_t::read(std::istream &is)
{
    is.read(reinterpret_cast<char *>(&id_), sizeof(id_));
    unsigned int name_len = 0;
    is.read(reinterpret_cast<char *>(&name_len), sizeof(name_len));
    char *tmp_name = new char[name_len + 1];
    memset(tmp_name, 0, name_len + 1);
    is.read(tmp_name, name_len);
    tmp_name[name_len] = 0;  // End of string
    file_ = std::string(tmp_name);
    delete[] tmp_name;
    stream_pos_ = offset_vector_.end();
    unsigned int offset_list_len = 0;
    is.read(reinterpret_cast<char *>(&offset_list_len), sizeof(offset_list_len));
    if (offset_list_len == 0) {
        return;
    }
    offset_vector_.resize(offset_list_len);
    is.read(reinterpret_cast<char *>(&offset_vector_[0]), offset_list_len * sizeof(pos_t));
    stream_pos_ = offset_vector_.begin();
}

template<template<typename> class StreamType, typename T>
inline
FileTrace<StreamType, T>::FileTrace(const char *trace_name) :
        base_path_(std::string(trace_name).substr(0, std::string(trace_name).find_last_of('/') + 1)),
        file_name_(std::string(trace_name).substr(std::string(trace_name).find_last_of('/') + 1))

{
    /* Work around for the buggy NANOS memory allocator;
     * Note to self: do not implement something you do not know how to implement
     */
    stream_vector_.reserve(8);

    /* Open the meta-data trace file */
    std::string trace_file = base_path_ + file_name_ + std::string(".streaminfo");
    T trace(trace_file.c_str(), std::ios::in);
    if (trace.is_open() == false) {
        return;
    }

    std::string path = base_path_ + "/" + file_name_;
    stream_t file_stream(0, input_map_, output_map_);
    while (StreamType<stream_t>::read(trace, file_stream) == true) {
        /* Create input stream (if necessary) */
        stream_vector_.resize(file_stream.id_ + 1, stream_t(0, input_map_, output_map_));
        stream_vector_[file_stream.id_] = file_stream;
        Log::debug2() << "Detected stream #" << file_stream.id_ << " at file " << file_stream.file_.c_str();
        typename std::vector<pos_t>::const_iterator n;
        for (n = stream_vector_[file_stream.id_].offset_vector_.begin();
                n != stream_vector_[file_stream.id_].offset_vector_.end(); ++n) {
            Log::debug4() << "\t" << n->first_ << ":" << n->last_;
        }
        stream_vector_[file_stream.id_].stream_pos_ = stream_vector_[file_stream.id_].offset_vector_.begin();
    }

    trace.close();
}


template<template<typename> class StreamType, typename T>
inline
FileTrace<StreamType, T>::~FileTrace()
{
    flush_to_disk<T>();
    typename file_map_t::const_iterator i;
    for (i = input_map_.begin(); i != input_map_.end(); ++i) {
        delete i->second.file_;
    }
    for (i = output_map_.begin(); i != output_map_.end(); ++i) {
        delete i->second.file_;
    }
}

template<template<typename> class StreamType, typename T>
template<typename Local>
inline typename std::enable_if<is_base_of<Local, std::ostream>::value == true>::type
FileTrace<StreamType, T>::flush_to_disk()
{
    Log::debug3() << "Updating trace meta-data";
    typename std::vector<stream_t>::iterator i;
    /* Finish active streams (if any) */
    for (i = stream_vector_.begin(); i != stream_vector_.end(); ++i) {
        if (i->needs_update_ == false) {
            continue;
        }
        if (i->output_->second.active_id_ != static_cast<int>(i->id_)) {
            assert(i->offset_vector_.back().last_ != 0);
            continue;
        }
        Log::debug3() << "Finishing chunk in stream #" << i->id_;
        i->offset_vector_.back().last_ = i->output_->second.file_->tellp();
    }

    typename file_map_t::iterator out;
    for (out = output_map_.begin(); out != output_map_.end(); ++out) {
        out->second.file_->flush();
    }

    std::string trace_file = base_path_ + file_name_ + std::string(".streaminfo");
    T trace(trace_file.c_str(), std::ios::out | std::ios::trunc);
    if (trace.is_open() == false) {
        err(1, "Cannot open file %s", trace_file.c_str());
    }
    for (i = stream_vector_.begin(); i != stream_vector_.end(); ++i) {
        /* Write the meta-data information in the .streamfile */
        if (i->offset_vector_.empty()) {
            continue;
        }
        StreamType<stream_t>::write(trace, *i);
    }
    trace.close();
}


template<template<typename> class StreamType, typename T>
template<typename Local>
inline typename std::enable_if<is_base_of<Local, std::ostream>::value == false>::type
FileTrace<StreamType, T>::flush_to_disk()
{
}

template<template<typename> class StreamType, typename T>
inline typename FileTrace<StreamType, T>::file_t &
FileTrace<StreamType, T>::input_stream(unsigned stream_id) const
{
    assert(stream_id < stream_vector_.size());
    if (stream_vector_[stream_id].input_ == input_map_.end()) {
        Log::debug3() << "Initializing input file stream " << stream_id;
        assert(stream_vector_[stream_id].file_.empty() == false);
        typename file_map_t::iterator i = input_map_.find(stream_vector_[stream_id].file_);
        if (i == input_map_.end()) {
            std::string file_name = std::string(base_path_ + stream_vector_[stream_id].file_ + ".trace");
            Log::debug2() << "Opening file " << file_name.c_str();
            i = input_map_.insert(typename file_map_t::value_type(
                    stream_vector_[stream_id].file_,
                    file_t(new T(file_name.c_str(), std::ios::in)))).first;
        }
        stream_vector_[stream_id].input_ = i;

        /* Set the file pointer to the first element, or start of file if there are
         * no events in the stream
         */
        if (stream_vector_[stream_id].stream_pos_ == stream_vector_[stream_id].offset_vector_.end()) {
            stream_vector_[stream_id].file_pos_ = 0;
        } else {
            stream_vector_[stream_id].file_pos_ = stream_vector_[stream_id].stream_pos_->first_;
        }
        Log::debug3() << "Setting file pointer for #" << stream_id
                      << " to " << static_cast<uint64_t>(stream_vector_[stream_id].file_pos_);
    }

    /* Check if we need to set a new active stream */
    return stream_vector_[stream_id].input_->second;
}


template<template<typename> class StreamType, typename T>
inline typename FileTrace<StreamType, T>::file_t &
FileTrace<StreamType, T>::output_stream(unsigned stream_id)
{
    if (stream_id >= stream_vector_.size()) {
        stream_vector_.resize(stream_id + 1, stream_t(0, input_map_, output_map_));
    }

    if (stream_vector_[stream_id].output_ == output_map_.end()) {
        assert(stream_vector_[stream_id].file_.empty() == false);
        typename file_map_t::iterator i = output_map_.find(stream_vector_[stream_id].file_);
        if (i == output_map_.end()) {
            std::string file_name = std::string(base_path_ + stream_vector_[stream_id].file_ + ".trace");
            Log::debug2() << "Creating output stream #" << stream_id << " in file " << file_name.c_str();
            i = output_map_.insert(typename file_map_t::value_type(
                    stream_vector_[stream_id].file_,
                    file_t(new T(file_name.c_str(), std::ios::out | std::ios::app)))).first;
            if (i->second.file_->is_open() == false) {
                err(1, "Cannot open file %s", file_name.c_str());
            }
        }
        stream_vector_[stream_id].output_ = i;
    }

    /* The output pointer is not placed to the end of the file until a write happens */
    return stream_vector_[stream_id].output_->second;
}


template<template<typename> class StreamType, typename T>
inline
void FileTrace<StreamType, T>::register_event_stream(unsigned stream_id, const std::string &stream_name)
{
    /* Resize the stram_vector, if necessary */
    if (stream_id >= stream_vector_.size()) {
        stream_vector_.resize(stream_id + 1, stream_t(0, input_map_, output_map_));
    }
    if (stream_vector_[stream_id].id_ == 0) {
        stream_vector_[stream_id].file_ = file_name_ + "." + stream_name;
        stream_vector_[stream_id].id_ = stream_id;
        Log::debug3() << "Registering new stream #" << stream_id << " at file " << stream_vector_[stream_id].file_.c_str();
    } else {
        assert(stream_vector_[stream_id].file_ == file_name_ + "." + stream_name);
        assert(stream_vector_[stream_id].id_ == stream_id);
        Log::debug3() << "Registering existing stream #" << stream_id << " at file " << stream_name.c_str();
    }
}

template<template<typename> class StreamType, typename T>
inline
unsigned FileTrace<StreamType, T>::get_number_of_streams() const
{
    return stream_vector_.size();
}

template<template<typename> class StreamType, typename T>
inline
unsigned FileTrace<StreamType, T>::get_actual_number_of_streams() const
{
    return stream_vector_.size();
}

template<template<typename> class StreamType, typename T>
inline
unsigned FileTrace<StreamType, T>::get_number_of_files() const
{
    return output_map_.size();
}

template<template<typename> class StreamType, typename T>
inline
void FileTrace<StreamType, T>::clear_stream(unsigned stream_id)
{
}

template<template<typename> class StreamType, typename T>
template<typename Local, typename EventType>
inline typename std::enable_if<is_base_of<Local, std::ostream>::value == true, bool>::type
FileTrace<StreamType, T>::conditional_append(unsigned stream_id, const EventType &event)
{
    file_t &file = output_stream(stream_id);

    if (file.active_id_ != static_cast<int>(stream_id)) {
        if (file.active_id_ >= 0) {
            assert(file.active_id_ < static_cast<int>(stream_vector_.size()));
            stream_vector_[file.active_id_].offset_vector_.back().last_ =
                    file.file_->tellp();
            Log::debug3() << "Finishing chunk in stream #" << stream_id;
        }
        Log::debug3() << "Adding chunk to stream #" << stream_id;
        file.active_id_ = stream_id;
        stream_vector_[stream_id].needs_update_ = true;
        stream_vector_[stream_id].offset_vector_.push_back(pos_t(file.file_->tellp()));
    }

    return  StreamType<EventType>::write(*file.file_, event);
}

template<template<typename> class StreamType, typename T>
template<typename Local, typename EventType>
inline typename std::enable_if<is_base_of<Local, std::ostream>::value == false, bool>::type
FileTrace<StreamType, T>::conditional_append(unsigned stream_id, const EventType &event)
{
    errx(1, "Cannot append data to an input trace.");
    return 0;
}
template<template<typename> class StreamType, typename T>
template<typename EventType>
inline
bool FileTrace<StreamType, T>::append(unsigned stream_id, const EventType &event)
{
    return conditional_append<T, EventType>(stream_id, event);
}


template<template<typename> class StreamType, typename T>
template<typename EventType>
inline
bool FileTrace<StreamType, T>::get(unsigned stream_id, unsigned event_id, EventType &event) const
{
    if (seek(stream_id, event_id)) {
        return get_next(stream_id, event);
    }
    return false;
}


template<template<typename> class StreamType, typename T>
template<typename EventType>
inline
bool FileTrace<StreamType, T>::get_next(unsigned stream_id, EventType &event) const
{
    if (stream_id >= stream_vector_.size()) {
        return false;
    }
    file_t &file = input_stream(stream_id);
    stream_t &stream = stream_vector_[stream_id];
    /* Set the current event stream as active if necessary */
    if (file.active_id_ != static_cast<int>(stream_id)) {
        /* If there is an active stream, save the file pointer */
        if (file.active_id_ > 0) {
            Log::debug3() << "Deactivating stream #" << file.active_id_
                          << " at offset " << static_cast<uint64_t>(file.file_->tellg());
            stream_vector_[file.active_id_].file_pos_ = file.file_->tellg();
        }
        /* Set the current stream as active and position its file pointer */
        file.active_id_ = static_cast<int>(stream_id);
        file.file_->seekg(stream.file_pos_);
        Log::debug3() << "Activating stream #" << stream_id << " at offset " << static_cast<uint64_t>(stream.file_pos_);
    }

    if (stream.stream_pos_ == stream.offset_vector_.end()) {
        return false;
    }
    if (file.file_->tellg() >= static_cast<std::ios::pos_type>(stream.stream_pos_->last_)) {
        if (++stream.stream_pos_ == stream.offset_vector_.end()) {
            return false;
        }
        file.file_->seekg(stream.stream_pos_->first_);
    }
    Log::debug3() << "Reading at offset " << static_cast<uint64_t>(file.file_->tellg())
                  << " in chunk [" << stream.stream_pos_->first_ << " : " << stream.stream_pos_->last_ << "]";

    /* TODO: this is a quick hack -- We need to avoid reusing an event,
     * which can be easily the case. Ideally, the read call should
     * initialize the event, but an explicit constructor cannot be
     * called
     */
    EventType e;
    bool ret = StreamType<EventType>::read(*file.file_, e);
    if (ret == true) {
        event = e;
    }
    return ret;
}


template<template<typename> class StreamType, typename T>
inline
void FileTrace<StreamType, T>::reset(unsigned stream_id) const
{
    assert(stream_vector_.size() > stream_id);
    /* Set the logical position to the first chunk */
    stream_vector_[stream_id].stream_pos_ =
            stream_vector_[stream_id].offset_vector_.begin();
    /* Reset the file pointer too */
    if (stream_vector_[stream_id].offset_vector_.empty() == false) {
        stream_vector_[stream_id].file_pos_ =
            stream_vector_[stream_id].offset_vector_.front().first_;
    } else {
        stream_vector_[stream_id].file_pos_ = 0;
    }
    /* If this is the active task, also reset the file pointer */
    file_t &file = input_stream(stream_id);
    if (file.active_id_ == static_cast<int>(stream_id)) {
        file.file_->seekg(stream_vector_[stream_id].file_pos_);
    }
}


template<template<typename> class StreamType, typename T>
template<typename EventType>
inline
bool FileTrace<StreamType, T>::seek(unsigned stream_id, unsigned event_id) const
{
    if (stream_id >= stream_vector_.size()) {
        return false;
    }

    file_t &file = input_stream(stream_id);
    std::ios::pos_type offset = event_id * sizeof(EventType);
    typename std::vector<pos_t>::iterator pos = stream_vector_[stream_id].offset_vector_.begin();
    while (offset >= 0) {
        if (pos == stream_vector_[stream_id].offset_vector_.end()) return false;
        std::ios::pos_type delta = pos->last_ - pos->first_;
        if (delta > offset) {
            stream_vector_[stream_id].stream_pos_ = pos;
            stream_vector_[stream_id].file_pos_ = pos->first_ + offset;
            if (file.active_id_ == static_cast<int>(stream_id)) {
                file.file_->seekg(stream_vector_[file.active_id_].file_pos_);
            }
            return true;
        }

        pos++;
        offset -= delta;
    }

    return false;
}

}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_FILETRACE_IMPL_H_
