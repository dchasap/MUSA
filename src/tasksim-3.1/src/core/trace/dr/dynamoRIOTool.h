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

#ifndef CORE_TRACE_DR_DYNAMORIOTOOL_H_
#define CORE_TRACE_DR_DYNAMORIOTOOL_H_

#include <cxxabi.h>
#include <cassert>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <stack>
#include <unordered_set>
#include <utility>

// Nanos++ headers:
#define _NANOS_WORK_DESCRIPTOR_H
#include "workdescriptor_decl.hpp"
#include "basethread_decl.hpp"
#include "smpdevice_decl.hpp"
#include "smpdd.hpp"

#include "core/trace/ompss/Trace.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/types.h"
#include "core/trace/dr/types.h"

#define reg_t nanos_reg_t
#ifdef ENABLE_PTLSIM
#include "core/trace/dr/IntelDecoder.h"
#define REG_lea     64
#else
#define REG_imm     75
#define REG_mem     76
#define REG_lea     64
#endif  // ENABLE_PTLSIM
#include "core/trace/dr/BasicDecoder.h"
#undef reg_t

// DynmamoRIO headers:
#include "dr_api.h"
#include "drmgr.h"
#include "drreg.h"
#include "drsyms.h"
#include "drutil.h"
#include "drwrap.h"

namespace sim {
namespace trace {
namespace dr {

#ifdef ENABLE_PTLSIM
typedef sim::trace::dr::IntelDecoder Decoder;
#else
typedef sim::trace::dr::BasicDecoder Decoder;
#endif

// DR code:
enum {
    REF_TYPE_READ  = 0,
    REF_TYPE_WRITE = 1,
};

typedef struct _mem_ref_t {
    ushort type; /* r(0), w(1), or opcode (assuming 0/1 are invalid opcode) */
    ushort size; /* mem ref size or instr length */
    app_pc addr; /* mem ref addr or instr pc */
} mem_ref_t;

typedef struct {
    byte      *seg_base;
    mem_ref_t *buf_base;
    file_t     log;
} per_thread_t;

static const int MAX_NUM_MEM_REFS = 4096;

/* The maximum size of buffer for holding mem_refs. */
#define MEM_BUF_SIZE (sizeof(mem_ref_t) * MAX_NUM_MEM_REFS)
mem_ref_t *mem_buffer;

/* Allocated TLS slot offsets */
enum {
    MEMTRACE_TLS_OFFS_BUF_PTR,
    MEMTRACE_TLS_COUNT, /* total number of TLS slots allocated */
};
static reg_id_t tls_seg;
static uint     tls_offs;
static int      tls_idx;
#define TLS_SLOT(tls_base, enum_val) (void **)((byte *)(tls_base)+tls_offs+(enum_val))
#define BUF_PTR(tls_base) *(mem_ref_t **)TLS_SLOT(tls_base, MEMTRACE_TLS_OFFS_BUF_PTR)

static const unsigned MASTER_WD_ID = 1;

class dynamoRIOTool {
public:
    /** TaskSim Trace that we are generating. */
    typedef sim::trace::ts::FileTrace<SelectedEventStream, sim::trace::selectedtrace::TraceInternal> trace_t;
    trace_t *ts_trace_;

    std::string output_trace_name_;
    std::string ompss_trace_filename_, ts_trace_filename_;
    /** Task names and their name_ids (index). */
    std::vector<std::string> task_names_;
    std::unordered_map<std::string, bool> used_ompss_functions_;
    std::unordered_map<std::string, unsigned> task_index_;

    /** Nanos phase map. */
    sim::trace::ompss::nanos_phase_map_t phase_map_;

    /** Stores the function callback that we want instrumented, by function name. */
    std::unordered_map<std::string,
                       std::pair<std::function<void(void*, const std::string&, void **)>,
                                 std::function<void(void*, const std::string&, void *)> > > callback_map_;

    /** List of functions that we want instrumented still not instrumented (debug purposes). */
    std::set<std::string> instrumentation_pending_;
    std::set<std::string> instrumented_;
    std::unordered_map<std::string, std::vector<std::string>> mangled_names_;

