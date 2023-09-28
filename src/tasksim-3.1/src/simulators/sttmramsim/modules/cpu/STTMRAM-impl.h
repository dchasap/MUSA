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


#ifndef MODULES_CPU_STTMRAM_IMPL_H_
#define MODULES_CPU_STTMRAM_IMPL_H_

namespace sim {
namespace cpu {

template<typename T, typename R>
inline
STTMRAM<T,R>::STTMRAM(engine::Simulator &sim, const engine::Config &conf) :
        engine::IModule<T>(sim, engine::ModuleType::CPU),
        pending_request_(NULL), send_cycle_(0),
        ldst_queue_size_(conf.get_value<std::size_t>("ldst-size")),
        memory_access_sent_(*this, "memory_access_sent"),
        memory_access_received_(*this, "memory_access_received"),
        finish_cycle_(0),
        write_request_failed_(false),
        cpi_(conf.get_value<double>("cpi")),
        cache_hit_latency_(conf.get_value<double>("cache_hit_latency")),
        memory_in_(NULL), dl1_(*this)
{
}


template<typename T, typename R>
inline
STTMRAM<T,R>::~STTMRAM()
{
	std::cout << finish_cycle_ << ":finish_cycle:STTMRAM:" << this->get_id() << std::endl;
}


template<typename T, typename R>
inline
bool STTMRAM<T,R>::reset()
{
    return true;
}


template<typename T, typename R>
inline
void STTMRAM<T,R>::get_mem_response()
{
    /* Check if there are pending memory requests */
    if(ldst_queue_.empty()) return;

    /* The memory response has not arrived yet */
    typename T::Ack mem_response;

    if (dl1_.read_ack(mem_response) == false) return;

    if (mem_response.get_op() == T::Ack::op_t::MISS) return;

    memory_access_received_++;

    typename ldst_queue_t::iterator i = ldst_queue_.find(mem_response.get_tag());
    assert(i != ldst_queue_.end());

    delete i->second;
    ldst_queue_.erase(i);
}


template<typename T, typename R>
inline
void STTMRAM<T,R>::fetch_mem_request()
{
    assert(memory_in_ != NULL);

    if (pending_request_ != NULL) return;

    if (ldst_queue_.size() >= ldst_queue_size_) return;

    trace::sttmram::mem_t mem_request;

    if (!memory_in_->read(mem_request)) return;

    if (mem_request.op_ == trace::sttmram::mem_t::op_t::READ)
    {
        pending_request_ = new typename T::Request(T::Request::op_t::READ, mem_request.addr_, 8);
    }
    else
    {
        pending_request_ = new typename T::Request(T::Request::op_t::WRITE, mem_request.addr_, 8);
    }

    send_cycle_ = this->get_simulator().get_clock() + ceil(mem_request.instruction_count_ * cpi_ + mem_request.cache_hit_count_ * cache_hit_latency_) + 1;
}


template<typename T, typename R>
inline
void STTMRAM<T,R>::send_mem_request()
{
    /* Check if the LD/ST queue is full */
    if (pending_request_ == NULL) return;

    if (ldst_queue_.size() >= ldst_queue_size_) return;

    if (this->get_simulator().get_clock() < send_cycle_) return;

    if(dl1_.write_request(*pending_request_) == false)
    {
        write_request_failed_ = true;
        dl1_.set_wake_up_on_read(true);
        return;
    }

    write_request_failed_ = false;
    dl1_.set_wake_up_on_read(false);

    memory_access_sent_++;

    assert(ldst_queue_.size() < ldst_queue_size_);
    ldst_queue_.insert(typename ldst_queue_t::value_type(pending_request_->get_tag(), pending_request_));
    pending_request_ = NULL;
}


template<typename T, typename R>
inline
void STTMRAM<T,R>::start()
{
    send_mem_request();
}


template<typename T, typename R>
inline
void STTMRAM<T,R>::end()
{
    get_mem_response();
    fetch_mem_request();

    if ((pending_request_ == NULL) || write_request_failed_)
    {
        this->set_module_activity(engine::NEVER);
        if (pending_request_ == NULL)
        {
        	finish_cycle_ = this->get_simulator().get_clock();
        }
    }
    else if (this->get_simulator().get_clock() < send_cycle_)
    {
        this->set_module_activity(send_cycle_ - this->get_simulator().get_clock());
    }
    else
    {
        this->set_module_activity(engine::NEXT);
    }

    write_request_failed_ = false;
}

template<typename T, typename R>
inline
const std::string STTMRAM<T,R>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename T, typename R>
inline
bool STTMRAM<T,R>::inspect(const typename T::Request &)
{
    return true;
}


template<typename T, typename R>
inline
bool STTMRAM<T,R>::inspect(const typename T::Ack &)
{
    return true;
}

}
}


#endif /* STTMRAM_IMPL_H_ */
