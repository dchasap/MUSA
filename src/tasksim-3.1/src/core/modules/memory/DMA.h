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


#ifndef CORE_MODULES_MEMORY_DMA_H_
#define CORE_MODULES_MEMORY_DMA_H_

#include <list>
#include <unordered_map>

#include "core/engine/IModule.h"
#include "core/engine/types.h"
#include "core/logic/memory/MMU.h"

namespace sim {
namespace memory {

template<typename Message, template<typename> class Interconnect>
class DMA : public engine::IModule<Message>, public sim::logic::memory::IDMA {
protected:
    struct memcpy_request_t {
        engine::addr_t source_addr;
        engine::addr_t destination_addr;
        std::size_t memcpy_request_size;
        unsigned write_ack_count;

        memcpy_request_t(const engine::addr_t &src_addr, const engine::addr_t &dst_addr, const std::size_t &size):
            source_addr(src_addr), destination_addr(dst_addr), memcpy_request_size(size), write_ack_count(0) {}
    };

    std::size_t line_size_;
    std::size_t page_size_;

    typedef std::list<memcpy_request_t> memcpy_request_list_t;
    memcpy_request_list_t memcpy_requests_;

    typedef std::unordered_map<engine::addr_t, typename memcpy_request_list_t::iterator> memcpy_request_map_t;
    memcpy_request_map_t memcpy_request_source_map_;
    memcpy_request_map_t memcpy_request_destination_map_;

    std::list<typename Message::Request> output_buffer_;
    bool write_failed_;

    sim::logic::memory::MMU &mmu_;

    stats::Counter requests_sent_;
    stats::Counter acks_received_;
    stats::Counter read_requests_sent_;
    stats::Counter read_acks_received_;
    stats::Counter write_requests_sent_;
    stats::Counter write_acks_received_;

public:
    DMA(engine::Simulator &, engine::Config, sim::logic::memory::MMU &);
    ~DMA();

    bool reset();
    void start();
    void end();
    const std::string get_name() const;

    bool inspect(const typename Message::Request &);
    bool inspect(const typename Message::Ack &);

    void memcpy(const engine::addr_t &, const engine::addr_t &, const std::size_t &, uint64_t ip);

    typename Interconnect<Message>::Output out_;
};

}  // namespace memory
}  // namespace sim

#include "DMA-impl.h"

#endif  // CORE_MODULES_MEMORY_DMA_H_
