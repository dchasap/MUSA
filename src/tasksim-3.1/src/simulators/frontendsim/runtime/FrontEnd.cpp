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


#include <fstream>
#include <functional>
#include <sstream>
#include <cstdint>

#include "FrontEnd.h"

#include "core/utils/Log.h"

namespace sim {
namespace runtime {

using sim::utils::Log;

/***********************************************
 *************** SEMAPHORE class ***************
 **********************************************/

Semaphore::Semaphore(unsigned c) : counter_(c) {
	waiters_ = std::queue<unsigned>();
}

Semaphore::~Semaphore() {}

state_t Semaphore::wait(unsigned id) {
	state_t ret = RUN;
	if (counter_ > 0) {
		counter_--;
	} else {
		waiters_.push(id);
		ret = WAIT;
	}
	return ret;
}

unsigned Semaphore::signal() {
	// maybe I could change it to max_uint or something...
	unsigned ret = std::numeric_limits<unsigned>::max();
	if (waiters_.empty()) {
		counter_++;
	} else {
		ret = waiters_.front();
		waiters_.pop();
	}
	return ret;
}

/***********************************************
 *************** BARRIER class *****************
 **********************************************/

Barrier::Barrier(unsigned t) : total_(t), counter_(0) {}

Barrier::~Barrier() {}

// arbitrator will be the one that will set all the other
// threads to RUN if the counter is reached
state_t Barrier::pass() {
	counter_++;
	if (counter_ == total_) {
		counter_ = 0;
		return RUN;
	} else
		return WAIT;
}

unsigned Barrier::get_total() {
	return total_;
}

unsigned Barrier::get_counter() {
	return counter_;
}

/***********************************************
 *************** ARBITRATOR class **************
 **********************************************/

Arbitrator::Arbitrator(FrontEnd* runtime) : runtime_(runtime), global_barrier_(NULL), num_of_threads_(0) {
	global_semaphore_ = new Semaphore(1);
}

Arbitrator::~Arbitrator() {
	if (global_barrier_)
		delete global_barrier_;
	if (global_semaphore_)
		delete global_semaphore_;
	runtime_ = NULL;
	std::map<uint64_t, Semaphore*>::iterator it;
	for (it = semaphores_.begin(); it != semaphores_.end(); it++)
		delete it->second;
}

void Arbitrator::inc_num_of_threads() {
	num_of_threads_++;
}

unsigned Arbitrator::get_num_of_threads() {
	return num_of_threads_;
}

// the most important method for the arbitration
state_t Arbitrator::handle_sync_event(unsigned id, trace::frontend::event_t& e) {

	state_t ret;
	switch (e.id_) {
		case trace::frontend::SIGNAL: {
			Semaphore* sem = global_semaphore_;
			if (e.type_ == trace::frontend::SYNC) {
				std::map<uint64_t, Semaphore*>::iterator it = semaphores_.find(e.address_);
				// we should never enter here from signal...
				if (it == semaphores_.end()) {
					semaphores_.insert(std::pair<uint64_t, Semaphore*>(e.address_, new Semaphore(1)));
					std::cout << "ERROR :: unmatched signals and waits!\n";
				}
				sem = semaphores_[e.address_];
			}
			unsigned wake_up = sem->signal();
			if (wake_up != std::numeric_limits<unsigned>::max())
				runtime_->cpu_states_[wake_up] = RUN;
			ret = RUN;
			break;
		}
		case trace::frontend::WAIT: {
			Semaphore* sem = global_semaphore_;
			if (e.type_ == trace::frontend::SYNC) {
				std::map<uint64_t, Semaphore*>::iterator it = semaphores_.find(e.address_);
				if (it == semaphores_.end())
					semaphores_.insert(std::pair<uint64_t, Semaphore*>(e.address_, new Semaphore(1)));
				sem = semaphores_[e.address_];
			}
			ret = sem->wait(id);
			break;
		}
		case trace::frontend::BARRIER: {
			if (global_barrier_ == NULL)
				global_barrier_ = new Barrier(num_of_threads_);
			ret = global_barrier_->pass();
			// here we have to release all the other threads if it's RUN
			if (ret == RUN) {
				for (unsigned i = 0; i < runtime_->cpu_states_.size(); i++)
					runtime_->cpu_states_[i]=RUN;
			}
			break;
		}
		case trace::frontend::PARALLEL_START: {
			Log::debug2() << runtime_->sim_.get_clock() << "::arbitrator - parallel_start in region["
			              << runtime_->current_parallel_regions_ << "]";
			ret = RUN;
			runtime_->in_parallel_region_ = true;
			for (unsigned i = 0; i < runtime_->cpu_states_.size(); i++)
				runtime_->cpu_states_[i]=RUN;
			break;
		}
		case trace::frontend::PARALLEL_END: {
			if (global_barrier_ == NULL)
				global_barrier_ = new Barrier(num_of_threads_);
			ret = global_barrier_->pass();

			// here is the difference from usual barrier... only master thread will continue!
			if (ret == RUN) {
				runtime_->in_parallel_region_ = false;
				runtime_->current_parallel_regions_++;
				for (unsigned i = 0; i < runtime_->cpu_states_.size(); i++)
					runtime_->cpu_states_[i]=(i == 0?RUN:(runtime_->current_parallel_regions_ == runtime_->total_parallel_regions_?RUN:WAIT));
				ret = runtime_->cpu_states_[id];
			}
			Log::debug2() << runtime_->sim_.get_clock() << "::arbitrator - parallel_end["
			              << global_barrier_->get_counter() << "] by id = " << id;
			break;
		}
		case trace::frontend::THREAD_CREATE: {
			ret = RUN;
			break;
		}
		default: {
			std::cout << "Something went wrong in synchronization!\n";
		}
	}
	return ret;
}

/***********************************************
 *************** FRONTEND class ****************
 **********************************************/

FrontEnd::cpu_buffer_t::cpu_buffer_t(engine::Simulator &sim, FrontEnd &provider,
                                  void (FrontEnd::*bbl_provider)(bbl_buffer_t &),
                                  std::size_t buffer_size, std::string trace_file_prefix, unsigned id) :
    bbl_(std::bind(bbl_provider, provider, std::placeholders::_1), buffer_size, id)
{
    std::stringstream ss;
    ss << id;
    tr_ = new sim::trace::frontend::TraceReader(trace_file_prefix + ".trace" + ss.str() + ".gz");
}

FrontEnd::FrontEnd(engine::Simulator &sim, const std::string &trace_file_prefix) :
        sim_(sim),
        trace_file_prefix_(trace_file_prefix),
        in_parallel_region_(false)
{
	arbitrator_ = new Arbitrator(this);
	fill_dictionary();
}

FrontEnd::~FrontEnd()
{
	// delete the dictionary
	dictionary_t::iterator it;
	for (it = dictionary_.begin(); it != dictionary_.end(); it++)
		if (it->second)
			delete (it->second);

    std::vector<cpu_buffer_t *>::const_iterator i;
    for (i = cpu_buffers_.begin(); i != cpu_buffers_.end(); ++i) {
        delete (*i);
    }

    delete arbitrator_;
}

void FrontEnd::fill_dictionary() {
	std::ifstream dict(trace_file_prefix_ + ".dict");
	assert (dict.is_open());
	// but first, read the number of parallel regions ;)
	dict >> total_parallel_regions_;
	current_parallel_regions_ = 0;
	while (!dict.eof()) {
		trace::frontend::bbl_stat_t* bbl_stat = new trace::frontend::bbl_stat_t();
		dict >> *bbl_stat;
		if (bbl_stat->size_ > 0)
			dictionary_.insert(std::pair< uint64_t, trace::frontend::bbl_stat_t* >(bbl_stat->id_, bbl_stat));
	}
	dict.close();
}

void FrontEnd::fill_bbl_buffer(bbl_buffer_t &buffer)
{
    unsigned buffer_id = buffer.get_id();

    assert(buffer_id < cpu_buffers_.size());
    assert(cpu_buffers_[buffer_id] != NULL);
    trace::frontend::event_t event;

    // send a 'sleep' event
    if (cpu_states_[buffer_id] == WAIT) {
    	trace::frontend::bbl_stat_t* sleep_bbl = new trace::frontend::bbl_stat_t();
    	sleep_bbl->id_ = UINT64_MAX;
    	sleep_bbl->size_ = in_parallel_region_?1:0;
    	buffer.push_event(sleep_bbl);
    	return;
    }

    sim::trace::frontend::TraceReader* tr = cpu_buffers_[buffer_id]->tr_;
    while ((buffer.space_left() > 0) && (!tr->eof())) {
    	trace::frontend::event_t event = tr->get_next_event();
		if (event.type_ != trace::frontend::BBL) {
			state_t ret = arbitrator_->handle_sync_event(buffer_id, event);
			if (ret == WAIT) {
    			cpu_states_[buffer_id] = ret;
    			trace::frontend::bbl_stat_t* sleep_bbl = new trace::frontend::bbl_stat_t();
    			sleep_bbl->id_ = UINT64_MAX;
    			sleep_bbl->size_ = 0;
    			buffer.push_event(sleep_bbl);
    			break;
    		}
    		continue;
    	}
        buffer.push_event(dictionary_[event.id_]);
    }
}

FrontEnd::cpu_buffer_t &FrontEnd::operator[](unsigned cpu_id)
{
    if (cpu_buffers_.size() <= cpu_id) {
        cpu_buffers_.resize(cpu_id + 1, NULL);
        // only master thread will have initial state RUN
        cpu_states_.resize(cpu_id + 1, cpu_id==0?RUN:WAIT);
        arbitrator_->inc_num_of_threads();
    }

    if (cpu_buffers_[cpu_id] == NULL) {
        cpu_buffers_[cpu_id] = new cpu_buffer_t(sim_, *this,
                &FrontEnd::fill_bbl_buffer, Event_Buffer_Size_, trace_file_prefix_, cpu_id);
    }

    return *cpu_buffers_[cpu_id];
}

}
}
