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


#ifndef CORE_LOGIC_COHERENCE_SINGLE_MESSAGE_IMPL_H_
#define CORE_LOGIC_COHERENCE_SINGLE_MESSAGE_IMPL_H_

#include <iostream>
#include "core/engine/Simulator.h"

namespace sim {
namespace logic {
namespace coherence {
namespace single {


inline
Packet::Packet() :
    tag_(0), size_(0), data_size_(0)
{}

inline
Packet::Packet(const engine::addr_t &tag,
        std::size_t size, std::size_t data_size, std::size_t data,
        const task_dep_t &dep_type, std::size_t task_type, uint16_t cpu_id, uint64_t user_code_id) :
    tag_(tag),
    size_(size),
    data_size_(data_size),
    data_(data),
    dep_type_(dep_type),
    task_type_(task_type),
    cpu_id_(cpu_id),
    user_code_id_(user_code_id)
{}

inline
Packet::Packet(const Packet &original) :
        tag_(original.tag_),
        size_(original.size_),
        data_size_(original.data_size_),
        data_(original.data_),
        dep_type_(original.dep_type_),
        task_type_(original.task_type_),
        cpu_id_(original.cpu_id_),
        user_code_id_(original.user_code_id_)
{}

inline
Packet::Packet(const Packet &original, size_t cache_line_size) :
        tag_(original.tag_ & ~(cache_line_size - 1)),
        size_(cache_line_size),
        data_size_(0),
        data_(original.data_),
        dep_type_(original.dep_type_),
        task_type_(original.task_type_),
        cpu_id_(original.cpu_id_),
        user_code_id_(original.user_code_id_)
{}

inline
Packet::tag_t Packet::get_tag() const
{
    return tag_;
}

inline
std::size_t Packet::get_size() const
{
    return size_;
}

inline
std::size_t Packet::get_data_size() const
{
    return data_size_;
}

inline
std::size_t Packet::get_data() const
{
    return data_;
}

inline
sim::runtime::task_dep_t Packet::get_dep_type() const
{
    return dep_type_;
}

inline
std::size_t Packet::get_task_type() const
{
    return task_type_;
}

inline
uint16_t Packet::get_cpu_id() const
{
    return cpu_id_;
}

inline
void Packet::set_issuer(int id) const
{
    issuer_ = id;
}

inline
int Packet::get_issuer() const
{
    return issuer_;
}

inline
uint64_t Packet::get_user_code_id() const
{
    return user_code_id_;
}

inline
void Packet::set_user_code_id(uint64_t user_code_id)
{
    user_code_id_ = user_code_id;
}

inline
std::ostream &operator<<(std::ostream &os, const Packet &p)
{
    os << ":0x" << std::hex << p.get_tag() << std::dec << ":"
       << p.get_size() << ":" << p.get_data_size() << ":" << p.get_data() << ":"
       << p.get_user_code_id();
    return os;
}

inline
void Packet::touch(const sim::engine::Simulator &sim)
{
}

inline
void Packet::touch(const Packet &packet)
{
}

inline
Request::Request() : op_(req_op_t::LAST), fill_(false)
{}

inline
Request::Request(op_t op, const engine::addr_t &addr,
        size_t size, size_t data_size, size_t data,
        const task_dep_t &dep_type, size_t task_type, uint16_t cpu_id, uint64_t user_code_id, bool fill) :
    Packet(addr, size, data_size, data, dep_type, task_type, cpu_id, user_code_id), op_(op), fill_(fill)
{}

inline
Request::Request(const Request &original, op_t op, size_t cache_line_size) :
        Packet(original, cache_line_size),
        op_(op)
{
}

inline
Request::Request(const Request &original) :
        Packet(original),
        op_(original.op_)
{
}

inline
Request::op_t Request::get_op() const
{
    return op_;
}

inline bool Request::operator==(const Request &b) const {
    if (op_ != b.get_op() or size_ != b.get_size() or data_size_ != b.get_data_size() or
           tag_ != b.get_tag() or data_ != b.get_data() or user_code_id_ != b.get_user_code_id()) {
        return false;
    } else {
        return true;
    }
}

inline
void Request::set_fill(bool fill)
{
    fill_ = fill;
}

inline
bool Request::is_fill() const
{
    return fill_;
}

inline
TimePacket::TimePacket() : Packet() {}

inline
TimePacket::TimePacket(const TimePacket &req) :
    Packet(),
    time_log_(req.time_log_)
{}

inline
std::ostream &operator<<(std::ostream &os, const Request &req)
{
    os << static_cast<unsigned>(req.get_op()) << *static_cast<const Packet*>(&req);
    return os;
}


inline
TimePacket::TimePacket(const engine::addr_t &tag, std::size_t size, std::size_t data_size, std::size_t data, uint64_t user_code_id, const stamp_vector_t &time_log) :
    Packet(tag, size, data_size, data, task_dep_t::NON_IN_OUT, 0, 0, user_code_id),
    time_log_(time_log)
{}

inline
TimePacket::TimePacket(const engine::addr_t &tag, std::size_t size, std::size_t data_size, std::size_t data, uint64_t user_code_id, sim::engine::cycles_t time) :
    Packet(tag, size, data_size, data, task_dep_t::NON_IN_OUT, 0, 0, user_code_id),
    time_log_(1, time)
{}


inline
const Packet::stamp_vector_t& TimePacket::get_time_log() const
{
    return time_log_;
}

inline
engine::cycles_t TimePacket::get_stamp(unsigned i) const
{
    assert(i < time_log_.size());
    if (i < time_log_.size()) {
      return time_log_.at(i);
    } else {
        return 0;
    }
}

inline
void TimePacket::write_time_log()
{
    std::cout << "|";
    for (unsigned i = 0; i < time_log_.size(); i++) {
        std::cout << time_log_[i] << '|';
    }
}

inline
void TimePacket::touch(const sim::engine::Simulator &sim)
{
    time_log_.push_back(sim.get_clock());
}

inline
void TimePacket::touch(const Packet &packet)
{
    const TimePacket *tp = dynamic_cast<const TimePacket*>(&packet);
    assert(tp != NULL);
    for (sim::engine::cycles_t time_stamp : tp->time_log_) {
        time_log_.push_back(time_stamp);
    }
}

inline
void TimePacket::update_time_log(engine::cycles_t start)
{
    assert(time_log_.size() > 5);
    engine::cycles_t missplacement = start - time_log_[0];
    for (unsigned i = 0; i < 5; i++) {
        time_log_[i] += missplacement;
    }
    if (time_log_.size() > 9) {
        time_log_[5] = time_log_[4] + 1;
        time_log_[6] = time_log_[4] + 1;
    }
}

inline
TimeRequest::TimeRequest() : TimePacket(), Request()
{}


inline
TimeRequest::TimeRequest(op_t op, const engine::addr_t &addr, std::size_t size, std::size_t data_size) :
    TimePacket(addr, size, data_size, /*data*/ 0, /* user_code_id*/ 0, /*cycles*/ 0),
    Request(op, addr, size, data_size, 0)
{
}

inline
TimeRequest::TimeRequest(sim::engine::cycles_t stamp, op_t op, const engine::addr_t &addr,
                         std::size_t size, std::size_t data_size) :
    TimePacket(addr, size, data_size, /*data*/ 0, /* user_code_id*/ 0, stamp),
    Request(op, addr, size, data_size, 0)
{
}

inline
TimeRequest::TimeRequest(const stamp_vector_t &time_log, op_t op, const tag_t &addr,
                         std::size_t size, std::size_t data_size) :
    TimePacket(addr, size, data_size, /*data*/ 0, /* user_code_id*/ 0, time_log),
    Request(op, addr, size, data_size, 0)
{
}

inline
Ack::Ack() : op_(ack_op_t::LAST)
{}


/* When we create an Ack as a response to a Request the parameters are defined:
 * tag_: the same tag as the request.
 * size_: 0, as it does not need any data sent back.
 * data_size: size_ as the size of the data we are providing the size of the data requested.
 * data_: the same data_ as the request.
 */
inline
Ack::Ack(const Request &req) :
    Packet(req.tag_, req.data_size_, req.size_, req.data_, req.dep_type_, req.task_type_,
            req.cpu_id_, req.user_code_id_)
{
    switch (req.get_op()) {
        case req_op_t::READ:
            op_ = ack_op_t::READ; break;
        case req_op_t::WRITE:
            op_ = ack_op_t::WRITE; break;
        case req_op_t::FLUSH:
            op_ = ack_op_t::FLUSH; break;
        case req_op_t::INVALIDATE:
            op_ = ack_op_t::INVALIDATE; break;
        case req_op_t::PREFETCH:
            op_ = ack_op_t::PREFETCH; break;
        case req_op_t::CLEAR:
            op_ = ack_op_t::CLEAR; break;
        default:
            op_ = ack_op_t::LAST; break;
    }
}

inline
Ack::Ack(op_t op, const engine::addr_t &addr, std::size_t size, std::size_t data_size, uint64_t user_code_id) :
    Packet(addr, size, data_size, 0, task_dep_t::NONE, 0, -1, user_code_id), op_(op)
{}

inline
Ack::op_t Ack::get_op() const
{
    return op_;
}

inline bool Ack::operator==(const Ack &b) const {
    if (op_ != b.get_op() or size_ != b.get_size() or data_size_ != b.get_data_size() or
           tag_ != b.get_tag() or data_ != b.get_data() or user_code_id_ != b.get_user_code_id()) {
        return false;
    } else {
        return true;
    }
}

inline
std::ostream &operator<<(std::ostream &os, const Ack &ack)
{
    os << static_cast<unsigned>(ack.get_op()) << *static_cast<const Packet*>(&ack);
    return os;
}

inline
TimeAck::TimeAck() : TimePacket(), Ack()
{}

inline
TimeAck::TimeAck(const TimeRequest &req) :
    TimePacket(req),
    Ack(req)
{
    for (unsigned i = 0; i < req.time_log_.size(); i++) {
        this->time_log_.push_back(req.time_log_[i]);
    }
}

inline
TimeAck::TimeAck(sim::engine::cycles_t stamp, op_t op, const sim::engine::addr_t &addr,
        std::size_t size, std::size_t data_size) :
    TimePacket(addr, size, data_size, /*data*/ 0, /* user_code_id*/ 0, stamp),
    Ack(op, addr, size, data_size)
{
}

inline
TimeAck::TimeAck(op_t op, const sim::engine::addr_t &addr, std::size_t size, std::size_t data_size) :
    TimePacket(addr, size, data_size, /*data*/ 0, /* user_code_id*/ 0, /*cycles*/ 0),
    Ack(op, addr, size, data_size)
{
}

inline
TimeAck::TimeAck(const stamp_vector_t &time_log, op_t op, const tag_t &addr, std::size_t size, std::size_t data_size) :
    TimePacket(addr, size, data_size, /*data*/ 0, /* user_code_id*/ 0, time_log),
    Ack(op, addr, size, data_size)
{
}

}  // namespace single
}  // namespace coherence
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_COHERENCE_SINGLE_MESSAGE_IMPL_H_
