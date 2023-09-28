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


#include <vector>
#include <string>

#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
#include "core/modules/cpu/Burst.h"
#include "core/utils/CheckTrace.h"
#include "simulators/tasksim/runtime/OmpSs.h"
#include "simulators/tasksim/runtime/ModeSelector.h"
#include "simulators/tasksim/runtime/types.h"

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
    // FIXME: multiple threads per cpu not supported yet, assumed to be 1
    // std::vector<unsigned> threads_per_cpu = conf.get_values<unsigned>("threads_per_cpu");

    /** Instantiate CPUs */
    std::vector<sim::cpu::Burst*> cpus;
    unsigned tcpus = 0;
    /** For each cpu type */
    for (unsigned c = 0; c < num_cpus.size(); c++) {
        /** Create a set of cpus of that type */
        for (unsigned i = 0; i < num_cpus[c]; ++i) {
            cpus.push_back(new sim::cpu::Burst(simulator,
                           sim::engine::Config(conf, "Burst", c), tcpus));
            ++tcpus;
        }
    }

    // Instantiate simulator runtime class
    sim::runtime::OmpSs ompss(simulator, conf, opt.trace, tcpus*1 /*threads_per_cpu != 1 not supported yet*/, opt.tid);

    // Connect runtime burst buffer to cpus
    for (unsigned i = 0; i < tcpus; i++) {
        ompss[i].burst_ >> *cpus[i];
    }

    simulator.run();

    for (unsigned i = 0; i < tcpus; ++i) {
        delete cpus[i];
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
