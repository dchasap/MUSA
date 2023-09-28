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


#ifndef CORE_TRACE_MAPEVENT_H_
#define CORE_TRACE_MAPEVENT_H_

#include "VectorEvent.h"

#include <map>
#include <unordered_map>


namespace sim {
namespace trace {

/**
 * MapEventCache defines a set of methods to access to a map of events
 */
template<typename EventType, template<typename> class StreamType, typename Store,
        typename T, T EventType::*Container, unsigned InitSize>
class MapEventCache : public VectorEventCache<EventType, StreamType, Store, InitSize>
{
public:
    typedef std::map<T, unsigned> map_t;
protected:
    map_t map_;
public:
    MapEventCache(unsigned, Store &store);
    virtual ~MapEventCache();

    /** adds a new event to the map */
    bool append(const EventType &);
    /** queries whether there is an associated event with the given index in the map */
    bool has_index(const T &) const;
    /** retrieves an event using its associated index in the map */
    const EventType &get_index(const T &) const;
    /** retrieves an event using its associated index in the map */
    EventType &get_reference(const T &);
    /** retrieves the event ID using its associated index in the map */
    unsigned get_index_id(const T &) const;

    void flush_contents();
    void print_contents() const;
    uint64_t get_size() const;
};

template<typename T, typename EventType, T EventType::*Container, unsigned InitSize = 8192>
class MapEvent {
public:
    template<template<typename> class StreamType, typename Store>
    struct Type : public MapEventCache<EventType, StreamType, Store, T, Container, InitSize> {
        Type(unsigned id, Store &store) :
                MapEventCache<EventType, StreamType, Store, T, Container, InitSize>(id, store)
        {}
    };
};


}  // namespace trace
}  // namespace sim

#include "MapEvent-impl.h"

#endif  // CORE_TRACE_MAPEVENT_H_
