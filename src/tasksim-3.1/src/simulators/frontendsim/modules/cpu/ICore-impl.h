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


#ifndef MODULES_CPU_ICORE_IMPL_H_
#define MODULES_CPU_ICORE_IMPL_H_

namespace sim {
namespace cpu {

template<typename T, typename R, template<typename> class InterConnect>
inline
ICore<T, R, InterConnect>::ICore(engine::Simulator &sim, const engine::Config &conf, unsigned id) :
    engine::IModule<T>(sim), id_(id), bbl_in_(NULL), pending_request_(NULL), current_bbl_(0),
    current_instr_(0), over_cycle_(0), next_cycle_(0),
    
    ftq_size_(conf.get_value<std::size_t>("ftq-size")), lb_size_(conf.get_value<std::size_t>("line-buffers-size")),

    ib_counter_(0), ib_limit_(conf.get_value<std::size_t>("ib-size")),
    iq_counter_(0), iq_limit_(conf.get_value<std::size_t>("iq-size")),
    fetch_pl_depth_(conf.get_value<std::size_t>("fetch-pl-depth")), decode_rate_(conf.get_value<std::size_t>("decode-rate")),
    ipc_(conf.get_value<double>("ipc")), ipc_bubble_(0), commit_bubble_(0), possible_usefulness_bubble_(false),
    usefulness_bubble_(*this, "usefulness-bubble"), avg_fb_size_(*this, "avg-fb-size"),
    avg_bbl_size_(*this, "avg-bbl-size"), avg_line_util_(*this, "avg-line-util"),
    cache_line_width_(conf.get_value<std::size_t>("il1-line-width")),
    mask_(~(cache_line_width_-1)), sleep_interval_(conf.get_value<std::size_t>("sleep-interval")),

    icache_access_sent_(*this, "icache_access_sent"), instr_fetched_(*this, "instructions_fetched"),
    wait_cycles_(0), bbl_commited_(*this, "bbl_commited"), ftq_commited_(*this, "ftq_commited"),

    avg_fetch_rate_(*this, "avg_fetch_rate"), avg_ib_size_(*this, "avg_ib_size"),
    avg_decode_rate_(*this, "avg_decode_rate"), avg_iq_size_(*this, "avg_iq_size"),
    avg_commit_rate_(*this, "avg_commit_rate"), avg_dispatch_rate_(*this, "avg_dispatch_rate"),

    topology_(conf.get_value<std::string>("topology")), lb_hit_(*this, "lb_hit"), il1_hit_(*this, "il1_hit"),
    il1_miss_(*this, "il1_miss"), l2_hit_(*this, "l2_hit"), l2_miss_(*this, "l2_miss"),
    il1_bus_lat_(*this, "il1_bus_lat"), il1_cache_lat_(*this, "il1_cache_lat"), l2_cache_lat_(*this, "l2_cache_lat"),
    l2_dram_bus_lat_(*this, "l2_dram_bus_lat"), dram_lat_(*this, "dram_lat"),

