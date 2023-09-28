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


#ifndef CORE_ENGINE_EVENTBUFFER_H_
#define CORE_ENGINE_EVENTBUFFER_H_

#include <queue>

#include "core/engine/Module.h"

namespace sim {
namespace engine {

class Simulator;

template<typename T>
class EventBuffer {
protected:
    std::function<void(EventBuffer<T>&)> fill_function_;

    std::size_t buffer_size_;
    std::queue<T> buffer_;

    unsigned buffer_id_;
    engine::Module *remote_module_;

public:
    EventBuffer(std::function<void(EventBuffer&)> func, std::size_t, unsigned = 0);
    ~EventBuffer();

    /* Emulation side interface */
    bool push_event(T &&);
    bool push_event(const T &);
    std::size_t space_left() const;
    bool empty() { return buffer_.empty(); }

    unsigned get_id() const;
    void set_remote_module(engine::Module* remote_module) { remote_module_ = remote_module; }
    engine::Module *const get_remote_module() const { return remote_module_; }

    bool read(T&);
};


}  // namespace engine
}  // namespace sim

#include "EventBuffer-impl.h"

#endif  // CORE_ENGINE_EVENTBUFFER_H_
