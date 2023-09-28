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


#ifndef CORE_UTILS_INSTRUMENTATION_MITOS_IMPL_H_
#define CORE_UTILS_INSTRUMENTATION_MITOS_IMPL_H_

#include <cxxabi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <utility>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unordered_set>
#include <vector>


namespace sim {
namespace utils {
namespace instrumentation {

inline
std::string Mitos::new_trace_base_name(const std::string trace_base)
{
    // First we check that the directory exists (if one is specified):
    if (trace_base.find_last_of("/\\") != std::string::npos) {
        std::string dir_name = trace_base.substr(0, trace_base.find_last_of("/\\"));
        struct stat info;
        if (stat(dir_name.c_str(), &info) != 0) {
            // Directory cannot be read
            std::cerr << "[Mitos] Cannot open output trace directory " << dir_name << std::endl;
            exit(-1);
        } else if (not(info.st_mode & S_IFDIR)) {
            // Alleged directory is not a directory
            std::cerr << "[Mitos] Output trace directory " << dir_name << " is a file." << std::endl;
            exit(-1);
        }
    }

    for (int index = 0; index < 10000; index++) {
        std::ostringstream trace_name_builder;
        trace_name_builder << trace_base;
        if (index > 0) {
            trace_name_builder << "-" << std::setw(4) << std::setfill('0') << index;
        }
        trace_name_builder << ".csv";
        const std::string &trace_name = trace_name_builder.str();

        // If we can open the file with x flag, it did not exist and we have all the necessary permissions.
        FILE *file_desc = fopen(trace_name.c_str(), "wbx");
        if (file_desc != NULL) {
            fclose(file_desc);
            return trace_name.substr(0, trace_name.size() - 4);
        }
    }

    std::cerr << "[Mitos] Can not generate unique mitos trace file name." << std::endl;
    exit(-1);
}

inline
std::string Mitos::generate_tmp_file_name()
{
    std::ostringstream oss;
    char *dir = getenv("TMPDIR");
    if (dir != NULL) {
        oss << dir;
    } else {
        oss << "/tmp";
    }
    oss << "/tmp_trace_pid_" << getpid() << ".csv";

    return oss.str();
}

inline
sim::engine::ModuleType Mitos::filter_instrumented_modules(std::vector<std::string> &accepted_module_type_names) {
    // Unknown mode is represented by 0 so we will never be able to instrument it.
    sim::engine::ModuleType accepted = sim::engine::ModuleType::UNKNOWN;
    for (unsigned int i = 0; i < accepted_module_type_names.size(); i++) {
        accepted = static_cast<sim::engine::ModuleType>(accepted |
                sim::engine::get_module_type(accepted_module_type_names[i]));
    }
    return accepted;
}

inline
void Mitos::flush(bool force)
{
		//std::cout << "Flushing precise memory events to temp file: " << std::endl; 
    for (unsigned int i = 0; i < item_buffer_.current_elements_; i++) {
        //temporary_file_ << item_buffer_.items_.get()[i] << std::endl;
				InstrumentationItem item = item_buffer_.items_.get()[i];
				
				temporary_file_ << "??,??,";
				//temporary_file_ << item.u.pmem_event_.op_ << ",";
				//std::cout << "post-op:" << item.u.pmem_event_.op_;
				op_t op = static_cast<op_t>(item.u.pmem_event_.op_);
				auto it = mitos_op_name.find(op);
				if (it != mitos_op_name.end()) {
					temporary_file_ << it->second << ",";
				}
				temporary_file_	<< "??,"; // could always be 8?  
				temporary_file_ << item.u.pmem_event_.ip_ << ",";
				temporary_file_ << "??,0,1,0,0,0,??,";
				temporary_file_ << item.cpu_id_ << ",";
				temporary_file_ << item.time_stamp_ << ",";
				temporary_file_ << item.u.pmem_event_.addr_ << ",";
				temporary_file_ << item.cpu_id_ << ",";

				uint64_t latency = 0; // need to get latency somehow
				switch (item.u.pmem_event_.level_) {
					case 1: 
						temporary_file_ << latency << cacheLatencies["L1"] << ",";
						temporary_file_ << "L1,";
						break;
					case 2: 
						temporary_file_ << latency << cacheLatencies["L2"] << ",";
						temporary_file_ << "L2,";
						break;
					case 3: 
						temporary_file_ << latency << cacheLatencies["L3"] << ",";
						temporary_file_ << "L3,";
						break;
					case 4: 
						temporary_file_ << latency << ",";
						temporary_file_ << "Local RAM,";
						break;
					default: 
						std::cerr << "[Mitos] Unknown memory level registered." << std::endl;
						temporary_file_ << latency << "??,";
						temporary_file_ << "??,";
						break;
				}

				temporary_file_ << "Hit,";
				switch(item.u.pmem_event_.access_type_) {
					case 0:
						temporary_file_ << "Load," << std::endl;
						break;
					case 1:
						temporary_file_ << "Store," << std::endl;
						break;
					default:
						temporary_file_ << "Other," << std::endl;
						break;
				}
				temporary_file_ << "N/A,";
				temporary_file_ << "TLB_Hit,";
				temporary_file_ << "0" << std::endl;
    }
    item_buffer_.current_elements_ = 0;
}

inline
void Mitos::init(const sim::engine::Config &config)
{
		std::cout << "Initializing Mitos trace instrumentation." << std::endl;

    // if someone has disabled the mitos trace generation by the disable_trace_generation = DISABLED:
		disable_trace_generation_ = false;
		keep_intermediate_ = true;
/*
		if (config.get_value<bool>("Mitos", "disable_trace_generation")) {
				disable_trace_generation_ = true;	
		}
*/
    if (not disable_trace_generation_) {
        // We reserve the final output file:
        trace_base_name_ = new_trace_base_name(config.get_value<std::string>(
                "Mitos", "trace_base_name") );

        num_modules_ = 0;
        num_components_ = 0;

        // Temporary file initialization:
        temporary_file_name_ = generate_tmp_file_name();
				std::cout << "Creating temporary mitos trace: " << temporary_file_name_ << std::endl;
        temporary_file_.open(temporary_file_name_.c_str(), std::ios_base::out);

        // Item buffer initialization:
        unsigned buffer_size = DEFAULT_BUFFER_SIZE;
        if (config.key_exists("Mitos", "buffer_size")) {
            buffer_size = config.get_value<unsigned>("Mitos", "buffer_size");
        }
        item_buffer_ = InstrumentationBuffer(buffer_size);

        // Instrumented modules:
        std::vector<std::string> accepted_module_type_names =
                config.get_values<std::string>("Mitos", "modules");
        instrumented_modules_ = filter_instrumented_modules(accepted_module_type_names);
				
				// Get the latencies from the configuration file here for the caches as they are fixed
//				cacheLatencies["L1"] = config.get_value<unsigned>("DL1Cache", "latency");
//				cacheLatencies["L2"] = config.get_value<unsigned>("L2Cache", "latency");
//				cacheLatencies["L3"] = config.get_value<unsigned>("L3Cache", "latency");
				cacheLatencies["L1"] = config.get_value<unsigned>("Mitos", "l1-latency");
				cacheLatencies["L2"] = config.get_value<unsigned>("Mitos", "l2-latency");
				cacheLatencies["L3"] = config.get_value<unsigned>("Mitos", "l3-latency");

//				cacheLatencies["L1"] = 2;
//				cacheLatencies["L2"] = 5;
//				cacheLatencies["L3"] = 20;

    }
    init_ = true;
}

inline
void Mitos::finalize()
{
    assert(init_);
    // We check if trace generation has been disabled.
    if (disable_trace_generation_) {
        // If so, we do nothing here.
        init_ = false;
        return;
    }

    // We flush the buffers and close the temporary file:
    flush(true);
    temporary_file_.close();


    // We move the data from the termporary file to the final file:
    generate_final_mitos_file(std::string(trace_base_name_ + ".csv"), temporary_file_name_);
    std::cout << "Opening Mitos trace file " << (trace_base_name_ + ".csv") << std::endl;
    // Here we decide wether to remove the intermediary file.
    if (not keep_intermediate_) {
        unlink(temporary_file_name_.c_str());
    }

    init_ = 0;
    return;
}

template<typename ModuleType>
inline
int Mitos::add_module(const ModuleType &module, unsigned components)
{
    assert(init_);
    // We check if trace generation has been disabled.
    if (disable_trace_generation_) {
        return -1;
    }

    bool instrumentalize = false;
    std::string module_name = module.get_name();
    if (instrumented_modules_ & module.get_module_type()) {
        instrumentalize = true;
    }
    int id = -1;
    if (instrumentalize) {
        // One Paraver CPU per module:
        ++num_modules_;
        ++num_components_;
        id = num_modules_;
        // Naming:
        unsigned sub_index = ++module_count_[module_name];
        std::ostringstream module_name_buffer;
        module_name_buffer << module_name << " " << sub_index;
        module_name_.push_back(module_name_buffer.str());
        if (components == 1) {
						component_name_.push_back(module_name_buffer.str());
        } else {
						for (unsigned i = 0; i < components; i++) {
                std::ostringstream name_buffer;
                name_buffer << module_name << " " << sub_index << "." << i;
                component_name_.push_back(name_buffer.str());
            }
        }
    }
    return id;
}



inline
void Mitos::start_state(const InstrumentationItem &trace_item)
{
		return;
}

inline
void Mitos::end_state(const InstrumentationItem &trace_item)
{
		return;
}

inline
void Mitos::start_comm(const InstrumentationItem &trace_item)
{
    return;
}

inline
void Mitos::end_comm(const InstrumentationItem &trace_item)
{
		return;
}

template<typename ModuleType>
inline
void Mitos::add_event(const InstrumentationItem &trace_item, const ModuleType &module)
{
		return;
}

inline
void Mitos::add_event(const InstrumentationItem &trace_item)
{
		return;
}

inline
void Mitos::add_function_name(const char* name)
{
		return;
}

inline
void Mitos::add_precise_memory_event(const InstrumentationItem &trace_item) 
{
		//std::cout << "[Mitos] Adding precise memory event." << std::endl;
		assert(init_);
    if (disable_trace_generation_ or trace_item.cpu_id_ == static_cast<unsigned>(-1)) {
				std::cout << "[Mitos] skipping precise memory event." << std::endl;
        return;
    }
		
    // the comparison is <= as cpu_id_ is 1-indexed
    assert(trace_item.cpu_id_ <= num_modules_);

    // check if the module has been instrumented.
    if (trace_item.cpu_id_ > num_modules_) {
        std::cerr << "found an event with module_id " << trace_item.cpu_id_ 
									<< " but we only have " << num_modules_ 
									<< " instrumented modules" << std::endl;
        return;
    }

    // check if we need to empty the temporary buffer:
    if (item_buffer_.current_elements_ == item_buffer_.max_elements_) {
        flush(false);
    }

    item_buffer_.items_.get()[item_buffer_.current_elements_] = trace_item;
    item_buffer_.current_elements_++;

		return;
}

inline
sim::engine::cycles_t Mitos::get_hardware_sampling_interval()
{
		return sim::engine::NEVER;
}

inline
void Mitos::generate_final_mitos_file(const std::string &output_filename, const std::string &input_filename)
{
		std::cout << "Saving final Mitos trace: " + output_filename << std::endl;
    // First we open the output file:
    std::fstream trace_file(output_filename.c_str(), std::ios_base::out);
    if (!trace_file.is_open()) {
        std::cerr << "Error while opening Instrumentation output file " << output_filename << std::endl;
        // We return gracefuly so other moduels counters can be printed in the execution log.
        return;
    }
    // Then open the intermediary file as input:
    std::fstream temporary_in;
    temporary_in.open(input_filename.c_str(), std::ios_base::in);

    if (!temporary_in.is_open()) {
        std::cerr << "Error when opening temporary file " << input_filename << std::endl;
        std::abort();
    }

    //trace_file.fill('0');
		trace_file << "source,line,instruction,bytes,ip,variable,buffer_size,dims,xidx,yidx,zidx,";
		trace_file << "pid,tid,time,addr,cpu,latency,level,hit_type,op_type,snoop_mode,tlb_access,numa";
		trace_file << std::endl;
	
		std::string temporary_item;
		while (std::getline(temporary_in, temporary_item)) {
				trace_file << temporary_item << std::endl;
    }

    temporary_in.close();
    trace_file.close();
}


inline void Mitos::dump_buffer()
{
    // We check if trace generation has been disabled.
    if (disable_trace_generation_) {
        // If so, we do nothing here.
        init_ = false;
        return;
    }

    // We flush the buffers and close the temporary file:
    flush(true);
    temporary_file_.close();

    std::cout << "========================= WARNING ============================================" << std::endl;
    std::cout << "Temporary Mitos trace " << temporary_file_name_ << " closed." << std::endl;
    std::cout << "No final trace generated." << std::endl;
    std::cout << "========================= WARNING ============================================" << std::endl;
    // To ensure that the destructor will not try to parse it again.
    disable_trace_generation_ = false;
}


}  // namespace instrumentation
}  // namespace utils
}  // namespace sim

#endif  // CORE_UTILS_INSTRUMENTATION_MITOS_IMPL_H_