    /**  Map from the functions address to the function names of instrumented functions. */
    std::unordered_map<app_pc, std::string> instrumented_functions_;
    std::unordered_map<app_pc, unsigned> addr_to_task_;

    /** Map between WD and task type. */
    std::map<unsigned, uint16_t> task_name_map_;
    /** Boolean that identifies if the simulation has started */
    bool started_;
    /** Booleans that identify if the region of interest has been detected and if we are inside it. */
    std::string roi_string_;
    bool roi_detected_, roi_, roi_used_, tracing_finished_;

    uint64_t total_detected_ld_, total_detected_st_;

    /** We store the different phases that are being executed for the current WD. */
    std::stack<unsigned> phase_stack_;
    std::map<unsigned, std::stack<unsigned>> saved_phases_map_;

    /** Stores the WD* of the inline parents. */
    std::map<unsigned, unsigned> inline_parent_;

    /** Currently executing Work Descriptor. 0 defines that we are not running any. */
    int current_wd_;
    int wd_counter_;
    /** Number of pending memory addresses. */
    int pending_mem_;
    /** Total number of instructions executed. */
    uint64_t total_instructions_;
    /** Total number of unidentified instructions executed. */
    uint64_t total_unidentified_instructions_;
    /** Last instrumented Basic Block and memory addresses. */
    int prev_bbl_id_;

    /** Marks if the current SVE code is in emulation mode. */
    bool in_emulation_;

    /** We store the tassk we are executing. */
    typedef std::stack<ts::subtrace_t> task_stack_t;
    task_stack_t task_stack_;

    /** Index in the basic bloc dictionary for the address of the basic block. */
    std::unordered_map<uint64_t, int> bbl_index_;
    std::unordered_set<int> bbl_instrumented_;

    /** Temporary storage of number of loads, stores, instructions and unidentified instructions by basic block. */
    std::vector<sim::trace::phase_data_t> bbl_info_;
    /** Number of instructions for each basic block. */
    std::vector<unsigned> n_instrs_;
    /** Number of loads, stores, instructions and unidentified instructions for the current user code phase. */
    sim::trace::phase_data_t current_phase_data_;

    /** Current master task alias. */
    unsigned master_alias_;
    /** Starting master task alias, can be defined via parameter */
    unsigned alias_c_;
    /** Boolean to distinguish if the Tool has been called with MPI enabled. */
    bool enable_mpi_;
    /** Boolean to distinguish if instrumentation is enabled or disabled due to being inside an MPI phase. */
    bool enabled_by_mpi_;
    /** Boolean to indicate if the tool has been invoqued without a Nanos run.*/
    bool no_nanos_;
    /** MPI Rank. */
    unsigned rank_;
    /** Mutex used to safeguard internal data from multiple threads. */
    void *dr_mutex_;
    /** Pointer to the main thread drcontext*, used to distinguish threads. */
    void *thread_context_;

    /** Temporarily store dependency count. */
    std::unordered_map<size_t, size_t> num_deps_wd_;
    /** We store the current MPI call status. */
    mpi_info_t mpi_info_;
    /** Counter for the number of MPI phases with Nanos++ code executed currently. */
    unsigned mpi_nanos_phases_;
    /** File from where we read the mpi phase information. */
    std::ifstream mpi_relation_file_;

    uint64_t bbl_count_;
    dynamoRIOTool() :
            started_(false),
            roi_detected_(false),
            roi_(false),
            roi_used_(false),
            tracing_finished_(true),
            total_detected_ld_(0),
            total_detected_st_(0),
            current_wd_(0),
            wd_counter_(0),
            pending_mem_(0),
            total_instructions_(0),
            total_unidentified_instructions_(0),
            prev_bbl_id_(-1),
            in_emulation_(false),
            master_alias_(MASTER_WD_ID),
            alias_c_(18005000),
            enabled_by_mpi_(true),
            no_nanos_(false),
            rank_(0),
            thread_context_(0),
            mpi_info_(),
            mpi_nanos_phases_(0),
            bbl_count_(0)
            {}

    // Function to instrument functions (rephrase?).
    inline void instrument(const std::string &function_name,
                           std::function<void(void*, const std::string &, void **)> entry_callback,
                           std::function<void(void*, const std::string &, void *)> exit_callback);

