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


#ifndef CORE_ENGINE_TYPES_H_
#define CORE_ENGINE_TYPES_H_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace sim {
namespace engine {

typedef uint64_t cycles_t;
const cycles_t NEVER = -1;
const cycles_t NEXT = 1;

typedef uint64_t addr_t;
struct mem_t {
    typedef enum { INVALID, READ, WRITE, READWRITE } op_t;
    op_t op_;
    addr_t addr_;
    std::size_t size_;
};

typedef enum {
    TASK_ID = 0xf800000000000000,
    TASK_END = 0xf800000000000007,
    TASK_DEP_REGION_START = 0xf800000000000008,
    TASK_DEP_DATA_SIZE = 0x8,
    TASK_DEP_ADDR = TASK_DEP_REGION_START,
    TASK_DEP_SIZE = TASK_DEP_REGION_START + TASK_DEP_DATA_SIZE,
    TASK_DEP_TYPE = TASK_DEP_REGION_START + 2 * TASK_DEP_DATA_SIZE
} special_address_t;


typedef enum {
    RUNNING,
    DRAINING,
    DRAINED,
    RESUMING
} drain_state_t;

typedef enum { UNKNOWN = 0,
               CPU = 1,
               CACHE = 2,
               NC = 4,  // Network Controller
               IN = 8,  // Interconnect
               MC = 16,  // Memory controller
               DMA = 32,
               TLB = 64,
               RAM = 128
             } ModuleType;

typedef enum { REGION_SIMULATOR = 0,
               REGION_CPU = 1,
               REGION_CACHE = 2,
               REGION_NC = 3,
               REGION_IN = 4,
               REGION_MC = 5,
               REGION_DMA = 6,
               REGION_TLB = 7,
               REGION_RAM = 8,
               REGION_TRACE_READ = 9,
               REGION_NANOS_RUNTIME = 10,
               REGION_UNKNOWN = 11,
               REGION_SIMULATOR_SETUP = 12,
               REGION_INSTRUMENTATION_TRACE = 13,
               NUMBER_OF_REGIONS = 14
} CodeRegions;

static const std::unordered_map<unsigned, std::string> region_names({
        {REGION_SIMULATOR, "Simulator"},
        {REGION_CPU, "Processor"},
        {REGION_CACHE, "Cache"},
        {REGION_NC, "Network"},
        {REGION_IN, "Interconnect"},
        {REGION_MC, "MemoryController"},
        {REGION_DMA, "DMA"},
        {REGION_TLB, "TLB"},
        {REGION_RAM, "Memory"},
        {REGION_TRACE_READ, "TreaceRead"},
        {REGION_NANOS_RUNTIME, "Runtime"},
        {REGION_UNKNOWN, "Unknown"},
        {REGION_SIMULATOR_SETUP, "Setup"},
        {REGION_INSTRUMENTATION_TRACE, "Instrumentation"}
        });

static const std::unordered_map<std::string, ModuleType> module_type_index({
    {"CPU", CPU},
    {"CACHE", CACHE},
    {"NC", NC},
    {"IN", IN},
    {"MC", MC},
    {"DMA", DMA},
    {"TLB", TLB},
    {"RAM", RAM}
});

static const std::unordered_map<ModuleType, sim::engine::CodeRegions, std::hash<int>> module_type_to_code_region({
    {UNKNOWN, REGION_UNKNOWN},
    {CPU, REGION_CPU},
    {CACHE, REGION_CACHE},
    {NC, REGION_NC},
    {IN, REGION_IN},
    {MC, REGION_MC},
    {DMA, REGION_DMA},
    {TLB, REGION_TLB},
    {RAM, REGION_RAM}
});



inline
static ModuleType get_module_type(const std::string &module_name)
{
    if (module_type_index.find(module_name) != module_type_index.end()) {
        return module_type_index.find(module_name)->second;
    } else {
        return UNKNOWN;
    }
}

inline
static CodeRegions get_code_region(const ModuleType module_type)
{
    return module_type_to_code_region.at(module_type);
}

}  // namespace engine
}  // namespace sim

#endif  // CORE_ENGINE_TYPES_H_
