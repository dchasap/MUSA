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


#ifndef RUNTIME_OMPSS_TEST_H_
#define RUNTIME_OMPSS_TEST_H_

#include <core/engine/EventBuffer.h>
#include <core/trace/ts/Trace.h>
#include <core/trace/TextEventStream.h>
#include <core/trace/ts/Trace.h>
#include <core/trace/ts/types.h>
#include <core/trace/intel/Trace.h>

#include <vector>

namespace sim {

namespace engine {
class Simulator;
}

namespace runtime {

class OmpSs {
public:

    typedef engine::EventBuffer<trace::ts::phase_t> burst_buffer_t;
    typedef engine::EventBuffer<trace::minst_t> instr_buffer_t;
    typedef engine::EventBuffer<trace::mem_t> mem_buffer_t;

    struct cpu_buffer_t {
        burst_buffer_t burst_;
        instr_buffer_t instr_;
        mem_buffer_t mem_;

        trace::ts::phase_t current_phase_;

        cpu_buffer_t(engine::Simulator &, OmpSs &,
                void (OmpSs::*)(burst_buffer_t &),
                void (OmpSs::*)(instr_buffer_t &),
                void (OmpSs::*)(mem_buffer_t &),
                std::size_t, unsigned);
    };
protected:
    trace::ts::FileTraceReader<trace::TextEventStream,trace::intel::TraceInternal> input_trace_;
    engine::Simulator &sim_;

    static const unsigned Event_Buffer_Size_ = 1024;
    std::vector<cpu_buffer_t *> cpu_buffers_;

    void fill_burst_buffer(burst_buffer_t &);
    void fill_instr_buffer(instr_buffer_t &);
    void fill_mem_buffer(mem_buffer_t &);
public:
    OmpSs(engine::Simulator &, const std::string &trace_file);
    ~OmpSs();

    cpu_buffer_t &operator[](unsigned n);
};

}
}


#endif /* OMPSS_TEST_H_ */
