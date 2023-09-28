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


#include "dynamoRIOTool.h"

#include <cxxabi.h>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stack>
#include <unordered_map>
#include <utility>

#include "dr_api.h"
#include "drmgr.h"
#include "drsyms.h"
#include "drwrap.h"

#include "core/utils/Log.h"

namespace nanos {
    inline bool WorkDescriptor::isReady () const { return _flags.is_ready; }
    inline DeviceData & WorkDescriptor::getActiveDevice () const { return *_devices[_activeDeviceIdx]; }
    inline WorkDescriptor * WorkDescriptor::getParent() const { return _parent; }
    inline WD & BaseThread::getThreadWD () const { return _threadWD; }
    inline DeviceData::work_fct DeviceData::getWorkFct() const { return _work; }
}

namespace sim {
namespace trace {
namespace dr {

using sim::utils::Log;

inline
void dynamoRIOTool::set_wd(unsigned new_wd_id) {
    if (new_wd_id == MASTER_WD_ID) {
        current_wd_ = master_alias_;
    } else {
        current_wd_ = new_wd_id;
    }
}

std::string dynamoRIOTool::get_function_name(app_pc addr) {
    std::string res("");
    drsym_error_t symres;
    drsym_info_t sym;
    const int MAX_SYM_RESULT = 256;
    char name[MAX_SYM_RESULT];
    char file[MAXIMUM_PATH];

    module_data_t *data;
    data = dr_lookup_module(addr);
    if (data == NULL) {
        dr_fprintf(STDOUT, "[ERROR] Could not find source module information.\n");
        return res;
    }
    sym.struct_size = sizeof(sym);
    sym.name = name;
    sym.name_size = MAX_SYM_RESULT;
    sym.file = file;
    sym.file_size = MAXIMUM_PATH;
    symres = drsym_lookup_address(data->full_path, addr - data->start, &sym, DRSYM_DEFAULT_FLAGS);
    std::string source, destination;
    if (symres == DRSYM_SUCCESS || symres == DRSYM_ERROR_LINE_NOT_AVAILABLE) {
        res = std::string(sym.name);
    }
    dr_free_module_data(data);
    return res;
}

static void static_task_entry(void* wrapctx, OUT void **user_data) {
    tool_.task_entry(wrapctx, user_data);
}

static void static_task_exit(void *wrapctx, void *user_data) {
    if (wrapctx == NULL) {
        dr_fprintf(STDOUT, "[ERROR] Abnormal stack unwind.\n");
        return;
    }
    tool_.task_exit(wrapctx, user_data);
}

void dynamoRIOTool::task_entry(void* wrapctx, OUT void **user_data) {
    // First we gather the addr of the instrumented function:
    app_pc addr = drwrap_get_func(wrapctx);
    // Then we gather its name:
    const std::string &function_name = instrumented_functions_.at(addr);
    // And finally the associated instrumentation function (first, as it's function entry):
    auto callback = callback_map_.at(function_name).first;
    // And we call it (locking the DynamoRIO mutex)
    dr_mutex_lock(tool_.dr_mutex_);
    callback(wrapctx, function_name, user_data);
    dr_mutex_unlock(tool_.dr_mutex_);
}

void dynamoRIOTool::task_exit(void *wrapctx, void *user_data) {
    // First we gather the addr of the instrumented function:
    app_pc addr = drwrap_get_func(wrapctx);
    // Then we gather its name:
    const std::string &function_name = instrumented_functions_.at(addr);
    // And finally the associated instrumentation function (second, as it's function exit):
    auto callback = callback_map_.at(function_name).second;
    // And we call it (locking the DynamoRIO mutex)
    dr_mutex_lock(tool_.dr_mutex_);
    callback(wrapctx, function_name, user_data);
    dr_mutex_unlock(tool_.dr_mutex_);
}

void module_load_event(void *drcontext, const module_data_t *mod, bool loaded)
{
    tool_.module_load_event(drcontext, mod, loaded);
}

bool enumerate_symbols(const char* name, size_t modoffs, void *data)
{
    tool_.instrument_symbol(name, modoffs, data);
    return true;
}

void dynamoRIOTool::instrument_symbol(const char* name, size_t modoffs, void *data)
{
    std::string symbol_name = std::string(name);
    static const int MAX_DEMANGLE_SYMBOL_NAME = 1000;
    char *demangle_buffer = reinterpret_cast<char*>(malloc(sizeof(char) * MAX_DEMANGLE_SYMBOL_NAME));
    drsym_demangle_symbol(demangle_buffer, MAX_DEMANGLE_SYMBOL_NAME, name, DRSYM_DEMANGLE);

    std::string demangled_name(demangle_buffer);
    free(demangle_buffer);

    for (auto it = instrumentation_pending_.begin(); it != instrumentation_pending_.end(); it++) {
        if (demangled_name.compare(*it) == 0) {
            mangled_names_[*it].push_back(symbol_name);
        }
        const std::string val("::" + *it);
        if (demangled_name.find(val) != std::string::npos &&
                (demangled_name.find(val) + val.size() == demangled_name.size()) ) {
            // We have found a namespaced version of _val_
            // We are adding the namespaced version as a possible alternative.
            mangled_names_[*it].push_back(symbol_name);
            return;
        }
        const std::string alternative(*it + "<>");
        if (demangled_name.compare(alternative) == 0) {
            // We have found a templated version of _val_
            // We are adding the namespaced version as a possible alternative.
            mangled_names_[*it].push_back(symbol_name);
            return;
        }
        const std::string alternative_2(*it + "_");
        if (demangled_name.compare(alternative_2) == 0) {
            // We have found a version of _val_ with a trailing underscore (Fortran).
            // We are adding the namespaced version as a possible alternative.
            mangled_names_[*it].push_back(symbol_name);
            return;
        }
        const std::string alternative_3("_MOD_" + (*it));
        if ((demangled_name.find(alternative_3) != std::string::npos) &&
                (demangled_name.find(alternative_3) + alternative_3.size() == demangled_name.size())) {
            // We have found a version of _val_ insider a fortran module (Fortran).
            // We are adding the namespaced version as a possible alternative.
            mangled_names_[*it].push_back(symbol_name);
            return;
        }
    }
}

bool dynamoRIOTool::instr_module_tracing_is_disabled(void *drcontext, instr_t *instr) {
    app_pc addr = instr_get_app_pc(instr);
    module_data_t *data;
    data = dr_lookup_module(addr);
    if (data == NULL) {
        dr_fprintf(STDOUT, "[ERROR] Could not find source module information.\n");
        return true;
    }
    const std::string module_name(dr_module_preferred_name(data));
    if (module_name.find("libompss-drtrace.so") != std::string::npos or
            module_name.find("libmemtrace") != std::string::npos or
            module_name.find("libsve_") != std::string::npos) {
        return true;
    }
    return false;
}

void dynamoRIOTool::module_load_event(void *drcontext, const module_data_t *mod, bool loaded)
{
    // Enumerate names:
    const std::string module_name(dr_module_preferred_name(mod));
    Log::info() << "[DRTool] Loading module " << module_name;
    if (module_name.find("libompss-drtrace.so") != std::string::npos or
            module_name.find("libmemtrace") != std::string::npos or
            module_name.find("libsve_") != std::string::npos) {
        // We do not want to instrument the instrumentation library (Contains THIS file)!!
        Log::info() << "[DRTool] Ignoring module.";
        return;
    }

    drsym_enumerate_symbols(mod->full_path, enumerate_symbols, NULL, DRSYM_LEAVE_MANGLED);

    std::vector<std::string> instrumented;
    for (auto it = instrumentation_pending_.begin(); it != instrumentation_pending_.end(); it++) {
        for (auto mangled_it = mangled_names_[*it].begin(); mangled_it != mangled_names_[*it].end(); mangled_it++) {
            // We are looking for the symbol mangled_it->c_str()
            size_t modoff;
            drsym_error_t err = drsym_lookup_symbol(mod->full_path, mangled_it->c_str(), &modoff, DRSYM_LEAVE_MANGLED);
            if (err == DRSYM_SUCCESS) {
                if ((*it == "main") and (*mangled_it != "main")) {
                    continue;
                }
                // We found the symbol inside the module we are loading
                bool ok;
                if (callback_map_.at(*it).second == NULL) {
                    // We only want to instrument the entry, not the exit.
                    ok = drwrap_wrap(mod->start + modoff, static_task_entry, NULL);
                } else if (callback_map_.at(*it).first == NULL) {
                    // We only want to instrument the exit, not the entry.
                    ok = drwrap_wrap(mod->start + modoff, NULL, static_task_exit);
                } else {
                    // We want to instrument both entry and exit.
                    ok = drwrap_wrap(mod->start + modoff, static_task_entry, static_task_exit);
                }
                if (ok) {
                    // We have instrumented the function.
                    instrumented_functions_.insert({(mod->start + modoff), *it});
                    instrumented.push_back(*it);
                    instrumented_.insert(*it);
                    dr_fprintf(STDOUT, "[DRTool] Instrumenting %s.\n", it->c_str());
                } else {
                    dr_fprintf(STDOUT, "[DRTool ERROR] Could not wrap %s.\n", it->c_str());
                }
            }
        }
    }
}

/* we transform string loops into regular loops so we can more easily
 * monitor every memory reference they make
 */
static dr_emit_flags_t
event_bb_app2app(void *drcontext, void *tag, instrlist_t *bb,
                 bool for_trace, bool translating)
{
    if (!drutil_expand_rep_string(drcontext, bb)) {
        /* in release build, carry on: we'll just miss per-iter refs */
        std::cout << "Could not convert REPMOV instruction to equivalent basic block." << std::endl;
        DR_ASSERT(false);
    }
    return DR_EMIT_DEFAULT;
}

void event_exit(void) {
    tool_.event_exit();
}

void dynamoRIOTool::event_exit(void)
{
    dr_mutex_lock(tool_.dr_mutex_);
    // We check that main() has exited:
    if (started_) {
        std::cout << "[DEBUG] Event exit before application end." << std::endl;
        app_end(NULL, "main_exit_at_tool_event_exit", NULL);
    }

    dr_fprintf(STDOUT, "[DRTool] Tasksim's OmpSs trace test with dynamoRIO finished.\n");
    if (roi_detected_ and not roi_used_) {
        dr_fprintf(STDOUT, "[DRTool] [WARNING] Region of interest functions in binary but never called.\n");
    }
    if (instrumentation_pending_.size() > instrumented_.size() + 5) {
        dr_fprintf(STDOUT, "[DRTool] %d functions were to be instrumented.\n",
                instrumentation_pending_.size());
        dr_fprintf(STDOUT, "[DRTool] %d functions were actually instrumented.\n",
                instrumented_.size());
    }
    dr_fprintf(STDOUT, "[DRTool] %llu memory accesses detected (total).\n",
            total_detected_ld_ + total_detected_st_);
    dr_fprintf(STDOUT, "[DRTool] %llu memory loads detected (total).\n", total_detected_ld_);
    dr_fprintf(STDOUT, "[DRTool] %llu memory stores detected (total).\n", total_detected_st_);
    if (instrumentation_pending_.size() > instrumented_.size() + 5) {
        for (auto it = instrumentation_pending_.begin();
                it != instrumentation_pending_.end(); it++) {
            if (instrumented_.find(*it) == instrumented_.end()) {
                dr_fprintf(STDOUT, "[DRTool WARNING] Function %s was not instrumented.\n",
                        it->c_str());
            }
        }
    }
    int a = 0, b = 0;
    for (auto it = bbl_index_.begin(); it!= bbl_index_.end(); it++) {
        if (bbl_instrumented_.find(it->second) == bbl_instrumented_.end()) {
            a++;
        } else {
            b++;
        }
    }
    std::cout << "[DRTool] Of all inserted basic blocks (" << (a+b) << "), "
              << a << " were not instrumented." << std::endl;

    if (pending_mem_ != 0) {
        dr_fprintf(STDOUT, "[DRTool] Pending memory accesses = %d.\n", pending_mem_);
    }
    dr_fprintf(STDOUT, "[DRTool] Got a total of %u instructions.\n", total_instructions_);
    dr_fprintf(STDOUT, "[DRTool] Got a total of %u unidentified instructions.\n", total_unidentified_instructions_);

    for (unsigned int i = 0; i < task_names_.size(); i++) {
        if (used_ompss_functions_.find(task_names_[i]) == used_ompss_functions_.end()) {
            dr_fprintf(STDOUT, "[DRTool] WARNING: Ompss function %s was not found .\n",
                       task_names_[i].c_str());
        }
    }
    std::cout << "[DRTool] Closing the trace." << std::endl;
    delete ts_trace_;
    std::cout << "[DRTool] Trace closed." << std::endl;

    dr_mutex_unlock(tool_.dr_mutex_);
    dr_mutex_destroy(dr_mutex_);
    dr_raw_tls_cfree(tls_offs, MEMTRACE_TLS_COUNT);
    drmgr_unregister_tls_field(tls_idx);
    drmgr_unregister_bb_instrumentation_event(::sim::trace::dr::event_basicblock_analysis);
    // We exit dynamoRIO
    drwrap_exit();
    drreg_exit();
    drutil_exit();
    drmgr_exit();
    drsym_exit();
}

dr_emit_flags_t event_basicblock_analysis(void *drcontext, void *tag, instrlist_t *bb,
                  bool for_trace, bool translating, OUT void **user_data)
{
    return tool_.event_basicblock_analysis(drcontext, tag, bb, for_trace, translating, user_data);
}

dr_emit_flags_t dynamoRIOTool::event_basicblock_analysis(void *drcontext, void *tag, instrlist_t *bb,
                  bool for_trace, bool translating, OUT void **user_data)
{
    if (translating or tracing_finished_) {
        return DR_EMIT_DEFAULT;
    }
    // We get a lock on the mutex.
    dr_mutex_lock(dr_mutex_);

    Log::info() << "====== Basic block analysis ======";
    if (Log::should_print(Log::INFO)) {
        instrlist_disassemble(drcontext, instr_get_app_pc(instrlist_first_app(bb)), bb, STDERR);
    }
    Log::info() << "====== End =======";

    bbl_t bbl;
    bbl.id_ = bbl_info_.size();

    // Decoder is typedef to IntelDecoder or BasicDecoder.
    sim::trace::phase_data_t bbl_data = Decoder::translate(drcontext, bb, bbl);
    ts_trace_->add_bbl(bbl);

    // We set the address and the number of loads/stores.
    uint64_t id = reinterpret_cast<uint64_t>(tag);
    bbl_index_[id] = bbl_info_.size();
    n_instrs_.push_back(bbl.macroops_.size());
    bbl_info_.push_back(bbl_data);

    dr_mutex_unlock(dr_mutex_);
    return DR_EMIT_DEFAULT;
}


dr_emit_flags_t event_bb_insert(void *drcontext, void *tag, instrlist_t *bb,
                instr_t *instr, bool for_trace, bool translating,
                void *user_data)
{
    return tool_.event_bb_insert(drcontext, tag, bb, instr, for_trace, translating, user_data);
}


dr_emit_flags_t dynamoRIOTool::event_bb_insert(void *drcontext, void *tag, instrlist_t *bb,
                instr_t *instr, bool for_trace, bool translating,
                void *user_data)
{
    if (tracing_finished_) {
        return DR_EMIT_DEFAULT;
    }

    dr_mutex_lock(dr_mutex_);
#ifdef ARM_64
    bool exclusive_mem = false;
    // We do not instrument basic blocs with exclusive load/stores.
    for (instr_t *instr = instrlist_first_app(bb); instr != NULL; instr = instr_get_next_app(instr)) {
        int op = instr_get_opcode(instr);
        if (op == OP_ldaxp or op == OP_ldaxr or op == OP_ldaxrb or op == OP_ldaxrh or op == OP_ldxp or
                op == OP_ldxr or op == OP_ldxrb or op == OP_ldxrh){
            // Exclusive load:
            exclusive_mem = true;
            break;
        } else if (op == OP_stxp or op == OP_stxr or op == OP_stxrb or op == OP_stxrh or
                op == OP_stlxp or op == OP_stlxr or op == OP_stlxrb or op == OP_stlxrh) {
            // Exclusive store:
            exclusive_mem = true;
            break;
        }
    }
    if (exclusive_mem) {
        dr_mutex_unlock(dr_mutex_);
        return DR_EMIT_DEFAULT;
    }
#endif
    // If it is the first instruction of a basic block, we insert a call to instrument it.
    if (drmgr_is_first_instr(drcontext, instr)) {
        uint64_t id = reinterpret_cast<uint64_t>(tag);
        dr_insert_clean_call(drcontext, bb, instr,
                             reinterpret_cast<void*>(sim::trace::dr::bbl_entry),
                             false, 1, OPND_CREATE_INT64(id));
        assert(bbl_index_.find(id) != bbl_index_.end());
        bbl_instrumented_.insert(bbl_index_[id]);
    }
    if (drmgr_is_emulation_start(instr)) {
        // We cannot actually instrument the SVE instruction.
        in_emulation_ = true;
        dr_mutex_unlock(dr_mutex_);
        return DR_EMIT_DEFAULT;
    }
    if (drmgr_is_emulation_end(instr)) {
        in_emulation_ = false;
        dr_mutex_unlock(dr_mutex_);
        return DR_EMIT_DEFAULT;
    }

    if (!instr_is_app(instr) or instr_module_tracing_is_disabled(drcontext, instr)) {
        dr_mutex_unlock(dr_mutex_);
        return DR_EMIT_DEFAULT;
    }

    if (instr_get_app_pc(instr) == NULL) {
        std::cout << "[ERROR] Inserting bbl with app address 0." << std::endl;
        dr_mutex_unlock(dr_mutex_);
        return DR_EMIT_DEFAULT;
    }
    // If we are on emulation, outside of application code or it does not read nor write memory, we are done.
    if (in_emulation_ or !instr_is_app(instr) or
            (not instr_reads_memory(instr) and not instr_writes_memory(instr))) {
        dr_mutex_unlock(dr_mutex_);
        return DR_EMIT_DEFAULT;
    }
    if (instr_needs_encoding(instr)) {
        dr_mutex_unlock(dr_mutex_);
        return DR_EMIT_DEFAULT;
    }
    // In case that we wanted to track the instructions as memory accesses themselves:
    // instrument_instr(drcontext, bb, instr);

    // Instrument_mem will store the operand addresses in an array.
    if (instr_reads_memory(instr)) {
        // As Load Effective Address calculates an address buyt does not read it nor writes it.
        for (int i = 0; i < instr_num_srcs(instr); i++) {
            if (opnd_is_memory_reference(instr_get_src(instr, i))) {
                instrument_mem(drcontext, bb, instr, instr_get_src(instr, i), false);
            }
        }
    }

    if (instr_writes_memory(instr)) {
       // As Load Effective Address calculates an address buyt does not read it nor writes it.
        for (int i = 0; i < instr_num_dsts(instr); i++) {
            if (opnd_is_memory_reference(instr_get_dst(instr, i))) {
                instrument_mem(drcontext, bb, instr, instr_get_dst(instr, i), true);
            }
        }
    }

    dr_mutex_unlock(dr_mutex_);
    return DR_EMIT_DEFAULT;
}


void dynamoRIOTool::instrument_instr(void *drcontext, instrlist_t *ilist, instr_t *where)
{
    /* We need two scratch registers */
    reg_id_t reg_ptr, reg_tmp;
    instrlist_set_auto_predicate(ilist, DR_PRED_NONE);
    if (drreg_reserve_register(drcontext, ilist, where, NULL, &reg_ptr) != DRREG_SUCCESS ||
        drreg_reserve_register(drcontext, ilist, where, NULL, &reg_tmp) != DRREG_SUCCESS) {
        DR_ASSERT(false); /* cannot recover */
        return;
    }
    insert_load_buf_ptr(drcontext, ilist, where, reg_ptr);
    insert_save_type(drcontext, ilist, where, reg_ptr, reg_tmp,
                     (ushort)instr_get_opcode(where));
    insert_save_size(drcontext, ilist, where, reg_ptr, reg_tmp,
                     (ushort)instr_length(drcontext, where));
    insert_save_pc(drcontext, ilist, where, reg_ptr, reg_tmp, instr_get_app_pc(where));
    insert_update_buf_ptr(drcontext, ilist, where, reg_ptr, sizeof(mem_ref_t));
    /* Restore scratch registers */
    if (drreg_unreserve_register(drcontext, ilist, where, reg_ptr) != DRREG_SUCCESS ||
        drreg_unreserve_register(drcontext, ilist, where, reg_tmp) != DRREG_SUCCESS)
        DR_ASSERT(false);
    instrlist_set_auto_predicate(ilist, instr_get_predicate(where));
}


void dynamoRIOTool::instrument_mem(void *drcontext, instrlist_t *ilist, instr_t *where, opnd_t ref, bool write)
{
    /* We need two scratch registers */
    reg_id_t reg_ptr, reg_tmp;
    // dr_fprintf(STDOUT, "Instrumenting a memory access %s.\n", write?"WRITE":"READ");
    if (drreg_reserve_register(drcontext, ilist, where, NULL, &reg_ptr) != DRREG_SUCCESS ||
        drreg_reserve_register(drcontext, ilist, where, NULL, &reg_tmp) != DRREG_SUCCESS) {
        DR_ASSERT(false); /* cannot recover */
        return;
    }
    /* save_addr should be called first as reg_ptr or reg_tmp maybe used in ref */
    insert_save_addr(drcontext, ilist, where, ref, reg_ptr, reg_tmp);
    insert_save_type(drcontext, ilist, where, reg_ptr, reg_tmp,
                     write ? REF_TYPE_WRITE : REF_TYPE_READ);
    insert_save_size(drcontext, ilist, where, reg_ptr, reg_tmp,
                     (ushort)drutil_opnd_mem_size_in_bytes(ref, where));
    insert_update_buf_ptr(drcontext, ilist, where, reg_ptr, sizeof(mem_ref_t));
    /* Restore scratch registers */
    if (drreg_unreserve_register(drcontext, ilist, where, reg_ptr) != DRREG_SUCCESS ||
            drreg_unreserve_register(drcontext, ilist, where, reg_tmp) != DRREG_SUCCESS) {
        DR_ASSERT(false);
    }
}


void dynamoRIOTool::insert_save_pc(void *drcontext, instrlist_t *ilist, instr_t *where,
                                   reg_id_t base, reg_id_t scratch, app_pc pc)
{
    instr_t *mov1, *mov2;
    instrlist_insert_mov_immed_ptrsz(drcontext, (ptr_int_t) pc, opnd_create_reg(scratch),
                                     ilist, where, &mov1, &mov2);
    DR_ASSERT(mov1 != NULL);
    instr_set_meta(mov1);
    if (mov2 != NULL) {
        instr_set_meta(mov2);
    }
    instrlist_meta_preinsert(ilist, where, XINST_CREATE_store(drcontext, OPND_CREATE_MEMPTR(base,
                             offsetof(mem_ref_t, addr)), opnd_create_reg(scratch)));
}

void dynamoRIOTool::insert_save_addr(void *drcontext, instrlist_t *ilist, instr_t *where,
                                     opnd_t ref, reg_id_t reg_ptr, reg_id_t reg_addr)
{
    /* we use reg_ptr as scratch to get addr */
    bool ok = drutil_insert_get_mem_addr(drcontext, ilist, where, ref, reg_addr, reg_ptr);
    DR_ASSERT(ok);
    insert_load_buf_ptr(drcontext, ilist, where, reg_ptr);
    instrlist_meta_preinsert(ilist, where, XINST_CREATE_store(drcontext,
                             OPND_CREATE_MEMPTR(reg_ptr, offsetof(mem_ref_t, addr)),
                             opnd_create_reg(reg_addr)));
}


void dynamoRIOTool::insert_save_type(void *drcontext, instrlist_t *ilist, instr_t *where,
                                     reg_id_t base, reg_id_t scratch, ushort type)
{
    scratch = reg_resize_to_opsz(scratch, OPSZ_2);
    instrlist_meta_preinsert(ilist, where, XINST_CREATE_load_int(drcontext,
                             opnd_create_reg(scratch), OPND_CREATE_INT16(type)));
    instrlist_meta_preinsert(ilist, where, XINST_CREATE_store_2bytes(drcontext,
                             OPND_CREATE_MEM16(base, offsetof(mem_ref_t, type)),
                             opnd_create_reg(scratch)));
}


void dynamoRIOTool::insert_save_size(void *drcontext, instrlist_t *ilist, instr_t *where,
                                     reg_id_t base, reg_id_t scratch, ushort size)
{
    scratch = reg_resize_to_opsz(scratch, OPSZ_2);
    instrlist_meta_preinsert(ilist, where, XINST_CREATE_load_int(drcontext,
                             opnd_create_reg(scratch), OPND_CREATE_INT16(size)));
    instrlist_meta_preinsert(ilist, where, XINST_CREATE_store_2bytes(drcontext,
                             OPND_CREATE_MEM16(base, offsetof(mem_ref_t, size)),
                             opnd_create_reg(scratch)));
}

void dynamoRIOTool::insert_load_buf_ptr(void *drcontext, instrlist_t *ilist, instr_t *where, reg_id_t reg_ptr)
{
    dr_insert_read_raw_tls(drcontext, ilist, where, tls_seg,
                           tls_offs + MEMTRACE_TLS_OFFS_BUF_PTR, reg_ptr);
}


void dynamoRIOTool::insert_update_buf_ptr(void *drcontext, instrlist_t *ilist, instr_t *where,
                                          reg_id_t reg_ptr, int adjust)
{
    instrlist_meta_preinsert(ilist, where, XINST_CREATE_add(drcontext, opnd_create_reg(reg_ptr),
                             OPND_CREATE_INT16(adjust)));
    dr_insert_write_raw_tls(drcontext, ilist, where, tls_seg,
                            tls_offs + MEMTRACE_TLS_OFFS_BUF_PTR, reg_ptr);
}


void dynamoRIOTool::bbl_entry(uint64_t id)
{
    dr_mutex_lock(dr_mutex_);
    assert(bbl_index_.find(id) != bbl_index_.end());
    assert(bbl_index_[id] < static_cast<int>(bbl_info_.size()));

    bbl_count_++;
    if ((bbl_count_ % 100000000) == 0) {
        Log::info() << "[DRTool] BBL count at " << bbl_count_;
    }
    // We check that:
    // 1 - We are in the proper thread for DynamoRIO.
    // 2 - MPI tracing is disabled or we are in a trceable MPI phase.
    // 3 - Program has started.
    // 4 - ROI detection is disabled or we are inside the ROI.
    // 5 - We are inside a task.
    // 6 - We are inside a traceable Nanos++ phase.

    bool trace_mem = (dr_get_current_drcontext() == thread_context_) and
            (not enable_mpi_ or tsmpi_is_traceable()) and
            started_ and
            (roi_ or not roi_detected_) and
            (not task_stack_.empty()) and
            (not phase_stack_.empty()) and
            phase_map_.trace(phase_stack_.top());
    if (trace_mem && prev_bbl_id_ != -1) {
        // unsigned index = bbl_index_[id];
        unsigned index = prev_bbl_id_;
        ts_trace_->add_bbl_trace(task_stack_.top(), sim::trace::bbl_id_t(index));
        if (pending_mem_) {
            std::cout << "[ERROR] Did not finish previous basic block memory instructions." << std::endl;
            std::cout << "[ERROR] Had " << pending_mem_ << " pending instructions." << std::endl;
        }
        pending_mem_ = bbl_info_[index].n_lds + bbl_info_[index].n_sts;
        task_stack_.top().n_instr += bbl_info_[index].n_instrs;
        total_instructions_ += n_instrs_[index];
        total_unidentified_instructions_ += bbl_info_[index].n_unid;
        total_detected_ld_ += bbl_info_[index].n_lds;
        total_detected_st_ += bbl_info_[index].n_sts;
        current_phase_data_.add_info(bbl_info_[index]);
    }
    mem_access();
    if (trace_mem) {
        int c = 0;
        while (pending_mem_ > 0) {
            ts_trace_->add_mem_trace(task_stack_.top(), mem_t(1));
            c++;
            pending_mem_--;
        }
        if (c) {
            std::cerr << "[DEBUG] Basic block " << bbl_index_[id]
                      << " missed " << c  << " memory references." << std::endl;
        }
        prev_bbl_id_ = bbl_index_[id];
    } else {
        prev_bbl_id_ = -1;
    }
    dr_mutex_unlock(dr_mutex_);
}


void dynamoRIOTool::memory_access(size_t address, int8_t size, bool write)
{
    if (pending_mem_ > 0 and task_stack_.size()) {
        assert(not task_stack_.empty());
        ts_trace_->add_mem_trace(task_stack_.top(), mem_t(address));
        pending_mem_--;
    }
}


void bbl_entry(uint64_t id)
{
    tool_.bbl_entry(id);
}


void mem_access()
{
    void *drcontext = dr_get_current_drcontext();
    assert(drcontext != nullptr);
    per_thread_t *data;
    mem_ref_t *mem_ref, *buf_ptr;

    data    = static_cast<per_thread_t*>(drmgr_get_tls_field(drcontext, tls_idx));
    if (data == nullptr) {
        // Prevents end of simulation crashes for certain applications.
        return;
    }
    buf_ptr = BUF_PTR(data->seg_base);

    for (mem_ref = reinterpret_cast<mem_ref_t*>(data->buf_base); mem_ref < buf_ptr; mem_ref++) {
        tool_.memory_access(reinterpret_cast<size_t>(mem_ref->addr), mem_ref->size, mem_ref->type == REF_TYPE_WRITE);
    }
    BUF_PTR(data->seg_base) = data->buf_base;
}

void dynamoRIOTool::nanos_wait_condition_entry(void *context, const std::string &function_name, void **user_data)
{
    size_t wd = current_wd_;
    *user_data = reinterpret_cast<void*>(wd);
}


void dynamoRIOTool::nanos_wait_condition_exit(void *context, const std::string &function_name, void *user_data)
{
    size_t data = (size_t)user_data;
    int new_wd_id = data;
    if (current_wd_  == 0) {
        set_wd(new_wd_id);
    }
}

void dynamoRIOTool::nanos_duplicate_wd_entry(void *context, const std::string &function_name, void **user_data)
{
    // C++ member function, first parameter is this
    void *new_wd_ptr = drwrap_get_arg(context, 1);
    // We store the data for the exit callback
    *user_data = new_wd_ptr;
}

void dynamoRIOTool::nanos_duplicate_wd_exit(void *context, const std::string &function_name, void *user_data)
{
    void *new_wd_ptr = user_data;

    nanos::WD *wd = *static_cast<nanos::WD**>(new_wd_ptr);
    unsigned new_wd_id = wd->getId();
    ts_trace_->add_wd_info(new_wd_id, {});
    app_pc funptr = (app_pc)((nanos::ext::SMPDD&)wd->getActiveDevice()).getWorkFct();

    unsigned task_type = task_index_[instrumented_functions_[funptr]];
    task_name_map_.insert(std::pair<unsigned, int16_t>(new_wd_id, task_type));
    //std::cout << "[DEBUG] Creating task " << new_wd_id << " at master task " << master_alias_ << " while at " << current_wd_ << std::endl;
    ts_trace_->add_phase_event(ompss::event_t(master_alias_, ompss::CREATE_TASK_EVENT, new_wd_id));
}

void dynamoRIOTool::nanos_wait_group_entry(void *context, const std::string &function_name, void **user_data)
{
    size_t wd = current_wd_;
    *user_data = reinterpret_cast<void*>(wd);
    rtn_entry(context, function_name, user_data, phase_map_["wg_wait_completion"]);
}

void dynamoRIOTool::nanos_wait_group_exit(void *context, const std::string &function_name, void *user_data)
{
    size_t data = (size_t)user_data;
    int new_wd_id = data;
    if (current_wd_  == 0) {
        set_wd(new_wd_id);
    }
    ts_trace_->add_phase_event(ompss::event_t(current_wd_, ompss::WAIT_GROUP_EVENT));
    rtn_exit(context, function_name, user_data, phase_map_["wg_wait_completion"]);
}

void dynamoRIOTool::nanos_wait_dependencies_entry(void *context, const std::string &function_name, void **user_data)
{
    size_t wd = current_wd_;
    *user_data = reinterpret_cast<void*>(wd);
    int num_deps = reinterpret_cast<size_t>(drwrap_get_arg(context, 0));
    nanos_data_access_internal_t *deps = static_cast<nanos_data_access_internal_t*>(drwrap_get_arg(context, 1));
    std::vector<ts::dep_t> dep_vector;
    dep_vector.reserve(num_deps);
    for (int i = 0; i < num_deps; i++) {
        dep_vector.push_back(deps[i]);
    }
    ts_trace_->add_wait_on_dependencies(current_wd_, dep_vector);
    rtn_entry(context, function_name, user_data, phase_map_["wait_on"]);
    num_deps_wd_[current_wd_] = num_deps;
}

void dynamoRIOTool::nanos_wait_dependencies_exit(void *context, const std::string &function_name, void *user_data)
{
    size_t data = (size_t)user_data;
    int new_wd_id = data;
    if (current_wd_  == 0) {
        set_wd(new_wd_id);
    }
    ts_trace_->add_phase_event(ompss::event_t(current_wd_, ompss::WAIT_ON_EVENT, num_deps_wd_[current_wd_]));
    rtn_exit(context, function_name, user_data, phase_map_["wait_on"]);
    assert(num_deps_wd_.find(current_wd_) != num_deps_wd_.end());
    num_deps_wd_.erase(current_wd_);
}

void dynamoRIOTool::nanos_submit(void *context, const std::string &function_name, void **user_data)
{
    void *wd_ptr = drwrap_get_arg(context, 0);
    size_t num_deps = reinterpret_cast<size_t>(drwrap_get_arg(context, 1));
    nanos_data_access_internal_t *deps = static_cast<nanos_data_access_internal_t*>(drwrap_get_arg(context, 2));

    nanos::WD *wd = static_cast<nanos::WD *>(wd_ptr);

    std::vector<ts::dep_t> dep_vector;
    for (unsigned i = 0; i < num_deps; i++) {
        dep_vector.push_back(ts::dep_t(deps[i]));
    }
    ts_trace_->add_wd_info(wd->getId(), {dep_vector});
    app_pc funptr = (app_pc)((nanos::ext::SMPDD&)wd->getActiveDevice()).getWorkFct();

    if (instrumented_functions_.find(funptr) == instrumented_functions_.end()) {
        // We make the check mandatory even without debug mode:
        dr_fprintf(STDOUT, "Could not find instrumented function in the dictionary.\n");
        dr_fprintf(STDOUT, "Address: 0x%x.\n", funptr);
        for (auto it = instrumented_functions_.begin(); it != instrumented_functions_.end(); it++) {
            dr_fprintf(STDOUT, "0x%x -> %s.\n", it->first, it->second.c_str());
        }
        dr_module_iterator_t *it = dr_module_iterator_start();
        bool finished = false;
        while (not finished and dr_module_iterator_hasnext(it)) {
            module_data_t *mod = dr_module_iterator_next(it);
            if (mod != NULL and dr_module_contains_addr(mod, funptr)) {
                std::cout << "Function is located on module "
                          << std::string(dr_module_preferred_name(mod)) << std::endl;
                finished = true;
                dr_symbol_export_iterator_t *it2 = dr_symbol_export_iterator_start(mod->handle);
                while (dr_symbol_export_iterator_hasnext(it2)) {
                    dr_symbol_export_t *symbol = dr_symbol_export_iterator_next(it2);
                    std::cout << std::string(symbol->name) << " 0x" << std::hex << std::setfill('0')
                              << reinterpret_cast<uint64_t>(symbol->addr) << std::dec << std::endl;
                    if (symbol != NULL and symbol->addr == funptr) {
                        std::cout << "Function is named " << std::string(symbol->name) << std::endl;
                        break;
                    }
                }
                dr_symbol_export_iterator_stop(it2);
            }
            dr_free_module_data(mod);
        }
        dr_module_iterator_stop(it);
        exit(-1);
    }

    if (task_index_.find(instrumented_functions_[funptr]) == task_index_.end()) {
        dr_fprintf(STDOUT, "Instrumented function %s is not listed as a task.\n",
                   instrumented_functions_[funptr].c_str());
        for (auto it = task_index_.begin(); it != task_index_.end(); it++) {
            dr_fprintf(STDOUT, "%d -> %s.\n", it->second, it->first.c_str());
        }
        exit(-1);
    }
    unsigned task_type = task_index_[instrumented_functions_[funptr]];
    used_ompss_functions_[instrumented_functions_[funptr]] = true;
    task_name_map_.insert(std::pair<unsigned, int16_t>(wd->getId(), task_type));

    rtn_entry(context, function_name, user_data, phase_map_["submit"]);
    ts_trace_->add_phase_event(ompss::event_t(current_wd_, ompss::CREATE_TASK_EVENT, wd->getId()));
}

void dynamoRIOTool::nanos_inline_WD_enter(void *context, const std::string &function_name, void **user_data)
{
    // Inline WD may happen after WD_enter, so the current_wd_ may already be properly set.
    // We store it at nanos_WD_enter
    nanos_WD_enter(context, function_name, user_data);
}

void dynamoRIOTool::nanos_inline_WD_exit(void *context, const std::string &function_name, void *user_data)
{
    // finishWork() is called to clean up the child WD if it finished, so no need to exit here
    // We just reenter the parent WD
    assert(inline_parent_.size() > 0);
    int next_wd_id = inline_parent_[current_wd_];
    inline_parent_.erase(current_wd_);
    set_wd(next_wd_id);

    assert(phase_stack_.empty() == true);

    // On coming back to a WD for which we have saved phases, we reenter at the same place
    auto saved_phases = saved_phases_map_.find(current_wd_);
    assert(saved_phases != saved_phases_map_.end());
    if (saved_phases == saved_phases_map_.end()) {
        std::cout << "[DRTool] [ERROR] Cannot find saved phases for WD " << current_wd_ << std::endl;
    }
    // Reenter previous phase: recover stack, its top will be used by start_phase
    phase_stack_.swap(saved_phases->second);
    saved_phases_map_.erase(saved_phases);
    if (phase_stack_.top() != ompss::WD_ID_PHASE) {
        // We do not start a phase if we are returning to WD_ID_PHASE as it is redundant.
        wd_counter_++;
        start_phase();
    }
}

void dynamoRIOTool::nanos_WD_enter(void *context, const std::string &function_name, void **user_data)
{
    void *wd_ptr = drwrap_get_arg(context, 0);
    nanos::WD *wd = static_cast<nanos::WD*>(wd_ptr);

    // Sometimes inlineWork may be called by switchTo: don't want to count that switch twice
    if (current_wd_ == wd->getId() and
            (function_name.compare("nanos::Scheduler::inlineWork") == 0)) {
        return;
    }

    // nanos_WD_enter might be instrumented before nanos_inline_WD_enter, so this check is necessary
    if (wd->getId() != current_wd_) {
        inline_parent_[wd->getId()] = current_wd_;
    }

    // Save state of WD before tracing stops so we can switch back to it later,
    // keep top to cleanly exit from current phase
    if (current_wd_ > 0) {
        end_phase();
        assert(saved_phases_map_.find(current_wd_) == saved_phases_map_.end());
        std::stack<unsigned> saved_phases;
        saved_phases.swap(phase_stack_);
        saved_phases_map_.insert({current_wd_, saved_phases});
    }

    set_wd(wd->getId());
    assert(phase_stack_.empty() == true);

    // Save the stack pointer for this WD
    nanos::ext::SMPDD& dd = static_cast<nanos::ext::SMPDD&>(wd->getActiveDevice());
    if (dd.hasStack()) {
        ts_trace_->set_task_stack_ptr(current_wd_, task_name_map_.at(current_wd_), dd.getState());
    } else {
        // Cross-platform name of stack pointer in machine contexnt dr_mcontext_t
        uint64_t stack_ptr = drwrap_get_mcontext_ex(context, DR_MC_CONTROL)->xsp;
        ts_trace_->set_task_stack_ptr(current_wd_, task_name_map_.at(current_wd_), reinterpret_cast<void*>(stack_ptr));
    }
    // On coming back to a WD for which we have saved phases, we reenter at the same place
    auto saved_phases = saved_phases_map_.find(current_wd_);

    if (saved_phases != saved_phases_map_.end()) {
        // Reenter previous phase: recover stack, its top will be used by start_phase
        phase_stack_.swap(saved_phases->second);
        saved_phases_map_.erase(saved_phases);
    } else {
        phase_stack_.push(ompss::WD_ID_PHASE);
    }
    start_phase();
}

void dynamoRIOTool::nanos_WD_exit(void *context, const std::string &function_name, void *user_data)
{
    end_phase();
    phase_stack_.pop();
    assert(phase_stack_.empty() == true);
}

void dynamoRIOTool::nanos_set_lock(void *context, const std::string &function_name, void **user_data)
{
    rtn_entry(context, function_name, user_data, phase_map_["set_lock"]);
    size_t address = reinterpret_cast<size_t>(drwrap_get_arg(context, 0));
    ts_trace_->add_phase_event(ompss::event_t(current_wd_, ompss::SET_LOCK, address));
}

void dynamoRIOTool::nanos_unset_lock(void *context, const std::string &function_name, void **user_data)
{
    rtn_entry(context, function_name, user_data, phase_map_["unset_lock"]);
    size_t address = reinterpret_cast<size_t>(drwrap_get_arg(context, 0));
    ts_trace_->add_phase_event(ompss::event_t(current_wd_, ompss::UNSET_LOCK, address));
}

void dynamoRIOTool::roi_begin(void *context, const std::string &function_name, void **user_data)
{
    roi_used_ = true;
    std::cout << "[DRTool] ROI entry at " << mpi_info_ << " by " << function_name << std::endl;
    std::cout << "[DRTool] ROI entry at the " << mpi_nanos_phases_ << " Nanos++ phase of the execution." << std::endl;
    roi_ = true;
    if (enable_mpi_ == false and tracing_finished_ == true) {
        tracing_finished_ = false;
        dr_mutex_unlock(tool_.dr_mutex_);
        dr_flush_region(0,-1);
        dr_mutex_lock(tool_.dr_mutex_);
    }
}

void dynamoRIOTool::roi_end(void *context, const std::string &function_name, void **user_data)
{
    std::cout << "[DRTool] ROI exit at " << mpi_info_ << " by " << function_name << std::endl;
    std::cout << "[DRTool] ROI exit at the " << mpi_nanos_phases_ << " Nanos++ phase of the execution." << std::endl;
    roi_ = false;
    if (enable_mpi_ == false and tracing_finished_ == false) {
        tracing_finished_ = true;
        dr_mutex_unlock(tool_.dr_mutex_);
        dr_flush_region(0,-1);
        dr_mutex_lock(tool_.dr_mutex_);
    }
}

void dynamoRIOTool::run_pre_instrument()
{
    if (no_nanos_) {
        dr_fprintf(STDOUT, "[DRTool] Executed without a Nanos++ run.\n");
        dr_fprintf(STDOUT, "[DRTool] DRTool trace file is %s.\n", ts_trace_filename_.c_str());
        ts_trace_ = new trace_t(ts_trace_filename_.c_str());
        task_names_.resize(1, "main");
        ts_trace_->add_task_name(0, task_names_[0]);
    } else {
        dr_fprintf(STDOUT, "[DRTool] OmpSs trace file is %s.\n", ompss_trace_filename_.c_str());
        dr_fprintf(STDOUT, "[DRTool] DRTool trace file is %s.\n", ts_trace_filename_.c_str());
        ompss::FileTrace<BinaryEventStream> ompss_trace(ompss_trace_filename_.c_str(), true);
        ts_trace_ = new trace_t(ts_trace_filename_.c_str());
        unsigned int number_of_task_types = ompss_trace.get_number_of_task_name();
        if (number_of_task_types == 0) {
            dr_fprintf(STDOUT, "[ERROR] Cannot read any Task from the Nanos++ trace.\n");
            exit(-1);
        } else {
            task_names_.resize(number_of_task_types);
            for (unsigned i = 0; i < number_of_task_types; i++) {
                task_names_[i] = ompss_trace.get_task_name(i);
                ts_trace_->add_task_name(i, task_names_[i]);
            }
        }
    }

    if (enable_mpi_ && (rank_ != static_cast<unsigned>(-1))) {
        unsigned pid = ::getpid();

        std::string output_pid_filename = ts_trace_filename_.substr(0, ts_trace_filename_.find_last_of('/') + 1) + "sve_pid_" + std::to_string(rank_) + ".txt";
        std::ofstream pid_file(output_pid_filename.c_str());
        pid_file << pid << std::endl;
        dr_fprintf(STDOUT, "[DRTool] Process ID = %d.\n", pid);
        pid_file.close();
    }

    // Add first null BBL
    bbl_t ts_bbl;
    ts_bbl.id_ = 0;
    bbl_info_.push_back(sim::trace::phase_data_t());
    n_instrs_.push_back(0);
    ts_trace_->add_bbl(ts_bbl);

    // Now, the tasks functions start and end:
    instrument("main",
               std::bind(&dynamoRIOTool::app_start, this, std::placeholders::_1,
                         std::placeholders::_2, std::placeholders::_3),
               std::bind(&dynamoRIOTool::app_end, this, std::placeholders::_1,
                         std::placeholders::_2, std::placeholders::_3));
    instrument(roi_string_ + "_begin",
               std::bind(&dynamoRIOTool::roi_begin, this, std::placeholders::_1,
                         std::placeholders::_2, std::placeholders::_3),
               NULL);

    instrument(roi_string_ + "_end",
               std::bind(&dynamoRIOTool::roi_end, this, std::placeholders::_1,
                         std::placeholders::_2, std::placeholders::_3),
               NULL);

    if (not no_nanos_) {
        int c = 1;
        // task_index_["__main__"] = c;
        task_index_["main"] = 0;
        for (auto it = task_names_.begin(); it != task_names_.end(); it++) {
            if (it->compare("main") == 0) {
                continue;
            }
            dr_fprintf(STDOUT, "[DRTool] Task %d is %s.\n", c, it->c_str());
            instrument(*it,
                       std::bind(&dynamoRIOTool::rtn_entry, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, ompss::USER_CODE_PHASE),
                       std::bind(&dynamoRIOTool::rtn_exit, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, ompss::USER_CODE_PHASE));
            task_index_[*it] = c++;
        }
        // First, functions from which we only instrument entry:
        const std::vector<std::string> switch_wd_functions_({"nanos::Scheduler::exitTo"});
        for (auto it = switch_wd_functions_.begin(); it != switch_wd_functions_.end(); it++) {
            instrument(*it,
                       std::bind(&dynamoRIOTool::nanos_WD_enter, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                       NULL);
        }
        instrument("nanos::Scheduler::finishWork",
                  NULL,
                  std::bind(&dynamoRIOTool::nanos_WD_exit, this, std::placeholders::_1,
                            std::placeholders::_2, std::placeholders::_3));

        instrument("nanos::Scheduler::inlineWork",
                   std::bind(&dynamoRIOTool::nanos_inline_WD_enter, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                   std::bind(&dynamoRIOTool::nanos_inline_WD_exit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3));


        instrument("waitOnCondition",
                   std::bind(&dynamoRIOTool::nanos_wait_condition_entry, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                   std::bind(&dynamoRIOTool::nanos_wait_condition_exit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3));

        instrument("duplicateWD",
                   std::bind(&dynamoRIOTool::nanos_duplicate_wd_entry, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                   std::bind(&dynamoRIOTool::nanos_duplicate_wd_exit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3));

        instrument("nanos_wg_wait_completion",
                   std::bind(&dynamoRIOTool::nanos_wait_group_entry, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                   std::bind(&dynamoRIOTool::nanos_wait_group_exit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3));

        instrument("nanos_wait_on",
                   std::bind(&dynamoRIOTool::nanos_wait_dependencies_entry, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                   std::bind(&dynamoRIOTool::nanos_wait_dependencies_exit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3));

        instrument("nanos_submit",
                   std::bind(&dynamoRIOTool::nanos_submit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                   std::bind(&dynamoRIOTool::rtn_exit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3, phase_map_["submit"]));

        instrument("nanos_set_lock",
                   std::bind(&dynamoRIOTool::nanos_set_lock, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                   std::bind(&dynamoRIOTool::rtn_exit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3, phase_map_["set_lock"]));

        instrument("nanos_unset_lock",
                   std::bind(&dynamoRIOTool::nanos_unset_lock, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3),
                   std::bind(&dynamoRIOTool::rtn_exit, this, std::placeholders::_1,
                             std::placeholders::_2, std::placeholders::_3, phase_map_["unset_lock"]));

        for (auto it = phase_map_.phase_names_.begin(); it != phase_map_.phase_names_.end(); it++) {
            std::string function_name = "nanos_" + it->first;
            instrument(function_name,
                       std::bind(&dynamoRIOTool::rtn_entry, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, it->second),
                       std::bind(&dynamoRIOTool::rtn_exit, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, it->second));
        }
    }
    if (enable_mpi_) {
        for (unsigned i = 0; i < mpi_name_list.size(); i++) {
            instrument(mpi_name_list[i],
                       std::bind(&dynamoRIOTool::mpi_entry, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, i),
                       std::bind(&dynamoRIOTool::mpi_exit, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, i));
        }
    }
}

void dynamoRIOTool::run_post_instrument()
{
    // Code here will be run after the initial DRTool call, but all the code might not have been instrumented yet.
}

void dynamoRIOTool::app_start(void *context, const std::string &function_name, void **user_data)
{
    if (started_) {
        dr_fprintf(STDOUT, "[DEBUG] Multiple calls to function main.\n");
        return;
    }
    dr_fprintf(STDOUT, "[DRTool] Application main starts.\n");
    started_ = true;
    if ((instrumented_.find("__parsec_roi_begin") != instrumented_.end() and
            instrumented_.find("__parsec_roi_end") != instrumented_.end()) or
             (instrumented_.find("__START_TRACE") != instrumented_.end() and
             instrumented_.find("__STOP_TRACE") != instrumented_.end())) {

        dr_fprintf(STDOUT, "[DRTool] ROI calls detected.\n");
        roi_detected_ = true;
    }
    set_wd(MASTER_WD_ID);
    task_name_map_[current_wd_] = 0;

    uint64_t stack_ptr = drwrap_get_mcontext_ex(context, DR_MC_CONTROL)->xsp;
    ts_trace_->set_task_stack_ptr(current_wd_, 0, reinterpret_cast<void*>(stack_ptr));
    // ts_trace_->add_wd_info(current_wd_, {});


    phase_stack_.push(ompss::WD_ID_PHASE);
    start_phase();
    rtn_entry(context, function_name, user_data, ompss::USER_CODE_PHASE);
    used_ompss_functions_[function_name] = true;
    if (enable_mpi_) {
        tsmpi_init(ts_trace_filename_.c_str());
    }
}

void dynamoRIOTool::app_end(void *context, const std::string &function_name, void *user_data)
{
    if (not started_) {
        dr_fprintf(STDOUT, "[DEBUG] Multiple exits from function main.\n");
        return;
    }
    dr_fprintf(STDOUT, "[DRTool] Application %s ends.\n", function_name.c_str());
    if (enabled_by_mpi_) {
        // Application ending with instrumentation enabled
        Log::info() << "[DRTool] app_end enabled_by_mpi.";
        rtn_exit(context, function_name, user_data, ompss::USER_CODE_PHASE);
        assert(not phase_stack_.empty() and phase_stack_.top() == ompss::WD_ID_PHASE);
        end_phase();
        assert(not phase_stack_.empty());
        phase_stack_.pop();
        assert(phase_stack_.empty());
    }
    started_ = false;
    if (enable_mpi_) {
        Log::info() << "[DRTool] app_end enable_mpi.";
        tsmpi_finish();
    }
}

void dynamoRIOTool::rtn_entry(void *context, const std::string &function_name, void **user_data, int phase_id)
{
    assert(not phase_stack_.empty());
    end_phase();
    phase_stack_.push(phase_id);
    start_phase();
}

void dynamoRIOTool::rtn_exit(void *context, const std::string &function_name, void *user_data, int phase_id)
{
    if (enabled_by_mpi_ and phase_stack_.empty()) {
        return;
    }
    // We close the phase, it should be at the top of the stack:
    assert(not phase_stack_.empty());
    if (phase_stack_.top() != static_cast<unsigned>(phase_id)) {
        std::cout << "[ERROR] Ending phase with id " << phase_stack_.top()
                  << " with an exit call for phase " << phase_id << std::endl;
        assert(phase_stack_.top() == static_cast<unsigned>(phase_id));
    }
    end_phase();

    assert(not phase_stack_.empty());
    phase_stack_.pop();

    // And now we restart the phase that was lower on the stack.
    assert(not phase_stack_.empty());
    start_phase();
}

void dynamoRIOTool::mpi_entry(void *context, const std::string &function_name, void **user_data, int mpi_id)
{
    // We check if the tool has the MPI Flag activated.
    if (not enable_mpi_) {
        return;
    }
    Log::info() << "[DRTool] mpi_entry call to " << function_name << " that has ID " << mpi_id  << " returned.";

    if (!phase_stack_.empty()) {
        // We empty the phase stack:
        end_phase();
        assert(phase_stack_.top() == ompss::USER_CODE_PHASE);
        phase_stack_.pop();
        end_phase();
        assert(phase_stack_.top() == ompss::WD_ID_PHASE);
        phase_stack_.pop();
        assert(phase_stack_.empty());
    } else {
        std::cout << "[ERROR] Entering MPI_entry  at " << function_name << " without a phase_Stack." << std::endl;
        std::cout << "[ERROR] Current phase data " << current_phase_data_ << std::endl;
        std::cout << "[ERROR] Current wd = " << current_wd_ << std::endl;
        std::abort();
    }
    enabled_by_mpi_ = false;
    set_wd(0);
}

void dynamoRIOTool::mpi_exit(void *context, const std::string &function_name, void *user_data, int mpi_id)
{
    // We check if the tool has the MPI Flag activated.
    if (not enable_mpi_) {
        return;
    }

    assert(phase_stack_.empty());
    Log::info() << "[DRTool] mpi_exit Call to " << function_name << " that has ID " << mpi_id  << " returned.";
    // We flush the File for all contents
    ts_trace_->flush_wd_contents();

    enabled_by_mpi_ = true;
    // We get a new master alias and set it.
    tsmpi_new_phase(mpi_id, function_name);
    if (not no_nanos_) {
        master_alias_ = mpi_info_.wd_id;
    } else {
        master_alias_ = alias_c_++;
    }
    set_wd(MASTER_WD_ID);

    task_name_map_.insert(std::pair<unsigned, int16_t>(current_wd_, 0));
    ts_trace_->add_wd_info(current_wd_, {});
    ts_trace_->set_task_stack_ptr(current_wd_, 0, reinterpret_cast<void*>(0));
    // We set the initial phases on the stack.
    phase_stack_.push(ompss::WD_ID_PHASE);
    start_phase();
    phase_stack_.push(ompss::USER_CODE_PHASE);
    start_phase();
}

void dynamoRIOTool::tsmpi_init(const std::string &trace_name) {
    std::cout << "[DRTool] MPI Init." << std::endl;
    // Get experiment path
    std::string exp_path = trace_name.substr(0, trace_name.find_first_of('/'));
    std::string suffix = trace_name.substr(trace_name.size() - 9, 6);
    Log::info() << "[DRTool] Using " << exp_path << " as experiment path and " << suffix << " as suffix.";
    std::string mpiphases_name = trace_name + ".mpiphases";
    mpi_relation_file_.open(mpiphases_name.c_str());
    if (not no_nanos_ and not mpi_relation_file_.good()) {
        std::cout << "[DRTool] Problems opening " << mpiphases_name << std::endl;
        exit(-1);
    } else if (no_nanos_) {
        std::cout << "[DRTool] No Nanos++ run." << std::endl;
    } else {
        std::cout << "[DRTool] Successfully opened " << mpiphases_name << std::endl;
    }
    std::string function_name = "INIT";
    // Read the first line in mpiphases to initiate the id:current_wd:is_nanos:is_mem:mpi_id:mpi_name
    tsmpi_new_phase(1, function_name);
}

void dynamoRIOTool::tsmpi_finish() {
    // Just close the mpiphase file
    if (not no_nanos_) {
       mpi_relation_file_.close();
    }
}

void dynamoRIOTool::tsmpi_new_phase(unsigned mpi_id, const std::string &function_name) {
    // Read mpiphases and not update master..?
    if (not no_nanos_) {
        mpi_relation_file_ >> mpi_info_;
        mpi_nanos_phases_ += (mpi_info_.is_nanos == true);
        if (tracing_finished_ && mpi_info_.is_mem) {
            std::cout << "[DRTool] Started instrumenting code at entry " << mpi_info_ << " WD counter : " << wd_counter_ << std::endl;
            tracing_finished_ = false;
            dr_mutex_unlock(tool_.dr_mutex_);
            dr_flush_region(0,-1);
            dr_mutex_lock(tool_.dr_mutex_);
        } else if (tracing_finished_ == false and mpi_info_.is_nanos and mpi_info_.is_mem == false) {
            std::cout << "[DRTool] Stopped instrumenting code at entry " << mpi_info_ << " WD counter : " << wd_counter_ << std::endl;
            tracing_finished_ = true;
            dr_mutex_unlock(tool_.dr_mutex_);
            dr_flush_region(0,-1);
            dr_mutex_lock(tool_.dr_mutex_);
        } else if (mpi_info_.is_mem) {
            std::cout << "[DRTool] Tracing phase " << mpi_info_ << " WD counter : " << wd_counter_ << std::endl;
        } else {
            std::cout << "[DRTool] Not tracing phase " << mpi_info_ << " WD counter : " << wd_counter_ << std::endl;
        }
    } else {
        mpi_nanos_phases_++;
        std::cout << "ITER:" << current_wd_ << ":1:1:" << mpi_id << ":" << function_name << std::endl;
    }
}

bool dynamoRIOTool::tsmpi_is_traceable() {
    if (no_nanos_) {
        return true;
    } else {
        return mpi_info_.is_nanos and mpi_info_.is_mem;
    }
}

void dynamoRIOTool::start_phase()
{
    assert(started_);
    assert(not phase_stack_.empty());

    // We add the phase:
    ts_trace_->add_phase_event(ompss::event_t(current_wd_, ompss::PHASE_EVENT, phase_stack_.top()));

    // If we don't want to do a detailed (memory) trace, we exit now.
    if (phase_map_.trace(phase_stack_.top()) == false) {
        return;
    }
    unsigned name_id = task_name_map_.at(current_wd_);
    task_stack_.push(ts_trace_->add_detailed_trace(current_wd_, name_id));
}

void dynamoRIOTool::end_phase()
{
    assert(started_ == true);
    assert(phase_stack_.empty() == false);

    if (phase_map_.trace(phase_stack_.top()) == false) {
        return;
    }
    assert(not task_stack_.empty());
    ts_trace_->add_bbl_trace(task_stack_.top(), bbl_id_t(0));
    ts_trace_->set_task_instructions(task_stack_.top().wd_id, task_stack_.top().n_instr);

    mem_access();
    int c = 0;
    while (pending_mem_ > 0) {
        ts_trace_->add_mem_trace(task_stack_.top(), mem_t(1));
        c++;
        pending_mem_--;
    }
    if (c) {
        std::cerr << "[DEBUG] Basic block at end of phase missed " << c << " memory references." << std::endl;
    }

    ts_trace_->add_mem_trace(task_stack_.top(), mem_t(0));
    assert(task_stack_.empty() == false);
    ts_trace_->add_phase_data(task_stack_.top(), current_phase_data_);
    current_phase_data_.clear();
    task_stack_.pop();
}

void dynamoRIOTool::instrument(const std::string &function_name,
        std::function<void(void*, const std::string &, void**)> entry_callback,
        std::function<void(void*, const std::string &, void*)> exit_callback)
{
    if (callback_map_.find(function_name) != callback_map_.end()) {
        // To avoid some collisions on special nanos_ functions.
        return;
    }
    callback_map_.insert({function_name, {entry_callback, exit_callback}});
    instrumentation_pending_.insert({function_name});
}

void dynamoRIOTool::usage()
{
    dr_fprintf(STDOUT, "[ERROR] Tool expected parameters are:\n");
    dr_fprintf(STDOUT, "        -i $ompss_trace -o $ts_trace [-m]\n");
}

void dynamoRIOTool::parse_args(int argc, char *argv[])
{
    roi_string_ = "__parsec_roi";
    rank_ = 1;
    int res;
    while ((res = ::getopt(argc, argv, "nmi:o:r:f:")) != -1) {
        char c = static_cast<char>(res);
        switch(c) {
        case 'i':
            ompss_trace_filename_ = std::string(optarg);
            break;
        case 'o':
            ts_trace_filename_ = std::string(optarg);
            break;
        case 'm':
            enable_mpi_ = true;
            break;
        case 'n':
            no_nanos_ = true;
            break;
        case 'r':
            rank_ = std::atoi(optarg);
            break;
        case 'f':
            roi_string_ = std::string(optarg);
            std::cout << "[DEBUG] Using " << roi_string_ << "_begin and "
                      << roi_string_ << "_end to mark Region of Interest." << std::endl;
            break;
        default:
            dr_fprintf(STDOUT, "[ERROR] Unknown symbol %c.\n", c);
            usage();
            exit(1);
        }
    }
}

void event_thread_init(void *drcontext)
{
    if (tool_.thread_context_ == 0) {
        tool_.thread_context_ = drcontext;
    }
    per_thread_t *data = static_cast<per_thread_t*>(dr_thread_alloc(drcontext, sizeof(per_thread_t)));
    DR_ASSERT(data != NULL);
    drmgr_set_tls_field(drcontext, tls_idx, data);

    /* Keep seg_base in a per-thread data structure so we can get the TLS
     * slot and find where the pointer points to in the buffer.
     */
    data->seg_base = static_cast<byte*>(dr_get_dr_segment_base(tls_seg));
    data->buf_base = static_cast<mem_ref_t*>(dr_raw_mem_alloc(MEM_BUF_SIZE, DR_MEMPROT_READ | DR_MEMPROT_WRITE, NULL));
    DR_ASSERT(data->seg_base != NULL && data->buf_base != NULL);
    /* put buf_base to TLS as starting buf_ptr */
    BUF_PTR(data->seg_base) = data->buf_base;
}

void event_thread_exit(void *drcontext)
{
    per_thread_t *data;
    mem_access(); /* dump any remaining buffer entries */
    data = static_cast<per_thread_t*>(drmgr_get_tls_field(drcontext, tls_idx));
    dr_raw_mem_free(data->buf_base, MEM_BUF_SIZE);
    dr_thread_free(drcontext, data, sizeof(per_thread_t));
}

std::string dynamoRIOTool::generate_random_trace_name()
{
    std::string trace_name = "tasksim_trace_XXXXXX.ts.streaminfo";
    char *tmp_filename = reinterpret_cast<char*>(malloc(sizeof(char)*256));
    snprintf(tmp_filename, 256, "%s",trace_name.c_str());
    int fd = mkstemps(tmp_filename, 14);
    if (fd < 0) {
        dr_fprintf(STDOUT, "[DRTool] Unable to create temporary trace file %s.\n", tmp_filename);
        char* error_msg = reinterpret_cast<char*>(sizeof(char)*512);
        perror(error_msg);
        dr_fprintf(STDOUT, "[DRTool] Error #%d, %s.\n,", errno, error_msg);
        free(error_msg);
        free(tmp_filename);
        exit(-1);
    }
    close(fd);
    trace_name = std::string(tmp_filename);
    free(tmp_filename);
    return trace_name;
}

}  // namespace dr
}  // namespace trace
}  // namespace sim


DR_EXPORT void
dr_client_main(client_id_t id, int argc, const char *argv[])
{
    dr_fprintf(STDOUT, "[DRTool] Starting TaskSim tool.\n");
    dr_fprintf(STDOUT, "[DRTool] Parsing arguments.\n");
    sim::trace::dr::tool_.parse_args(argc, (char**)argv);
    dr_set_client_name("TaskSim OmpSs tracing test",
                       "tasksim@bsc.es");
    // We initialize dynamoRIO
    drmgr_init();
    drwrap_init();
    drsym_init(0);
    // We register the repeated string instruction alteration rutine.
    drmgr_register_bb_app2app_event(::sim::trace::dr::event_bb_app2app, NULL);
    // We register the Basic block instrumentation function:
    drmgr_register_bb_instrumentation_event(sim::trace::dr::event_basicblock_analysis,
                                            sim::trace::dr::event_bb_insert, NULL);
    // We register the application exit event function.
    dr_register_exit_event(sim::trace::dr::event_exit);
    drmgr_register_module_load_event(sim::trace::dr::module_load_event);

    sim::trace::dr::tool_.run_pre_instrument();

    sim::trace::dr::tls_idx = drmgr_register_tls_field();
    DR_ASSERT(sim::trace::dr::tls_idx != -1);
    if (!dr_raw_tls_calloc(&sim::trace::dr::tls_seg,
                           &sim::trace::dr::tls_offs,
                           sim::trace::dr::MEMTRACE_TLS_COUNT, 0)) {
        DR_ASSERT(false);
    }
    drreg_options_t ops = {sizeof(ops), 3, false};
    if (drreg_init(&ops) != DRREG_SUCCESS || !drutil_init()) {
        DR_ASSERT(false);
    }
    sim::trace::dr::tool_.run_post_instrument();
    drmgr_register_thread_init_event(sim::trace::dr::event_thread_init);
    drmgr_register_thread_exit_event(sim::trace::dr::event_thread_exit);
    sim::trace::dr::tool_.dr_mutex_ = dr_mutex_create();
}