    void nanos_wait_condition_entry(void *context, const std::string &function_name, void **user_data);
    void nanos_wait_condition_exit(void *context, const std::string &function_name, void *user_data);

    void nanos_duplicate_wd_entry(void *context, const std::string &function_name, void **user_data);
    void nanos_duplicate_wd_exit(void *context, const std::string &function_name, void *user_data);

    void nanos_wait_group_entry(void *context, const std::string &function_name, void **user_data);
    void nanos_wait_group_exit(void *context, const std::string &function_name, void *user_data);
    void nanos_wait_dependencies_entry(void *context, const std::string &function_name, void **user_data);
    void nanos_wait_dependencies_exit(void *context, const std::string &function_name, void *user_data);
    void nanos_submit(void *context, const std::string &function_name, void **user_data);

    void nanos_WD_enter(void *context, const std::string &function_name, void **user_data);
    void nanos_WD_exit(void *context, const std::string &function_name, void *user_data);

    void nanos_inline_WD_enter(void *context, const std::string &function_name, void **user_data);
    void nanos_inline_WD_exit(void *context, const std::string &function_name, void *user_data);

    void nanos_set_lock(void *context, const std::string &function_name, void **user_data);
    void nanos_unset_lock(void *context, const std::string &function_name, void **user_data);

    /** Function called when the instrumentation should be disabled (MPI phase entry). */
    void nanos_instrumentation_disable_entry(void *context, const std::string &function_name, void **user_data);
    /** Function called when the instrumentation should be re-enabled (MPI phase exit). */
    void nanos_instrumentation_enable_entry(void *context, const std::string &function_name, void **user_data);

    /** Function used to set the current WD (checks for current master alias). */
    inline void set_wd(unsigned new_wd_id);
    /** Functions to be called when roi_start and roi_end are found. */
    void roi_begin(void *context, const std::string &function_name,  void **user_data);
    void roi_end(void *context, const std::string &function_name, void **user_data);

    /** Function to track application start. */
    void app_start(void *context, const std::string &function_name, void **user_Data);
    /** Function to track application end. */
    void app_end(void *context, const std::string &function_name, void *user_data);

    /** Function to track function entry. */
    void rtn_entry(void *context, const std::string &function_name, void **user_data, int phase_id);
    /** Function to track function exit. */
    void rtn_exit(void *context, const std::string &function_name, void *user_data, int phase_id);

    /** Function to track MPI function entry. */
    void mpi_entry(void *context, const std::string &function_name, void **user_data, int mpi_id);
    /** Function to track MPI function exit. */
    void mpi_exit(void *context, const std::string &function_name, void *user_data, int mpi_id);

    /** Function to be called on MPI Initialization. */
    void tsmpi_init(const std::string &trace_name);
    /** Function to be called on MPI Finalization. */
    void tsmpi_finish();
    /** Loads the information for the next MPI phase. */
    void tsmpi_new_phase(unsigned mpi_id, const std::string &function_name);
    /** Returns wether the current MPI phase should be traced or not. */
    bool tsmpi_is_traceable();

    // Functions to be called whenever a phase must be started. */
    void start_phase();
    // Functions to be called whenever a phase must be ended. */
    void end_phase();

    /** Function that is called to record each memory access. */
    void memory_access(size_t address, int8_t size, bool write);
    /** Function that is called to record each basic block execution. */
    void bbl_entry(uint64_t id);

    /** Function that handles all the dynamoRIO specific calls to instrument an instruction. */
    void instrument_instr(void *drcontext, instrlist_t *ilist, instr_t *where);
    /** Function that handles all the dynamoRIO specific calls to instrument one memory acccess. */
    void instrument_mem(void *drcontext, instrlist_t *ilist, instr_t *where, opnd_t ref, bool write);

