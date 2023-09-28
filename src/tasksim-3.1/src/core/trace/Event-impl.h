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


#ifndef CORE_TRACE_EVENT_IMPL_H_
#define CORE_TRACE_EVENT_IMPL_H_

namespace sim {
namespace trace {

template<typename EventType, template<typename> class StreamType>
inline
EventCache<EventType, StreamType>::EventCache(unsigned id) :
    id_(id)
{
}

template<typename EventType, template<typename> class StreamType, typename Store>
inline
NullEventCache<EventType, StreamType, Store>::NullEventCache(unsigned id, Store &store) :
    EventCache<EventType, StreamType>(id),
    store_(store)
{
}

template<typename EventType, template<typename> class StreamType, typename Store>
inline
NullEventCache<EventType, StreamType, Store>::~NullEventCache()
{
}

template<typename EventType, template<typename> class StreamType, typename Store>
inline
bool NullEventCache<EventType, StreamType, Store>::append(const EventType &event)
{
    return store_.append(this->id_, event);
}



template<typename EventType, template<typename> class StreamType, typename Store>
inline
bool NullEventCache<EventType, StreamType, Store>::get_next(EventType &event) const
{
    return store_.get_next(this->id_, event);
}


template<typename EventType, template<typename> class StreamType, typename Store>
inline
void NullEventCache<EventType, StreamType, Store>::reset() const
{
    return store_.reset(this->id_);
}

template<typename EventType, template<typename> class StreamType, typename Store>
inline
bool NullEventCache<EventType, StreamType, Store>::seek(unsigned nelem) const
{
    return store_.template seek<EventType>(this->id_, nelem);
}

}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_EVENT_IMPL_H_
