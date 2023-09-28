/*
 * Copyright (c) 2016, Barcelona Supercomputing Center
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

// Authors: Thomas Grass (thomas.grass@bsc.es)

#include "TaskPoint.h"

#include <string>
#include <vector>

#include "core/engine/Simulator.h"
#include "core/utils/Log.h"
#include "core/utils/split.h"

namespace sim {
namespace runtime {

using sim::utils::Log;

TaskPoint::TaskPoint(engine::Simulator &sim, const sim::engine::Config &config, unsigned num_threads) :
    num_cpus_(num_threads),
    fast_forward_limit_(config.get_value<uint64_t>("fast_forward_limit")),
    num_samples_history_(config.get_value<uint64_t>("num_samples_history")),
    sampling_cut_off_(config.get_value<uint64_t>("sampling_cut_off")),
    initial_warmup_complete_(false),
    num_warmup_instances_start_(config.get_value<uint64_t>("num_warmup_instances_start")),
    num_warmup_instances_(config.get_value<uint64_t>("num_warmup_instances")),
    curr_num_warmup_instances_(num_warmup_instances_start_),
    sim_(sim),
    sample_history_complete_(false),
    start_cycle_detailed_(num_cpus_),
    detailed_instance_count_(num_cpus_),
    burst_instance_count_(num_cpus_),
    thread_warming_states_(num_cpus_, ThreadState::IDLE),
    serialization_file_name_(config.get_value<std::string>("serialization_file")),
    sample_history_warmed_(config.get_value<uint64_t>("num_samples_history")),
    sample_history_all_(config.get_value<uint64_t>("num_samples_history")),
    curr_phase_signatures_(num_cpus_),
    replay_mode_(false)
{
    std::string sample_replacement_policy = config.get_value<std::string>("sample_replacement_policy");
    if (sample_replacement_policy.compare("REPLACE_ONE") == 0) {
        sample_replacement_policy_ = SampleReplacementPolicy::REPLACE_ONE;
    } else if (sample_replacement_policy.compare("REPLACE_ALL") == 0) {
        sample_replacement_policy_ = SampleReplacementPolicy::REPLACE_ALL;
    } else {
        std::cerr << "Error: Unknown sample replacement policy" << std::endl;
        exit(-1);
    }

    // Load serialized state from previous simulation from file
    if (serialization_file_name_ != "") {
        unserialize(serialization_file_name_);
    }
    std::string ipc_output_file_name = config.get_value<std::string>("ipc_output_file");
    if (ipc_output_file_name != "NULL") {
        ipc_output_file_.open(ipc_output_file_name);
    }

    std::string ipc_input_file_name = config.get_value<std::string>("ipc_input_file");
    if (ipc_input_file_name != "NULL") {
        replay_mode_ = true;
        Log::info() << "[TaskPoint] Running TaskPoint in replay mode.";
        load_per_task_ipc(ipc_input_file_name);
    }
}

SimulationMode
TaskPoint::get_mode(PhaseSignature signature)
{
    Log::info() << "Get mode for signature phase of task " << signature.task_id_;
    // Called from the runtime, we must answer Burst or Memory.
    if (signature.subtrace_id_ < 0 or signature.task_type_ == 0 or signature.inst_count_ == 0) {
        // Master, Nanos++ or empty phase: BURST.
        Log::info() << "[TaskPoint] Will be simulated in Burst mode.";
        return BURST;
    }

    assert(sim_mode_.find(signature.task_id_) != sim_mode_.end());
    if (sim_mode_[signature.task_id_] == SimulationMode::BURST) {
        signature.duration_ = predict_cycle_count(signature);
        start_burst_phase(signature);
        Log::info() << "[TaskPoint] Will be simulated in Burst mode.";
    } else {
        start_detailed_phase(signature);
        Log::info() << "[TaskPoint] Will be simulated in Memory mode.";
    }
    return sim_mode_[signature.task_id_];
}

void
TaskPoint::start_detailed_phase(PhaseSignature signature)
{
    uint64_t thread_id = signature.thread_id_;
    if (signature.task_type_ == 0) {
        // We are running a master task phase.
        return;
    }

    assert(thread_id < start_cycle_detailed_.size());
    start_cycle_detailed_[thread_id] = sim_.get_clock();
    assert(sim_mode_.find(signature.task_id_) != sim_mode_.end());
    assert(sim_mode_[signature.task_id_] == SimulationMode::MEMORY);
    PhaseSignature& stored_signature = get_current_phase_signature(thread_id);
    if (signature.task_id_ != stored_signature.task_id_) {
        std::cout << "[ERROR] Starting phase for Task " << signature.task_id_
                  << " while we have " << stored_signature.task_id_ << " stored." << std::endl;
    }
}

void
TaskPoint::end_detailed_phase(PhaseSignature signature)
{
    if (signature.task_type_ == 0) {
        // We are finishing a master task phase.
        return;
    }
    uint64_t thread_id = signature.thread_id_;
    sim::engine::cycles_t cycle_count = sim_.get_clock() - start_cycle_detailed_[thread_id];

    PhaseSignature& stored_signature = get_current_phase_signature(thread_id);
    if (signature.task_id_ != stored_signature.task_id_) {
        std::cout << "[ERROR] Ending detailed phase for Task " << signature.task_id_
                  << " while we have " << stored_signature.task_id_ << " stored." << std::endl;
    }
    stored_signature.duration_ += cycle_count;
    stored_signature.inst_count_ += signature.inst_count_;
}

void
TaskPoint::start_burst_phase(PhaseSignature signature)
{
    // When this function is entered for the first time initial warmup is completed. Set number of warmup instances
    if (not initial_warmup_complete_) {
        initial_warmup_complete_ = true;
        curr_num_warmup_instances_ = num_warmup_instances_;
    }

    if (signature.task_type_ == 0) {
        // We are running master task phase.
        return;
    }
    // We update task duration and executed instructions.
    PhaseSignature &stored_signature = get_current_phase_signature(signature.thread_id_);
    if (signature.task_id_ != stored_signature.task_id_) {
        std::cout << "[ERROR] Starting phase for Task " << signature.task_id_
                  << " while we have " << stored_signature.task_id_ << " stored." << std::endl;
    }
    stored_signature.duration_ += signature.duration_;
    stored_signature.inst_count_ += signature.inst_count_;
}

void
TaskPoint::end_burst_phase(PhaseSignature signature)
{
    // Things moved to task_end.
    // We can  only trust signature.thread_id_ and the stored signature.
}

void
TaskPoint::task_start(unsigned task_id, unsigned thread_id, unsigned task_type_id)
{
    Log::info() << "[TaskPoint] Starting task " << task_id;
    // Here we decide the task execution mode.
    if (task_type_id == 0) {
        // Starting master task
        return;
    }
    if (get_current_phase_signature(thread_id).task_id_ != 0 && task_type_id != 0) {
        unsigned past_task_id = get_current_phase_signature(thread_id).task_id_;
        // We are starting a task in a thread that is already running another task.
        // This causes TaskPoint to give potentially erroneous estimates.
        Log::warning() << "[WARNING] Task " << past_task_id << " did not finish executing at thread "
                  << thread_id << " and task " << task_id << " is starting there.";
    }

    float ipc = 0.0f;
    sim_mode_[task_id] = get_simulation_mode(task_id, thread_id, task_type_id);
    task_start_cycle_[task_id] = sim_.get_clock();
    if (replay_mode_) {
        // We read its IPC from the input file:
        assert(ipc_per_task_.find(task_id) != ipc_per_task_.end());
        ipc = ipc_per_task_[task_id];
    } else if (task_type_id > 0 and sample_history_warmed_.has_sample(task_type_id)) {
        // Task type is at least partially sampled in history of warmed samples
        ipc = sample_history_warmed_.get_mean_ipc(task_type_id);
        extrapolate_from_[task_id] = sample_history_warmed_.get_last_id(task_type_id);
    } else if (task_type_id > 0 and sample_history_all_.has_sample(task_type_id)) {
        // Task type is at least partially sampled in history of all samples
        ipc = sample_history_all_.get_mean_ipc(task_type_id);
        extrapolate_from_[task_id] = sample_history_all_.get_last_id(task_type_id);
    } else if (sim_mode_[task_id] == SimulationMode::BURST) {
        std::cout << "[ERROR] Cannot find anything to get the ipc from." << std::endl;
        std::cout << "        Changing to MEMORY mode." << std::endl;
        sim_mode_[task_id] = SimulationMode::MEMORY;
    }

    PhaseSignature sig(0, task_type_id, thread_id, -1, 0, ipc, task_id);
    set_current_phase_signature(thread_id, sig);
    if (sim_mode_[task_id] == SimulationMode::BURST) {
        if (task_type_id >= burst_instance_count_[thread_id].size()) {
            burst_instance_count_[thread_id].resize(task_type_id + 1, 0);
        }
        ++burst_instance_count_[thread_id][task_type_id];
    }
    return;
}

void
TaskPoint::task_end(unsigned task_id, unsigned thread_id, unsigned task_type_id)
{
    Log::info() << "[TaskPoint] Ending task " << task_id;
    // Here we should print the ipc_out file. Also add to the histogram.
    if (task_type_id == 0) {
        // The master task, we do NOTHING.
        return;
    }

    PhaseSignature stored_sig = get_current_phase_signature(thread_id);
    if (stored_sig.task_id_ != task_id) {
        std::cout << "[WARNING] Stored signature has Work Descriptor " << stored_sig.task_id_
                  << " but the runtime says that thread is running task " << task_id << std::endl;
        std::cout << "[WARNING] This may be due to task nesting. Currently not supported on TaskPoint." << std::endl;
    }
    assert(sim_mode_.find(task_id) != sim_mode_.end());
    if (sim_mode_[task_id] == SimulationMode::BURST) {
        // If we extrapolated the task duration , we extrapolate the hardware counters.
        sim_.extrapolate_stats(extrapolate_from_[task_id], task_id);
        // We ended a Detailed phases executed in burst mode.
        print_phase_data(stored_sig, "FAST_FORWARD");
        // Remove no longer needed data:
        extrapolate_from_.erase(task_id);
    } else {
        stored_sig.ipc_ = static_cast<float>(stored_sig.inst_count_) /
                     static_cast<float>(stored_sig.duration_);
        add_detailed_instance_count(thread_id, task_type_id);

        // Add task instance to history of all samples
        sample_history_all_.add_sample(stored_sig);

        // Add task instance to history of "good" samples
        bool warm_sample = (thread_warming_states_[thread_id] == ThreadState::WARMED);
        // If thread_state == BURST_SAMPLE we already have enough samples
        // Also, some thread may already be fast forwarding so the value may be tainted.
        if (warm_sample) {
            sample_history_warmed_.add_sample(stored_sig);
        }
        print_phase_data(stored_sig, (warm_sample ? "DETAILED" : "WARMUP"));
    }
    sim_mode_.erase(task_id);
    task_start_cycle_.erase(task_id);
    // We reset the phase signature.
    PhaseSignature sig;
    set_current_phase_signature(thread_id, sig);
}

engine::cycles_t
TaskPoint::predict_cycle_count(PhaseSignature signature)
{
    // Simulation mode is now decided at ::task_start
    if (signature.task_type_ == 0 or signature.subtrace_id_ < 0 or signature.inst_count_ == 0) {
        // We do not predict master.
        return engine::NEVER;
    }

    assert(sim_mode_.find(signature.task_id_) != sim_mode_.end());
    assert(sim_mode_[signature.task_id_] == SimulationMode::BURST);

    PhaseSignature &stored_sig = get_current_phase_signature(signature.thread_id_);
    Log::info() << "[TaskPoint] We predict " << (static_cast<float>(signature.inst_count_) / stored_sig.ipc_);
    return static_cast<float>(signature.inst_count_) / stored_sig.ipc_;
}

inline
bool
TaskPoint::all_idle_or_warming()
{
    for (unsigned thread_id = 0; thread_id < thread_warming_states_.size(); ++thread_id) {
        if (not (thread_warming_states_[thread_id] == ThreadState::IDLE or
                thread_warming_states_[thread_id] == ThreadState::WARMING or
                thread_warming_states_[thread_id] == ThreadState::WARMED)) {
            return false;
        }
        // We also check that it has executed enough task instances.
        if (thread_warming_states_[thread_id] == ThreadState::WARMING) {
            uint64_t detailed_instances_current_thread = 0;
            for (uint64_t count : detailed_instance_count_[thread_id]) {
                detailed_instances_current_thread += count;
            }
            if (detailed_instances_current_thread < curr_num_warmup_instances_) {
                return false;
            }
        }
    }
    return true;
}

inline
bool
TaskPoint::all_idle_or_warmed()
{
    for (const ThreadState &state : thread_warming_states_) {
        if (not(state == ThreadState::IDLE or state == ThreadState::WARMED)) {
            return false;
        }
    }
    return true;
}

inline
bool
TaskPoint::all_task_types_sampled()
{
    for (unsigned thread_id = 0; thread_id < detailed_instance_count_.size(); ++thread_id) {
        for (unsigned i = 1; i < detailed_instance_count_[thread_id].size(); ++i) {
            if (detailed_instance_count_[thread_id][i] < 1) {
                return false;
            }
        }
    }
    return true;
}

inline
void
TaskPoint::set_all_active_threads(ThreadState state)
{
    for (unsigned i = 0; i < thread_warming_states_.size(); ++i) {
        if (thread_warming_states_[i] != ThreadState::IDLE) {
            thread_warming_states_[i] = state;
        }
    }
}

inline
void
TaskPoint::add_detailed_instance_count(uint64_t thread_id, uint64_t task_type)
{
    if (task_type + 1 > detailed_instance_count_[thread_id].size()) {
        detailed_instance_count_[thread_id].resize(task_type + 1, 0);
    }
    detailed_instance_count_[thread_id][task_type]++;
}

inline
TaskPoint::~TaskPoint()
{
    if (serialization_file_name_ != "") {
        serialize(serialization_file_name_);
    }
    if (not task_start_cycle_.empty()) {
        std::cout << "[ERROR] Task start cycle dictionary still has values:" << std::endl;
        for (auto &pair : task_start_cycle_) {
            std::cout << pair.first << " - " << pair.second << std::endl;
        }
    }
}

inline
void
TaskPoint::resample()
{
    // Set thread warming states to ThreadState::IDLE
    for (unsigned i = 0; i < thread_warming_states_.size(); ++i) {
        if (thread_warming_states_[i] != ThreadState::IDLE) {
            if (num_warmup_instances_start_ == 0 and num_warmup_instances_ == 0) {
                thread_warming_states_[i] = ThreadState::WARMED;
            } else {
                thread_warming_states_[i] = ThreadState::WARMING;
            }
        }
    }

    // Reset detailed and burst instance counts
    for (auto &v : detailed_instance_count_) {
        v.clear();
    }
    for (auto &v : burst_instance_count_) {
        v.clear();
    }
}

inline
bool
TaskPoint::sample_history_complete()
{
    sample_history_complete_ = sample_history_warmed_.all_task_types_fully_sampled();
    return sample_history_complete_;
}

inline
bool
TaskPoint::sampling_cut_off()
{
    for (unsigned thread_id = 0; thread_id < detailed_instance_count_.size(); ++thread_id) {
        if (thread_warming_states_[thread_id] == ThreadState::WARMED) {
            uint64_t total_instances_current_thread = 0;
            for (unsigned task_type = 1; task_type < detailed_instance_count_[thread_id].size(); ++task_type) {
                total_instances_current_thread += detailed_instance_count_[thread_id][task_type];
            }
            if (total_instances_current_thread <= sampling_cut_off_) {
                return false;
            }
        }
    }
    return true;
}


inline
void
TaskPoint::serialize(const std::string &filename)
{
    if (filename == "NULL") {
        return;
    }

    std::ofstream output_file;
    output_file.open(filename);
    assert(output_file.is_open());

    output_file << sample_history_warmed_;
    output_file << sample_history_all_;

    // Detailed instance count per thread per task type
    unsigned threads = detailed_instance_count_.size();
    unsigned task_types = threads > 0 ? detailed_instance_count_[0].size() : 0;
    output_file << threads;
    output_file << ":" << task_types;
    output_file << std::endl;
    for (unsigned i = 0; i < threads; ++i) {
        if (detailed_instance_count_[i].size() == 0) continue;
        output_file << detailed_instance_count_[i][0];
        for (unsigned j = 1; j < task_types; ++j) {
            output_file << ":" << detailed_instance_count_[i][j];
        }
        output_file << std::endl;
    }

    // Burst instance count per thread per task type
    threads = burst_instance_count_.size();
    task_types = threads > 0 ? burst_instance_count_[0].size() : 0;
    output_file << threads;
    output_file << ":" << task_types;
    output_file << std::endl;
    for (unsigned i = 0; i < threads; ++i) {
        if (burst_instance_count_[i].size() == 0) {
            continue;
        }
        output_file << burst_instance_count_[i][0];
        for (unsigned j = 1; j < task_types; ++j) {
            output_file << ":" << burst_instance_count_[i][j];
        }
        output_file << std::endl;
    }

    // Thread warming state
    threads = thread_warming_states_.size();
    output_file << threads << std::endl;
    output_file << static_cast<unsigned>(thread_warming_states_[0]);
    for (unsigned i = 1; i < threads; ++i) {
        output_file << ":" << static_cast<unsigned>(thread_warming_states_[i]);
    }
    output_file << std::endl;


    // Scalar members
    output_file << initial_warmup_complete_ << std::endl;
    output_file << curr_num_warmup_instances_ << std::endl;
    output_file << sample_history_complete_ << std::endl;
}

inline
void
TaskPoint::unserialize(const std::string &filename)
{
    if (filename == "NULL") {
        return;
    }

    std::ifstream input_file(filename);
    if (not input_file.is_open()) {
        std::cerr << "[WARNING] Failed to open serialiazation file "
                  << filename << std::endl;
        return;
    }

    input_file >> sample_history_warmed_;
    input_file >> sample_history_all_;

    // Detailed instance count per thread per task type
    unsigned threads, task_types;
    char sep;
    input_file >> threads; input_file >> sep; assert(sep == ':');
    input_file >> task_types;
    detailed_instance_count_.resize(threads);
    for (unsigned i = 0; i < threads; ++i) {
        detailed_instance_count_[i].resize(task_types);
        input_file >> detailed_instance_count_[i][0];
        for (unsigned j = 1; j < task_types; ++j) {
            input_file >> sep; assert(sep == ':');
            input_file >> detailed_instance_count_[i][j];
        }
    }

    // Burst instance count per thread per task type
    input_file >> threads; input_file >> sep; assert(sep == ':');
    input_file >> task_types;
    burst_instance_count_.resize(threads);
    for (unsigned i = 0; i < threads; ++i) {
        burst_instance_count_[i].resize(task_types);
        if (task_types == 0) {
            continue;
        }
        input_file >> burst_instance_count_[i][0];
        for (unsigned j = 1; j < task_types; ++j) {
            input_file >> sep; assert(sep == ':');
            input_file >> burst_instance_count_[i][j];
        }
    }

    // Thread warming state
    input_file >> threads;
    thread_warming_states_.resize(threads);
    int temp;
    input_file >> temp;
    thread_warming_states_[0] = static_cast<ThreadState>(temp);
    for (unsigned i = 1; i < threads; ++i) {
        int temp;
        input_file >> sep; assert(sep == ':');
        input_file >> temp;
        thread_warming_states_[i] = static_cast<ThreadState>(temp);
    }

    // Scalar members
    input_file >> initial_warmup_complete_;

    input_file >> curr_num_warmup_instances_;

    input_file >> sample_history_complete_;
}

inline
void
TaskPoint::set_current_phase_signature(unsigned thread_id, PhaseSignature& signature)
{
    assert(thread_id < curr_phase_signatures_.size());
    curr_phase_signatures_[thread_id] = signature;
}

inline
PhaseSignature&
TaskPoint::get_current_phase_signature(unsigned thread_id)
{
    assert(thread_id < curr_phase_signatures_.size());
    return curr_phase_signatures_[thread_id];
}

inline
void
TaskPoint::load_per_task_ipc(const std::string &filename)
{
    if (filename == "NULL") {
        return;
    }

    std::ifstream input_file;
    input_file.open(filename);
    assert(input_file.is_open());
    std::string line;
    unsigned wd_id;
    float ipc;

    while (input_file.good()) {
        input_file >> line;
        std::vector<std::string> line_split = core::utils::split(line, ':');
        wd_id = atoll(line_split[1].c_str());
        ipc = atof(line_split[6].c_str());
        ipc_per_task_[wd_id] = ipc;
    }
}

inline
SimulationMode
TaskPoint::get_simulation_mode(unsigned task_id, unsigned thread_id, unsigned task_type)
{
    // Check if we have read its IPC.
    if (replay_mode_) {
        assert(ipc_per_task_.find(task_id) != ipc_per_task_.end());
        Log::info() << "[TaskPoint] Task " << task_id << " will be simulated in burst mode, we have its IPC.";
        return SimulationMode::BURST;
    }
    // Check if we need to resample
    if (burst_instance_count_[thread_id].size() > task_type
            and burst_instance_count_[thread_id][task_type] >= fast_forward_limit_) {
        // Reached fast forward limit.
        if (sample_replacement_policy_ == SampleReplacementPolicy::REPLACE_ALL) {
            sample_history_warmed_.clear();
        }
        resample();
    }
    // For some reason master is always simulated in Burst mode.
    if (task_type == 0) {
        Log::info() << "[TaskPoint] Task " << task_id
                    << " will be simulated in burst mode as is of main type.";
        return SimulationMode::BURST;
    }
    assert(thread_id < thread_warming_states_.size());
    switch (thread_warming_states_[thread_id]) {
    case ThreadState::IDLE:
        // The thread was idle, now will be warming up.
        thread_warming_states_[thread_id] = ThreadState::WARMING;
        sample_history_warmed_.clear();
        resample();
        Log::info() << "[TaskPoint] Task " << task_id
                    << " will be simulated in Memory mode, we were idling.";
        return SimulationMode::MEMORY;
        break;
    case ThreadState::WARMING:
        if (all_idle_or_warming()) {
            // We finally warmed up, now sampling.
            thread_warming_states_[thread_id] = ThreadState::WARMED;
        }
        Log::info() << "[TaskPoint] Task " << task_id
                    << " will be simulated in Memory mode,we are still warming up.";
        return SimulationMode::MEMORY;
        break;
    case ThreadState::WARMED:
        if (can_extrapolate(task_type)) {
            // We have enough samples, no need for further sampling
            set_all_active_threads(ThreadState::BURST_SAMPLE);
            Log::info() << "[TaskPoint] Task " << task_id
                        << " will be simulated in Burst mode, we have samples.";
            return SimulationMode::BURST;
        } else {
            // Cannot extrapolate this task, continue warming/sampling
            Log::info() << "[TaskPoint] Task " << task_id
                        << " will be simulated in Memory mode, we are warm but cannot extrapolate this task.";
            return SimulationMode::MEMORY;
        }
        break;
    case ThreadState::BURST_SAMPLE:
        if (not have_sample(task_type)) {
            // We cannot predict its duration, we need to start warming up again:
            resample();
            Log::info() << "[TaskPoint] Task " << task_id
                        << " will be simulated in Memory mode, we are FastForwarding"
                        << ", but have no samples for this task.";
            return SimulationMode::MEMORY;
        } else {
            // We can still predict the duration.
            Log::info() << "[TaskPoint] Task " << task_id
                        << " will be simulated in Burst mode, we are FastForwarding.";
            return SimulationMode::BURST;
        }
        break;
    default:
        // Should never reach this
        assert(false);
        return SimulationMode::BURST;
    }
}

inline
void TaskPoint::print_phase_data(const PhaseSignature &signature, std::string mode)
{
    if (not ipc_output_file_.is_open()) {
        return;
    }
    if (signature.duration_ == 0) {
        std::cerr << "[ERROR] Task finished without executing any user code phase?." << std::endl;
        std::cerr << signature.thread_id_ << ":"
                  << signature.task_id_ << ":"
                  << 0 << ":"
                  << task_start_cycle_[signature.task_id_] << ":"
                  << sim_.get_clock() << ":"
                  << mode << ":"
                  << signature.ipc_ << std::endl;
        return;
    }
    assert(task_start_cycle_.find(signature.task_id_) != task_start_cycle_.end());
    ipc_output_file_ << signature.thread_id_ << ":"
                     << signature.task_id_ << ":"
                     << 0 << ":"
                     << task_start_cycle_[signature.task_id_] << ":"
                     << sim_.get_clock() << ":"
                     << mode << ":"
                     << signature.ipc_ << std::endl;
    return;
}

bool TaskPoint::have_sample(unsigned task_type) {
    return sample_history_warmed_.has_sample(task_type) or sample_history_all_.has_sample(task_type);
}

bool TaskPoint::can_extrapolate(unsigned task_type) {
    return (all_idle_or_warmed() and
           ((all_task_types_sampled() and sample_history_complete()) or sampling_cut_off()) and
           have_sample(task_type));
}

}  // namespace runtime
}  // namespace sim
