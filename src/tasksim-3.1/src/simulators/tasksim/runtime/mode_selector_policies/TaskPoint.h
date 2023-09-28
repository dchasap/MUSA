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

#ifndef SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_TASKPOINT_H_
#define SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_TASKPOINT_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "core/engine/Simulator.h"
#include "PolicyInterface.h"
#include "SampleHistory.h"
#include "simulators/tasksim/runtime/types.h"

namespace sim {
namespace runtime {

class TaskPoint : public PolicyInterface
{
public:
    TaskPoint(engine::Simulator &sim, const sim::engine::Config &config, unsigned num_threads);
    ~TaskPoint();

    // Get simulation mode (BURST or MEMORY) for current phase
    SimulationMode get_mode(PhaseSignature signature);
    // Called by runtime env. class to signal start of detailed phase
    void start_detailed_phase(PhaseSignature signature);
    // Called by runtime env. class to signal end of detailed phase
    void end_detailed_phase(PhaseSignature signature);
    // Called by the runtime when a detailled phase is executed in burst mode.
    void start_burst_phase(PhaseSignature signature);
    // Called by the runtime when any burst mode phase finishes.
    void end_burst_phase(PhaseSignature signature);

    // Called by the runtime, signals that a task starts.
    void task_start(unsigned task_id, unsigned thread_id, unsigned task_type_id);
    // Called by the runtime, signals that a task has ended.
    void task_end(unsigned task_id, unsigned thread_id, unsigned task_type_id);

    // Get predicted cycle count for phase which is ito be simulated in BURST mode
    engine::cycles_t predict_cycle_count(PhaseSignature signature);

private:
    enum class SampleReplacementPolicy : unsigned {REPLACE_ONE = 0, REPLACE_ALL = 1};
    // Warming state for each simulated thread
    enum class ThreadState : unsigned {IDLE = 0, WARMING = 1, WARMED = 2, BURST_SAMPLE = 3};
    // Number of cpus/threads simulated
    unsigned num_cpus_;
    // Calculated number of task instances simulated in fast forward mode
    uint64_t fast_forward_limit_;
    // Lenght of history of sample task instances
    uint64_t num_samples_history_;
    uint64_t sampling_cut_off_;
    // Indicates if warmup at simulation start is complete
    bool initial_warmup_complete_;
    // Number of task instances to be simulated for warmup at beginning of simulation
    uint64_t num_warmup_instances_start_;
    // Number of task instances to be simulated for warmup before measuring samples
    uint64_t num_warmup_instances_;
    // Current number of warmup instances. Can be num_warmup_instances_start or num_warmup_instances
    uint64_t curr_num_warmup_instances_;
    // Sample replacement policy: REPLACE_ONE or REPLACE_ALL
    SampleReplacementPolicy sample_replacement_policy_;
    // TaskSim's simulator reference.
    sim::engine::Simulator &sim_;

    // Set to true if we have num_samples_history samples of all task types
    bool sample_history_complete_;

    // Start cycle of detailed simulation interval per thread.
    std::vector<sim::engine::cycles_t> start_cycle_detailed_;

    // Number of task instances simulated in detail per thread per task type
    std::vector<std::vector<uint64_t>> detailed_instance_count_;

    // Number of task instances simulated in burst mode per thread and task type
    std::vector<std::vector<uint64_t>> burst_instance_count_;

    // Vector of thread states for simulated threads
    std::vector<ThreadState> thread_warming_states_;

    std::string serialization_file_name_;

    // History of valid samples
    SampleHistory sample_history_warmed_;
    // History of all samples
    SampleHistory sample_history_all_;
    // Current PhaseSignature per thread
    std::vector<PhaseSignature> curr_phase_signatures_;
    // Output file stream for ipc output file
    std::ofstream ipc_output_file_;
    // IPC per task instance as read from IPC input file
    std::unordered_map<unsigned, float> ipc_per_task_;

    // Tracks which tasks are executed in which mode:
    std::unordered_map<unsigned, SimulationMode> sim_mode_;
    // Tracks from which task to extraploate hardware counters on task completion
    std::unordered_map<unsigned, unsigned> extrapolate_from_;
    // Tracks from which task to extraploate hardware counters on task completion
    std::unordered_map<unsigned, sim::engine::cycles_t> task_start_cycle_;
    // Specifies if we are using input ipc values
    bool replay_mode_;

    // Checks if all threads are either in state IDLE or in state WARMING
    bool all_idle_or_warming();
    // Checks of all threads are either in state IDLE or in state WARMED
    bool all_idle_or_warmed();
    // Checks if at least one instance of all (known) task types has been executed in the corresponding thread
    bool all_task_types_sampled();
    // Switch all non-idle threads to state
    void set_all_active_threads(ThreadState state);
    // Adds a detailed instace to the structure that counts them
    void add_detailed_instance_count(uint64_t thread_id, uint64_t task_type);
    // Check if all task types are fully sampled
    bool sample_history_complete();
    // Check if cut-off condition is fulfilled and we start fast forwarding with the task instances we have so far
    bool sampling_cut_off();
    // Set current PhaseSignature for a thread
    void set_current_phase_signature(unsigned thread_id, PhaseSignature &signature);
    // Return current PhaseSignature of a thread
    PhaseSignature& get_current_phase_signature(unsigned thread_id);
    // Read per-task IPCs from IPC input file
    void load_per_task_ipc(const std::string &file_name);
    // Decides execution mode per task.
    SimulationMode get_simulation_mode(unsigned task_id, unsigned thread_id, unsigned task_type);
    // Prints the PhaseSignature data to the ipc_output_file
    void print_phase_data(const PhaseSignature &signature, std::string mode);
    // Returns if we have a sample value for said task type
    bool have_sample(unsigned task_type);
    // Returns if we have enough data to fast forward a task of said type
    bool can_extrapolate(unsigned task_type);

    // Resample all task types
    void resample();

    // Serialize sample history
    void serialize(const std::string &filename);

    // Unserialize sample history
    void unserialize(const std::string &filename);
};

}  // namespace runtime
}  // namespace sim

#endif  // SIMULATORS_TASKSIM_RUNTIME_MODE_SELECTOR_POLICIES_TASKPOINT_H_
