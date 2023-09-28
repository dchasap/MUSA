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


#ifndef CORE_MODULES_MEMORY_TLB_H_
#define CORE_MODULES_MEMORY_TLB_H_

#include <list>
#include <map>

#include "core/engine/IModule.h"
#include "core/engine/types.h"
#include "core/logic/memory/MMU.h"
#include "core/utils/TimeList.h"

namespace sim {
namespace memory {

template<typename Message, template<typename> class Interconnect>
class TLB : public engine::IModule<Message>, public sim::logic::memory::ITLB {
protected:
    stats::Counter clears_requested_;
    stats::Counter clears_completed_;
    stats::Counter clear_requests_sent_;
    stats::Counter clear_acks_received_;
    stats::Counter total_requests_sent_;
    stats::Counter total_acks_received_;
    stats::Counter translations_held_back_;

    stats::Average translation_hold_back_time_;

    std::size_t page_size_;

    std::list<typename Message::Request> output_request_buffer_;
    std::list<typename Message::Ack> output_ack_buffer_;
    utils::TimeList<typename Message::Request> pending_translations_buffer_;
    // std::list<typename Message::Request> pending_translations_buffer_;

    bool write_request_failed_;

    sim::logic::memory::MMU &mmu_;

    unsigned cpu_id_;

    struct page_stat_item_t {
            unsigned requests_sent_;
            bool clear_;
            page_stat_item_t(unsigned, bool);
    };

    typedef std::map<engine::addr_t, page_stat_item_t> page_stat_map_t;
    page_stat_map_t page_stat_map_;

    void send_clear_request(engine::addr_t &);

    struct page_clear_item_t {
        unsigned acks_pending_;
        explicit page_clear_item_t(const unsigned acks_pending):
            acks_pending_(acks_pending) {}
    };
    typedef std::map<engine::addr_t, page_clear_item_t> page_clear_map_t;
    page_clear_map_t page_clear_map_;

    void send_translated_request(typename Message::Request &, engine::addr_t &);

public:
    TLB(engine::Simulator &, engine::Config, sim::logic::memory::MMU &, unsigned);
    ~TLB();

    bool reset();
    void start();
    void end();
    const std::string get_name() const;

    bool inspect(const typename Message::Request &);
    bool inspect(const typename Message::Ack &);

    typename Interconnect<Message>::Input in_;
    typename Interconnect<Message>::Output out_;

    void clear_page(engine::addr_t &);
    virtual void notify_translation();
};

}  // namespace memory
}  // namespace sim

#include "TLB-impl.h"

#endif  // CORE_MODULES_MEMORY_TLB_H_
