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

#include <getopt.h>

#include <vector>
#include <string>

#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Controller.h"
#include "core/logic/coherence/single/Directory.h"
#include "core/logic/coherence/single/Message.h"
#include "core/modules/cpu/Burst.h"
#include "core/modules/cpu/Memory.h"
#include "core/modules/interconnect/Bus.h"
#include "core/modules/memory/Cache.h"
#include "core/modules/memory/DMA.h"
#include "core/modules/memory/DRAM.h"
#include "core/modules/memory/Perfect.h"
#include "core/utils/CheckTrace.h"
#include "core/utils/Log.h"
#include "simulators/tasksim/runtime/OmpSs.h"
#include "simulators/tasksim/runtime/ModeSelector.h"
#include "simulators/tasksim/runtime/types.h"

#ifdef ENABLE_DRAMSIM   // ENABLE_DRAMSIM
#include "core/modules/memory/Dramsim2Controller.h"

#elif ENABLE_RAMULATOR  // ENABLE_RAMULATOR
#include "core/modules/memory/RamulatorController.h"
#endif

using sim::utils::Log;
typedef struct {
    std::string cfg;
    std::string trace;
    int tid;
} Options;

void usage(const std::string&);
bool parse_options(int, char*[], Options&);

int main(int argc, char *argv[])
{
    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_SIMULATOR_SETUP);
    Options opt;
    if (parse_options(argc, argv, opt) == false) {
        usage(argv[0]);
        exit(0);
    }
    sim::engine::Config conf(opt.cfg);
    sim::engine::Simulator simulator(conf);

    std::vector<unsigned> num_cpus = conf.get_values<unsigned>("ncpus");
    assert(num_cpus.size() == 2);
    // FIXME: multiple threads per cpu not supported yet, assumed to be 1
    // std::vector<unsigned> threads_per_cpu = conf.get_values<unsigned>("threads_per_cpu");

    /** Instantiate CPUs */
    std::vector<sim::cpu::Burst*> burst_cpus;
    std::vector<sim::cpu::Memory<sim::cpu::timing_models::ROA, sim::logic::coherence::single::Message>*> mem_cpus;
    unsigned tcpus = 0;
    /** For each cpu type */
    for (unsigned c = 0; c < num_cpus.size(); c++) {
        /** Create a set of cpus of that type */
        for (unsigned i = 0; i < num_cpus[c]; ++i) {
            burst_cpus.push_back(new sim::cpu::Burst(simulator,
                    sim::engine::Config(conf, "Burst", c), tcpus));
            mem_cpus.push_back(new sim::cpu::Memory<sim::cpu::timing_models::ROA,
                    sim::logic::coherence::single::Message>(simulator,
                    sim::engine::Config(conf, "MemCPU", c), tcpus));
            ++tcpus;
        }
    }

    // Instantiate the coherence directory
    sim::logic::coherence::single::Directory<sim::logic::coherence::single::Message> directory(tcpus, 3);

    // Instantiate MMU
    sim::logic::memory::MMU mmu(&simulator, sim::engine::Config(conf, "MMU"), sim::engine::Config(conf, "L2Cache"));
    sim::engine::Config mmu_config(sim::engine::Config(conf, "MMU"));

    // Instantiate simulator runtime class
    sim::runtime::OmpSs ompss(simulator, conf, opt.trace, tcpus*1 /*threads_per_cpu != 1 not supported yet*/, opt.tid);
    simulator.set_abnormal_termination_function(std::bind(&sim::runtime::OmpSs::print_finish_message, &ompss));

    // Connect runtime burst buffer to burst cpus and mitem buffer to memory cpu
    for (unsigned i = 0; i < tcpus; i++) {
        ompss[i].burst_ >> *burst_cpus[i];
        ompss[i].mem_ >> *mem_cpus[i];
    }

    // Instantiate private L1 and L2 caches
    std::vector<sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>*> dl1_caches;
    std::vector<sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>*> l2_caches;
    std::vector<sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>*> little_caches;
    unsigned current_cpu;
    unsigned big_cores = num_cpus[0];
    unsigned little_cores = num_cpus[1];
    Log::debug() << "[DEBUG] Total cpus: " << tcpus << " big: " << big_cores << " little:" << little_cores;
    for (current_cpu = 0; current_cpu < big_cores; current_cpu++) {
        dl1_caches.push_back(new sim::memory::Cache<sim::logic::coherence::single::Message,
                sim::logic::coherence::single::Controller,
                sim::interconnect::Direct>(simulator, directory,
                sim::engine::Config(conf, "DL1Cache"), current_cpu));

        l2_caches.push_back(new sim::memory::Cache<sim::logic::coherence::single::Message,
                sim::logic::coherence::single::Controller,
                sim::interconnect::Direct>(simulator, directory,
                sim::engine::Config(conf, "L2Cache"), current_cpu));
    }

    for (current_cpu = big_cores; current_cpu < big_cores + little_cores; current_cpu++) {
        little_caches.push_back(new sim::memory::Cache<sim::logic::coherence::single::Message,
                sim::logic::coherence::single::Controller,
                sim::interconnect::Direct>(simulator, directory,
                sim::engine::Config(conf, "LittleCache"), current_cpu - big_cores));
    }

    // Instantiate L2 bus
    sim::interconnect::Bus<sim::logic::coherence::single::Message,
                sim::interconnect::Direct> l2_bus(simulator,
                sim::engine::Config(conf, "L2Bus"));


    // Instantiate DRAM or perfect memory model
    #ifdef ENABLE_DRAMSIM
        sim::memory::Dramsim2Controller<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>
                memory(simulator, sim::engine::Config(conf, "DRAM"), &mmu);
    #elif ENABLE_RAMULATOR  // ENABLE_RAMULATOR
        sim::memory::RamulatorController<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>
                memory(simulator, sim::engine::Config(conf, "RAMULATOR"),
                        sim::engine::Config(conf, "L2Cache"), tcpus, &mmu);
    #elif 1
        sim::memory::Perfect<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>
                memory(simulator, sim::engine::Config(conf, "Memory"));
    #else
        sim::memory::DRAM<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>
                memory(simulator, sim::engine::Config(conf, "DRAM"), NULL);
    #endif  // ENABLE_DRAMSIM

    // Connect the big cores.
    for (unsigned i = 0; i < big_cores; ++i) {
        mem_cpus[i]->dl1_ >> dl1_caches[i]->in_[0];
        dl1_caches[i]->out_ >> l2_caches[i]->in_[0];
        l2_caches[i]->out_ >> l2_bus.input(0);
    }
    // Connect the little cores.
    for (unsigned i = 0; i < little_cores; ++i) {
        mem_cpus[big_cores + i]->dl1_ >> little_caches[i]->in_[0];
        little_caches[i]->out_ >> l2_bus.input(0);
    }

    l2_bus.output(0) >> memory.in_;

    simulator.run();

    for (unsigned i = 0; i < tcpus; ++i) {
        delete burst_cpus[i];
        delete mem_cpus[i];
    }

    for (unsigned i = 0; i < big_cores; ++i) {
        delete dl1_caches[i];
        delete l2_caches[i];
    }

    for (unsigned i = 0; i < little_cores; ++i) {
        delete little_caches[i];
    }
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_SIMULATOR_SETUP);
    return 0;
}

void usage(const std::string& cmd) {
    std::cerr << "Usage: " << cmd << " config trace [tid]" << std::endl;
    std::cerr << "\tconfig\tConfiguration file name" << std::endl;
    std::cerr << "\ttrace\tTrace file prefix" << std::endl;
    std::cerr << "\ttid\tTask ID where to start simulation, optional, default: 1" << std::endl;
}

bool parse_options(int argc, char* argv[], Options& opt) {
    if (argc != 3 && argc != 4) return false;
    opt.cfg   = argv[1];
    opt.trace = argv[2];

    // Check that input trace exists.
    sim::utils::check_trace_exists(opt.trace);

    if (argc == 4) {
        opt.tid = std::atoi(argv[3]);
    } else {
        opt.tid = 1;
    }
    return true;
}
