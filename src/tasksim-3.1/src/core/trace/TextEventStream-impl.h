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


#ifndef CORE_TRACE_TEXTEVENTSTREAM_IMPL_H_
#define CORE_TRACE_TEXTEVENTSTREAM_IMPL_H_

#include <limits>
#include <sstream>

namespace sim {
namespace trace {

template<typename Event>
TextEventStream<Event>::TextEventStream()
{
}

template<typename Event>
inline TextEventStream<Event>::~TextEventStream()
{
}

template<typename Event>
template<typename T>
inline
bool TextEventStream<Event>::write(T &os, const Event &event)
{
    os << event << std::endl;
    return os.good();
}

template<typename Event>
template<typename T>
inline
bool TextEventStream<Event>::read(T &is, Event &event)
{
    // We clear the line return and peek the next character to make sure it is not
    // the EOF character. If eof() has been read, peek will return EOF.
    // We do not check eof() as we can be on it without having tried to read it.

    if (is.peek() == std::ifstream::traits_type::eof() || is.good() == false) {
        return false;
    }
    is >> event;
    // We read now the good() value as some events read the ENDL and some don't.
    // To mantain compatibility with both the good flag has to be read before trying to
    // eliminate the ENDL, as if it has been already read it would cause it to flip to
    // false, and would cause the last element on a TextEventStream not to be read.
    bool ret = is.good();
    if (is.peek() == '\n') {
        is.ignore(1);
    }
    return ret;
}

}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_TEXTEVENTSTREAM_IMPL_H_
