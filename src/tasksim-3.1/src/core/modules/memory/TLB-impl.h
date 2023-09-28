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


#ifndef CORE_MODULES_MEMORY_TLB_IMPL_H_
#define CORE_MODULES_MEMORY_TLB_IMPL_H_

#include <utility>

namespace sim {
namespace memory {

template<typename Message, template<typename> class Interconnect>
inline
TLB<Message, Interconnect>::page_stat_item_t::page_stat_item_t(unsigned requests_sent, bool clear) :
    requests_sent_(requests_sent), clear_(clear)
{}

template<typename Message, template<typename> class Interconnect>
inline
TLB<Message, Interconnect>::TLB(engine::Simulator &sim,
                                engine::Config config,
                                sim::logic::memory::MMU &mmu,
                                unsigned cpu_id) :
    engine::IModule<Message>(sim, engine::ModuleType::TLB),
    clears_requested_(*this, "clears_requested"),
    clears_completed_(*this, "clears_completed"),
    clear_requests_sent_(*this, "clear_requests_sent"),
    clear_acks_received_(*this, "clear_acks_received"),
    total_requests_sent_(*this, "total_requests_sent"),
    total_acks_received_(*this, "total_acks_received"),
    translations_held_back_(*this, "translations_held_back"),
    translation_hold_back_time_(*this, "translation_hold_back_time"),
    page_size_(config.get_value<std::size_t>("page-size")),
    write_request_failed_(false),
    mmu_(mmu), cpu_id_(cpu_id),
    in_(*this), out_(*this)
{
    mmu.register_tlb(this);
}


template<typename Message, template<typename> class Interconnect>
inline
TLB<Message, Interconnect>::~TLB()
{
    if ((0 < output_request_buffer_.size()) || (0 < output_ack_buffer_.size()) ||
            (0 < pending_translations_buffer_.size()) || (0 < page_clear_map_.size())) {
        std::cout << "ERROR - PENDING OPERATIONS IN TLB:" << cpu_id_ << std::endl;
        std::cout << "output_request_buffer:" << output_request_buffer_.size() << std::endl;
        std::cout << "output_ack_buffer:" << output_ack_buffer_.size() << std::endl;
        std::cout << "pending_translation_buffer:" << pending_translations_buffer_.size() << std::endl;
        std::cout << "page_clear_map:" << page_clear_map_.size() << std::endl;

        typename utils::TimeList<typename Message::Request>::iterator i = pending_translations_buffer_.begin();
        while (i != pending_translations_buffer_.end()) {
            std::cout << "Translation unresolved: cpu_id(" << cpu_id_ << "), log_addr("
                      << i->data_.get_tag() << ")" << std::endl;
            i++;
        }
    }
}


template<typename Message, template<typename> class Interconnect>
inline
bool TLB<Message, Interconnect>::reset()
{
    return true;
}


template<typename Message, template<typename> class Interconnect>
inline
void TLB<Message, Interconnect>::start()
{
    if (!output_request_buffer_.empty()) {
        if (out_.write_request(output_request_buffer_.front()) == true) {
            total_requests_sent_++;
            if (output_request_buffer_.front().get_op() == Message::Request::op_t::CLEAR)
                clear_requests_sent_++;
            output_request_buffer_.pop_front();
            write_request_failed_ = false;
            out_.set_wake_up_on_read(true);
        } else {
            write_request_failed_ = true;
            out_.set_wake_up_on_read(false);
        }
    }

    if ((!output_ack_buffer_.empty()) && (in_.write_ack(output_ack_buffer_.front())))
        output_ack_buffer_.pop_front();
}


template<typename Message, template<typename> class Interconnect>
inline
void TLB<Message, Interconnect>::end()
{
    typename Message::Request req;
    if (in_.read_request(req) == true) {
        engine::addr_t* translation = mmu_.translate(cpu_id_, req.get_tag());
        if (translation != NULL) {
            send_translated_request(req, *translation);
            delete translation;
        } else {
            pending_translations_buffer_.emplace_back(this->get_simulator().get_clock(),
                                                      typename Message::Request(req));
            translations_held_back_++;
        }
    }

    typename Message::Ack ack;
    if ((out_.read_ack(ack) == true) && (ack.get_op() != Message::Ack::op_t::MISS)) {
        engine::addr_t physical_addr = ack.get_tag();
        engine::addr_t physical_page = physical_addr & (~(page_size_ - 1));

        total_acks_received_++;
        if (ack.get_op() == Message::Ack::op_t::CLEAR) {
            clear_acks_received_++;
            typename page_clear_map_t::iterator i = page_clear_map_.find(physical_page);
            i->second.acks_pending_--;
            if (i->second.acks_pending_ == 0) {
                page_clear_map_.erase(i);
                mmu_.notify_clear_complete(cpu_id_, physical_page);
                clears_completed_++;
            }
        } else {
            typename page_stat_map_t::iterator i = page_stat_map_.find(physical_page);
            i->second.requests_sent_--;
            if (0 == i->second.requests_sent_) {
                if (i->second.clear_)
                    send_clear_request(physical_page);

                page_stat_map_.erase(i);
            }

            output_ack_buffer_.emplace_back(typename Message::Ack(ack.get_op(),
                                            mmu_.reverse_translate(ack.get_tag()),
                                            ack.get_size()));
            // output_ack_buffer_.emplace_back(typename Message::Ack(ack.get_op(), ack.get_tag(), ack.get_size()));
        }
    }

    if (((!output_request_buffer_.empty()) && (!write_request_failed_)) || (!output_ack_buffer_.empty()))
        this->set_module_activity(engine::NEXT);
    else
        this->set_module_activity(engine::NEVER);

    write_request_failed_ = false;
}


template<typename Message, template<typename> class Interconnect>
inline
const std::string TLB<Message, Interconnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename Message, template<typename> class Interconnect>
inline
bool TLB<Message, Interconnect>::inspect(const typename Message::Request &req)
{
    return false;
}


template<typename Message, template<typename> class Interconnect>
inline
bool TLB<Message, Interconnect>::inspect(const typename Message::Ack &ack)
{
    return true;
}


template<typename Message, template<typename> class Interconnect>
inline
void TLB<Message, Interconnect>::clear_page(engine::addr_t &physical_page)
{
    clears_requested_++;

    typename page_stat_map_t::iterator i = page_stat_map_.find(physical_page);
    if (i != page_stat_map_.end()) {
        i->second.clear_ = true;
    } else {
        send_clear_request(physical_page);
        this->set_module_activity(engine::NEXT);
    }
}


template<typename Message, template<typename> class Interconnect>
inline
void TLB<Message, Interconnect>::send_clear_request(engine::addr_t &physical_page)
{
    output_request_buffer_.emplace_back(
            typename Message::Request(
                    Message::Request::op_t::CLEAR,
                    physical_page,
                    page_size_));

    page_clear_map_.insert(
            std::pair<engine::addr_t, page_clear_item_t>(
                    physical_page,
                    page_clear_item_t(1)));
}

template<typename Message, template<typename> class Interconnect>
inline
void TLB<Message, Interconnect>::send_translated_request(typename Message::Request &req, engine::addr_t &physical_addr)
{
    engine::addr_t physical_page = physical_addr & (~(page_size_ - 1));

    typename page_stat_map_t::iterator i = page_stat_map_.find(physical_page);
    if (i != page_stat_map_.end())
        i->second.requests_sent_++;
    else
        page_stat_map_.insert(std::pair<engine::addr_t, page_stat_item_t>(physical_page, page_stat_item_t(1, false)));

    output_request_buffer_.emplace_back(typename Message::Request(req.get_op(), physical_addr, req.get_size()));
}

template<typename Message, template<typename> class Interconnect>
inline
void TLB<Message, Interconnect>::notify_translation()
{
    bool pending_translation_resolved = false;
    typename utils::TimeList<typename Message::Request>::iterator i = pending_translations_buffer_.begin();
    while (i != pending_translations_buffer_.end()) {
        engine::addr_t* translation = mmu_.translate(cpu_id_, i->data_.get_tag(), false);
        if (NULL != translation) {
            send_translated_request(i->data_, *translation);
            // delete translation;
            translation_hold_back_time_.sample(this->get_simulator().get_clock() - i->time_);
            pending_translations_buffer_.erase(i++);
            pending_translation_resolved = true;
        } else {
            ++i;
        }
    }
    if (pending_translation_resolved) {
        this->set_module_activity(engine::NEXT);
    }
}

}  // namespace memory
}  // namespace sim

#endif  // CORE_MODULES_MEMORY_TLB_IMPL_H_
