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


#ifndef CORE_ENGINE_OUTPORT_IMPL_H_
#define CORE_ENGINE_OUTPORT_IMPL_H_

#include <cassert>
#include <memory>
#include <list>

#include "InPort.h"
#include "Module.h"

namespace sim {
namespace engine {

template<typename T>
inline OutPort<T>::OutPort(Module &owner, unsigned size) :
        max_size_(size),
        peer_(nullptr),
        owner_(owner),
        connected_(false),
        wake_up_on_read_(true)
{
}

template<typename T>
inline OutPort<T>::~OutPort()
{
    assert(is_empty());
}

template<typename T>
inline
bool OutPort<T>::is_empty() const
{
    return value_.size() == 0;
}

template<typename T>
inline
bool OutPort<T>::is_full() const
{
    return value_.size() == max_size_;
}

template<typename T>
inline
T&& OutPort<T>::get()
{
    return std::move(value_.front());
}

template<typename T>
inline
bool OutPort<T>::write(T value)
{
    assert(peer_ != nullptr);
    if (is_full()) {
        return false;
    }
    value_.emplace_back(value);
    value_.back().touch(this->owner_.get_simulator());
    owner_.get_simulator().wake_up_module(peer_->owner_);
    return true;
}

template<typename T>
inline
void OutPort<T>::consume()
{
    assert(peer_ != nullptr);
    assert(not is_empty());
    value_.pop_front();
}

template<typename T>
inline
void OutPort<T>::operator >>(InPort<T> &in_port)
{
    in_port.peer_ = this;
    peer_ = &in_port;
    connected_ = true;
}

template<typename T>
inline
bool OutPort<T>::is_connected() const
{
    return connected_;
}


template<typename T>
inline
engine::Module &OutPort<T>::get_peer()
{
    assert(peer_ != nullptr);
    return peer_->owner_;
}

template<typename T>
inline
void OutPort<T>::set_wake_up_on_read(bool wake_up_on_read)
{
    wake_up_on_read_ = wake_up_on_read;
}

template<typename T>
inline
unsigned OutPort<T>::get_max_size() const
{
    return max_size_;
}

template<typename T>
inline
void OutPort<T>::set_max_size(unsigned size)
{
    max_size_ = size;
}

}  // namespace engine
}  // namespace sim

#endif  // CORE_ENGINE_OUTPORT_IMPL_H_