    /** This functions insert different instructions to instrument memory adresses. */
    void insert_save_pc(void *drcontext, instrlist_t *ilist, instr_t *where,
                        reg_id_t base, reg_id_t scratch, app_pc pc);
    void insert_save_addr(void *drcontext, instrlist_t *ilist, instr_t *where,
                          opnd_t ref, reg_id_t reg_ptr, reg_id_t reg_addr);
    void insert_save_type(void *drcontext, instrlist_t *ilist, instr_t *where,
                          reg_id_t base, reg_id_t scratch, ushort type);
    void insert_save_size(void *drcontext, instrlist_t *ilist, instr_t *where,
                          reg_id_t base, reg_id_t scratch, ushort size);
    void insert_load_buf_ptr(void *drcontext, instrlist_t *ilist, instr_t *where,
                             reg_id_t reg_ptr);
    void insert_update_buf_ptr(void *drcontext, instrlist_t *ilist, instr_t *where,
                               reg_id_t reg_ptr, int adjust);

    virtual void run_pre_instrument();
    virtual void run_post_instrument();

    void event_exit(void);

    dr_emit_flags_t event_basicblock_analysis(void *drcontext, void *tag, instrlist_t *bb,
                                                     bool for_trace, bool translating,
                                                     OUT void **user_data);


    dr_emit_flags_t event_bb_insert(void *drcontext, void *tag, instrlist_t *bb,
                                           instr_t *instr, bool for_trace, bool translating,
                                           void *user_data);

    static std::string get_function_name(app_pc addr);

    /** Function to be called on task entry. */
    void task_entry(void* wrapctx, OUT void **user_data);
    /** Function to be called on task exit. */
    void task_exit(void *wrapctx, void *user_data);

    /** Function called when a module is loaded in order to parse it symbols
      * and add the proper instrumentation to function calls.
      */
    void instrument_symbol(const char* name, size_t modoffs, void *data);

    /** Function to be called by dynamoRIO when it loads a new module.
      * It tries to find the instrumented functions inside it and instrument them.
      */
    void module_load_event(void *drcontext, const module_data_t *mod, bool loaded);

    /** Checks if the basic bloc is not part of the MPI libraries. */
    bool instr_module_tracing_is_disabled(void* drcontext, instr_t *instr);

    void usage();
    void parse_args(int argc, char *argv[]);
    std::string generate_random_trace_name();
};

dynamoRIOTool tool_;
/** Static function that will call the dynamoRIOTool bbl_entry function. */
static void bbl_entry(uint64_t id);

/** Static function that will call the dynamoRIOTool memory_access function. */
static void mem_access();

/** Static function that will call the appropiate dynamoRIOTool task_entry function. */
static void static_task_entry(void* wrapctx, OUT void **user_data);

/** Static function that will call the appropiate dynamoRIOTool task_exit function. */
static void static_task_exit(void *wrapctx, void *user_data);

/** Rutine that replaces repeated string instructions for loops to properly track memory accesses. */
static dr_emit_flags_t event_bb_app2app(void *drcontext, void *tag, instrlist_t *bb,
                                            bool for_trace, bool translating);

/** Static function that will call the dynamoRIOTool Event exit function (end of execution). */
void event_exit(void);

/** Static function that will call the dynamoRIOTool module_load_event funcition. */
void module_load_event(void *drcontext, const module_data_t *mod, bool loaded);

/** Static function that will call the dynamoRIOTool function (Basic block translation and instrumentation. */
dr_emit_flags_t event_basicblock_analysis(void *drcontext, void *tag, instrlist_t *bb,
                                          bool for_trace, bool translating,
                                          OUT void **user_data);

/** Static function that will call the dynamoRIOTool function (Instrumentation of memory addresses. */
dr_emit_flags_t event_bb_insert(void *drcontext, void *tag, instrlist_t *bb,
                                instr_t *instr, bool for_trace, bool translating,
                                void *user_data);

/** Static function that is called by DynamoRIO when a thread is initialiized. */
static void event_thread_init(void *drcontext);

/** Static function that is called by DynamoRIO when a thread exits. */
static void event_thread_exit(void *drcontext);

/** Static function that is called by DynamoRIO to check al symbols in a module when it is loaded. */
static bool enumerate_symbols(const char* name, size_t modofss, void *data);

}  // namespace dr
}  // namespace trace
}  // namespace sim

/** Main DynamoRIO function has to be outside of any namespace. */
DR_EXPORT void dr_client_main(client_id_t id, int argc, const char *argv[]);

#endif  // CORE_TRACE_DR_DYNAMORIOTOOL_H_
