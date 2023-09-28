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


#ifndef CORE_TRACE_VECTOREVENT_IMPL_H_
#define CORE_TRACE_VECTOREVENT_IMPL_H_

#include "nextsim/core/utils/Log.h"

namespace sim {
namespace trace {

using sim::utils::Log;

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
VectorEventCache<EventType, StreamType, Store, InitSize>::VectorEventCache(unsigned id, Store &store) :
    EventCache<EventType, StreamType>(id),
    store_(store),
    needs_update_(false)
{
    vector_.reserve(InitSize);
    EventType event;
    while (store_.get_next(this->id_, event)) {
        vector_.push_back(event);
    }
    pos_ = vector_.begin();
    if (vector_.empty() == false) {
        Log::debug3() << "Stream #" << this->id_ << " has " << vector_.size() << " events";
    }
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
VectorEventCache<EventType, StreamType, Store, InitSize>::~VectorEventCache()
{
    if (needs_update_ == false) {
        return;
    }
    Log::debug3() << "Flushing vector stream " << this->id_;
    typename vector_t::const_iterator i = vector_.begin();
    for (i = vector_.begin(); i != vector_.end(); ++i) {
        store_.append(this->id_, *i);
    }
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
template<typename Local>
inline
typename std::enable_if<is_base_of<Local, std::ostream>::value == true, bool>::type
VectorEventCache<EventType, StreamType, Store, InitSize>::conditional_append(const EventType &event)
{
    needs_update_ = true;
    vector_.push_back(event);
    return true;
}


template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
template<typename Local>
inline
typename std::enable_if<is_base_of<Local, std::ostream>::value == false, bool>::type
VectorEventCache<EventType, StreamType, Store, InitSize>::conditional_append(const EventType &event)
{
    // We cannot apend to an input file.
    assert(0);
    return false;
}


template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
bool VectorEventCache<EventType, StreamType, Store, InitSize>::append(const EventType &event)
{
    return conditional_append<typename Store::Stream>(event);
}


template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
bool VectorEventCache<EventType, StreamType, Store, InitSize>::get_next(EventType &event) const
{
    if (pos_ == vector_.end()) {
        return false;
    }
    event = *pos_;
    ++pos_;
    return true;
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
void VectorEventCache<EventType, StreamType, Store, InitSize>::reset() const
{
    pos_ = vector_.begin();
}


template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
unsigned VectorEventCache<EventType, StreamType, Store, InitSize>::size() const
{
    return vector_.size();
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
const EventType & VectorEventCache<EventType, StreamType, Store, InitSize>::get(unsigned id) const
{
    assert(id < vector_.size());
    return vector_[id];
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
template<typename Local>
typename std::enable_if<is_base_of<Local, std::ostream>::value == true, EventType &>::type
VectorEventCache<EventType, StreamType, Store, InitSize>::conditional_get(unsigned id)
{
    if (id >= vector_.size()) {
        vector_.resize(id + 1);
        needs_update_ = true;
    }
    return vector_[id];
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
template<typename Local>
typename std::enable_if<is_base_of<Local, std::ostream>::value == false, EventType &>::type
VectorEventCache<EventType, StreamType, Store, InitSize>::conditional_get(unsigned id)
{
    // We cannot get from an input stream as this is only used when writting the trace.
    assert(0);
}


template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
EventType & VectorEventCache<EventType, StreamType, Store, InitSize>::get(unsigned id)
{
    return conditional_get<typename Store::Stream>(id);
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
template<typename Local>
inline
typename std::enable_if<is_base_of<Local, std::ostream>::value == true, bool>::type
VectorEventCache<EventType, StreamType, Store, InitSize>::conditional_put(unsigned id, const EventType &event)
{
    needs_update_ = true;
    if (vector_.size() <= id) {
        vector_.resize(id + 1);
    }
    vector_[id] = event;
    return true;
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
template<typename Local>
inline
typename std::enable_if<is_base_of<Local, std::ostream>::value == false, bool>::type
VectorEventCache<EventType, StreamType, Store, InitSize>::conditional_put(unsigned id, const EventType &event)
{
    // We cannot put to an input stream.
    assert(0);
    return false;
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
bool VectorEventCache<EventType, StreamType, Store, InitSize>::put(unsigned id, const EventType &event)
{
    return conditional_put<typename Store::Stream>(id, event);
}


template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
bool VectorEventCache<EventType, StreamType, Store, InitSize>::seek(unsigned nelem) const
{
    if (nelem >= vector_.size()) {
        return false;
    }
    pos_ = vector_.begin() + nelem;
    return true;
}


template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
typename VectorEventCache<EventType, StreamType, Store, InitSize>::vector_t::const_iterator
VectorEventCache<EventType, StreamType, Store, InitSize>::begin() const
{
    return vector_.begin();
}


template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
typename VectorEventCache<EventType, StreamType, Store, InitSize>::vector_t::const_iterator
VectorEventCache<EventType, StreamType, Store, InitSize>::end() const
{
    return vector_.end();
}

template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
inline
uint64_t VectorEventCache<EventType, StreamType, Store, InitSize>::get_size() const
{
    return vector_.size();
}


}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_VECTOREVENT_IMPL_H_
