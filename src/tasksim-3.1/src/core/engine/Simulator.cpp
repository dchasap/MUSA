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

#include "Simulator.h"

#include <cassert>
#include <csignal>
#include <iostream>
#include <functional>

#include "core/stats/TimeStatistics.h"
#include "Module.h"

namespace sim {
namespace engine {

static bool abort_;

void handle_sigterm(int param)
{
    sim::engine::abort_ = true;
}

Simulator* Simulator::instance_ = nullptr;

Simulator* Simulator::get_instance()
{
    return instance_;
}

void Simulator::extrapolate_stats(uint64_t from, uint64_t to)
{
    for (auto i = stat_vector_.begin(); i != stat_vector_.end(); ++i) {
        (*i)->extrapolate(from, to);
    }
}

Simulator::Simulator(Config &config) :
        current_cycle_(NEVER),
        phase_(Low),
        next_hardware_sampling_(0),
        hardware_sampling_interval_(NEVER),
        deadlock_detection_interval_(config.get_value_with_default<unsigned>("deadlock_detection_interval", 100000)),
        next_deadlock_detection_cycle_(deadlock_detection_interval_),
        config_(config), end_(false), instrumentation_()
{
    if (deadlock_detection_interval_ != sim::engine::NEVER and deadlock_detection_interval_ <= config.get_value<unsigned>("idle_cycles")) {
        deadlock_detection_interval_ = 2 * config.get_value<unsigned>("idle_cycles") * config.get_value<float>("tracing_cpu_freq") / 1000.0f;
        utils::Log::info() << "[WARNING] Deadlock detection interval switcher to "
                           << deadlock_detection_interval_ << " because it was too short.";
    }
    instrumentation_.init(config);
    // Done this way to avoid consulting [Paraver] when it is disabled.
    hardware_sampling_interval_ = instrumentation_.get_hardware_sampling_interval();
    sim::engine::abort_ = false;
    std::signal(SIGTERM, handle_sigterm);
    std::signal(SIGUSR2, handle_sigterm);
    instance_ = this;
}

Simulator::~Simulator()
{
    instrumentation_.finalize();
}

void Simulator::run()
{
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_SIMULATOR);
    current_cycle_ = 0;
    for (unsigned int mod_id = 0; mod_id < module_vector_.size(); ++mod_id) {
        if (module_vector_[mod_id]->reset() == false) continue;
        module_vector_[mod_id]->set_module_activity(0);
    }

    active_module_list_t::iterator list_end = active_module_list_.end();
    active_module_list_t::iterator list_begin;
    active_module_list_t::iterator list_iterator;

    unsigned mod_id;
    do {
        /* Update the clock */
        current_cycle_ = active_module_list_.front().first;

        /* We print the hardware counters (if we need to). */
        hardware_counters_sampling();

        /* Set the next cycle to the end of the simulation and clock phase */
        phase_ = Low;

        /* Call the start of the cycle for each module */

        for (list_iterator = active_module_list_.begin();
                list_iterator != list_end and list_iterator->first == current_cycle_; ++list_iterator) {
            assert(list_iterator->second < module_vector_.size());
            sim::stats::Time_Stats.phaseIn(get_code_region(module_vector_[list_iterator->second]->module_type_));
            module_vector_[list_iterator->second]->start();
            sim::stats::Time_Stats.phaseOut(get_code_region(module_vector_[list_iterator->second]->module_type_));
        }

        phase_ = High;

        /* Call the end of the cycle for each module */
        while (true) {
            list_begin = active_module_list_.begin();
            if (list_begin == list_end or list_begin->first != current_cycle_) {
                break;
            }

            mod_id = list_begin->second;
            sim::stats::Time_Stats.phaseIn(get_code_region(module_vector_[mod_id]->module_type_));
            module_vector_[mod_id]->end();
            sim::stats::Time_Stats.phaseOut(get_code_region(module_vector_[mod_id]->module_type_));
            if (module_vector_[mod_id]->get_next_activity() == current_cycle_)
                module_vector_[mod_id]->set_module_activity(engine::NEVER);
        }
        check_deadlock();
    } while (active_module_list_.empty() == false and not end_ and not sim::engine::abort_);

    if (sim::engine::abort_) {
        // If the abnormal termination function has been set, we call it.
        if (abnormal_termination_function) {
          abnormal_termination_function();
        }
        std::cout << "========================= WARNING ========================" << std::endl;
        std::cout << "Simulation ended prematurely." << std::endl;
        std::cout << "========================= WARNING ========================" << std::endl;
        instrumentation_.dump_buffer();
        for (auto &list_elem : active_module_list_) {
            std::cerr << "Module with  ID: " << list_elem.second
                      << " will awaken at cycle " << list_elem.first << std::endl;
            module_vector_[list_elem.second]->print_status();
        }

        return;
    }
    std::cout << "Final cycle count: " << current_cycle_ << std::endl;
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_SIMULATOR);
}

void Simulator::check_deadlock()
{
    if (next_deadlock_detection_cycle_ != engine::NEVER and current_cycle_ >= next_deadlock_detection_cycle_) {
        // We do not need exact periodic sampling:
        next_deadlock_detection_cycle_ = current_cycle_ + deadlock_detection_interval_;
        bool activity = false;
        for (unsigned int mod_id = 0; mod_id < module_vector_.size(); ++mod_id) {
            bool mod_active = module_vector_[mod_id]->cpu_activity_poll();
            activity = activity || mod_active;
        }
        if (activity == false) {
            std::cerr << "===================== WARNING =========================" << std::endl
                      << "Deadlock detection mechanism has detected a possible deadlock." << std::endl
                      << "At cycle " << current_cycle_ << " there has been no active cpu for "
                      << deadlock_detection_interval_ << " cycles." << std::endl
                      << "Printing information for active modules:" << std::endl;
            for (auto &list_elem : active_module_list_) {
                std::cerr << "Module with  ID: " << list_elem.second
                          << " will awaken at cycle " << list_elem.first << std::endl;
                module_vector_[list_elem.second]->print_status();
            }
            std::cerr << "[DEBUG] Now all modules:" << std::endl;
            for (auto &list_elem : module_vector_) {
                list_elem->print_status();
            }
            exit(-1);
        }
    }
}

void Simulator::hardware_counters_sampling()
{
    /* First of all, check if we need to output Hardware counters on the instrumentation */
    if (next_hardware_sampling_ != engine::NEVER and current_cycle_ >= next_hardware_sampling_) {
        sim::stats::Time_Stats.phaseIn(sim::engine::REGION_INSTRUMENTATION_TRACE);
        for (unsigned int mod_id = 0; mod_id < module_vector_.size(); ++mod_id) {
            module_vector_[mod_id]->print_hardware_counters(next_hardware_sampling_);
        }
        // If we still have not covered the next sampling interval, proceed as usual.
        if (next_hardware_sampling_ + hardware_sampling_interval_ >= current_cycle_) {
            next_hardware_sampling_ += hardware_sampling_interval_;
        } else {
            // We need to fast forward, a lot.
            next_hardware_sampling_ = (current_cycle_ / hardware_sampling_interval_) * hardware_sampling_interval_;
            for (unsigned int mod_id = 0; mod_id < module_vector_.size(); ++mod_id) {
                module_vector_[mod_id]->print_hardware_counters(next_hardware_sampling_);
            }
            next_hardware_sampling_ += hardware_sampling_interval_;
        }
        sim::stats::Time_Stats.phaseOut(sim::engine::REGION_INSTRUMENTATION_TRACE);
    }
}

}  // namespace engine
}  // namespace sim
