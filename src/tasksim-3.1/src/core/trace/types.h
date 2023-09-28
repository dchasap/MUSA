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


#ifndef CORE_TRACE_TYPES_H_
#define CORE_TRACE_TYPES_H_

namespace sim {
namespace trace {

// Some forward declarations:
template <typename> class TextEventStream;
template <typename> class BinaryEventStream;
template<template<typename> class StreamType, typename T> class FileTrace;
template<template<typename> class StreamType, typename T> class CompressedFileTrace;


#ifdef TESTING_VERSION

// Use RLE as encoding, binary outputs into compressed files
template<typename Event>
using SelectedEventStream = BinaryEventStream<Event>;
template<template<typename> class StreamType, typename T>
using SelectedFileTrace = sim::trace::CompressedFileTrace<StreamType, T>;
namespace rle { }
namespace selectedtrace = sim::trace::rle;

#else  // TESTING_STREAMS

#ifdef USE_TEXT_EVENT_STREAM

template<typename Event>
using SelectedEventStream = TextEventStream<Event>;

#else  // USE_TEXT_EVENT_STREAM

template<typename Event>
using SelectedEventStream = BinaryEventStream<Event>;

#endif  // USE_TEXT_EVENT_STREAM

#ifdef USE_COMPRESSED_TRACES

template<template<typename> class StreamType, typename T>
using SelectedFileTrace = sim::trace::CompressedFileTrace<StreamType, T>;

#else  // USE_COMPRESSED_TRACES

template<template<typename> class StreamType, typename T>
using SelectedFileTrace = sim::trace::FileTrace<StreamType, T>;

#endif  // USE_COMPRESSED_TRACES

#ifdef USE_MEM_TRACES

// Mem format:
namespace mem { }
namespace selectedtrace = sim::trace::mem;


#elif USE_UOP_TRACES

// uop format:
namespace uop { }
namespace selectedtrace = sim::trace::uop;

#elif USE_RLE_TRACES

// rle format:
namespace rle { }
namespace selectedtrace = sim::trace::rle;

#else  // USE_X_TRACES

// mem format:
namespace mem { }
namespace selectedtrace = sim::trace::mem;

#endif  // USE_X_TRACES

#endif  // TESTING_STREAMS

class phase_data_t {
public:
    uint64_t n_lds, n_sts, n_instrs, n_unid;
    phase_data_t();
    phase_data_t(uint64_t lds, uint64_t sts, uint64_t instrs, uint64_t unid);
    void write(std::ostream &) const;
    void read(std::istream &);
    void clear();
    void add_info(const phase_data_t &data);
};

class user_code_data_t {
public:
    unsigned wd_id_;
    std::vector<phase_data_t> data;
    user_code_data_t();
    user_code_data_t(unsigned wd_id, const phase_data_t &data);
    void write(std::ostream &) const;
    void read(std::istream &);
};

}  // namespace trace
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_TRACE_TYPES_H_
