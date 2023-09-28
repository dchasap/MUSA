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


#ifndef RUNTIME_FASTMEM_H_
#define RUNTIME_FASTMEM_H_

#include <core/engine/EventBuffer.h>
#include <simulators/fastmemsim/trace/types.h>
#include <simulators/fastmemsim/trace/TraceReader.h>

namespace sim {
namespace runtime {

class FastMem {
public:
    typedef engine::EventBuffer<trace::fastmem::mem_t> mem_buffer_t;

    struct cpu_buffer_t {
        mem_buffer_t mem_;
        sim::trace::fastmem::TraceReader* tr_;

        cpu_buffer_t(engine::Simulator &, FastMem &,
                       void (FastMem::*)(mem_buffer_t &),
                       std::size_t, std::string, unsigned);
    };

protected:
    engine::Simulator &sim_;

    static const unsigned Event_Buffer_Size_ = 8192;
    std::vector<cpu_buffer_t *> cpu_buffers_;
    std::string trace_file_prefix_;

    void fill_mem_buffer(mem_buffer_t &);

public:
    FastMem(engine::Simulator &, const std::string &);
    ~FastMem();

    cpu_buffer_t &operator[](unsigned n);
};

}
}


#endif /* FASTMEM_H_ */
