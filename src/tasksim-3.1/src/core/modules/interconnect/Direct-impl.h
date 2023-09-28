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


#ifndef CORE_MODULES_INTERCONNECT_DIRECT_IMPL_H_
#define CORE_MODULES_INTERCONNECT_DIRECT_IMPL_H_


#include "core/utils/Log.h"

namespace sim {
namespace interconnect {

using sim::utils::Log;


template<typename Message>
inline
Direct<Message>::Interface::Interface() :
    peer_(nullptr)
{}


template<typename Message>
inline
engine::Module &Direct<Message>::Interface::get_peer()
{
    assert(peer_ != nullptr);
    return *peer_;
}


template<typename Message>
inline
Direct<Message>::Input::Input(engine::Module &owner, unsigned output_size) :
    in_(owner), out_(owner, output_size), miss_(owner)
{}


template<typename Message>
inline
Direct<Message>::Input::~Input()
{
}


template<typename T>
inline
bool Direct<T>::Input::can_read_request()
{
    return in_.can_read();
}


template<typename Message>
template<typename Container, typename... Args>
bool Direct<Message>::Input::read_request(Container &container, const Args &... args)
{
    return in_.read(container, args...);
}


template<typename Message>
inline
bool Direct<Message>::Input::read_request(typename Message::Request &payload)
{
    return in_.read(payload);
}

template<typename Message>
inline
bool Direct<Message>::Input::can_write_ack() const
{
    return not out_.is_full();
}

template<typename Message>
inline
bool Direct<Message>::Input::write_ack(const typename Message::Ack &payload)
{
    assert(payload.get_op() < Message::Ack::op_t::LAST);
    return out_.write(payload);
}


template<typename Message>
inline
void Direct<Message>::Input::signal_miss()
{
    const bool miss_value = true;
    // We should not need to clear the miss before sending another...
    assert(miss_.is_empty());
    bool ret = miss_.write(miss_value);
    assert(ret == true);
    _unused(ret);
}


template<typename Message>
inline
Direct<Message>::Output::Output(engine::Module &owner, unsigned output_size) :
    out_(owner, output_size), in_(owner), miss_(owner)
{}


template<typename Message>
inline
bool Direct<Message>::Output::can_write_request() const
{
    return not out_.is_full();
}

template<typename Message>
inline
bool Direct<Message>::Output::write_request(const typename Message::Request &payload)
{
    assert(payload.get_op() < Message::Request::op_t::LAST);
    Log::debug4() << "Direct::Output write_request " << payload;
    if (out_.write(payload)) {
        Log::debug4() << "Direct::Output write_request success.";
        out_.get_peer().notify_wakeup(input_ptr_);
        return true;
    }
    Log::debug4() << "Direct::Output write_request fail.";
    return false;
}


template<typename Message>
inline
bool Direct<Message>::Output::can_read_ack()
{
    return in_.can_read();
}


template<typename Message>
inline
bool Direct<Message>::Output::read_ack(typename Message::Ack &payload)
{
    return in_.read(payload);
}


template<typename Message>
inline
bool Direct<Message>::Output::read_miss(bool &miss)
{
    return miss_.read(miss);
}


template<typename Message>
inline
void Direct<Message>::Output::operator>>(Input &input)
{
    out_ >> input.in_;
    this->peer_ = dynamic_cast<engine::Module *>(&(out_.get_peer()));

    input.miss_ >> miss_;
    input.out_ >> in_;
    input.out_.set_max_size(out_.get_max_size());
    input.peer_ = dynamic_cast<engine::Module*>(&(input.out_.get_peer()));

    input_ptr_ = &input;
}


template<typename Message>
inline
void Direct<Message>::Output::set_wake_up_on_read(bool wake_up_on_read)
{
    out_.set_wake_up_on_read(wake_up_on_read);
    in_.get_connected_port()->set_wake_up_on_read(wake_up_on_read);
    miss_.get_connected_port()->set_wake_up_on_read(wake_up_on_read);
}


}  // namespace interconnect
}  // namespace sim

#endif  // CORE_MODULES_INTERCONNECT_DIRECT_IMPL_H_
