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
#include "simulators/tasksim/runtime/OmpSs.h"
#include "simulators/tasksim/runtime/ModeSelector.h"
#include "simulators/tasksim/runtime/types.h"

#ifdef ENABLE_DRAMSIM   // ENABLE_DRAMSIM
#include "core/modules/memory/Dramsim2Controller.h"

#elif ENABLE_RAMULATOR  // ENABLE_RAMULATOR
#include "core/modules/memory/RamulatorController.h"
#endif

typedef struct {
    std::string trace;
    int tid;
} Options;

void usage(const std::string&);
sim::engine::Config parse_options(int, char*[], Options&);

int main(int argc, char *argv[])
{

    sim::stats::Time_Stats.phaseIn(sim::engine::REGION_SIMULATOR_SETUP);
    Options opt;
    sim::engine::Config conf = parse_options(argc, argv, opt);
    sim::engine::Simulator simulator(conf);

    std::vector<unsigned> num_cpus = conf.get_values<unsigned>("ncpus");
    unsigned private_cache_levels = conf.key_exists("private_cache_levels") ? conf.get_value<unsigned>("private_cache_levels") : 2;
    unsigned shared_cache_levels = conf.key_exists("shared_cache_levels") ? conf.get_value<unsigned>("shared_cache_levels") : 1;
    unsigned total_cpus = 0;
    for (unsigned c : num_cpus) {
        total_cpus += c;
    }
    bool enable_bus = total_cpus > 1;
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

    // Instantiate simulator runtime class
    sim::runtime::OmpSs ompss(simulator, conf, opt.trace, tcpus*1 /*threads_per_cpu != 1 not supported yet*/, opt.tid);
    simulator.set_abnormal_termination_function(std::bind(&sim::runtime::OmpSs::print_finish_message, &ompss));

    // Connect runtime burst buffer to burst cpus and mitem buffer to memory cpu
    for (unsigned i = 0; i < tcpus; i++) {
        ompss[i].burst_ >> *burst_cpus[i];
        ompss[i].mem_ >> *mem_cpus[i];
    }

    // Instantiate private L1 and L2 caches
    std::vector<std::vector<sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>*>> private_caches(private_cache_levels);
    std::vector<sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>*> shared_caches;
    for (unsigned i = 0; i < private_cache_levels; i++) {
        std::string level_name = (i == 0) ? "DL1Cache" : "L" + std::to_string(i + 1) + "Cache";
        for (unsigned j = 0; j < mem_cpus.size(); j++) {
            private_caches[i].push_back(new sim::memory::Cache<sim::logic::coherence::single::Message,
                sim::logic::coherence::single::Controller,
                sim::interconnect::Direct>(simulator, directory,
                sim::engine::Config(conf, level_name), j));
        }
    }

    for (unsigned i = 0; i < shared_cache_levels; i++) {
        std::string level_name = "L" + std::to_string(i + private_cache_levels + 1) + "Cache";
        shared_caches.push_back(new sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>(simulator, directory,
            sim::engine::Config(conf, level_name), i));
    }

    // Instantiate DRAM or perfect memory model
    #ifdef ENABLE_DRAMSIM
        sim::memory::Dramsim2Controller<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>
                *memory = new sim::memory::Dramsim2Controller<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "DRAM"), &mmu);
    #elif ENABLE_RAMULATOR  // ENABLE_RAMULATOR
        sim::memory::RamulatorController<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>
                *memory = new sim::memory::RamulatorController<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "RAMULATOR"),
                        sim::engine::Config(conf, "DL1Cache"), tcpus, &mmu);
    #elif 1
        sim::memory::Perfect<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>
                *memory = new sim::memory::Perfect<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "Memory"));
    #else
        sim::memory::DRAM<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>
                *memory = new sim::memory::DRAM<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "DRAM"), &mmu);
    #endif  // ENABLE_DRAMSIM

    // Instantiate L3 bus
    sim::interconnect::Bus<sim::logic::coherence::single::Message,
                    sim::interconnect::Direct> *cache_bus;

    if (enable_bus) {
        cache_bus = new sim::interconnect::Bus<sim::logic::coherence::single::Message,
                    sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "CacheBus"));
    }


    // Connect models of memory hierarchy
    for (unsigned i = 0; i < mem_cpus.size(); ++i) {
        if (private_cache_levels) {
            mem_cpus[i]->dl1_ >> private_caches[0][i]->in_[0];
            if (enable_bus) {
                private_caches[private_cache_levels - 1][i]->out_ >> cache_bus->input(0);
            } else if (shared_cache_levels) {
                private_caches[private_cache_levels - 1][i]->out_ >> shared_caches[0]->in_[i];
            } else {
                private_caches[private_cache_levels - 1][i]->out_ >> memory->in_;
            }
        } else {
            if (enable_bus) {
                mem_cpus[i]->dl1_ >> cache_bus->input(0);
            } else if (shared_cache_levels) {
                mem_cpus[i]->dl1_ >> shared_caches[0]->in_[i];
            } else {
                mem_cpus[i]->dl1_ >> memory->in_;
            }
        }
    }
    for (unsigned i = 1; i < private_cache_levels; i++) {
        for (unsigned j = 0; j < mem_cpus.size(); j++) {
            private_caches[i - 1][j]->out_ >> private_caches[i][j]->in_[0];
        }
    }
    if (enable_bus) {
        if (shared_cache_levels) {
            cache_bus->output(0) >> shared_caches[0]->in_[0];
        } else {
            cache_bus->output(0) >> memory->in_;
        }
    }

    for (unsigned i = 1; i < shared_cache_levels; i++) {
        shared_caches[i - 1]->out_ >> shared_caches[i]->in_[0];
    }
    if (shared_cache_levels) {
        shared_caches[shared_cache_levels - 1]->out_ >> memory->in_;
    }
    // We check that CPU request-size is the same as L1 Cache line size.
    if (conf.get_value<unsigned>("MemCPU", "request-size") !=
            conf.get_value<unsigned>("DL1Cache", "line-size")) {
        std::cerr << "[ERROR] MemCPU.request-size != DL1Cache.line-size: "
                  << conf.get_value<unsigned>("MemCPU", "request-size") << " != "
                  << conf.get_value<unsigned>("DL1Cache", "line-size") << std::endl;
        exit(1);
    }

    simulator.run();

    for (unsigned i = 0; i < total_cpus; ++i) {
        delete burst_cpus[i];
        delete mem_cpus[i];
    }
    for (unsigned i = 0; i < private_cache_levels; i++) {
        for (unsigned j = 0; j < mem_cpus.size(); j++) {
            delete private_caches[i][j];
        }
    }
    if (enable_bus) {
        delete cache_bus;
    }
    for (unsigned i = 0; i < shared_cache_levels; i++) {
        delete shared_caches[i];
    }
    delete(memory);
    sim::stats::Time_Stats.phaseOut(sim::engine::REGION_SIMULATOR_SETUP);
    return 0;
}

