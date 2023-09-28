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


#ifndef CORE_UTILS_INSTRUMENTATION_PARAVER_IMPL_H_
#define CORE_UTILS_INSTRUMENTATION_PARAVER_IMPL_H_

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
std::string Paraver::new_trace_base_name(const std::string trace_base)
{
    // First we check that the directory exists (if one is specified):
    if (trace_base.find_last_of("/\\") != std::string::npos) {
        std::string dir_name = trace_base.substr(0, trace_base.find_last_of("/\\"));
        struct stat info;
        if (stat(dir_name.c_str(), &info) != 0) {
            // Directory cannot be read
            std::cerr << "[Paraver] Cannot open output trace directory " << dir_name << std::endl;
            exit(-1);
        } else if (not(info.st_mode & S_IFDIR)) {
            // Alleged directory is not a directory
            std::cerr << "[Paraver] Output trace directory " << dir_name << " is a file." << std::endl;
            exit(-1);
        }
    }

    for (int index = 0; index < 10000; index++) {
        std::ostringstream trace_name_builder;
        trace_name_builder << trace_base;
        if (index > 0) {
            trace_name_builder << "-" << std::setw(4) << std::setfill('0') << index;
        }
        trace_name_builder << ".prv";
        const std::string &trace_name = trace_name_builder.str();

        // If we can open the file with x flag, it did not exist and we have all the necessary permissions.
        FILE *file_desc = fopen(trace_name.c_str(), "wbx");
        if (file_desc != NULL) {
            fclose(file_desc);
            return trace_name.substr(0, trace_name.size() - 4);
        }
    }

    std::cerr << "[Paraver] Can not generate unique paraver trace file name." << std::endl;
    exit(-1);
}

inline
std::string Paraver::generate_tmp_file_name()
{
    std::ostringstream oss;
    char *dir = getenv("TMPDIR");
    if (dir != NULL) {
        oss << dir;
    } else {
        oss << "/tmp";
    }
    oss << "/tmp_trace_pid_" << getpid() << ".prv";

    return oss.str();
}

inline
sim::engine::ModuleType Paraver::filter_instrumented_modules(std::vector<std::string> &accepted_module_type_names) {
    // Unknown mode is represented by 0 so we will never be able to instrument it.
    sim::engine::ModuleType accepted = sim::engine::ModuleType::UNKNOWN;
    for (unsigned int i = 0; i < accepted_module_type_names.size(); i++) {
        accepted = static_cast<sim::engine::ModuleType>(accepted |
                sim::engine::get_module_type(accepted_module_type_names[i]));
    }
    return accepted;
}

inline
void Paraver::flush(bool force)
{
    if (text_trace_) {
        if (not intermediate_file_header_) {
            temporary_file_ << "#Paraver(01/01/2016 at 00:00):1:1(" << num_modules_
                            << "):1:1("<< num_components_ << ":1)" << std::endl;
            intermediate_file_header_ = true;
        }
        for (unsigned int i = 0; i < item_buffer_.current_elements_; i++) {
            temporary_file_ << item_buffer_.items_.get()[i] << std::endl;
        }
    } else {
        temporary_file_.write(reinterpret_cast<char*>(item_buffer_.items_.get()),
                    item_buffer_.current_elements_ * sizeof(InstrumentationItem));
    }
    item_buffer_.current_elements_ = 0;
}

