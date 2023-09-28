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


#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Controller.h"
#include "core/logic/coherence/single/Directory.h"
#include "core/logic/coherence/single/Message.h"
#include "core/modules/cpu/SimpleMemory.h"
#include "core/modules/interconnect/Direct.h"
#include "core/modules/interconnect/Bus.h"
#include "core/modules/memory/Cache.h"
#include "core/modules/memory/DRAM.h"
#include "core/modules/memory/Perfect.h"
#include "simulators/examples/simplememsim/runtime/SingleInst.h"


int main(int argc, char *argv[])
{
    sim::engine::Config conf(argv[2]);
    sim::engine::Simulator simulator(conf);
    sim::runtime::SingleInst runtime(simulator, argv[1]);

    // Instantiate the coherence directory
    sim::logic::coherence::single::Directory<sim::logic::coherence::single::Message> directory(1, 3);

    sim::memory::Cache<sim::logic::coherence::single::Message,
                  sim::logic::coherence::single::Controller,
                  sim::interconnect::Direct>
            dl1_cache(simulator, directory, sim::engine::Config(conf, "DL1Cache"), 0);

    sim::memory::Cache<sim::logic::coherence::single::Message,
                  sim::logic::coherence::single::Controller,
                  sim::interconnect::Direct>
            l2_cache(simulator, directory, sim::engine::Config(conf, "L2Cache"), 0);

    sim::interconnect::Bus<sim::logic::coherence::single::Message,
                           sim::interconnect::Direct>
            l2_bus(simulator, sim::engine::Config(conf, "L2Bus"));

#if 0
    sim::memory::Perfect<sim::logic::coherence::single::Message,
                    sim::interconnect::Direct>
        memory(simulator, sim::engine::Config(conf, "Memory"));
#endif
    sim::memory::DRAM<sim::logic::coherence::single::Message,
                    sim::interconnect::Direct>
        memory(simulator, sim::engine::Config(conf, "DRAM"));


    sim::cpu::SimpleMemory<sim::logic::coherence::single::Message>
        cpu(simulator, sim::engine::Config(conf, "CPU"));

    cpu.dl1_ >> dl1_cache.in_[0];
    dl1_cache.out_ >> l2_cache.in_[0];

    l2_cache.out_ >> l2_bus.input(0);
    l2_bus.output(0) >> memory.in_;

    runtime[0].instr_ >> cpu;
    runtime[0].mem_ >> cpu;

    simulator.run();

    return 0;
}
