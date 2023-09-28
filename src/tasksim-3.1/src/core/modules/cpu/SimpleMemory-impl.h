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


#ifndef CORE_MODULES_CPU_SIMPLEMEMORY_IMPL_H_
#define CORE_MODULES_CPU_SIMPLEMEMORY_IMPL_H_

#include <string>

#include <core/utils/macros.h>

namespace sim {
namespace cpu {

template<typename T>
inline
SimpleMemory<T>::SimpleMemory(engine::Simulator &sim, const engine::Config &conf) :
        engine::IModule<T>(sim, engine::ModuleType::CPU),
        ldst_queue_size_(conf.get_value<std::size_t>("ldst-size")), halt_(false),
        instr_in_(NULL), memory_in_(NULL),
        dl1_(*this)
{}


template<typename T>
inline
SimpleMemory<T>::~SimpleMemory()
{
    assert(next_instruction_.isInvalid());
}


template<typename T>
inline
bool SimpleMemory<T>::reset()
{
    halt_ = false;
    return true;
}


template<typename T>
inline
void SimpleMemory<T>::fetch_instruction()
{
    assert(instr_in_ != NULL);

    /* We do not have space to store the request */
    if (next_instruction_.isValid()) {
        return issue_instruction();
    }

    /* Fetch an instruction */
    if (instr_in_->read(next_instruction_) == false) {
        halt_ = true;
        return; /* No more instructions */
    }
    assert(next_instruction_.isValid());
    assert(next_instruction_.ld_ + next_instruction_.st_ <= 2);

    issue_instruction();
}


template<typename T>
inline
void SimpleMemory<T>::issue_instruction()
{
    assert(memory_in_ != NULL);
    assert(next_instruction_.isValid());
    /* Check if there is enough space in the LD/ST queue */
    if (ldst_queue_.size() >= ldst_queue_size_) {
        return;
    }
    if (pending_queue_.size() >= ldst_queue_size_) {
        return;
    }

    if (next_instruction_.ld_ == 0 && next_instruction_.st_ == 0) {
        return;
    }

    trace::mem_t mem_request;
    bool ret = memory_in_->read(mem_request);
    assert(ret == true);
    _unused(ret);

    engine::addr_t mem_addr = mem_request.addr_;
    std::size_t mem_size = 0;
    typename T::Request::op_t mem_op = T::Request::op_t::LAST;

    if (next_instruction_.ld_ > 0) {
        mem_op = T::Request::op_t::READ;
        mem_size = next_instruction_.ld_size_;
        next_instruction_.ld_--;
    } else if (next_instruction_.st_ > 0) {
        mem_op = T::Request::op_t::WRITE;
        mem_size = next_instruction_.st_size_;
        next_instruction_.st_--;
    } else {
        assert(0);
    }

    assert(mem_op != T::Request::op_t::LAST);
    pending_queue_.push(typename T::Request(mem_op, mem_addr, mem_size));
}


template<typename T>
inline
void SimpleMemory<T>::commit_instruction()
{
    if (halt_) {
        return;
    }

    if (next_instruction_.isInvalid()) {
        return this->set_module_activity(engine::NEXT);
    }

    unsigned mem_ops = next_instruction_.ld_ + next_instruction_.st_;
    if (mem_ops == 0) {
        next_instruction_.type_ = trace::op_t::INVALID;
    }
    this->set_module_activity(engine::NEXT);
}


template<typename T>
inline
void SimpleMemory<T>::get_mem_response()
{
    /* Check if there are pending memory requests */
    if (ldst_queue_.empty()) {
        return;
    }

    /* The memory response has not arrived yet */
    typename T::Ack mem_response;
    if (dl1_.read_ack(mem_response) == false || mem_response.get_op() == T::Ack::op_t::MISS) {
        return;
    }
    typename ldst_queue_t::iterator i = ldst_queue_.find(mem_response.get_tag());
    assert(i != ldst_queue_.end());
    ldst_queue_.erase(i);
}


template<typename T>
inline
void SimpleMemory<T>::send_mem_request()
{
    /* Check if there are memory requests to send */
    if (pending_queue_.empty()) {
        return;
    }

    typename ldst_queue_t::const_iterator i = ldst_queue_.find(pending_queue_.front().get_tag());
    if (i != ldst_queue_.end()) {
        /* TODO: Account for LD and ST hitting on the LD/ST queue */
        return;
    }

    /* Check if the LD/ST queue is full */
    if (ldst_queue_.size() >= ldst_queue_size_) {
        return;
    }

    typename T::Request &req = pending_queue_.front();
    /* Write the request into the output port */
    if (dl1_.write_request(req) == false) {
        return;
    }

    /* If the write succeeds, set the request as pending */
    assert(ldst_queue_.size() < ldst_queue_size_);
    ldst_queue_.insert(typename ldst_queue_t::value_type(req.get_tag(), req));
    pending_queue_.pop();
}


template<typename T>
inline
void SimpleMemory<T>::start()
{
    send_mem_request();
}


template<typename T>
inline
void SimpleMemory<T>::end()
{
    get_mem_response();
    fetch_instruction();
    commit_instruction();
}


template<typename T>
inline
bool SimpleMemory<T>::inspect(const typename T::Request &)
{
    return true;
}


template<typename T>
inline
bool SimpleMemory<T>::inspect(const typename T::Ack &)
{
    return true;
}

template<typename T>
inline
const std::string SimpleMemory<T>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

}  // namespace cpu
}  // namespace sim

#endif  // CORE_MODULES_CPU_SIMPLEMEMORY_IMPL_H_