inline
void Paraver::init(const sim::engine::Config &config)
{
    // Section to enable text trace instead of binary, to be able to load it while generating.
    text_trace_ = false;
    if (config.key_exists("Paraver", "text_trace") and config.get_value<int>("Paraver", "text_trace") == 1) {
        text_trace_ = true;
    }
    keep_intermediate_ = false;
    if (config.key_exists("Paraver", "keep_intermediate") and
            config.get_value<int>("Paraver", "keep_intermediate") == 1) {
        keep_intermediate_ = true;
    }
    intermediate_file_header_ = false;

    // Section to configure the sampling policy and hardware sampling interval.
    hardware_sampling_interval_ = config.get_value<sim::engine::cycles_t>("Paraver", "hardware_sampling_interval");
    sampling_policy_ = sampling_modes.at(config.get_value<std::string>("Paraver", "sampling_policy"));
    if (sampling_policy_ == sampling_policy_t::PER_TASK) {
        hardware_sampling_interval_ = sim::engine::NEVER;
    }
    if (config.key_exists("Paraver", "distribution_mode")) {
        distribution_mode_ = distribution_modes.at(config.get_value<std::string>("Paraver", "distribution_mode"));
    } else if (sampling_policy_ == sampling_policy_t::PER_TASK) {
        distribution_mode_ = counter_distribution_t::PER_TASK;
    } else {
        distribution_mode_ = counter_distribution_t::PER_MODULE;
    }

    // Now, if someone has disabled the paraver trace generation by the sampling_olicy = DISABLED:
    if (sampling_policy_ != sampling_policy_t::DISABLED) {
        // We reserve the final output file:
        trace_base_name_ = new_trace_base_name(config.get_value<std::string>(
                "Paraver", "trace_base_name") );

        pcf_filename_ = config.get_value<std::string>("Paraver", "pcf_filename");
        num_modules_ = 0;
        num_components_ = 0;
        end_time_stamp_ = 0;

        // Temporary file initialization:
        temporary_file_name_ = generate_tmp_file_name();
        if (text_trace_) {
            temporary_file_.open(temporary_file_name_.c_str(), std::ios_base::out);
        } else {
            temporary_file_.open(temporary_file_name_.c_str(), std::ios_base::binary | std::ios_base::out);
        }

        // Item buffer initialization:
        unsigned buffer_size = DEFAULT_BUFFER_SIZE;
        if (config.key_exists("Paraver", "buffer_size")) {
            buffer_size = config.get_value<unsigned>("Paraver", "buffer_size");
        }
        item_buffer_ = InstrumentationBuffer(buffer_size);

        // Instrumented modules:
        std::vector<std::string> accepted_module_type_names =
                config.get_values<std::string>("Paraver", "modules");
        instrumented_modules_ = filter_instrumented_modules(accepted_module_type_names);
    }
    init_ = true;
}

inline
void Paraver::finalize()
{
    assert(init_);
    // We check if trace generation has been disabled.
    if (sampling_policy_ == sampling_policy_t::DISABLED) {
        // If so, we do nothing here.
        init_ = false;
        return;
    }

    // We flush the buffers and close the temporary file:
    flush(true);
    temporary_file_.close();


    // We move the data from the termporary file to the final file:
    generate_final_paraver_file(std::string(trace_base_name_ + ".prv"), temporary_file_name_);
    std::cout << "Opening Paraver trace file " << (trace_base_name_ + ".prv") << std::endl;
    // Here we decide wether to remove the intermediary file.
    if (not keep_intermediate_) {
        unlink(temporary_file_name_.c_str());
    }

    // Creation of the .row file:
    create_row_file(std::string(trace_base_name_ + ".row"));

    // Copying the configuration file:
    copy_pcf_file(std::string(trace_base_name_ + ".pcf"), pcf_filename_);

    init_ = 0;
    return;
}

template<typename ModuleType>
inline
int Paraver::add_module(const ModuleType &module, unsigned components)
{
    assert(init_);
    // We check if trace generation has been disabled.
    if (sampling_policy_ == sampling_policy_t::DISABLED) {
        return -1;
    }

    bool instrumentalize = false;
    std::string module_name = module.get_name();
    if (instrumented_modules_ & module.get_module_type()) {
        instrumentalize = true;
    }
    int id = -1;
    if (instrumentalize) {
        // Module-Component indexing:
        if (distribution_mode_ == counter_distribution_t::PER_MODULE) {
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
        } else if (distribution_mode_ == counter_distribution_t::PER_TASK) {
            unsigned sub_index = ++module_count_[module_name];
            id = sub_index;
            // All counters are distributed to the appropiate CPU.
            // We get the Module name sub_index, if we have less than the sub-index, we must add
            // a new cpu
            if (sub_index > num_modules_) {
                ++num_modules_;
                ++num_components_;

                std::ostringstream module_name_buffer;
                module_name_buffer << "CPU " << sub_index;
                module_name_.push_back(module_name_buffer.str());
                component_name_.push_back(module_name_buffer.str());
            }
        }
    }
    return id;
}



