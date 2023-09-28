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


#include <sstream>

#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Controller.h"
#include "core/logic/coherence/single/Directory.h"
#include "core/logic/coherence/single/Message.h"
#include "core/logic/memory/MMU.h"
#include "core/modules/interconnect/Direct.h"
#include "core/modules/memory/Cache.h"
#include "core/modules/memory/Perfect.h"
#include "core/modules/memory/TLB.h"
#include "simulators/sttmramsim/modules/cpu/STTMRAM.h"
#include "simulators/sttmramsim/runtime/STTMRAM.h"


int main(int argc, char *argv[])
{
    sim::engine::Config conf(argv[2]);
    sim::engine::Simulator simulator(conf);
    sim::logic::memory::MMU* mmu = new sim::logic::memory::MMU(&simulator,
            sim::engine::Config(conf, "MMU"), sim::engine::Config(conf, "L3Cache"));
    sim::runtime::STTMRAM runtime(simulator, argv[1]);

    int number_of_cpus = conf.get_value<unsigned>("cpu-count");

    // Instantiate the coherence directory
    sim::logic::coherence::single::Directory<sim::logic::coherence::single::Message> directory(number_of_cpus, 3);

    sim::cpu::STTMRAM<sim::logic::coherence::single::Message, sim::runtime::STTMRAM>* cpu[number_of_cpus];
    sim::memory::TLB<sim::logic::coherence::single::Message, sim::interconnect::Direct>* tlb[number_of_cpus];
    sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>* l1_cache[number_of_cpus];
    sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct>* l2_cache[number_of_cpus];

    sim::memory::Cache<sim::logic::coherence::single::Message,
            sim::logic::coherence::single::Controller,
            sim::interconnect::Direct> l3_cache(simulator, directory, sim::engine::Config(conf, "L3Cache"), 0);
    sim::memory::Perfect<sim::logic::coherence::single::Message, sim::interconnect::Direct> memory(simulator,
            sim::engine::Config(conf, "Memory"));
    l3_cache.out_ >> memory.in_;
    l3_cache.out_.set_wake_up_on_read(false);

    for (int i = 0; i < number_of_cpus; i++) {
        cpu[i] = new sim::cpu::STTMRAM<sim::logic::coherence::single::Message, sim::runtime::STTMRAM>(simulator,
                sim::engine::Config(conf, "CPU"));
        l1_cache[i] = new sim::memory::Cache<sim::logic::coherence::single::Message,
                sim::logic::coherence::single::Controller,
                sim::interconnect::Direct>(simulator, directory, sim::engine::Config(conf, "L1Cache"), i);
        tlb[i] = new sim::memory::TLB<sim::logic::coherence::single::Message,
                sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "TLB"), *mmu, i);
        l2_cache[i] = new sim::memory::Cache<sim::logic::coherence::single::Message,
                sim::logic::coherence::single::Controller,
                sim::interconnect::Direct>(simulator, directory, sim::engine::Config(conf, "L2Cache"), i);

        cpu[i]->dl1_ >> l1_cache[i]->in_[0];
        cpu[i]->dl1_.set_wake_up_on_read(false);
        l1_cache[i]->out_ >> tlb[i]->in_;
        l1_cache[i]->out_.set_wake_up_on_read(false);
        tlb[i]->out_ >> l2_cache[i]->in_[0];
        tlb[i]->out_.set_wake_up_on_read(false);
        l2_cache[i]->out_ >> l3_cache.in_[i];
        l2_cache[i]->out_.set_wake_up_on_read(false);

        runtime[i].mem_ >> *cpu[i];
    }

    simulator.run();

    for (int i = 0; i < number_of_cpus; i++) {
        delete cpu[i];
        delete l1_cache[i];
        delete tlb[i];
        delete l2_cache[i];
    }

    delete mmu;

    return 0;
}