    write_request_failed_(false), over_bbl_(false), over_ftq_(false), il1_(*this)
{
    lb_counters_ = std::vector<unsigned>(lb_size_);
    for (std::size_t i = 0; i < lb_size_; i++) {
        lb_counters_[i] = 0;
        free_lb_list_.push_back(i);
        util_matrix_.push_back(std::vector<ICore::interval_t>());
    }
    for (std::size_t i = 0; i < fetch_pl_depth_; i++) {
        fetch_pl_.push(0);
        bubble_pl_.push(0);
    }
}

template<typename T, typename R, template<typename> class InterConnect>
inline
ICore<T, R, InterConnect>::~ICore()
{
    // print me the statistic at the end
    std::cout << id_ << '\t' <<ftq_commited_.get_value() << '\t' <<  bbl_commited_.get_value() << '\t' << instr_fetched_.get_value() << '\t';
	std::cout << over_cycle_ << '\t' << wait_cycles_ << '\t' << ipc_ << '\t';
	std::cout << avg_fetch_rate_.get_value() << '\t' << ib_limit_ << '\t' << avg_ib_size_.get_value() << '\t';
	std::cout << decode_rate_ << '\t' << fetch_pl_depth_ << '\t';
	std::cout << iq_limit_ << '\t' << avg_iq_size_.get_value() << '\t';
	std::cout << avg_commit_rate_.get_value() << '\t' << ipc_bubble_ << '\t';
	std::cout << icache_access_sent_.get_value() << '\t' << usefulness_bubble_.get_value() << "\t";
	std::cout << il1_hit_.get_value() << '\t' << il1_miss_.get_value() << '\t';
	std::cout << l2_hit_.get_value() << '\t' << l2_miss_.get_value() << '\t';
	std::cout << il1_bus_lat_.get_value() << '\t' << il1_cache_lat_.get_value() << '\t';
	std::cout << l2_cache_lat_.get_value() << '\t' << l2_dram_bus_lat_.get_value() << '\t';
	std::cout << dram_lat_.get_value() << '\t' << lb_hit_.get_value() << '\t' << avg_fb_size_.get_value() << '\t';
	std::cout << avg_bbl_size_.get_value() << '\t' << avg_line_util_.get_value() << '\n';

	if (pending_request_)
		delete pending_request_;
}


template<typename T, typename R, template<typename> class InterConnect>
inline
bool ICore<T, R, InterConnect>::reset()
{
    return true;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
bool ICore<T, R, InterConnect>::commit() {
	bool ret = false;
	unsigned commit_ins = 0;

	// handling the non-whole-number ipc values...
	unsigned wake_up_step = 1;

	// if for example your ipc_=0.25 what you want is to be awaken
	// every 4 cycles and commit 1 instruction
	if (ipc_ > floor(ipc_))
        wake_up_step = (unsigned)floor(1.0 / (ipc_ - floor(ipc_)));

	if (this->get_simulator().get_clock() % wake_up_step == 0) {
		unsigned max_to_commit = (unsigned)(wake_up_step * ipc_);
		// ipc_bubble is telling us at the end how many instructions we couldn't
		// commit due to not enough instructions in IQ
	    ipc_bubble_ += max_to_commit;
	    if (max_to_commit > iq_counter_) {
			commit_ins = iq_counter_;
			iq_counter_ = 0;
		} else {
			commit_ins = max_to_commit;
			iq_counter_ -= max_to_commit;
		}
		ipc_bubble_ -= commit_ins;
	}
	avg_commit_rate_.sample(commit_ins);

	// if the IQ is full, stop the front-end
	if (iq_counter_ >= iq_limit_)
		ret = true;
	return ret;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
void ICore<T, R, InterConnect>::dispatch() {
	std::size_t dispatch_ins = fetch_pl_.front();
	iq_counter_ += dispatch_ins;
	fetch_pl_.pop();

	// if the iq_counter < ipc that means that potentially it can be
	// because of usefulness (if in bubble_pl_ we have something > 0)
	if (iq_counter_ < ipc_)
		usefulness_bubble_ += bubble_pl_.front();

	bubble_pl_.pop();
	avg_dispatch_rate_.sample(dispatch_ins);

	return;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
void ICore<T, R, InterConnect>::decode() {
	// try to empty the instruction buffer with decode_rate_
	std::size_t decode_ins = 0;
	if (ib_counter_ < decode_rate_) {
		decode_ins = ib_counter_;
		ib_counter_ = 0;
	} else {
		decode_ins = decode_rate_;
		ib_counter_ -= decode_rate_;
	}
	avg_decode_rate_.sample(decode_ins);
	if (decode_ins < decode_rate_ && possible_usefulness_bubble_)
		bubble_pl_.push(decode_rate_ - decode_ins);
	else
		bubble_pl_.push(0);

	fetch_pl_.push(decode_ins);

    return;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
void ICore<T, R, InterConnect>::sample_utilization(unsigned id, unsigned length)
{
	// calculate the usefulness of line buffer before you
	// remove the cache line it stores
	std::vector<ICore::interval_t> vector = util_matrix_[id];
	std::sort(vector.begin(), vector.end());
	engine::addr_t current_position = vector[0].start_;
	engine::addr_t sum = 0;

	for (unsigned i = 0; i < vector.size(); ++i) {
        if (vector[i].end_ < current_position) {
            continue;
	    } else if (vector[i].start_ <= current_position) {
            sum += vector[i].end_ - current_position + 1;
            current_position = vector[i].end_ + 1;
        } else {
            sum += vector[i].end_ - vector[i].start_ + 1;
            current_position = vector[i].end_ + 1;
        }
    }
    avg_line_util_.sample(1.0 * sum);
    util_matrix_[id].clear();
    return;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
unsigned ICore<T, R, InterConnect>::process_lbs_and_update_ftq()
{
    unsigned instr = 0;
	ftq_entry_t* ftq_head = ftq_.front();

	// did we reach the end
	if (ftq_head->s_address_ == 0) {
		over_ftq_ = true; over_cycle_ = this->get_simulator().get_clock();
		delete ftq_.front(); ftq_.pop_front();
		return instr;
	}
	// is this wait ftq entry (s_address_==1 is test for that)
	if (ftq_head->s_address_ == 1) {
		next_cycle_ = this->get_simulator().get_clock() + sleep_interval_;
		wait_cycles_ += sleep_interval_;
		delete ftq_.front();
		ftq_.pop_front();
		return instr;
	}

	// find the 1st not allocated entry
	typename std::list<ftq_entry_t*>::iterator alloc = ftq_.begin(), it = ftq_.begin();
	while ((it != ftq_.end()) && (*it)->allocated_)
		it++;

	// it can happen that all of the entries are allocated
	if (it != ftq_.end()) {
		// see if we can reuse the line
		for (alloc = ftq_.begin(); alloc != it; alloc++) {
			if (((*alloc)->s_address_ & mask_) == ((*it)->s_address_ & mask_)) {
				(*it)->allocated_ = true;
	            (*it)->ready_ = (*alloc)->ready_;
				(*it)->lb_id_ = (*alloc)->lb_id_;
				lb_counters_[(*it)->lb_id_]++;
				lb_hit_++;
	            break;
			}
		}
	}

	// nevertheless, try to push the FTQ head if it is ready on the IB
	if (ftq_head->ready_) {
		// put these instructions on the top of IB
		while (ib_counter_ < ib_limit_ && ftq_head->curr_instr_ < ftq_head->instr_.size()) {
			instr++;
			ib_counter_++;
			ftq_head->curr_instr_++;
		}
		if (ftq_head->curr_instr_ == ftq_head->instr_.size()) {
			// add this interval to a util_matrix
			util_matrix_[ftq_head->lb_id_].push_back(ICore::interval_t(ftq_head->s_address_, ftq_head->length_));
			lb_counters_[ftq_head->lb_id_]--;
			ftq_commited_++;
			avg_fb_size_.sample(1.0 * ftq_head->length_);
			if (lb_counters_[ftq_head->lb_id_] == 0) {
				free_lb_list_.push_back(ftq_head->lb_id_);
				sample_utilization(ftq_head->lb_id_, ftq_head->length_);
			}
			delete ftq_.front(); ftq_.pop_front();
		}
	}

	// now, let's get back to our FTQ entry that we have to allocate... but only
	// if we didn't reach the end of FTQ
	// try to reserve one line buffer if there is a free one
	if (it != ftq_.end() && *alloc == *it && !free_lb_list_.empty() && pending_request_ == NULL && (*it)->s_address_ > 1) {
		unsigned free_lb_id = free_lb_list_.front();
		free_lb_list_.pop_front();
		(*it)->allocated_ = true;
		(*it)->lb_id_ = free_lb_id;
		lb_counters_[free_lb_id]++;
		pending_request_ = new typename T::Request(this->get_simulator().get_clock(), T::Request::op_t::READ, id_, (*it)->s_address_, cache_line_width_);
	}
    return instr;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
bool ICore<T, R, InterConnect>::special_bbl(ftq_entry_t* fte)
{
	// is this BBL some special BBL that we got from our runtime class
	bool ret = false;
	if (!bbl_in_->read(current_bbl_)) {
		// if we can not read anymore that means we are done
		over_bbl_ = true;
		if (fte != NULL)
			ftq_.push_back(fte);
		ftq_.push_back(new ftq_entry_t(0));
		ret = true;
	}
	if (current_bbl_->id_ == UINT64_MAX) {
		// this is a test from runtime that we have to wait (barrier, semaphore...)
		if (fte != NULL)
			ftq_.push_back(fte);
		ftq_.push_back(new ftq_entry_t(1));
		delete current_bbl_; current_bbl_ = NULL;
		ret = true;
	}
	if (!ret) {
		bbl_commited_++;
		avg_bbl_size_.sample(current_bbl_->size_in_bytes_);
	}
	current_instr_ = 0;
	return ret;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
void ICore<T, R, InterConnect>::generate_new_ftq_entry()
{
	// we set current_bbl to NULL only when we reach the end of trace
	if (over_bbl_)
		return;

	// only if we can store more ftq entries
	if (ftq_.size() < ftq_size_) {
		if (current_bbl_ == NULL) {
			if (special_bbl(NULL))
				return;
		}

		engine::addr_t start_address = current_bbl_->start_address_;
		for (unsigned i = 0; i < current_instr_; i++)
			start_address += current_bbl_->ins_[i];

		// what can happen is that start_address fits in one cache line, but the next instruction
		// goes to another... we have to handle that situation here!
		if (current_instr_ >= current_bbl_->ins_.size())
			// we should never end up here
			std::cout << "ERROR :: something is wrong with current instructions and basic blocks...\n";

		engine::addr_t next_address = start_address + current_bbl_->ins_[current_instr_];
		if ((start_address & mask_) != (next_address & mask_)) {
			if (current_instr_ < current_bbl_->ins_.size()-1) {
				start_address += current_bbl_->ins_[current_instr_++];
			}
			else {
				while (true) {
					if (special_bbl(NULL))
						return;
					start_address = current_bbl_->start_address_;
					next_address = start_address + current_bbl_->ins_[current_instr_];
					if ((start_address & mask_) != (next_address & mask_)) {
						if (current_bbl_->ins_.size() > 1) {
							start_address += current_bbl_->ins_[current_instr_++];
							break;
						}
					}
					else
						break;
				}
			}
		}

		engine::addr_t start_tag = start_address & mask_;
		ftq_entry_t* ftq_entry = new ftq_entry_t(start_address);
		while ((start_tag == ((start_address + current_bbl_->ins_[current_instr_]) & mask_))) {
			ftq_entry->instr_.push_back(current_bbl_->ins_[current_instr_]);
			start_address += current_bbl_->ins_[current_instr_];
			current_instr_++;
			if (current_instr_ >= current_bbl_->ins_.size()) {
				if (special_bbl(ftq_entry))
					return;
				if (start_address != current_bbl_->start_address_)
					break;
			}
		}
		// we need this length only for usefulness
		for (unsigned i = 0; i < ftq_entry->instr_.size(); i++)
			ftq_entry->length_ += ftq_entry->instr_[i];
		ftq_.push_back(ftq_entry);
	}
	return;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
unsigned ICore<T, R, InterConnect>::fetch()
{
    unsigned fetched_inst = 0;
    typename T::Ack ack;
    bool new_ack = false;
    possible_usefulness_bubble_ = false;
    // read the ack... even if we haven't received anything now, maybe we can use
    // a line from line_buffers_
    new_ack = il1_.read_ack(ack);

    // if we've just received the new ack we have to process it (usage of time stamps),
    // we have to update the FTQ by setting the ready bit and we have to put it in one of the
    // line buffers
    if (new_ack) {
        engine::cycles_t req_time = 0;
        typename std::list<typename T::Request*>::iterator iter;
        // because we handle as many requests as line buffers we have to find the right one
        for (iter = request_list_.begin(); iter != request_list_.end(); iter++) {
            if ((*iter)->get_tag() == ack.get_tag()) {
                req_time = (*iter)->get_time_log()[0];
                delete *iter;
                request_list_.erase(iter);
                break;
            }
        }

        // mark that entry in ftq as ready to be processed
        typename std::list<ftq_entry_t*>::iterator it;
        for (it = ftq_.begin(); it != ftq_.end() && (*it)->allocated_; it++) {
            if (((*it)->s_address_ & mask_) == (ack.get_tag() & mask_)) {
                (*it)->ready_ = true;
            }
        }
        process_ack(ack, req_time);
    }

    // let's see if we can put something in the instruction buffer and update FTQ
    fetched_inst = process_lbs_and_update_ftq();

    if ((fetched_inst < decode_rate_) && (ib_counter_ < decode_rate_) && new_ack)
        possible_usefulness_bubble_ = true;

    instr_fetched_ += fetched_inst;
    return fetched_inst;
}


template<typename T, typename R, template<typename> class InterConnect>
inline
void ICore<T, R, InterConnect>::start()
{
    // generate new FTQ entry here, because it is actually the same if it's here
    // or in the end() but if we do it here, we are ok at the beginning (fist cycle)
    generate_new_ftq_entry();

    // if the request is already prepared then send it
    if (over_ftq_)
	    return;
    if (pending_request_) {
        if (il1_.write_request(*pending_request_) == false) {
            write_request_failed_ = true;
            il1_.set_wake_up_on_read(true);
            return;
        }
        icache_access_sent_++;
        write_request_failed_ = false;
        il1_.set_wake_up_on_read(true);
        request_list_.push_back(pending_request_);

        pending_request_ = NULL;
	}
}


template<typename T, typename R, template<typename> class InterConnect>
inline
void ICore<T, R, InterConnect>::end()
{
    // let's start backward...
    bool stall = commit();
    if (!stall) {
        dispatch();
        decode();
    }
    // fetch only if the IB is not full
    unsigned f_inst = 0;
    if (ib_counter_ < ib_limit_)
	    f_inst = fetch();
	avg_fetch_rate_.sample(f_inst);
	avg_ib_size_.sample(ib_counter_);
    avg_iq_size_.sample(iq_counter_);

    if (write_request_failed_ || over_ftq_) {
        this->set_module_activity(engine::NEVER);
    } else if (this->get_simulator().get_clock() < next_cycle_) {
        this->set_module_activity(next_cycle_ - this->get_simulator().get_clock());
    } else {
        this->set_module_activity(engine::NEXT);
    }
    write_request_failed_ = false;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
const std::string ICore<T, R, InterConnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename T, typename R, template<typename> class InterConnect>
inline
bool ICore<T, R, InterConnect>::inspect(const typename T::Request &)
{
    return true;
}


template<typename T, typename R, template<typename> class InterConnect>
inline
bool ICore<T, R, InterConnect>::inspect(const typename T::Ack &)
{
    return true;
}

template<typename T, typename R, template<typename> class InterConnect>
inline
void ICore<T, R, InterConnect>::process_ack(typename T::Ack& ack, engine::cycles_t req_time)
{
    //there could be possible misleading in the first three members of time_log
	//because of the inability of MSHR time_entry_t to remember and match corresponding
	//requests and later acknowledgments in proper order...

	// this method is very specific. for different CMP configuration, topology and
	// set of counters all of this will look rather different
    enum BusLogSize { BUS_HIT_L1 = 9, BUS_HIT_L2 = 13, BUS_HIT_DRAM = 21 };
    enum DirectLogSize { DIRECT_HIT_L1 = 5, DIRECT_HIT_L2 = 9, DIRECT_HIT_DRAM = 17 };

    if (topology_.compare("bus") == 0) {
	    if (req_time != ack.get_stamp(0))
		    ack.update_time_log(req_time);

		//depending on what's the size of time log vector, we know where was a hit
	    switch (ack.get_time_log().size()) {
			//hit in IL1 cache
			case BUS_HIT_L1: {
				// |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |
				//          |           |     |     |     |     |
				//          -------------     -------     -------
				//           bus forward        IL1     bus backward
				il1_hit_++;
				// bus latency forward and backward
				il1_bus_lat_ += ack.get_stamp(3) - ack.get_stamp(1) + ack.get_stamp(7) - ack.get_stamp(6);
				// IL1 cache latency
				il1_cache_lat_ += ack.get_stamp(5) - ack.get_stamp(4);
				break;
			}
			//hit in L2 cache
			case BUS_HIT_L2: {
				// |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10  |  11  |  12  |
				//          |           |     |     |     |     |     |     |      |      |
				//          -------------     -------     -------     -------      --------
				//           bus forward    IL1 forward      L2    IL1 backward   bus backward
				il1_miss_++; l2_hit_++;
				// bus latency forward and backward
				il1_bus_lat_ += ack.get_stamp(3) - ack.get_stamp(1) + ack.get_stamp(11) - ack.get_stamp(10);
				// IL1 cache latency forward and backward
				il1_cache_lat_ += ack.get_stamp(5) - ack.get_stamp(4) + ack.get_stamp(9) - ack.get_stamp(8);
				// L2 cache latency
				l2_cache_lat_ += ack.get_stamp(7) - ack.get_stamp(6);
				break;
			}
			//hit in DRAM
			case BUS_HIT_DRAM: {
				il1_miss_++;
				l2_miss_++;
				// bus latency forward and backward
				il1_bus_lat_ += ack.get_stamp(3) - ack.get_stamp(1) + ack.get_stamp(19) - ack.get_stamp(18);
				// IL1 cache latency forward and backward
				il1_cache_lat_ += ack.get_stamp(5) - ack.get_stamp(4) + ack.get_stamp(17) - ack.get_stamp(16);
				// L2 cache latency
				l2_cache_lat_ += ack.get_stamp(7) - ack.get_stamp(6) + ack.get_stamp(15) - ack.get_stamp(14);
				// L2-to-DRAM bus latency forward and backward
				l2_dram_bus_lat_ += ack.get_stamp(9) - ack.get_stamp(8) + ack.get_stamp(13) - ack.get_stamp(12);
				dram_lat_ += ack.get_stamp(11) - ack.get_stamp(10);
					break;
			}
			default: {
				std::cout << "ICORE::process_ack::something is wrong with this ack...\n";
				break;
			}
		}
	}
	else {
		//depending on what's the size of time log vector, we know where was a hit
		switch (ack.get_time_log().size()) {
			//hit in IL1 cache
			case DIRECT_HIT_L1: {
				il1_hit_++;
				il1_cache_lat_ += ack.get_stamp(3) - ack.get_stamp(1);
				break;
			}
			//hit in L2 cache
			case DIRECT_HIT_L2: {
				il1_miss_++; l2_hit_++;
				il1_cache_lat_ += ack.get_stamp(3) - ack.get_stamp(1) + ack.get_stamp(7) - ack.get_stamp(6);
				l2_cache_lat_ += ack.get_stamp(5) - ack.get_stamp(4);
				break;
			}
			//hit in DRAM
			case DIRECT_HIT_DRAM: {
				il1_miss_++;
				l2_miss_++;
				il1_cache_lat_ += ack.get_stamp(3) - ack.get_stamp(1) + ack.get_stamp(15) - ack.get_stamp(14);
				l2_cache_lat_ += ack.get_stamp(5) - ack.get_stamp(4) + ack.get_stamp(13) - ack.get_stamp(12);
				l2_dram_bus_lat_ += ack.get_stamp(7) - ack.get_stamp(5) + ack.get_stamp(11) - ack.get_stamp(10);
				dram_lat_ += ack.get_stamp(9) - ack.get_stamp(8);
				break;
			}
			default: {
				std::cout << "ICORE::process_ack::something is wrong with this ack...\n";
				break;
			}
		}
	}
}

}
}


#endif /* ICORE_IMPL_H_ */
