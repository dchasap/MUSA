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


#ifndef CORE_TRACE_VECTOREVENT_H_
#define CORE_TRACE_VECTOREVENT_H_


#include <vector>

#include "nextsim/core/trace/Event.h"
#include "nextsim/core/trace/type_traits.h"


namespace sim {
namespace trace {

/**
 * VectorEventCache defines a set of methods to access to a vector of events sequentially or randomly
 */
template<typename EventType, template<typename> class StreamType, typename Store, unsigned InitSize>
class VectorEventCache : public EventCache<EventType, StreamType>
{
public:
    typedef std::vector<EventType> vector_t;
protected:
    Store &store_;
    mutable vector_t vector_;
    mutable typename vector_t::iterator pos_;
    bool needs_update_;

    template<typename Local>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == true, EventType &>::type
    conditional_get(unsigned);

    template<typename Local>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == false, EventType &>::type
    conditional_get(unsigned);

    template<typename Local>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == true, bool>::type
    inline
    conditional_put(unsigned id,  const EventType &event);

    template<typename Local>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == false, bool>::type
    inline
    conditional_put(unsigned id,  const EventType &event);

    template<typename Local>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == true, bool>::type
    inline
    conditional_append(const EventType &event);

    template<typename Local>
    typename std::enable_if<is_base_of<Local, std::ostream>::value == false, bool>::type
    inline
    conditional_append(const EventType &event);

public:
    VectorEventCache(unsigned, Store &store);
    virtual ~VectorEventCache();

    /** adds a new event to the end of the event vector */
    bool append(const EventType &);
    /** retrieves events sequentially, and returns if it was successful or reached the end of the vector */
    bool get_next(EventType &) const;
    /** places the reading point to the beginning */
    void reset() const;

    /** retrieves the number of events in the vector */
    unsigned size() const;
    /** retrieves an event using its ID (read-only) */
    const EventType &get(unsigned) const;
    /** retrieves an event using its ID*/
    EventType &get(unsigned);
    /** writes an event in the vector using its ID */
    bool put(unsigned, const EventType &);

    /** places the reading point to a specific event using its ID */
    bool seek(unsigned) const;

    /** returns an iterator to the first event in the vector */
    typename vector_t::const_iterator begin() const;
    /** returns an iterator to the position right after the last event in the vector */
    typename vector_t::const_iterator end() const;

    uint64_t get_size() const;
};


template<typename EventType, unsigned InitSize = 8192>
class VectorEvent {
public:
    template<template<typename> class StreamType, typename Store>
    struct Type : public VectorEventCache<EventType, StreamType, Store, InitSize> {
        Type(unsigned id, Store &store) : VectorEventCache<EventType, StreamType, Store, InitSize>(id, store) {}
    };
};

}  // namespace trace
}  // namespace sim

#include "VectorEvent-impl.h"

#endif  // CORE_TRACE_VECTOREVENT_H_
