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
#include <sstream>

#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Controller.h"
#include "core/logic/coherence/single/Directory.h"
#include "core/logic/coherence/single/Message.h"
#include "core/logic/memory/MMU.h"
#include "core/modules/interconnect/Direct.h"
#include "core/modules/interconnect/Bus.h"
#include "core/modules/memory/Cache.h"
#include "core/modules/memory/DRAM.h"
#include "core/modules/memory/DMA.h"
#include "core/modules/memory/TLB.h"
#include "simulators/fastmemsim/runtime/FastMem.h"
#include "simulators/fastmemsim/modules/cpu/FastMem.h"


int main(int argc, char *argv[])
{
    sim::engine::Config conf(argv[2]);
    sim::engine::Simulator simulator(conf);
    sim::logic::memory::MMU* mmu = new sim::logic::memory::MMU(&simulator, sim::engine::Config(conf, "MMU"),
            sim::engine::Config(conf, "L2Cache"));
    sim::runtime::FastMem runtime(simulator, argv[1]);

    sim::engine::Config mmu_config(sim::engine::Config(conf, "MMU"));

    int number_of_drams = conf.get_value<unsigned>("dram-count");
    int number_of_cpus = conf.get_value<unsigned>("cpu-count");

    // Instantiate the coherence directory
    sim::logic::coherence::single::Directory<sim::logic::coherence::single::Message> directory(number_of_cpus, 3);

    sim::interconnect::Bus<sim::logic::coherence::single::Message,
            sim::interconnect::Direct>* l2_bus;
    sim::memory::DRAM<sim::logic::coherence::single::Message, sim::interconnect::Direct>* memory[number_of_drams];
    sim::cpu::FastMem<sim::logic::coherence::single::Message, sim::runtime::FastMem>* cpu[number_of_cpus];
    sim::memory::TLB<sim::logic::coherence::single::Message, sim::interconnect::Direct>* tlb[number_of_cpus];
    sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>* dl1_cache[number_of_cpus];
    sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>* l2_cache[number_of_cpus];

    l2_bus = new sim::interconnect::Bus<sim::logic::coherence::single::Message,
                       sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "L2Bus"));

    sim::memory::DMA<sim::logic::coherence::single::Message, sim::interconnect::Direct>* dma =
            new sim::memory::DMA<sim::logic::coherence::single::Message,
            sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "DMA"), *mmu);
    dma->out_ >> l2_bus->input(0);

    for (int i = 0; i < number_of_drams; i++) {
        std::stringstream ss;
        ss << i;
        memory[i] = new sim::memory::DRAM<sim::logic::coherence::single::Message, sim::interconnect::Direct>(simulator,
                sim::engine::Config(conf, "DRAM-" + ss.str()), mmu);
        typename sim::interconnect::Bus<sim::logic::coherence::single::Message,
                             sim::interconnect::Direct>::Output &bus_output = l2_bus->output(0);
        bus_output >> memory[i]->in_;
        bus_output.set_wake_up_on_read(false);
    }

    for (int i = 0; i < number_of_cpus; i++) {
        cpu[i] = new sim::cpu::FastMem<sim::logic::coherence::single::Message, sim::runtime::FastMem>(simulator,
                sim::engine::Config(conf, "CPU"));
        tlb[i] = new sim::memory::TLB<sim::logic::coherence::single::Message, sim::interconnect::Direct>(simulator,
                sim::engine::Config(conf, "TLB"), *mmu, i);
        dl1_cache[i] = new sim::memory::Cache<sim::logic::coherence::single::Message,
                sim::logic::coherence::single::Controller,
                sim::interconnect::Direct>(simulator, directory, sim::engine::Config(conf, "DL1Cache"), i);
        l2_cache[i] = new sim::memory::Cache<sim::logic::coherence::single::Message,
                sim::logic::coherence::single::Controller,
                sim::interconnect::Direct>(simulator, directory,
                sim::engine::Config(conf, "L2Cache"), i);

        cpu[i]->dl1_ >> tlb[i]->in_;

        tlb[i]->out_ >> dl1_cache[i]->in_[0];
        tlb[i]->out_.set_wake_up_on_read(false);

        dl1_cache[i]->out_ >> l2_cache[i]->in_[0];
        l2_cache[i]->out_ >> l2_bus->input(0);

        runtime[i].mem_ >> *cpu[i];
    }

    mmu->load_access_profile();
    // sim::engine::addr_t clear_page_address = 1024 * 1024;
    // tlb[0]->clear_page(clear_page_address);
    // dma.memcpy(0, 1024*1024, 8192);
    simulator.run();

    if (mmu_config.key_exists("output-access-profile")) {
        mmu->dump_access_table(mmu_config.get_value<std::string>("output-access-profile"));
    }

    for (int i = 0; i < number_of_cpus; i++) {
        delete cpu[i];
        delete tlb[i];
        delete dl1_cache[i];
        delete l2_cache[i];
    }

    delete l2_bus;

    for (int i = 0; i < number_of_drams; i++) {
        delete memory[i];
    }

    delete dma;
    delete mmu;

    return 0;
}
