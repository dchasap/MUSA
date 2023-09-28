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

#include <string>

#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Controller.h"
#include "core/logic/coherence/single/Directory.h"
#include "core/logic/coherence/single/Message.h"
#include "core/modules/cpu/Memory.h"
#include "core/modules/cpu/models/ROA.h"
#include "core/modules/interconnect/Direct.h"
#include "core/modules/interconnect/Bus.h"
#include "core/modules/memory/Cache.h"
#include "core/modules/memory/DMA.h"
#include "core/modules/memory/DRAM.h"
#include "core/modules/memory/Perfect.h"
#include "core/modules/memory/TLB.h"
#include "simulators/examples/memsim/runtime/SimpleMemory.h"

typedef struct {
    std::string cfg;
    std::string trace;
} Options;

void usage(const std::string&);
bool parse_options(int, char*[], Options&);

int main(int argc, char *argv[])
{
    Options opt;
    if (parse_options(argc, argv, opt) == false) {
        usage(argv[0]);
        exit(0);
    }

    sim::engine::Config conf(opt.cfg);
    sim::engine::Simulator simulator(conf);

    // Instantiate the coherence directory
    sim::logic::coherence::single::Directory<sim::logic::coherence::single::Message> directory(1, 3);

    sim::logic::memory::MMU mmu(&simulator, sim::engine::Config(conf, "MMU"), sim::engine::Config(conf, "L2Cache"));
    sim::engine::Config mmu_config(sim::engine::Config(conf, "MMU"));

    sim::runtime::SimpleMemory runtime(simulator, opt.trace);

    // sim::memory::TLB<sim::logic::coherence::single::Message, sim::interconnect::Direct> tlb(simulator,
    //         sim::engine::Config(conf, "TLB"), mmu, 0);

    sim::memory::Cache<sim::logic::coherence::single::Message,
                  sim::logic::coherence::single::Controller,
                  sim::interconnect::Direct>
            dl1_cache(simulator, directory, sim::engine::Config(conf, "DL1Cache"), 0);

    sim::memory::Cache<sim::logic::coherence::single::Message,
                  sim::logic::coherence::single::Controller,
                  sim::interconnect::Direct>
            l2_cache(simulator, directory, sim::engine::Config(conf, "L2Cache"), 0);

    sim::interconnect::Bus<sim::logic::coherence::single::Message,
                           sim::interconnect::Direct> l2_bus(simulator,
            sim::engine::Config(conf, "L2Bus"));

#if 1
    sim::memory::Perfect<sim::logic::coherence::single::Message,
                    sim::interconnect::Direct>
        memory(simulator, sim::engine::Config(conf, "Memory"));
#else
    sim::memory::DRAM<sim::logic::coherence::single::Message,
                    sim::interconnect::Direct>
        memory(simulator, sim::engine::Config(conf, "DRAM"), &mmu);
#endif

    sim::cpu::Memory<sim::cpu::timing_models::ROA, sim::logic::coherence::single::Message>
        cpu(simulator, sim::engine::Config(conf, "CPU"));

    // sim::memory::TLB<sim::logic::coherence::single::Message, sim::interconnect::Direct>* tlb; // one per CPU

    // cpu.dl1_ >> tlb.in_;
    // tlb.out_ >> dl1_cache.in_[0];
    cpu.dl1_ >> dl1_cache.in_[0];

    // tlb.out_.set_wake_up_on_read(false);

    dl1_cache.out_ >> l2_cache.in_[0];

    l2_cache.out_ >> l2_bus.input(0);
    l2_bus.output(0) >> memory.in_;

//    cpu.dl1_ >> memory.in_;


    // For now only we consider only memory accesses
    // runtime[0].instr_ >> cpu;
    runtime[0].mem_ >> cpu;
    std::cout << simulator.get_clock() << ": before simulator.run()" << std::endl;
    simulator.run();
    std::cout << simulator.get_clock() << ": after simulator.run()" << std::endl;
    return 0;
}

void usage(const std::string& cmd) {
    std::cerr << "Usage: " << cmd << " config trace" << std::endl;
    std::cerr << "\tconfig\tConfiguration file name" << std::endl;
    std::cerr << "\ttrace\tTrace file prefix" << std::endl;
}

bool parse_options(int argc, char* argv[], Options& opt) {
    if (argc < 3) return false;
    opt.cfg = argv[1];
    opt.trace = argv[2];
    return true;
}
