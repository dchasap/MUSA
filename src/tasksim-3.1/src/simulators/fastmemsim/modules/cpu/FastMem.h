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


#ifndef MODULES_CPU_FASTMEM_H_
#define MODULES_CPU_FASTMEM_H_

#include <unordered_map>

#include <core/engine/InPort.h>
#include <core/engine/Module.h>
#include <core/engine/IModule.h>
#include <core/engine/types.h>
#include <core/engine/EventBuffer.h>
#include <core/modules/interconnect/Direct.h>
#include <core/stats/Counter.h>
#include <simulators/fastmemsim/trace/types.h>

namespace sim {
namespace cpu {

template<typename T, typename R>
class FastMem : public engine::IModule<T> {
public:
    typedef engine::EventBuffer<trace::fastmem::mem_t> mem_buffer_t;
protected:
    typename T::Request *pending_request_;
    engine::cycles_t send_cycle_;
    typedef std::unordered_multimap<engine::addr_t, typename T::Request *> ldst_queue_t;
    ldst_queue_t ldst_queue_;
    std::size_t ldst_queue_size_;
    stats::Counter memory_access_sent_;
    stats::Counter memory_access_received_;
    bool write_request_failed_;

    mem_buffer_t *memory_in_;

    void get_mem_response();
    void fetch_mem_request();
    void send_mem_request();
public:
    FastMem(engine::Simulator &, const engine::Config &);
    ~FastMem();

    bool reset();
    void start();
    void end();
    const std::string get_name() const;

    bool inspect(const typename T::Request &);
    bool inspect(const typename T::Ack &);

    /* Interface to memory hierarchy */
    typename interconnect::Direct<T>::Output dl1_;

    /** Operator to connect to the event buffer */
    friend void operator>>(mem_buffer_t &e, FastMem<T,R> &c) {
        c.memory_in_ = &e;
    }
};

}
}

#include "FastMem-impl.h"

#endif /* FASTMEM_H_ */