inline
void Paraver::start_state(const InstrumentationItem &trace_item)
{
    assert(init_);
    assert(trace_item.cpu_id_ < num_modules_);

    if (current_states_.find(trace_item.cpu_id_) != current_states_.end()) {
        std::cerr << "Error: tried to start a new state before finishing the previous one" << std::endl;
        std::abort();
    }
    // We put it in the list of current states.
    current_states_[trace_item.cpu_id_] = trace_item;
}

inline
void Paraver::end_state(const InstrumentationItem &trace_item)
{
    assert(init_);

    if (current_states_.find(trace_item.cpu_id_) == current_states_.end()) {
        std::cerr << "Error: tried to finish a non-existant state." << std::endl;
        std::abort();
    }

    if (item_buffer_.current_elements_ == item_buffer_.max_elements_) {
        flush(false);
    }

    if (trace_item.time_stamp_ >=  current_states_[trace_item.cpu_id_].time_stamp_) {
        // If the state has some real time:
        InstrumentationItem &new_item = item_buffer_.items_.get()[item_buffer_.current_elements_];
        new_item = current_states_[trace_item.cpu_id_];
        new_item.u.state_.end_time_stamp_ = trace_item.time_stamp_;
        item_buffer_.current_elements_++;
        end_time_stamp_ = std::max(end_time_stamp_, trace_item.time_stamp_);
        current_states_.erase(trace_item.cpu_id_);
    } else {
        // Events cannot finish before they start
        std::cerr << "Event finishing before it starts." << std::endl;
        std::abort();
    }
}

inline
void Paraver::start_comm(const InstrumentationItem &trace_item)
{
    assert(init_);
}

inline
void Paraver::end_comm(const InstrumentationItem &trace_item)
{
    assert(init_);
    if (item_buffer_.current_elements_ == item_buffer_.max_elements_) {
        flush(false);
    }
}

template<typename ModuleType>
inline
void Paraver::add_event(const InstrumentationItem &trace_item, const ModuleType &module)
{
    assert(init_);
    if (sampling_policy_ == sampling_policy_t::DISABLED or
            trace_item.cpu_id_ == static_cast<unsigned>(-1)) {
        return;
    }

    // We have a shared module and we have periodic sampling and per task hardware event distribution,
    // this events need to be replicated for all cpus (or several).
    if (sampling_policy_ == sampling_policy_t::PERIODIC and distribution_mode_ == counter_distribution_t::PER_TASK) {
        std::string module_name = module.get_name();
        int num_reps = num_modules_ / module_count_[module_name];

        unsigned initial_module = (trace_item.cpu_id_ - 1) * num_reps + 1;
        unsigned limit_module = trace_item.cpu_id_ * num_reps + 1;

        for (unsigned new_id = initial_module; new_id < limit_module; new_id++) {
            InstrumentationItem replicated_item(trace_item);
            replicated_item.cpu_id_ = new_id;
            add_event(replicated_item);
        }
    } else {
        add_event(trace_item);
    }
}

inline
void Paraver::add_event(const InstrumentationItem &trace_item)
{
    assert(init_);
    if (sampling_policy_ == sampling_policy_t::DISABLED or
            trace_item.cpu_id_ == static_cast<unsigned>(-1)) {
        return;
    }
    // The comparison is <= as cpu_id_ is 1-indexed
    assert(trace_item.cpu_id_ <= num_modules_);

    // Check if the module has been instrumented.
    if (trace_item.cpu_id_ > num_modules_) {
        std::cerr << "Found an Event with module_id " << trace_item.cpu_id_ << " but we only have "
                  << num_modules_ << " instrumented modules" << std::endl;
        return;
    }

    // Check if we need to empty the temporary buffer:
    if (item_buffer_.current_elements_ == item_buffer_.max_elements_) {
        flush(false);
    }

    item_buffer_.items_.get()[item_buffer_.current_elements_] = trace_item;
    item_buffer_.current_elements_++;
    end_time_stamp_ = std::max(end_time_stamp_, trace_item.time_stamp_);
}

inline
void Paraver::add_function_name(const char* name)
{
    function_name_.push_back(std::string(name));
}

inline
void Paraver::add_precise_memory_event(const InstrumentationItem &trace_item) { }

inline
sim::engine::cycles_t Paraver::get_hardware_sampling_interval()
{
    return hardware_sampling_interval_;
}

