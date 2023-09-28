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

#ifndef CORE_UTILS_INSTRUMENTATION_PARAVERDEFS_H_
#define CORE_UTILS_INSTRUMENTATION_PARAVERDEFS_H_


namespace sim {
namespace utils {
namespace instrumentation {

enum FUNCTION_NAME { END = 0,
                     UNRESOLVED = 1,
                     NOT_FOUND = 2,
                     RESERVED = 3,
                     TYPE = 9200011 };

/** Do NOT Add any new event in the 42000000 - 42000100 range:
 *  they are reserved for PAPI counters.
 */
enum HARDWARE_EVENTS { PAPI_L1_DCM = 42000000,  // Cache L1 data misses
                       PAPI_L1_ICM = 42000001,  // Cache L1 instr, misses
                       PAPI_L2_DCM = 42000002,  // Cache L2 data misses
                       PAPI_L2_ICM = 42000003,  // Cache L2 instr.misses
                       PAPI_L3_DCM = 42000004,  // Cache L3 data misses
                       PAPI_L3_ICM = 42000005,  // Cache L3 instr. misses
                       PAPI_L1_TCM = 42000006,  // Cache L1 total misses
                       PAPI_L2_TCM = 42000007,  // Cache L2 total misses
                       PAPI_L3_TCM = 42000008,  // Cache L3 total misses

                       PAPI_L3_LDM = 42000014,  // Cache L2 total misses
                       PAPI_L3_STM = 42000015,  // Cache L3 total misses

                       PAPI_L1_LDM = 42000023,  // Cache L1 load misses
                       PAPI_L1_STM = 42000024,  // Cache L1 store misses
                       PAPI_L2_LDM = 42000025,  // Cache L2 load misses
                       PAPI_L2_STM = 42000026,  // Cache L2 store misses

                       PAPI_L3_DCH = 42000029,  // Cache L3 data hits

                       PAPI_MEM_SCY = 42000034,  // Total number of stalled cycles due to memory access
                       PAPI_MEM_RCY = 42000035,  // Total number of stalled cycles due to memory reads
                       PAPI_MEM_WCY = 42000036,  // Total number of stalled cycles due to memory writes
                       PAPI_STL_ICY = 42000037,  // Cycles with no instruction issued

                       PAPI_TOT_IIS = 42000049,  // Total number of instruction issued
                       PAPI_TOT_INS = 42000050,  // Total number of instructions completed

                       PAPI_LD_INS  = 42000053,  // Total number of load instructions (issued or completed?)
                       PAPI_ST_INS  = 42000054,  // Total number of store instructions (issued or completed?)
                       PAPI_BR_INS  = 42000055,  // Total number of branch instructions (issued or completed?)

                       PAPI_RES_STL = 42000057,  // Cycles stalled on any resource

                       PAPI_TOT_CYC = 42000059,  // Total cycles

                       PAPI_L1_DCH  = 42000062,  // Cache L1 data hits
                       PAPI_L2_DCH  = 42000063,  // Cache L2 data hits

                       PAPI_L1_TCH  = 42000085,  // Cache L1 total hits
                       PAPI_L2_TCH  = 42000086,  // Cache L2 total hits
                       PAPI_L3_TCH  = 42000087,  // Cache L3 total hits
                       PAPI_L1_TCA  = 42000088,  // Cache L1 total accesses
                       PAPI_L2_TCA  = 42000089,  // Cache L2 total accesses
                       PAPI_L3_TCA  = 42000090,  // Cache L3 total accesses
                       PAPI_L1_TCR  = 42000091,  // Cache L1 total reads
                       PAPI_L2_TCR  = 42000092,  // Cache L2 total reads
                       PAPI_L3_TCR  = 42000093,  // Cache L3 total reads
                       PAPI_L1_TCW  = 42000094,  // Cache L1 total writes
                       PAPI_L2_TCW  = 42000095,  // Cache L2 total writes
                       PAPI_L3_TCW  = 42000096,  // Cache L3 total writes

                       DRAM_LATENCY = 42001000,  // RAM total latency
                       DRAM_COUNT   = 42001001,  // RAM number of operations
                       DRAM_OCCUPANCY = 42001002,  // RAM average occupancy
                       DRAM_BLOCKED_CYCLES = 42001003,  // RAM Requests blocked cycles

