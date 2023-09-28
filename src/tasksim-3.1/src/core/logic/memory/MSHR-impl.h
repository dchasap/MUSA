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

#ifndef CORE_LOGIC_MEMORY_MSHR_IMPL_H_
#define CORE_LOGIC_MEMORY_MSHR_IMPL_H_

#include <cassert>
#include <vector>

#include "core/utils/Log.h"
#include "core/utils/macros.h"
#include "core/runtime/TaskDataDep.h"
#include "core/engine/types.h"
#include "core/logic/coherence/single/Message.h"

namespace sim {
namespace logic {
namespace memory {

using sim::utils::Log;

template<typename Message>
inline
MSHR<Message>::MSHRState::MSHRState(state_t state) :
    value_(state), time_(0)
{}

template<typename Message>
inline
MSHR<Message>::subentry_t::subentry_t(unsigned port, const typename Message::Request &request) :
        request_(request), port_(port)
{}

template<typename Message>
inline
MSHR<Message>::entry_t::entry_t(sim::engine::addr_t tag) :
        tag_(tag), state_(MSHRState::INVALID), request_(), list_(), dirty_(false), fill_(false)
{}

template<typename Message>
inline
MSHR<Message>::entry_t::entry_t(sim::engine::addr_t tag, const typename Message::Request &request) :
        tag_(tag), state_(MSHRState::INVALID), request_(request), list_(), dirty_(false)
{}

template<typename Message>
inline
MSHR<Message>::entry_t::~entry_t() {
    assert(list_.empty());
}

template<typename Message>
inline
MSHR<Message>::MSHR(const engine::Config &mshr_config, std::size_t cache_line_size) :
    data_size_(mshr_config.get_value<std::size_t>("size")),
    mask_(~(cache_line_size - 1))
{
}

template<typename Message>
inline
MSHR<Message>::~MSHR()
{
}

template<typename Message>
inline
std::size_t MSHR<Message>::get_size() const
{
    return data_size_;
}

template<typename Message>
inline
std::size_t MSHR<Message>::get_free_size() const
{
    return data_size_ - data_.size();
}

template<typename Message>
inline
typename MSHR<Message>::entry_t* MSHR<Message>::allocate_entry(const sim::engine::addr_t &addr,
        const typename Message::Request &request)
{
    /* Sanity check: there should be no entry for this address */
    sim::engine::addr_t line_addr = addr & mask_;
    if (data_size_ == data_.size()) {
        return NULL;
    }
    assert(data_.find(line_addr) == data_.end());
    data_[line_addr] = entry_t(line_addr, request);
    return &data_[line_addr];
}

template<typename Message>
inline
typename MSHR<Message>::entry_t* MSHR<Message>::lookup_entry(const sim::engine::addr_t &addr)
{
    sim::engine::addr_t line_addr = addr & mask_;
    auto it = data_.find(line_addr);
    if (it == data_.end()) {
        return NULL;
    }
    return &it->second;
}

template<typename Message>
inline
void MSHR<Message>::remove_entry(entry_t &entry)
{
    assert(entry.list_.empty() == true);
#if !defined(NDEBUG)
    auto it = data_.find(entry.tag_);
    assert(it != data_.end());
    assert(&it->second == &entry);
#endif
    size_t removed = data_.erase(entry.tag_);
    assert(removed > 0);
    _unused(removed);
}

template<typename Message>
inline
void MSHR<Message>::clear_entries()
{
    for (auto &elem : data_) {
        elem.second.list_.clear();
    }
    data_.clear();
}

template<typename Message>
inline
void MSHR<Message>::print_status() const
{
    Log::warning() << " - MSHR has " << data_.size() << " entries of a limit of " << data_size_;
    for (auto &elem : data_) {
        Log::warning() << "TAG 0x" << std::hex << elem.first << std::dec;
    }
}


}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_MSHR_IMPL_H_
