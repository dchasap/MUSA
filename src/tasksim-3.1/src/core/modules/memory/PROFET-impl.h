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


#ifndef CORE_MODULES_MEMORY_PROFET_IMPL_H_
#define CORE_MODULES_MEMORY_PROFET_IMPL_H_

#include <memory>
#include <string>

#include "core/engine/Config.h"
#include "core/stats/VulnerabilityStat.h"
#include "core/utils/instrumentation/ParaverDefs.h"
#include "core/utils/Log.h"


namespace sim {
namespace memory {

using sim::utils::Log;

template<typename Message, template<typename> class Interconnect>
inline
PROFET<Message, Interconnect>::PROFET(  sim::engine::Simulator &sim,
                                        sim::engine::Config conf,
                                        unsigned id) :
    engine::IModule<Message>(sim, engine::ModuleType::RAM),
    in_(Input(*this)),
    max_input_buffer_size_(conf.get_value<std::size_t>("input-buffer")),
		latency_(conf.get_value<unsigned>("max-latency") * sim.get_config().get_value<unsigned>("cpu_frequency") / 1000), // we setup initial latency
		//FIXME: Make update also update max_concurrent based on latency?
    max_concurrent_(ceil(conf.get_value<float>("bandwidth") * latency_ / (conf.get_value<unsigned>("request-size") *
            1e6 * sim.get_config().get_value<unsigned>("cpu_frequency")))),
    id_(id),
    stats_(*this),
    blocked_(false),
    last_nonblocked_cycle_(0)
{
		std::cout << "Using PROFET model as main memory simlator." << std::endl;
    // Max concurrent_requests := Bandwidth[bytes/s] * latency[cycles] / ( request_size[bytes] * cpu_freq[cycles/s])
    this->instrumentation_id_ = sim.get_instrumentation().add_module(*this);
    if (max_concurrent_ == 0) {
        Log::error() << "[ERROR] This configuration has a lower bandwidth than possible.";
        exit(-1);
    }
    Log::debug2() << "Max concurrent RAM requests = " << max_concurrent_;
    stats_.vulnerability_ = sim::stats::VulnerabilityStat::template create_from_config<PROFET>(*this, conf);

    /** PROFET initialization **/
		std::string curveAddress = conf.get_value<std::string>("curve-folder-path");
		curveWindowSize = conf.get_value<uint32_t>("curve-window-size");
		// change the bandwidth to access per cycles
		double MaxTheoreticalBW =  (conf.get_value<double>("max-theoretical-bandwidth")  / 64) / 2.1; //??

		leadOffLatency = 100000; // how we get this value? skylake?
		maxBandwidth = 0;
		maxLatency = 0;
		// curves_data
		for (uint32_t i=0; i<101; i+=2) {
			std::string fileAddress;
			std::ifstream curveFiles;
			double inputBandwidth, inputLatency; 

			// generate the address of the files
			if (i < 10)
				fileAddress = curveAddress + "/bwlat_" + '0' + std::to_string(i) + ".txt";
			else
				fileAddress = curveAddress + "/bwlat_" + std::to_string(i) + ".txt";
		
			// cout << fileAddress << endl;
			curveFiles.open(fileAddress, std::ifstream::in);

			// create read the curve file into the curves_date
			// also set leadOffLatency, maxBandwidth, maxLatency
		

		double maxBandwidthTemp = 0;
		double maxLatencyTemp = 0;

		std::vector<std::vector<double>> curve_data;
		while(curveFiles >> inputBandwidth >> inputLatency) {
			std::vector<double> dataPoint;
			// change the bandwidth to access per cycles
			// this is hard coded for skylake it should read the config frecuency
			inputBandwidth = (inputBandwidth / 64) / (2.1*1000)  ;
			// decrease LLC latency; this is hard coded for skylake. it should read all parent 
			// cache latency and sum them at the end. for Skylake this value is 37 (L3) + 10 (L2) + 4 (L1) = 51
			inputLatency = inputLatency - 51; //TODO: We should get it from config file


			dataPoint.push_back(inputBandwidth);
			dataPoint.push_back(inputLatency);			
			// cout << dataPoint[0] << " " << dataPoint[1] << endl;

			if (leadOffLatency > inputLatency)
				leadOffLatency = inputLatency;
			if (maxLatency < inputLatency)
				maxLatency = inputLatency;
			if (maxLatencyTemp < inputLatency)
				maxLatencyTemp = inputLatency;
			if (maxBandwidth < inputBandwidth)
				maxBandwidth = inputBandwidth;
			if (maxBandwidthTemp < inputBandwidth)
				maxBandwidthTemp = inputBandwidth;


			curve_data.push_back(dataPoint);
		}
		maxBandwidthPerRdRatio.push_back(maxBandwidthTemp);
		maxLatencyPerRdRatio.push_back(maxLatencyTemp); 
		curves_data.push_back(curve_data);
		curveFiles.close();
	}

	std::cout << "leadOffLatency: " << (leadOffLatency + 51) / 2.1 << " ns, cycles: " << (leadOffLatency + 51) << std::endl;
	std::cout << "maxLatency: " << (maxLatency + 51) / 2.1 << " ns"<< std::endl;
	std::cout << "maxBandwidth: " << maxBandwidth * 64  * 2.1 << " GB/s" << std::endl;
	std::cout << "MaxTheoreticalBW: " << MaxTheoreticalBW * 64 * 2.1 << " GB/s" << std::endl;

	// test to see if the data is loaded correctly.
	// for (uint32_t i=0; i< curves_data.size(); i++)
	// {
	// 	cout << "read percentage is: " << i*2 << endl;
	// 	for (uint32_t j = 0; j < curves_data[i].size(); ++j)
	// 	{
	// 		cout << curves_data[i][j][0] << " " << curves_data[i][j][1] << endl;
	// 	}
	// }

	std::cout << "maxBandwidth: " << std::endl;
	for (uint32_t j = 0; j < maxBandwidthPerRdRatio.size(); ++j) {
		std::cout << maxBandwidthPerRdRatio[j] << std::endl;
	}

	std::cout << "maxlatency: " << std::endl;
	for (uint32_t j = 0; j < maxLatencyPerRdRatio.size(); ++j) {
		std::cout << maxLatencyPerRdRatio[j] << std::endl;
	}

	// initialization of the counters
	currentWindowAccessCount = 0;
	currentWindowAccessCount_wr = 0;
	currentWindowAccessCount_rd = 0;

	// initialize the state
	lastEstimatedBandwidth = 0;
	lastEstimatedLatency = leadOffLatency;
	latency_ = (uint32_t) lastEstimatedLatency;
	std::cout << "PROFET init done." << std::endl;
}

template<typename Message, template<typename> class Interconnect>
inline
PROFET<Message, Interconnect>::~PROFET()
{
    if (stats_.vulnerability_) {
        stats_.vulnerability_->finalize(this->get_simulator().get_clock());
    }
}

template<typename Message, template<typename> class Interconnect>
inline
bool PROFET<Message, Interconnect>::reset()
{
    return true;
}

template<typename Message, template<typename> class Interconnect>
inline
void PROFET<Message, Interconnect>::start()
{
		//std::cout << "start()" << std::endl;
    // We do not have any pending requests.
    if (pending_.empty()) {
        return;
    }
    // First pending request is due in the future.
    if (pending_.front().time_ > this->get_simulator().get_clock()) {
        return;
    }

    // Check if we can write back the Ack.
    while (not(pending_.empty()) and
            pending_.front().time_ <= this->get_simulator().get_clock()) {
        bool success = in_.write_ack(typename Message::Ack(pending_.front().data_));
        if (success) {
            typename Message::Request req = pending_.front().data_; 
            uint64_t user_code_id = req.get_user_code_id();
            stats_.latency[user_code_id] += latency_;
            stats_.count[user_code_id]++;

            pending_.pop_front();
        }
    }
}

template<typename Message, template<typename> class Interconnect>
inline
void PROFET<Message, Interconnect>::end()
{
		//std::cout << "end()" << std::endl;
    // We read from the buffer
    while ((input_buffer_.size() < max_input_buffer_size_) and in_.can_read_request()) {
        typename Message::Request request;
        if (in_.read_request(request)) {
            input_buffer_.emplace_back(std::move(request));
        }
    }

    // If we can, we take care of a request.
    if (input_buffer_.size() > 0 && pending_.size() < max_concurrent_) {
        typename Message::Request req = input_buffer_.front();
        uint64_t user_code_id = req.get_user_code_id();
        if (stats_.vulnerability_) {
            stats_.vulnerability_->mem_access(this->get_simulator().get_clock(), req);
        }

        // reset window for PROFET
        if (currentWindowAccessCount == 0) {
    	    currentWindowStartCycle = this->get_simulator().get_clock(); //FIXME: add proper cycle
        }

        if (req.get_op() == Message::Request::op_t::READ) {
            currentWindowAccessCount_rd++;
            stats_.read_requests[user_code_id]++;
        } else if (req.get_op() == Message::Request::op_t::WRITE) {
            currentWindowAccessCount_wr++;
            stats_.write_requests[user_code_id]++;
        }

        if(currentWindowAccessCount == curveWindowSize) {
    	    updateLatency(this->get_simulator().get_clock());
    	    currentWindowAccessCount = 0; // reset the access as we enter a new window
    	    currentWindowAccessCount_wr = 0; // reset the access as we enter a new window
    	    currentWindowAccessCount_rd = 0; // reset the access as we enter a new window
        }

        pending_.emplace_back(this->get_simulator().get_clock() + latency_, std::move(req));
        input_buffer_.pop_front();
    }
    stats_.occupancy.sample(pending_.size(), this->get_simulator().get_clock());

    if (not blocked_ and input_buffer_.size()) {
        // So, if we were not blocked, and have pending requests in the input buffer:
        blocked_ = true;
        last_nonblocked_cycle_ = this->get_simulator().get_clock() - 1;
    }
    if (blocked_ and input_buffer_.size() == 0) {
        // We where blocked, but have no more blocked Requests:
        blocked_ = false;
        stats_.blocked_cycles += this->get_simulator().get_clock() - last_nonblocked_cycle_ - 1;
    }

    set_next_cycle();
}

template<typename Message, template<typename> class Interconnect>
inline
bool PROFET<Message, Interconnect>::inspect(const typename Message::Request &)
{
		std::cout << "inspect_req()" << std::endl;
    return true;
}

template<typename Message, template<typename> class Interconnect>
inline
bool PROFET<Message, Interconnect>::inspect(const typename Message::Ack &)
{
		std::cout << "inspect_ack()" << std::endl;
    return true;
}

template<typename Message, template<typename> class Interconnect>
inline
void PROFET<Message, Interconnect>::print_hardware_counters(sim::engine::cycles_t time, bool propagate,
        unsigned overwrite_cpu_id)
{
    sim::utils::instrumentation::SelectedInstrumentation& instrumentation =
            this->get_simulator().get_instrumentation();
    unsigned instrumentation_id = (overwrite_cpu_id != 0) ? overwrite_cpu_id : this->instrumentation_id_;
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {sim::utils::instrumentation::DRAM_LATENCY,
                stats_.latency.get_value()}},
            *this);
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {sim::utils::instrumentation::DRAM_COUNT,
                stats_.count.get_value()}},
            *this);

    uint64_t occ = static_cast<uint64_t>(stats_.occupancy.get_local_average());
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {sim::utils::instrumentation::DRAM_OCCUPANCY, occ}},
            *this);
    instrumentation.add_event(
            utils::instrumentation::InstrumentationItem {
            instrumentation_id,
            0,
            time,
            utils::instrumentation::EVENT_RECORD,
            utils::instrumentation::Event {sim::utils::instrumentation::DRAM_BLOCKED_CYCLES,
                stats_.blocked_cycles.get_value()}},
            *this);
}

