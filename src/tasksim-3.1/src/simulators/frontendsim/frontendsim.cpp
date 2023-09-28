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


#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
// #include "core/logic/coherence/single/Controller.h"
#include "core/logic/coherence/single/Directory.h"
#include "core/logic/coherence/single/Message.h"
#include "core/logic/memory/MMU.h"
#include "core/modules/interconnect/Direct.h"
#include "core/modules/interconnect/Bus.h"
#include "core/modules/memory/Cache.h"
#include "core/modules/memory/DRAM.h"
#include "core/modules/memory/TLB.h"
#include "simulators/frontendsim/logic/coherence/single/TimeController.h"
#include "simulators/frontendsim/modules/cpu/ICore.h"
#include "simulators/frontendsim/runtime/FrontEnd.h"

int main(int argc, char *argv[])
{
    sim::engine::Config conf(argv[2]);
    sim::engine::Simulator simulator(conf);
    sim::logic::memory::MMU* mmu = new sim::logic::memory::MMU(&simulator,
            sim::engine::Config(conf, "MMU"), sim::engine::Config(conf, "L2Cache"));
    sim::runtime::FrontEnd runtime(simulator, argv[1]);
    sim::engine::Config mmu_config(sim::engine::Config(conf, "MMU"));

    std::string topology = conf.get_value<std::string>("topology");
    unsigned number_of_drams = conf.get_value<unsigned>("dram-count");
    unsigned cpc = 0;
    std::vector<unsigned> num_cpus = conf.get_values<unsigned>("ncpus");

    unsigned total_cpus = 0;
    for (unsigned i = 0; i < num_cpus.size(); i++)
        total_cpus += num_cpus[i];

    unsigned num_of_seg = 0;
    if (topology.compare("bus") == 0) {
        cpc = conf.get_value<unsigned>("cpc");
        num_of_seg = total_cpus / cpc;
        if (total_cpus % cpc != 0) {
            std::cout << "ERROR :: Number of cores and number of cores per cache have to correspond!\n";
            return 0;
        }
    }

    // Instantiate the coherence directory
    sim::logic::coherence::single::Directory<sim::logic::coherence::single::Message> directory(tcpus, 3);

    sim::interconnect::Bus<sim::logic::coherence::single::TimeMessage,
            sim::interconnect::Direct>* l2_dram_bus;
    sim::memory::DRAM<sim::logic::coherence::single::TimeMessage,
            sim::interconnect::Direct>* memory[number_of_drams];
    sim::cpu::ICore<sim::logic::coherence::single::TimeMessage,
            sim::runtime::FrontEnd, sim::interconnect::Direct>* cpu[total_cpus];

    sim::memory::Cache<sim::logic::coherence::single::TimeMessage,
            sim::logic::coherence::single::TimeController,
            sim::interconnect::Direct>* il1_cache[num_of_seg > 0 ? num_of_seg : total_cpus];
    sim::memory::Cache<sim::logic::coherence::single::TimeMessage,
            sim::logic::coherence::single::TimeController,
            sim::interconnect::Direct>* l2_cache[num_of_seg > 0 ? num_of_seg : total_cpus];

    sim::interconnect::Bus<sim::logic::coherence::single::TimeMessage,
            sim::interconnect::Direct>* icache_bus[num_of_seg];
    l2_dram_bus = new sim::interconnect::Bus<sim::logic::coherence::single::TimeMessage,
                sim::interconnect::Direct>(simulator,
            sim::engine::Config(conf, "L2Bus"));

    for (unsigned i = 0; i < number_of_drams; i++) {
        memory[i] = new sim::memory::DRAM<sim::logic::coherence::single::TimeMessage,
                sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "DRAM"), mmu);
        typename sim::interconnect::Direct<sim::logic::coherence::single::TimeMessage>::Output &bus_output =
                l2_dram_bus->output(0);
        bus_output >> memory[i]->in_;
        bus_output.set_wake_up_on_read(false);
    }

    if (num_of_seg > 0) {
        for (unsigned i = 0; i < num_of_seg; i++) {
            il1_cache[i] = new sim::memory::Cache<sim::logic::coherence::single::TimeMessage,
                    sim::logic::coherence::single::TimeController,
                    sim::interconnect::Direct>(simulator, directory,
                    sim::engine::Config(conf, "IL1Cache"), i);
            l2_cache[i] = new sim::memory::Cache<sim::logic::coherence::single::TimeMessage,
                    sim::logic::coherence::single::TimeController,
                    sim::interconnect::Direct>(simulator, directory,
                    sim::engine::Config(conf, "L2Cache"), i);
            icache_bus[i] = new sim::interconnect::Bus<sim::logic::coherence::single::TimeMessage,
                    sim::interconnect::Direct>(simulator,
                    sim::engine::Config(conf, "ICacheBus"));
            icache_bus[i]->output(0) >> il1_cache[i]->in_[0];
            il1_cache[i]->out_ >> l2_cache[i]->in_[0];
            l2_cache[i]->out_ >> l2_dram_bus->input(0);
        }
    }

    // For each cpu type
    total_cpus = 0;
    for (unsigned c = 0; c < num_cpus.size(); c++) {
        // Create a set of cpus of that type
        for (unsigned i = 0; i < num_cpus[c]; i++) {
            cpu[total_cpus+i] = new sim::cpu::ICore<sim::logic::coherence::single::TimeMessage,
                    sim::runtime::FrontEnd, sim::interconnect::Direct>(simulator, sim::engine::Config(conf, "CPU", c), total_cpus+i);
            if (num_of_seg > 0) {
                cpu[total_cpus+i]->il1_ >> icache_bus[(total_cpus + i) / cpc]->input(0);
            } else {
                il1_cache[total_cpus+i] = new sim::memory::Cache<sim::logic::coherence::single::TimeMessage,
                        sim::logic::coherence::single::TimeController,
                        sim::interconnect::Direct>(simulator, directorym
                        sim::engine::Config(conf, "IL1Cache"), total_cpus + i);
                l2_cache[total_cpus+i] = new sim::memory::Cache<sim::logic::coherence::single::TimeMessage,
                        sim::logic::coherence::single::TimeController,
                        sim::interconnect::Direct>(simulator, directory,
                        sim::engine::Config(conf, "L2Cache"), total_cpus + i);
                cpu[total_cpus+i]->il1_ >> il1_cache[total_cpus+i]->in_[0];
                il1_cache[total_cpus+i]->out_ >> l2_cache[total_cpus+i]->in_[0];
                l2_cache[total_cpus+i]->out_ >> l2_dram_bus->input(0);
            }
            cpu[total_cpus+i]->il1_.set_wake_up_on_read(false);
            runtime[total_cpus+i].bbl_ >> *cpu[total_cpus+i];
        }
        total_cpus += num_cpus[c];
    }

    mmu->load_access_profile();
    simulator.run();

    if (mmu_config.key_exists("output-access-profile")) {
        mmu->dump_access_table(mmu_config.get_value<std::string>("output-access-profile"));
    }

    // time to write statistic as we want
    std::cout << "ICORES\n";
    std::cout << "id\t#fb\t#bbl\t#instr\tend_c\twait_c\tmax_ipc\tavg_fetch_rate\t";
    std::cout << "ib_size\tavg_ib_size\tdec_rate\tfetch_pl\t";
    std::cout << "iq_size\tavg_iq_size\tavg_com_rate\tipc_bubble\t#il1_access_s\tuseful_bubble\t";
    std::cout << "il1_hit\til1_miss\tl2_hit\tl2_miss\til1_bus_lat\til1_cache_lat\t";
    std::cout << "l2_cache_lat\tl2_dram_bus_lat\tdram_lat\tlb_hit\tavg_fb_size\tavg_bbl_size\tavg_line_util\n";
    for (unsigned i = 0; i < total_cpus; i++)
        delete cpu[i];
    std::cout << "\n";
    std::cout << "\nI_CACHE\n";
    std::cout << "id\tlat\t#hit\t#excl_hit\t#shared_hit\t#miss\t#comp_miss\t#non_comp_miss\n";
    for (unsigned i = 0; i < (num_of_seg > 0 ? num_of_seg : total_cpus); i++) {
        std::cout << i << '\t'; delete il1_cache[i];
    }
    std::cout << "\n";
    std::cout << "\nL2_CACHE\n";
    std::cout << "id\tlat\t#hit\t#excl_hit\t#shared_hit\t#miss\t#comp_miss\t#non_comp_miss\n";
    for (unsigned i = 0; i < (num_of_seg > 0 ? num_of_seg : total_cpus); i++) {
        std::cout << i << '\t'; delete l2_cache[i];
    }

    delete l2_dram_bus;

    std::cout << "\nDRAM\n";
    std::cout << "id\ta_time\tread\twrite\tprecharge\topen\n";
    for (unsigned i = 0; i < number_of_drams; i++) {
        std::cout << i << "\t"; delete memory[i];
    }

    if (num_of_seg > 0) {
        for (unsigned i = 0; i < num_of_seg; i++)
            delete icache_bus[i];
    }

    delete mmu;
    return 0;
}




