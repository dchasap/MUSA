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


#ifndef CORE_ENGINE_INPORT_IMPL_H_
#define CORE_ENGINE_INPORT_IMPL_H_

#include <cassert>
#include <memory>

#include "Module.h"
#include "OutPort.h"


namespace sim {
namespace engine {

template<typename T>
inline InPort<T>::InPort(Module &owner) :
        peer_(nullptr), owner_(owner)
{
}

template<typename T>
inline InPort<T>::~InPort()
{
    peer_ = nullptr;
}

template<typename T>
inline
bool InPort<T>::can_read()
{
    return not peer_->is_empty();
}

template<typename T>
inline
bool InPort<T>::read(T &value)
{
    assert(peer_ != nullptr);
    if (peer_->is_empty()) {
        return false;
    }
    value = peer_->get();
    value.touch(owner_.get_simulator());
    peer_->consume();
    if (peer_->wake_up_on_read_ == true) {
        peer_->owner_.get_simulator().wake_up_module(peer_->owner_);
    }
    return true;
}

template<typename T>
template<typename Container, typename... Args>
inline
bool InPort<T>::read(Container &container, const Args &... args)
{
    assert(peer_ != nullptr);
    if (peer_->is_empty()) {
        return false;
    }
    container.emplace_back(args..., std::move(peer_->get()));
    container.back().touch(owner_.get_simulator());
    peer_->consume();
    if (peer_->wake_up_on_read_ == true) {
        peer_->owner_.get_simulator().wake_up_module(peer_->owner_);
    }
    return true;
}

template<typename T>
inline
void InPort<T>::operator <<(OutPort<T> &out_port)
{
    peer_ = &out_port;
    out_port.connected_ = true;
}

template<typename T>
inline
OutPort<T>* InPort<T>::get_connected_port()
{
    assert(peer_ != nullptr);
    return peer_;
}

}  // namespace engine
}  // namespace sim

#endif  // CORE_ENGINE_INPORT_IMPL_H_
