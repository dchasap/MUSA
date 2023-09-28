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

#ifndef RUNTIME_SIMPLE_MEMORY_H_
#define RUNTIME_SIMPLE_MEMORY_H_

#include <vector>
#include <core/engine/EventBuffer.h>
#include <core/trace/mem/Trace.h>
#include <core/trace/mem/types.h>
#include <core/trace/types.h>


namespace sim {

namespace engine {
class Simulator;
}

namespace runtime {

class SimpleMemory {
public:
    typedef engine::EventBuffer<sim::trace::mitem_t> mem_buffer_t;

    struct cpu_buffer_t {
        mem_buffer_t mem_;

        cpu_buffer_t(engine::Simulator &, SimpleMemory &,
                void (SimpleMemory::*)(mem_buffer_t &),
                std::size_t, unsigned);
    };
protected:
    trace::mem::FileTraceReader<trace::SelectedEventStream> input_trace_;
    engine::Simulator &sim_;

    static const unsigned Event_Buffer_Size_ = 1;
    std::vector<cpu_buffer_t *> cpu_buffers_;

    //void fill_instr_buffer(instr_buffer_t &);
    void fill_mem_buffer(mem_buffer_t &);

    trace::bbl_id_t pending_bbl_;
    bool halt_;
public:
    SimpleMemory(engine::Simulator &, const std::string &trace_file);
    ~SimpleMemory();

    cpu_buffer_t &operator[](unsigned n);
};


}
}



#endif
