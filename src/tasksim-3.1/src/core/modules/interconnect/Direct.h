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


#ifndef CORE_MODULES_INTERCONNECT_DIRECT_H_
#define CORE_MODULES_INTERCONNECT_DIRECT_H_

#include <core/engine/InPort.h>
#include <core/engine/OutPort.h>
#include <core/utils/macros.h>

namespace sim {
namespace interconnect {

template<typename Message>
class Direct {
public:
    class Interface {
    protected:
        engine::Module *peer_;
    public:
        Interface();
        engine::Module &get_peer();
    };

    /* We need an object because complex interconnects
     * require calling the object in some situations */
    class Input : public Interface {
    protected:
        engine::InPort<typename Message::Request> in_;
        engine::OutPort<typename Message::Ack> out_;
        engine::OutPort<bool> miss_;

        friend class Output;
        friend class Direct;
    public:
        explicit Input(engine::Module &owner, unsigned out_size = 1);
        virtual ~Input();

        bool can_read_request();
        template<typename Container, typename... Args>
        bool read_request(Container &, const Args &...);
        bool read_request(typename Message::Request &request);
        bool can_write_ack() const;
        bool write_ack(const typename Message::Ack &ack);
        void signal_miss();
    };


    class Output : public Interface {
    protected:
        engine::OutPort<typename Message::Request> out_;
        engine::InPort<typename Message::Ack> in_;
        engine::InPort<bool> miss_;

        Input* input_ptr_;

    public:
        explicit Output(engine::Module &owner, unsigned out_size = 1);

        bool can_write_request() const;
        bool write_request(const typename Message::Request &request);
        bool can_read_ack();
        template<typename Container, typename... Args>
        bool read_ack(Container &, const Args &...);
        bool read_ack(typename Message::Ack &miss);
        bool read_miss(bool &miss);

        void operator >>(Input &input);
        void set_wake_up_on_read(bool wake_up_on_read);
    };
};


}  // namespace interconnect
}  // namespace sim

#include "Direct-impl.h"

#endif  // CORE_MODULES_INTERCONNECT_DIRECT_H_