inline
void Paraver::generate_final_paraver_file(const std::string &output_filename, const std::string &input_filename)
{
    // First we open the output file:
    std::fstream trace_file(output_filename.c_str(), std::ios_base::out);
    if (!trace_file.is_open()) {
        std::cerr << "Error while opening Instrumentation output file " << output_filename << std::endl;
        // We return gracefuly so other moduels counters can be printed in the execution log.
        return;
    }
    // Then open the intermediary file as input:
    std::fstream temporary_in;
    if (text_trace_) {
        temporary_in.open(input_filename.c_str(), std::ios_base::in);
        // We take out the header
        std::string header;
        getline(temporary_in, header);
    } else {
        temporary_in.open(input_filename.c_str(), std::ios_base::in | std::ios_base::binary);
    }

    if (!temporary_in.is_open()) {
        std::cerr << "Error when opening temporary file " << input_filename << std::endl;
        std::abort();
    }

    // We add the proper header:
    int day, month, year, hour, minute;
    std::time_t current_time = std::time(NULL);
    // localtime returns a poitnter to a singleton, so we cannot free it.
    tm &time_manager = *std::localtime(&current_time);
    day = time_manager.tm_mday;
    month = time_manager.tm_mon;
    // tm_year starts at 1900, we want from 2000.
    year = time_manager.tm_year - 100;
    hour = time_manager.tm_hour;
    minute = time_manager.tm_min;


    trace_file.fill('0');
    trace_file << "#Paraver(" << std::setw(2) << day << "/"  << std::setw(2)
               << month << "/" << std::setw(2) << year << " at "
               << std::setw(2) << hour << ":" << std::setw(2) << minute
               << "):" << end_time_stamp_;
    // We add the machine description:
    // When some modules have several components this might have to change:
    trace_file << "_ns:1(" << num_modules_ << "):";
    // Application description: #appl:[#tasks([#cpus_per_task:$task_id]:)]
    trace_file << "1:1(" << num_components_ << ":1)" << std::endl;

    InstrumentationItem temporary_item;


    if (text_trace_) {
        while (temporary_in >> temporary_item) {
            trace_file << temporary_item << std::endl;
        }
    } else {
        while (temporary_in.read(reinterpret_cast<char*>(&temporary_item), sizeof(InstrumentationItem))) {
            trace_file << temporary_item << std::endl;
        }
    }

    temporary_in.close();
    trace_file.close();
}

inline
void Paraver::create_row_file(const std::string &filename)
{
    std::fstream row_file(filename.c_str(), std::ios_base::out);
    if (!row_file.is_open()) {
        std::cerr << "Error while opening Instrumentation .row file."
                  << std::endl;
        std::abort();
    }
    row_file << "LEVEL CPU           SIZE " << num_modules_ << std::endl;
    for (unsigned i = 0; i < num_modules_; i++) {
        row_file << module_name_[i] << std::endl;
    }
    row_file<< std::endl;

    row_file << "LEVEL APPL          SIZE 1" << std::endl;
    row_file << "Tasksim appl" << std::endl << std::endl;

    row_file << "LEVEL THREAD        SIZE " << num_components_ << std::endl;
    for (unsigned i = 0; i < num_components_; i++) {
        row_file << component_name_[i] << std::endl;
    }
    row_file << std::endl;

    row_file << "LEVEL NODE          SIZE 1" << std::endl;
    row_file << "Node Tasksim" << std::endl << std::endl;

    row_file << "LEVEL SYSTEM        SIZE 1" << std::endl;
    row_file << "Tasksim simulation" << std::endl << std::endl;

    row_file << "LEVEL WORKLOAD      SIZE 1" << std::endl;
    row_file << "Tasksim workload" << std::endl;

    row_file.close();
}

