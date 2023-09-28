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


#ifndef CORE_TRACE_EVENT_H_
#define CORE_TRACE_EVENT_H_

namespace sim {
namespace trace {

/**
 * BaseEventCache is the base class for event caches specifying the basic interface
 */
class BaseEventCache {
public:
    virtual ~BaseEventCache() {}
    virtual void reset() const = 0;
};

template<typename EventType> class StreamType;

/**
 * EventCache specifies the cache has an ID
 */
template<typename EventType, template<typename> class StreamType>
class EventCache : public BaseEventCache {
protected:
    unsigned id_;

public:
    explicit EventCache(unsigned);
    virtual ~EventCache() {}
};

/**
 * NullEventCache defines a set of methods to manage a set of events
 */
template<typename EventType, template<typename> class StreamType, typename Store>
class NullEventCache : public EventCache<EventType, StreamType> {
protected:
    Store &store_;
public:
    NullEventCache(unsigned, Store &store);
    ~NullEventCache();

    /** adds a new event to the end of the event cache */
    bool append(const EventType &);
    /** retrieves events sequentially, and returns if it was successful or reached the end of the cache */
    bool get_next(EventType &) const;
    /** places the reading point to the beginning */
    void reset() const;
    /** places the reading point to a specific event using its ID */
    bool seek(unsigned) const;
};

template<typename EventType>
class Event {
public:
    template<template<typename> class StreamType, typename Store>
    struct Type : public NullEventCache<EventType, StreamType, Store> {
        Type(unsigned id, Store &store) : NullEventCache<EventType, StreamType, Store>(id, store) {}
    };
};

}  // namespace trace
}  // namespace sim

#include "Event-impl.h"

#endif  // CORE_TRACE_EVENT_H_
