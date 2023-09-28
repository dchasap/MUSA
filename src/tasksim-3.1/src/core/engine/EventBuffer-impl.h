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


#ifndef CORE_ENGINE_EVENTBUFFER_IMPL_H_
#define CORE_ENGINE_EVENTBUFFER_IMPL_H_

#include <cassert>
#include <utility>
#include <iostream>
#include <algorithm>

#include "core/engine/Module.h"
#include "core/stats/TimeStatistics.h"

namespace sim {
namespace engine {

template<typename T>
inline
EventBuffer<T>::EventBuffer(std::function<void(EventBuffer<T>&)> func,
                               std::size_t size, unsigned buffer_id) :
        fill_function_(func),
        buffer_size_(size),
        buffer_id_(buffer_id),
        remote_module_(NULL)
{
}


template<typename T>
inline
EventBuffer<T>::~EventBuffer()
{
    if (!buffer_.empty()) {
        std::cerr << "Error: EventBuffer not empty, " << buffer_.size() << " elements remaining" << std::endl;
    }
}

template<typename T>
inline
bool EventBuffer<T>::push_event(T &&e)
{
    if (buffer_.size() >= buffer_size_) {
        return false;
    }
    buffer_.emplace(e);
    return true;
}

template<typename T>
inline
bool EventBuffer<T>::push_event(const T &e)
{
    if (buffer_.size() >= buffer_size_) {
        return false;
    }
    buffer_.push(e);
    return true;
}


template<typename T>
inline
std::size_t EventBuffer<T>::space_left() const
{
    return buffer_size_ - buffer_.size();
}


template<typename T>
inline
unsigned EventBuffer<T>::get_id() const
{
    return buffer_id_;
}

template<typename T>
inline
bool EventBuffer<T>::read(T &event)
{
    if (buffer_.empty()) {
        fill_function_(*this);
    }

    if (buffer_.empty()) {
        return false;
    } else {
        event = buffer_.front();
        buffer_.pop();
        return true;
    }
}

}  // namespace engine
}  // namespace sim

#endif  // CORE_ENGINE_EVENTBUFFER_IMPL_H_