inline
void Paraver::copy_pcf_file(const std::string &output_filename, const std::string &input_filename)
{
    std::fstream pcf_file(output_filename.c_str(), std::ios_base::out);
    if (!pcf_file.is_open()) {
        std::cerr << "Error while opening Instrumentation .pcf file."
                  << std::endl;
        std::abort();
    }
    std::fstream pcf_input_file(input_filename.c_str(), std::ios_base::in);
    if (!pcf_input_file.is_open()) {
        std::cerr << "Error while opening Input .pcf file." << std::endl;
        std::cerr << "You will have to copy it yourself." << std::endl;
        pcf_input_file.close();
        pcf_file.close();
        unlink(output_filename.c_str());
        return;
    }
    pcf_file << pcf_input_file.rdbuf();
    pcf_file << "EVENT_TYPE" << std::endl;
    pcf_file << "0    " << FUNCTION_NAME::TYPE
             << "    User Function Name" << std::endl;
    pcf_file << "VALUES" << std::endl;
    pcf_file << "0   End" << std::endl
             << "1   Unresolved" << std::endl;
    pcf_file << "2   _NOT_Found" << std::endl;
    for (unsigned int i = 0; i < function_name_.size(); i++) {
        pcf_file << std::setw(4) << std::left << i + FUNCTION_NAME::RESERVED
                 << function_name_[i] << std::endl;
    }
    pcf_file << "EVENT_TYPE" << std::endl;
    for (unsigned int i = 0; i < function_name_.size(); i++) {
        pcf_file << std::setw(4) << std::left << "0"
                << std::setw(12) << std::left << i + SIMULATOR_EVENTS::POLICY_TRAINER_CURRENT_POLICY
                << "PT_current_policy_" << function_name_[i] << std::endl;
    }
    for (unsigned int i = 0 ; i < function_name_.size(); i++) {
        pcf_file << std::setw(4) << std::left << "0"
                << std::setw(12) << std::left << i + SIMULATOR_EVENTS::POLICY_TRAINER_RETRAIN_COUNTER
                << "PT_retrain_counter_" << function_name_[i] << std::endl;
    }
    pcf_file.close();
    pcf_input_file.close();
}


inline void Paraver::dump_buffer()
{
    // We check if trace generation has been disabled.
    if (sampling_policy_ == sampling_policy_t::DISABLED) {
        // If so, we do nothing here.
        init_ = false;
        return;
    }

    // We flush the buffers and close the temporary file:
    flush(true);
    temporary_file_.close();

    // Creation of the .row file:
    create_row_file(std::string(trace_base_name_ + ".row"));

    // Copying the configuration file:
    copy_pcf_file(std::string(trace_base_name_ + ".pcf"), pcf_filename_);
    std::cout << "========================= WARNING ============================================" << std::endl;
    std::cout << "Temporary Paraver trace " << temporary_file_name_ << " closed without parsing." << std::endl;
    std::cout << "To parse it to text format please execute the conversion tool." << std::endl;
    std::cout << "$(TASKSIM_INSTALL_DIR)/bin/paraver_intermediate_to_txt -i " << temporary_file_name_
              << " -o " << (trace_base_name_ + ".prv");
    if (text_trace_) {
        std::cout << " -t";
    }
    std::cout << std::endl;
    std::cout << "========================= WARNING ============================================" << std::endl;
    // To ensure that the destructor will not try to parse it again.
    sampling_policy_ = sampling_policy_t::DISABLED;
}

inline
void Paraver::set_text_trace(bool text_trace)
{
    text_trace_ = text_trace;
}

inline
void Paraver::load_temporary_trace_restrictions(const std::string &input_filename)
{
    // We open the intermediary file as input:
    std::fstream temporary_in;
    if (text_trace_) {
        temporary_in.open(input_filename.c_str(), std::ios_base::in);
        // We take out the header
        std::string header;
        getline(temporary_in, header);
    } else {
        temporary_in.open(input_filename.c_str(), std::ios_base::in | std::ios_base::binary);
    }

    if (!temporary_in.is_open()) {
        std::cerr << "Error when opening temporary file " << input_filename << std::endl;
        std::abort();
    }


    InstrumentationItem temporary_item;
    end_time_stamp_ = 0;
    num_modules_ = 0;
    num_components_ = 0;
    while (temporary_in.good()) {
        if (text_trace_) {
            temporary_in >> temporary_item;
        } else {
            temporary_in.read(reinterpret_cast<char*>(&temporary_item), sizeof(InstrumentationItem));
        }
        end_time_stamp_ = std::max(end_time_stamp_, temporary_item.time_stamp_);
        num_modules_ = std::max(num_modules_, temporary_item.cpu_id_);
        num_components_ = std::max(num_components_, temporary_item.cpu_id_);
    }
    temporary_in.close();
}

}  // namespace instrumentation
}  // namespace utils
}  // namespace sim

#endif  // CORE_UTILS_INSTRUMENTATION_PARAVER_IMPL_H_
