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


#ifndef RUNTIME_FRONTEND_H_
#define RUNTIME_FRONTEND_H_

#include <map>
#include <limits>

#include <core/engine/EventBuffer.h>
#include <simulators/frontendsim/trace/types.h>
#include <simulators/frontendsim/trace//TraceReader.h>

namespace sim {
namespace runtime {

typedef enum {RUN = 0, WAIT} state_t;

class FrontEnd;

// synchronization primitives: Semaphore and Barrier
class Semaphore {
protected:
	unsigned counter_;
	std::queue<unsigned> waiters_;

public:
	Semaphore(unsigned c);
	~Semaphore();

	state_t wait(unsigned id);
	unsigned signal();
};

class Barrier {
protected:
	unsigned total_;
	unsigned counter_;

public:
	Barrier(unsigned t);
	~Barrier();
	state_t pass();
	unsigned get_total();
	unsigned get_counter();
};

class Arbitrator {
protected:
	FrontEnd* runtime_;
	Barrier* global_barrier_;
	Semaphore* global_semaphore_;
	std::map<uint64_t, Semaphore*> semaphores_;
	unsigned num_of_threads_;

public:
	Arbitrator(FrontEnd* runtime);
	~Arbitrator();

	void inc_num_of_threads();
	unsigned get_num_of_threads();
	state_t handle_sync_event(unsigned id, trace::frontend::event_t& e);
};

class FrontEnd {
public:
	typedef std::map<uint64_t, trace::frontend::bbl_stat_t*> dictionary_t;
    typedef engine::EventBuffer<trace::frontend::bbl_stat_t*> bbl_buffer_t;

    struct cpu_buffer_t {
        bbl_buffer_t bbl_;
        sim::trace::frontend::TraceReader* tr_;

        cpu_buffer_t(engine::Simulator &, FrontEnd &,
                       void (FrontEnd::*)(bbl_buffer_t &),
                       std::size_t, std::string, unsigned);
    };

protected:
    engine::Simulator &sim_;
    Arbitrator* arbitrator_;
    static const unsigned Event_Buffer_Size_ = 2;
    std::vector<cpu_buffer_t *> cpu_buffers_;
    std::vector<state_t> cpu_states_;
    std::string trace_file_prefix_;
    dictionary_t dictionary_;

    bool in_parallel_region_;			// if I am in parallel region or not (if I am, worker threads should count for waiting cycles)
    uint64_t total_parallel_regions_;
    uint64_t current_parallel_regions_;

    void fill_bbl_buffer(bbl_buffer_t &);
    void fill_dictionary();

public:

    friend class Arbitrator;
    FrontEnd(engine::Simulator &, const std::string &);
    ~FrontEnd();

    cpu_buffer_t &operator[](unsigned n);
};

}
}


#endif /* FRONTEND_H_ */