void version(const std::string& cmd)
{
    std::cout << PACKAGE_NAME << ' ' << PACKAGE_VERSION << " (" << cmd << ") compiled with";
    // mirrors src/core/trace/types.h
#if TESTING || USE_COMPRESSED_TRACES
    std::cout << " compressed";
#else
    std::cout << " uncompressed";
#endif
#if !TESTING && USE_TEXT_EVENT_STREAM
    std::cout << " text traces";
#else
    std::cout << " binary traces";
#endif
#if USE_RLE_TRACES || TESTING_VERSION
    std::cout << ", in RLE format." << std::endl;
#elif USE_UOP_TRACES
    std::cout << ", in UOP format." << std::endl;
#else  // USE_MEM_TRACES is fallback
    std::cout << ", in MEM format." << std::endl;
#endif

#if ENABLE_DRAMSIM
    std::cout << "DRAM uses Dramsim2 integration" << std::endl;
#elif ENABLE_RAMULATOR
    std::cout << "DRAM uses Ramulator integration" << std::endl;
#elif 1
    std::cout << "DRAM uses \"Perfect\" module" << std::endl;
#else
    std::cout << "DRAM uses \"DRAM\" module" << std::endl;
#endif

#if ENABLE_DEBUG
    std::cout << "Debug mode" << std::endl;
#else
    std::cout << "Performance mode (-O3)" << std::endl;
#endif

#if ENABLE_PARAVER
    std::cout << "With Paraver trace generation." << std::endl;
#else
    std::cout << "Without Paraver trace generation." << std::endl;
#endif

#if ENABLE_MITOS
    std::cout << "With Mitos trace generation." << std::endl;
#else
    std::cout << "Without Mitos trace generation." << std::endl;
#endif


#ifdef SIMLOG_LEVEL
    std::cout << "Compiled with System logging infrastructure at level " << SIMLOG_LEVEL << std::endl;
#else  // SIMLOG_LEVEL
    std::cout << "Compiled with System logging infrastructure without a set level" << std::endl;
#endif  // SIMLOG_LEVEL
    std::cout << '\n' << PACKAGE_COPYRIGHT << "\nPlease report bugs to " << PACKAGE_BUGREPORT << std::endl;
}