                       CPU_LD_FIN = 42002000,
                       CPU_ST_FIN = 42002001,
                       CPU_LD_ATIME = 42002002,
                       CPU_ST_ATIME = 42002003,
                       CPU_AMAT = 42002004,

                       CACHE_FREE_LINES = 43000000,
                       CACHE_TOTAL_LINES = 43000001
                     };

enum SIMULATOR_EVENTS {
    INSTRUCTION_BUFFER_FILLED = 43001001,
    CACHE_REQUEST = 43002001,
    POLICY_TRAINER_MISSES = 43003001,
    POLICY_TRAINER_CURRENT_POLICY = 43008000,
    POLICY_TRAINER_RETRAIN_COUNTER = 43009000,
    POLICY_TRAINER_SAMPLED_TASK_TYPE = 43010000,
    POLICY_TRAINER_TASK_START = 43020000,
    POLICY_TRAINER_TASK_END = 43030000,
};

// Beyond this line the ids are kept for archiving purposes but they are not mantained.

#define PARAVER_EVENT_APPLICATION 40000001
#define PARAVER_EVENT_FLUSH_TRACE 40000003

#define DIMEMAS_EVENT 50000001

#define PARAVER_EVENT_RUNTIME 70000001
#define PARAVER_EVENT_PHASE 70000002
#define PARAVER_EVENT_TASK 70000003
#define PARAVER_EVENT_DISPATCHING_TOKEN 70000004
#define PARAVER_EVENT_BARRIER 70000005
#define PARAVER_EVENT_UNUSED_70000006 70000006
#define PARAVER_EVENT_TRANSFERING_DATA_IN 70000007
#define PARAVER_EVENT_TRANSFERING_DATA_OUT 70000008
#define PARAVER_EVENT_RUNNING_FUNCTION 70000009
#define PARAVER_EVENT_RUNNING_TASK 70000010

#define PARAVER_EVENT_PHASE1 70000011
#define PARAVER_EVENT_PHASE2 70000012
#define PARAVER_EVENT_PHASE3 70000013
#define PARAVER_EVENT_PHASE4 70000014
#define PARAVER_EVENT_PHASE5 70000015
#define PARAVER_EVENT_STAGE_IN_SYNC 70000016
#define PARAVER_EVENT_FILL_RATIO 70000017

#define PARAVER_EVENT_DEBUG 70000040

#define PARAVER_INFO_FUNCTION 70010001
#define PARAVER_INFO_TASK 70010002
#define PARAVER_INFO_FREE_TASKS 70010003
#define PARAVER_INFO_GRAPH_TASKS 70010004

#define PARAVER_EVENT_TASKSIM 9300001
#define PARAVER_EVENT_NANOS_RUNTIME_API 9200001
#define PARAVER_INFO_WD_ID 9200002

#define PARAVER_STATUS_UNINITIALIZED 999999
#define PARAVER_STATUS_IDLE 0
#define PARAVER_STATUS_RUNNING 1
#define PARAVER_STATUS_SYNCHRONIZATION 5

#define DIMEMAS_DONE 0
#define DIMEMAS_SEND 1
#define DIMEMAS_RECEIVE 2
#define DIMEMAS_ASYNC_SEND 3

#define PARAVER_TOKEN_RELEASE 0
#define PARAVER_TOKEN_ACQUIRE 1
#define PARAVER_TOKEN_WAKE 2
#define PARAVER_TOKEN_REQUEST 3

#define PARAVER_MASTER_PHASE_RETURN_TO_USER 1
#define PARAVER_MASTER_PHASE_INITIALISATION 2
#define PARAVER_MASTER_PHASE_SHUTDOWN 3
#define PARAVER_MASTER_PHASE_ADD_TASK 4
#define PARAVER_MASTER_PHASE_WAIT_ON 5
#define PARAVER_MASTER_PHASE_REGISTER_STRUCT 6
#define PARAVER_MASTER_PHASE_SCHEDULE 7
#define PARAVER_MASTER_PHASE_PREPARE_BUNDLE 8
#define PARAVER_MASTER_PHASE_PREPARE_BUNDLE_SUBMISSION 9
#define PARAVER_MASTER_PHASE_MANAGE_LOCALITY_CACHE 10
#define PARAVER_MASTER_PHASE_SUBMIT_BUNDLE 11
#define PARAVER_MASTER_PHASE_WAIT_FOR_FINISHED_TASKS 12
#define PARAVER_MASTER_PHASE_SYNCHRONISE_USER_SPACE 13
#define PARAVER_MASTER_PHASE_ATTEND_EXECUTION_FINISHED_CALLBACK 14
#define PARAVER_MASTER_PHASE_REMOVE_TASKS 15
#define PARAVER_HELPER_PHASE_WAIT_FOR_EVENTS 16
#define PARAVER_HELPER_PHASE_WAIT_FOR_LOCK 17
#define PARAVER_HELPER_PHASE_HANDLE_EVENT 18

#define FIRST_PARAVER_EXTERNAL_PHASE PARAVER_HELPER_PHASE_HANDLE_EVENT+1
#define PARAVER_EXTERNAL_PHASE_WAIT_FOR_TASK  0+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_GET_TASK 1+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_STAGE_IN 2+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_ALIGN_ARGUMENTS 3+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_EXECUTION 4+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_STAGE_OUT 5+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_CACHE_ADMIN 6+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_NOTIFICATION 7+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_WAIT_DMA 8+FIRST_PARAVER_EXTERNAL_PHASE
#define PARAVER_EXTERNAL_PHASE_WAIT_DMA_OUT_DONE 9+FIRST_PARAVER_EXTERNAL_PHASE

#define PARAVER_EXTERNAL_DMA_DIR_IN 70000018
#define PARAVER_EXTERNAL_DMA_SIZE_IN 70000019
#define PARAVER_EXTERNAL_DMA_DIR_OUT 70000020
#define PARAVER_EXTERNAL_DMA_SIZE_OUT 70000021

#define FIRST_SIM_EVENT                         70000200
#define SIM_EVENT_SIGNAL_TASK                   FIRST_SIM_EVENT
#define SIM_EVENT_WAIT_TASK                     1+FIRST_SIM_EVENT
#define SIM_EVENT_SIGNAL_DISPATCH               2+FIRST_SIM_EVENT
#define SIM_EVENT_WAIT_DISPATCH                 3+FIRST_SIM_EVENT
#define SIM_EVENT_SIGNAL_DMA                    4+FIRST_SIM_EVENT
#define SIM_EVENT_WAIT_DMA                      5+FIRST_SIM_EVENT

/* DRAM channel status */
#define DRAM_CHANNEL_IDLE       1
#define DRAM_CHANNEL_BUSY       2

/* L2 cache events */
#define PARAVER_EVENT_L2_HIT_RATE               80000001
#define PARAVER_EVENT_L2_HIT_RATE_ON_INVALID    80000002
#define PARAVER_EVENT_L2_MSHR_SIZE              80000004
#define PARAVER_EVENT_L2_CYCLES_PROC_IN_FULL    80000005
#define PARAVER_EVENT_L2_CACHEABLE_REQUESTS     80000006
#define PARAVER_EVENT_L2_REQUESTS               80000007

/* Nic events */
#define PARAVER_EVENT_NIC_QUEUE_SIZE            90000001
#define PARAVER_EVENT_NIC_QUEUE_READS           90000002
#define PARAVER_EVENT_NIC_QUEUE_WRITES          90000003

/* Bus events */
#define PARAVER_EVENT_BUS_BUSY_TARGET           100000001
#define PARAVER_EVENT_BUS_CYCLES_REQUEST        100000002

/* Mic events */
#define PARAVER_EVENT_MIC_DRAM_BANK_STATUS      111000001
#define PARAVER_EVENT_MIC_LOADS_PER_DRAM        110000001
#define PARAVER_EVENT_MIC_STORES_PER_DRAM       110000002
#define PARAVER_EVENT_MIC_LOADS_OTHER_DRAM      110000003
#define PARAVER_EVENT_MIC_STORES_OTHER_DRAM     110000004
#define PARAVER_EVENT_MIC_NS_PRECHARGED         110000005
#define PARAVER_EVENT_MIC_PAGE_CLOSES           110000006
#define PARAVER_EVENT_MIC_DRAM_STATUS           110000007


/* Memory controller events */
#define PARAVER_EVENT_MC_LOADS_LM               120000001
#define PARAVER_EVENT_MC_STORES_LM              120000002


}  // namespace instrumentation
}  // namespace utils
}  // namespace sim


#endif  // CORE_UTILS_INSTRUMENTATION_PARAVERDEFS_H_
