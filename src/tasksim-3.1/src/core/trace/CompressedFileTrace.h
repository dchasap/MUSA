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


#ifndef CORE_TRACE_COMPRESSEDFILETRACE_H_
#define CORE_TRACE_COMPRESSEDFILETRACE_H_

#include <cassert>
#include <map>
#include <memory>
#include <vector>
#include <string>

#include "nextsim/core/utils/CompressedStream.h"
#include "type_traits.h"

namespace sim {
namespace trace {

/**
 * CompressedFileTrace is a collection of event streams and defines a set of methods to read/write from/to them
 */
template<template<typename> class StreamType, typename T>
class CompressedFileTrace {
public:
    typedef T Stream;

private:
    std::string base_path_;
    std::string file_name_;
    T streaminfo_;
    struct file_t {
        int active_id_;
        std::shared_ptr<T> file_;
        std::unique_ptr<utils::CompressedStream<T>> buffer_;

        explicit file_t(T *file = NULL, int = -1);
        file_t(const file_t&);
        ~file_t();
        file_t operator=(const file_t&);
    };
    typedef std::map<const std::string, file_t> file_map_t;

    mutable file_map_t input_map_;
    file_map_t output_map_;

    struct pos_t {
        uint64_t first_;
        uint64_t last_;

        explicit pos_t(uint64_t = 0, uint64_t = 0);
    };

    struct stream_t {
        unsigned id_;
        std::string file_;
        std::vector<pos_t> offset_vector_;
        bool needs_update_;

        std::ios::pos_type file_pos_;
        typename std::vector<pos_t>::iterator stream_pos_;
        typename file_map_t::iterator input_;
        typename file_map_t::iterator output_;

        // This buffer is for reading purposes:
        std::shared_ptr<utils::CompressedStream<T>> buffer_;

        stream_t(unsigned, file_map_t &, file_map_t &);
        stream_t(const stream_t &);
        stream_t();
        ~stream_t();
        stream_t& operator=(const stream_t &);

        void write(std::ostream &) const;
        void read(std::istream &);
    };

    friend std::ostream &operator<<(std::ostream &os, const stream_t &s) {
        os << s.id_ << ":" << s.file_.length() << ":" << s.file_ << ":" << s.offset_vector_.size();
        typename std::vector<typename CompressedFileTrace<StreamType, T>::pos_t>::const_iterator i;
        for (i = s.offset_vector_.begin(); i != s.offset_vector_.end(); ++i)
            os << ":" << i->first_ << ":" << i->last_;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, stream_t &s) {
        char sep;
        unsigned name_len, offset_list_len;

        is >> s.id_; is >> sep; assert(sep == ':');
        is >> name_len; is >> sep; assert(sep == ':');
        if (name_len > 0) {
            std::vector<char> tmp(name_len + 1);
            is.read(&tmp[0], name_len); is >> sep; assert(sep == ':');
            s.file_.assign(&tmp[0], name_len);
        }
        is >> offset_list_len;
        s.stream_pos_ = s.offset_vector_.end();
        if (offset_list_len == 0) return is;

        s.offset_vector_.resize(offset_list_len);
        typename std::vector<typename CompressedFileTrace<StreamType, T>::pos_t>::iterator i;
        for (i = s.offset_vector_.begin(); i != s.offset_vector_.end(); ++i) {
            is >> sep; assert(sep == ':');
            is >> i->first_;
            is >> sep; assert(sep == ':');
            is >> i->last_;
        }
        s.stream_pos_ = s.offset_vector_.begin();
        return is;
    }

    mutable std::map<unsigned, stream_t> stream_vector_;
    unsigned registered_streams_;

    file_t &input_stream(unsigned) const;
    file_t &output_stream(unsigned);

    template<typename Local>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == true>::type
    flush_to_disk();

    template<typename Local>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == false>::type
    flush_to_disk();

    template<typename Local, typename EventType>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == true, bool>::type
    conditional_append(unsigned, const EventType &);

    template<typename Local, typename EventType>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == false, bool>::type
    conditional_append(unsigned, const EventType &);

public:
    explicit CompressedFileTrace(const char *);
    virtual ~CompressedFileTrace();

    /** adds a new event stream */
    void register_event_stream(unsigned, const std::string &);
    /** retrieves the number of streams */
    unsigned get_number_of_streams() const;
    unsigned get_actual_number_of_streams() const;
    unsigned get_number_of_files() const;
    void clear_stream(unsigned stream_id);
    /** adds an event to a stream given the stream ID */
    template<typename EventType>
    bool append(unsigned, const EventType &);

    /** retrieves an event from a stream given the event ID and the stream ID */
    template<typename EventType>
    bool get(unsigned, unsigned, EventType &) const;

    /** retrieves events sequentially from a stream given the stream ID,
      * return if successful or reached the end of the stream
      */
    template<typename EventType>
    bool get_next(unsigned, EventType &) const;

    /** places the reading point of a stream using its ID to the beginning of the stream */
    void reset(unsigned) const;

    /** places the reading point of a stream to a given event using the stream ID and event ID */
    template<typename EventType>
    bool seek(unsigned, unsigned) const;
};

}  // namespace trace
}  // namespace sim

#include "CompressedFileTrace-impl.h"

#endif  // CORE_TRACE_COMPRESSEDFILETRACE_H_