void usage(const std::string& cmd)
{
    std::cerr << "Usage: " << cmd << " [option] config trace [tid]" << std::endl;
    std::cerr << "\tconfig\tConfiguration file name" << std::endl;
    std::cerr << "\ttrace\tTrace file prefix" << std::endl;
    std::cerr << "\ttid\tTask ID where to start simulation, optional, default: 1" << std::endl;
    std::cerr << "\noptions:" << std::endl;
    std::cerr << "\t-h, --help        Print this help and exit" << std::endl;
    std::cerr << "\t-V, --version     Print version information and exit" << std::endl;
    std::cerr << "\t-c, --config=VAL  Override configuration file option, formatted as section.key=value" << std::endl;
}

sim::engine::Config parse_options(int argc, char* argv[], Options& opt)
{
    std::unordered_map<std::string, std::string> option_overrides;
    const char short_options[] = "hVc:";
    const option long_options[] = {
        {"help",    0, nullptr, 'h'},
        {"version", 0, nullptr, 'V'},
        {"config",  1, nullptr, 'c'}
    };

    for (int opt = 0; opt != -1; opt = getopt_long(argc, argv, short_options, long_options, nullptr)) {
        switch (opt) {
            case 'V':
                version(argv[0]);
                exit(0);
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
                break;
            case 'c':
                char *end = optarg + strlen(optarg);
                char *sep = std::find(optarg, end, '=');
                if (sep == end) {
                    std::cerr << "Error: Wrong argument for -c \"" << optarg << "\", "
                              << "can not find = to separate key and value" << std::endl;
                    exit(-1);
                }

                // strip whitespace around '='
                auto find_whitespace = [](int c) -> bool { return std::isspace(c); };
                std::string val(std::find_if_not(sep + 1, end, find_whitespace), end);
                std::string key(optarg, std::find_if_not(std::reverse_iterator<char*>(sep),
                                                         std::reverse_iterator<char*>(optarg),
                                                         find_whitespace).base());

                option_overrides[std::move(key)] = std::move(val);
                break;
        }
    }

    if (argc - optind < 2 or argc - optind > 3) {
        usage(argv[0]);
        exit(-1);
    }

    sim::engine::Config conf(argv[optind++]);
    opt.trace = argv[optind++];
    opt.tid = optind < argc ? std::atoi(argv[optind++]) : 1;

    // Check that input trace exists.
    sim::utils::check_trace_exists(opt.trace);

    for (const auto &keyval : option_overrides) {
        size_t section_sep = keyval.first.find(".");
        std::string sec = keyval.first.substr(0, section_sep);
        std::string key = keyval.first.substr(section_sep + 1);

        if (section_sep == std::string::npos) {
            conf.set_values(keyval.second, key);
        } else {
            conf.set_values(keyval.second, key, sec);
        }
    }

    return conf;
}
