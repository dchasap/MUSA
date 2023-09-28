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


#ifndef CORE_MODULES_MEMORY_DMA_IMPL_H_
#define CORE_MODULES_MEMORY_DMA_IMPL_H_

#include <utility>

#include "core/utils/Log.h"

namespace sim {
namespace memory {

using sim::utils::Log;

template<typename Message, template<typename> class Interconnect>
inline
DMA<Message, Interconnect>::DMA(engine::Simulator &sim, engine::Config config, sim::logic::memory::MMU &mmu) :
    engine::IModule<Message>(sim, engine::ModuleType::DMA),
    line_size_(config.get_value<unsigned>("line-size")),
    page_size_(config.get_value<unsigned>("page-size")),

    write_failed_(false),
    mmu_(mmu),
    requests_sent_(*this, "requests_sent"),
    acks_received_(*this, "acks_received"),
    read_requests_sent_(*this, "read_requests_sent"),
    read_acks_received_(*this, "read_acks_received"),
    write_requests_sent_(*this, "write_requests_sent"),
    write_acks_received_(*this, "write_acks_received"),
    out_(*this)
{
    mmu_.register_dma(this);
}


template<typename Message, template<typename> class Interconnect>
inline
DMA<Message, Interconnect>::~DMA()
{
}


template<typename Message, template<typename> class Interconnect>
inline
bool DMA<Message, Interconnect>::reset()
{
    return true;
}


template<typename Message, template<typename> class Interconnect>
inline
void DMA<Message, Interconnect>::start()
{
    if (output_buffer_.empty())
        return;

    typename Message::Request req = output_buffer_.front();
    if (out_.write_request(req) == false) {
        write_failed_ = true;
        out_.set_wake_up_on_read(true);
        return;
    }

    out_.set_wake_up_on_read(false);

    requests_sent_++;
    if (req.get_op() == Message::Request::op_t::READ)
        read_requests_sent_++;
    if (req.get_op() == Message::Request::op_t::WRITE)
        write_requests_sent_++;

    output_buffer_.pop_front();
}


template<typename Message, template<typename> class Interconnect>
inline
void DMA<Message, Interconnect>::end()
{
    typename Message::Ack ack_req;
    if (out_.read_ack(ack_req) == true) {
        acks_received_++;

        engine::addr_t page_base_addr = ack_req.get_tag() & (~(page_size_ - 1));

        if (ack_req.get_op() == Message::Ack::op_t::READ) {
            typename memcpy_request_map_t::iterator it = memcpy_request_source_map_.find(page_base_addr);
            if (it != memcpy_request_source_map_.end()) {
                read_acks_received_++;
                memcpy_request_t memcpy_request = *(it->second);
                output_buffer_.push_back(typename Message::Request(Message::Request::op_t::WRITE,
                        memcpy_request.destination_addr + (ack_req.get_tag() - memcpy_request.source_addr),
                        0, line_size_));
            }
        }

        if (ack_req.get_op() == Message::Ack::op_t::WRITE) {
            typename memcpy_request_map_t::iterator it = memcpy_request_destination_map_.find(page_base_addr);
            if (it != memcpy_request_destination_map_.end()) {
                write_acks_received_++;
                memcpy_request_t memcpy_request = *(it->second);
                (it->second)->write_ack_count++;
                if ((it->second)->write_ack_count == memcpy_request.memcpy_request_size / line_size_) {
                    typename memcpy_request_map_t::iterator source_it =
                            memcpy_request_source_map_.find(memcpy_request.source_addr);
                    memcpy_request_source_map_.erase(source_it);
                    memcpy_requests_.erase(it->second);
                    memcpy_request_destination_map_.erase(it);

                    // Migration completed. Notify MMU
                    mmu_.notify_migration_complete(memcpy_request.source_addr,
                                                   memcpy_request.destination_addr,
                                                   memcpy_request.memcpy_request_size);
                }
            }
        }
    }

    if ((output_buffer_.empty() == true) || (write_failed_))
        this->set_module_activity(engine::NEVER);
    else
        this->set_module_activity(engine::NEXT);

    write_failed_ = false;
}

template<typename Message, template<typename> class Interconnect>
inline
const std::string DMA<Message, Interconnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}


template<typename Message, template<typename> class Interconnect>
inline
bool DMA<Message, Interconnect>::inspect(const typename Message::Request &req)
{
    return false;
}


template<typename Message, template<typename> class Interconnect>
inline
bool DMA<Message, Interconnect>::inspect(const typename Message::Ack &ack)
{
    engine::addr_t page_base_addr = ack.get_tag() & (~(page_size_ - 1));

    if ((ack.get_op() == Message::Ack::op_t::READ) &&
            (memcpy_request_source_map_.find(page_base_addr) != memcpy_request_source_map_.end())) {
        return true;
    }

    if ((ack.get_op() == Message::Ack::op_t::WRITE) &&
            (memcpy_request_destination_map_.find(page_base_addr) != memcpy_request_destination_map_.end())) {
        return true;
    }

    return false;
}


template<typename Message, template<typename> class Interconnect>
inline
void DMA<Message, Interconnect>::memcpy(const engine::addr_t &source_addr,
                                        const engine::addr_t &destination_addr,
                                        const std::size_t &size)
{
    Log::debug2() << this->get_simulator().get_clock() << ":memcpy_start:" << source_addr
                  << ":" << destination_addr << ":" << size;
    memcpy_request_t memcpy_request(source_addr, destination_addr, size);
    memcpy_requests_.push_back(memcpy_request);
    typename memcpy_request_list_t::iterator it = --memcpy_requests_.end();

    memcpy_request_source_map_.insert(std::pair<engine::addr_t,
                                      typename memcpy_request_list_t::iterator>(source_addr, it));
    memcpy_request_destination_map_.insert(std::pair<engine::addr_t,
                                      typename memcpy_request_list_t::iterator>(destination_addr, it));

    if (output_buffer_.empty()) {
        this->set_module_activity(engine::NEXT);
    }
    for (engine::addr_t addr = source_addr; addr < source_addr + size; addr += line_size_) {
        output_buffer_.push_back(typename Message::Request(Message::Request::op_t::READ, addr, line_size_));
    }
}


}  // namespace memory
}  // namespace sim


#endif  // CORE_MODULES_MEMORY_DMA_IMPL_H_