template<typename Message, template<typename> class Interconnect>
inline
const std::string PROFET<Message, Interconnect>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename Message, template<typename> class Interconnect>
inline
unsigned PROFET<Message, Interconnect>::get_id() const
{
    return id_;
}

template<typename Message, template<typename> class Interconnect>
inline
void PROFET<Message, Interconnect>::print_status() const
{
		std::cout << "Something is wrotten..." << std::endl;
    Log::warning() << "[STATUS] PROFET memory module with id " << get_id() << " deadlock report:";
    Log::warning() << " - Have " << input_buffer_.size() << " requests in queue:";
    for (const auto &req : input_buffer_) {
        Log::warning() << " Request " << req;
    }
    Log::warning() << " - Have " << pending_.size() << " requests being processed:";
    for (const auto &req : pending_) {
        Log::warning() << " Ack due on cycle " << req.time_ << " " << req.data_;
    }
}

template<typename Message, template<typename> class Interconnect>
inline
void PROFET<Message, Interconnect>::set_next_cycle()
{
		//std::cout << "set_next_cycle()" << std::endl;
    if (in_.can_read_request()) {
        // We have a Request in the Input port.
        this->set_module_activity(engine::NEXT);
    } else if (pending_.size() < max_concurrent_ && input_buffer_.size() > 0) {
        // We can process a requests next cycle:
        this->set_module_activity(engine::NEXT);
    } else if (pending_.size() > 0 and pending_.front().time_ <= this->get_simulator().get_clock()) {
        // We can send an Ack next cycle:
        this->set_module_activity(engine::NEXT);
    } else if (pending_.size() > 0) {
        // We have to way until we can send an Ack.
        this->set_module_activity(pending_.front().time_ - this->get_simulator().get_clock());
    } else {
        // We have to wait until we get a new Request.
        this->set_module_activity(engine::NEVER);
    }
		//std::cout << "end-set_next_cycle()" << std::endl;
}

