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


#ifndef MODULES_CPU_ICORE_H_
#define MODULES_CPU_ICORE_H_

#include <cstdint>
#include <queue>
#include <deque>
#include <list>
#include <algorithm>

#include <core/engine/InPort.h>
#include <core/engine/Module.h>
#include <core/engine/types.h>
#include <core/engine/EventBuffer.h>
#include <core/modules/interconnect/Direct.h>
#include <core/stats/Counter.h>
#include <core/stats/Average.h>
#include <simulators/frontendsim/trace/types.h>

namespace sim {
namespace cpu {

template<typename T, typename R, template<typename> class InterConnect>
class ICore : public engine::IModule<T> {
public:
    typedef engine::EventBuffer<trace::frontend::bbl_stat_t*> bbl_buffer_t;

    // UGI: FTQ entry. FetchTargetQueue is a structure that holds the fetch requests
    //		from branch predictor to I-cache (ref the FTQ paper)
    struct ftq_entry_t {
    	engine::addr_t s_address_;
    	std::vector<unsigned> instr_;
    	unsigned curr_instr_;
    	unsigned length_;
    	unsigned lb_id_;
    	bool allocated_;
    	bool ready_;

    	ftq_entry_t(engine::addr_t sa) : s_address_(sa), curr_instr_(0),
    				length_(0), lb_id_(0), allocated_(false), ready_(false) {
    	}
    };

    // UGI: we use this structure to find the usefulness of cache line
    //		multiple fetch blocks can be placed in one cache line in a
    //		undefined order, potentially overlapped, so we need a way to find
    //		the number of all bytes used in a cache line
    struct interval_t {
    	engine::addr_t start_, end_;
    	interval_t(engine::addr_t s, unsigned l) : start_(s), end_(s + l) {}
    	bool operator<(const interval_t& other) const {
    	    if (start_ != other.start_) {
    	    	return start_ < other.start_;
    	    }
    	    return end_ < other.end_;
    	}
    };

protected:
    unsigned id_;
    bbl_buffer_t* bbl_in_;
    typename T::Request *pending_request_;
    trace::frontend::bbl_stat_t* current_bbl_;
    unsigned current_instr_;
    std::list<typename T::Request*> request_list_; // we handle multiple requests (as many as line buffers)

    engine::cycles_t over_cycle_;
    engine::cycles_t next_cycle_;

    std::list<ftq_entry_t*> ftq_;		// fetch target queue
    std::size_t ftq_size_;				// max size of the FTQ... not sure if we need it
    std::size_t lb_size_;				// how many line buffers
    std::vector<unsigned> lb_counters_;	// how many FTQ entries refer to the corresponding line buffer
    std::list<unsigned> free_lb_list_;	// list of available line buffers... maybe not needed...

    std::size_t ib_counter_;
    std::size_t ib_limit_;
    std::size_t iq_counter_;
    std::size_t iq_limit_;
    std::queue< size_t, std::deque<size_t> > fetch_pl_;
    std::queue< size_t, std::deque<size_t> > bubble_pl_;
    std::size_t fetch_pl_depth_;
    std::size_t decode_rate_;
    double ipc_;						// max ipc
    double ipc_bubble_;					// overhead of not being able to commit ipc_ inst every cycle
    double commit_bubble_;				// for managing the non-whole-number ipc_ values
    bool possible_usefulness_bubble_;	// to detect usefulness bubbles
    stats::Counter usefulness_bubble_;	// usefulness bubble
   	stats::Average avg_fb_size_;		// average fetch block size (up to cache line size)
   	stats::Average avg_bbl_size_;		// average BBL size
   	stats::Average avg_line_util_;		// average line utilization
   	std::vector< std::vector<ICore::interval_t> > util_matrix_; // usefulness per line buffer

    std::size_t cache_line_width_;		// we need to to send the right I-cache req size
    engine::addr_t mask_;				// to calculate if the current FB reaches the end of cache line
    std::size_t sleep_interval_;		// on how many cycles we check should we wake up or not

    stats::Counter icache_access_sent_;	// how many times we've sent a request to IL1
    stats::Counter instr_fetched_;		// how many instructions we've fetched
    engine::cycles_t wait_cycles_;		// waiting on sync points, inside the parallel region
    stats::Counter bbl_commited_;		// how many basic blocks we've consumed
    stats::Counter ftq_commited_;		// how many fetch blocks we've consumed

    stats::Average avg_fetch_rate_;
    stats::Average avg_ib_size_;
    stats::Average avg_decode_rate_;
    stats::Average avg_iq_size_;
    stats::Average avg_commit_rate_;
    stats::Average avg_dispatch_rate_;

    // counters for time stamps... this depends on the topology of course and CMP configuration
    // UGI: i needed this set of counter for CPI stack generation
    std::string topology_;				// core has to be aware of topology in order to process the Ack properly
    stats::Counter lb_hit_;				// hit in line buffers
    stats::Counter il1_hit_;			// hit in I-cache
    stats::Counter il1_miss_;			// miss in I-cache
    stats::Counter l2_hit_;				// hit in L2
    stats::Counter l2_miss_;			// miss in L2
    stats::Counter il1_bus_lat_;		// cycles spent on core-to-IL1 bus
    stats::Counter il1_cache_lat_;		// cycles spent in IL1
    stats::Counter l2_cache_lat_;		// cycles spent in L2
    stats::Counter l2_dram_bus_lat_;	// cycles spent on L2-to-DRAM bus
    stats::Counter dram_lat_;			// cycles spent in DRAM

    bool write_request_failed_;
    bool over_bbl_;
    bool over_ftq_;

    void process_ack(typename T::Ack& ack, engine::cycles_t req_time);
    unsigned process_lbs_and_update_ftq();
    void generate_new_ftq_entry();
    bool special_bbl(ftq_entry_t* fte);
    void sample_utilization(unsigned id, unsigned length);

    bool commit();
    void dispatch();
    void decode();
    unsigned fetch();

public:
    ICore(engine::Simulator &, const engine::Config &, unsigned id);
    ~ICore();

    bool reset();
    void start();
    void end();
    const std::string get_name() const;

    bool inspect(const typename T::Request &);
    bool inspect(const typename T::Ack &);

    /* Interface to memory hierarchy */
    typename InterConnect<T>::Output il1_;

    /* Operator to connect to the event buffer */
    friend void operator>>(bbl_buffer_t &b, ICore<T, R, InterConnect> &c) {
        c.bbl_in_ = &b;
    }
};

}
}

#include "ICore-impl.h"

#endif /* ICORE_H_ */
