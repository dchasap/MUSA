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


#include <err.h>
#include <sys/wait.h>
#include <errno.h>
#include <cctype>
#include <string>
#include <utility>

#include "core/tools/ompss_util.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/ompss/Trace.h"
#include "core/trace/ompss/types.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/types.h"

static void usage(char * const argv0, int exit_val)
{
    std::cout << "Usage: " << argv0 << " [-out trace_name] program [program args]" << std::endl;
    std::cout << "\tWhere 'program' is an executable, and 'program'_instr is its version "
              << "compiled with instrumentation support." << std::endl;
    std::cout << "\t-out trace_name       Sets the output trace's name to trace_name (default is program.ts)." << std::endl;
    std::cout << std::endl;
    std::cout << "Environment variables:" << std::endl;
    std::cout << "\tDR_PATH, DRTOOL_PATH: To enable DynamoRIO tracing, set respectively to dynamorio's "
              << "drrun and tasksim's libompss-drtrace.so paths." << std::endl;
    std::cout << "\tNX_ARGS               : Arguments to the nanox runtime for both nextstim and PinTool phases."
              << std::endl;
    std::cout << "\tDR_ARGS               : Additional arguments to DynamoRIO (see drrun --help)." << std::endl;
    exit(exit_val);
}

namespace sim {
namespace tools {

static void execute_dr(int argc, char * const argv[], const char *out_file_name)
{
    const char *c_flag = "-c";
    const char *o_flag = "-o";
    const char *n_flag = "-n";
    const char *dash_flag = "--";
    const char *dr_path = getenv("DR_PATH");
    if (dr_path == NULL) {
        std::cerr << "[OMPSS ERROR] Cannot find DynamoRIO path, please set DR_PATH" << std::endl;
        return;
    }

    const char *tool_path = getenv("DRTOOL_PATH");
    if (tool_path == NULL) {
        std::cerr << "[OMPSS ERROR] Cannot find drtool path, please set DRTOOL_PATH" << std::endl;
        return;
    }

    int status;
    pid_t pid;

    std::cout << "[OMPSS] Launching dynamoRIO Tool" << std::endl;

    if ((pid = fork()) == 0) {
        /* Child process */
        // Get NX_ARGS from environment and add tracing specific flags
        char *nx_args = getenv("NX_ARGS");
        std::string nx_args_string;
        if (nx_args != NULL) {
            nx_args_string += nx_args;
        }
        nx_args_string += " --disable-ut --smp-workers=1 --throttle-upper=9999999";
        setenv("NX_ARGS", nx_args_string.c_str(), 1);

        /* optional extra args */
        char *dr_args = getenv("DR_ARGS");
        std::string dr_args_string;
        if (dr_args != NULL) {
            dr_args_string += dr_args;
        }
        dr_args_string += " -max_bb_instrs 512";

        int max_extra_words = 1 + std::count_if(dr_args_string.begin(), dr_args_string.end(),
                [] (char c) {return std::isspace((int)c);} );

        char * argv_dr[8 + argc + max_extra_words];
        int nparams = 0;
        /* DynamoRIO Executable */
        alloc_and_copy(argv_dr, nparams++, dr_path);

        /* DR_ARGS */
        std::istringstream dr_args_oss(dr_args_string);
        std::string aux;
        while (dr_args_oss >> aux) {
            alloc_and_copy(argv_dr, nparams++, aux.c_str());
        }
        /* DynamoRIO Tool */
        alloc_and_copy(argv_dr, nparams++, c_flag);
        alloc_and_copy(argv_dr, nparams++, tool_path);
        /* TS trace */
        alloc_and_copy(argv_dr, nparams++, o_flag);
        alloc_and_copy(argv_dr, nparams++, out_file_name);
        alloc_and_copy(argv_dr, nparams++, n_flag);

        alloc_and_copy(argv_dr, nparams++, dash_flag);

        /* Application and parameters */
        for (int i = 0; i < argc; i++) {
            argv_dr[nparams++] = argv[i];
        }
        argv_dr[nparams] = NULL;

        std::cout << "[OMPSS] Executing:";
        for (int i = 0; i < nparams; i++) {
            std::cout << " " << argv_dr[i];
        }
        std::cout << std::endl;

        execv(argv_dr[0], argv_dr);
        std::cerr << "[OMPSS ERROR] Unable to execute: ";
        for (int i = 0; i < nparams; i++) {
            std::cerr << argv_dr[i] << " ";
        }
        std::cerr << std::endl;
        exit(-1);
    } else if (pid < 0) {
        std::cerr << "[OMPSS ERROR] Unable to create child process" << std::endl;
        exit(-1);
    }

    wait(&status);
    if (WEXITSTATUS(status) != 0) {
        err(1, "[ERROR] DynamoRIO tool exited with error");
    }
    std::cout << "[OMPSS] Instruction trace generated" << std::endl;
}

}  // namespace tools
}  // namespace sim

int main(int argc, char * const argv[])
{
    if (argc <= 1) {
        usage(argv[0], -1);
    }

    /* Generate tmp file */
    char *base_trace_name = NULL;
    int opt = 0;

    /* 'pedestrian' parsing of options so we don't have to look at the whole 'command line' that follows */
    while (++opt < argc - 1) {
        if (std::string("-h").compare(argv[opt]) == 0 || std::string("--help").compare(argv[opt]) == 0) {
            usage(argv[0], 0);
        } else if (std::string("-out").compare(argv[opt]) == 0) {
            if (opt + 1 < argc-1) {
                base_trace_name = argv[++opt];
            } else {
                usage(argv[0], -1);
            }
        } else {
            break;
        }
    }

    char * const * command_line_v = &argv[opt];
    int command_line_c = argc - opt;
    if (base_trace_name == NULL) {
        base_trace_name = argv[opt];
    }

    /* Generate the output file name */
    std::string output_file_name(std::string(base_trace_name) + ".ts");

    /* Check if the trace already exists */
    if (sim::tools::file_exists(output_file_name + ".streaminfo")) {
        std::cerr << "[OMPSS ERROR] Trace " << output_file_name << " already exists." << std::endl;
        std::cerr << "[OMPSS ERROR] Please, delete all trace files before creating a new trace" << std::endl;
        exit(-1);
    }

    /* Launch DynamoRIO tool (if environment variables have been set up */
    sim::tools::execute_dr(command_line_c, command_line_v, output_file_name.c_str());

    std::cout << "[NONANOS] DynamoRIO tool generated." << std::endl;
    /* We need to translate ompss::events into ts::events. */
    sim::trace::ts::FileTrace<sim::trace::SelectedEventStream,
            sim::trace::selectedtrace::TraceInternal> trace(output_file_name.c_str());

    sim::trace::ompss::event_t ompss_event;                                            
    while (trace.get_next_phase_event(ompss_event)) {
        unsigned wd_id = ompss_event.wd_id_;
        int subtrace_id = -1;
        if (ompss_event.value_ == sim::trace::ompss::phase_id_t::USER_CODE_PHASE){
            subtrace_id = trace.get_task_trace_id_by_wd_id(wd_id);
        }
        trace.add_task_event(wd_id, sim::trace::ts::PHASE,
                    sim::trace::ts::phase_t({static_cast<unsigned>(ompss_event.value_),
                    subtrace_id, 1}));
    }
    std::cout << "[NONANOS] Tirace finalized." << std::endl;
}