template<typename Message, template<typename> class Interconnect>
inline
sim::engine::cycles_t PROFET<Message, Interconnect>::searchForLatencyOnCurve(double bandwidth, double readPercentage) {

	std::cout << "searchForLatencyOnCurve()" << std::endl;
	// curves_data
	// first select the correct read to write ratio. an even number between 0 and 100
	uint32_t intReadPercentage;
	//intReadPercentage = round( (100*readPercentage) * 0.5f ) * 2;
    intReadPercentage = std::floor(((100*readPercentage) * 0.5f ) + 0.5) * 2;
	assert(intReadPercentage >= 0);
	assert(intReadPercentage <= 100);
	assert(intReadPercentage % 2 == 0);

	uint32_t curveDataIndex = intReadPercentage/2;


	uint32_t finalLatency=0;
	double finalBW=0;

	// TODO...
	// sanity check 
	std::cout << std::endl << "new" << std::endl;
	std::cout << "MaxTheoreticalBW: " << MaxTheoreticalBW  << " acesses/cycles" << std::endl;
	std::cout << "maxBandwidth in all the curves : " << maxBandwidth  << " acesses/cycles" << std::endl;
	std::cout << "measured bandwidth: " << bandwidth << " max from curve: "<< maxBandwidthPerRdRatio[curveDataIndex] << std::endl;
	std::cout << "rd percentage: " << intReadPercentage << std::endl;


	//bool overflow=false;
	// if (MaxTheoreticalBW < bandwidth) {
	if (maxBandwidth < bandwidth) {
		
		// bandwidth =  maxBandwidthPerRdRatio[curveDataIndex];
		// cout << "more than max! bandwidth: " << bandwidth << " max from curve: "<< maxBandwidthPerRdRatio[curveDataIndex] << endl;
		//overflow=true;
		// let's converge slowly
		finalBW = 0.5f * maxBandwidthPerRdRatio[curveDataIndex] + 0.5f * lastEstimatedBandwidth;
		// simulate the wave form at least for a very high bandwidth
		// finalLatency = curves_data[curveDataIndex][0][1];
		finalLatency = maxLatencyPerRdRatio[curveDataIndex];
		lastEstimatedBandwidth = finalBW;

		std::cout << "finalLatency " << finalLatency << std::endl;
		finalLatency = finalLatency + 0.1*(double)(finalLatency);
		std::cout << "overflow..." << std::endl;
		std::cout << "lastEstimatedBandwidth: " << lastEstimatedBandwidth << std::endl;
		std::cout << "finalLatency (updated)" << finalLatency << std::endl;

		// put more assert
		assert(finalLatency>=leadOffLatency);

		return finalLatency;

		// cout << "more than max! bandwidth: " << bandwidth << " max from curve: "<< maxBandwidthPerRdRatio[curveDataIndex] << endl;
	}

	// use j later for extrapolation
	uint32_t j;
	for (j = 0; j < curves_data[curveDataIndex].size(); ++j)
	{

		// cout << "finalLatency " << finalLatency << " finalBW: " << finalBW << endl;
		// cout << "curves_data[curveDataIndex][j][0]: " << curves_data[curveDataIndex][j][0] << " bandwidth " << bandwidth << endl;
		
		if (finalBW == 0) {
			finalBW = curves_data[curveDataIndex][j][0];
			finalLatency = curves_data[curveDataIndex][j][1];
		}

		if (curves_data[curveDataIndex][j][0] >= bandwidth) {
			finalBW = curves_data[curveDataIndex][j][0];
			finalLatency = curves_data[curveDataIndex][j][1];
		}
		else {
			break;
		}

		// cout << curves_data[curveDataIndex][j][0] << " " << curves_data[curveDataIndex][j][1] << endl;
	}
	if (j!=0) {
		// for easier readability
		double x1 = curves_data[curveDataIndex][j][0];
		double y1 = curves_data[curveDataIndex][j][1];
		double x2 = curves_data[curveDataIndex][j-1][0];
		double y2 = curves_data[curveDataIndex][j-1][1];
		double x = bandwidth;

		finalLatency = y1 + ((x - x1) / (x2 - x1)) * (y2 - y1);
	}
	
	lastEstimatedBandwidth = finalBW;

	std::cout << "lastEstimatedBandwidth: " << lastEstimatedBandwidth << std::endl;
	std::cout << "finalLatency " << finalLatency << std::endl;

	assert(finalLatency>=leadOffLatency);
	// cout << finalLatency << " " << finalBW << endl;


	return finalLatency;

}

template<typename Message, template<typename> class Interconnect>
inline
void PROFET<Message, Interconnect>::updateLatency(sim::engine::cycles_t currentWindowEndCyclen) {
	// bandwidth unit is accesses per cycles
	
	std::cout << "updateLatency()" << std::endl;
	double bandwidth = ((double)currentWindowAccessCount) / (double)(currentWindowEndCyclen - currentWindowStartCycle);
	double readPercentage = ((double)(currentWindowAccessCount_rd))/ (currentWindowAccessCount_rd + currentWindowAccessCount_wr);


	lastEstimatedLatency = searchForLatencyOnCurve(bandwidth, readPercentage);
	assert(lastEstimatedLatency>=0);
	assert(lastEstimatedLatency<=maxLatency);

	latency_ = (uint32_t)lastEstimatedLatency;
	// cout << "latency: " << latency << " cycles. readPercentage: " << readPercentage << endl;
}

}  // namespace memory
}  // namespace sim


#endif  // CORE_MODULES_MEMORY_PERFECT_IMPL_H_
