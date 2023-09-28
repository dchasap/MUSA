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
#include <cctype>
#include <unistd.h>
#include <dirent.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <map>

#include "mpi.h"

#include "core/trace/dr/types.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/types.h"
#include "core/utils/CheckTrace.h"
#include "core/utils/Log.h"
#include "core/tools/ompss_util.h"

using sim::utils::Log;

int execute(std::vector<std::string> &command) {
    const char** c_command = new const char*[command.size() + 1];
    for(unsigned i = 0; i < command.size(); i++) {
        c_command[i] = command[i].c_str();
    }
    std::string full_command;

    for (auto &word : command) {
        full_command = full_command + " " + word;
    }
    std::cout  << "Executing:" << full_command << std::endl;
    Log::info() << "Executing:" << full_command;
    c_command[command.size()] = nullptr;
    pid_t pid;
    int status;
    if ((pid = fork()) == 0) {
        /* Child process */
        execv(c_command[0], (char**)c_command);
    } else if (pid < 0) {
        std::cout << "[ERROR] Could not execute ArmIE pluggin." << std::endl;
    } else {
        std::cout << "[INFO] Child is generating the trace in process with ID: " << pid << std::endl;
    }

    wait(&status);
    if (WEXITSTATUS(status) != 0) {
        err(1, "[ERROR] Nanos++ instrumented execution exited with error");
    }
    delete[](c_command);
    return pid;
}

void create_temporal_trace(char *filename, const char *base_trace_name) {
    /* Generate tmp file : mkstemps opens it, so the unique file name is then reserved. */
    snprintf(filename, 255, "%s_XXXXXX.ts.streaminfo", base_trace_name);
    int fd = mkstemps(filename, 14);
    if (fd < 0) {
        std::cerr << "[OMPSS ERROR] Unable to create temporary trace file " << filename << std::endl;
        std::cerr << fd << " " << errno << std::endl;
        char error_msg[512];
        perror(error_msg);
        std::cerr << error_msg << std::endl;
        exit(-1);
    }
    close(fd);
}

void execute_ompss(int argc, char * const argv[], const char *tmp_file_name)
{
    int status;
    pid_t pid;
    if ((pid = fork()) == 0) {
        std::cout << "[OMPSS] Launching instrumented application" << std::endl;
        setenv("OMPSS_TRACE_FILE", tmp_file_name, 1);
        // Get NX_ARGS from environment and add tracing specific flags
        char *nx_args;
        nx_args = getenv("NX_ARGS");
        std::string nx_args_string;
        if (nx_args != NULL) {
            nx_args_string = std::string(nx_args);
        }
        nx_args_string += " --instrument-default=all --disable-ut --smp-workers=1 --throttle-upper=9999999 --instrumentation=nextsim";
        setenv("NX_ARGS", nx_args_string.c_str(), 1);
        std::string instrumented_binary(std::string(argv[0]) + "_instr");
        std::cout << "[OMPSS] Executing:";
        std::cout << "OMPSS_TRACE_FILE=" << tmp_file_name << " ";
        std::cout << "NX_ARGS=\"" << nx_args_string << "\"" << std::endl;
        std::cout << "[DEBUG] command size " << argc << std::endl;
        for (int i = 0; i < argc; i++) {
            std::cout << " " << argv[i];
        }
        std::cout << std::endl;
        execv(instrumented_binary.c_str(), argv);

        std::cerr << "[OMPSS ERROR] Unable to execute: ";
        for (int i = 0; i < argc; i++) {
            std::cerr << argv[i] << " ";
        }
        std::cerr << std::endl;
        std::cerr << "Error number " << errno << " message: " << strerror(errno) << std::endl;
        exit(-1);
    } else if (pid < 0) {
        std::cerr << "[OMPSS ERROR] Unable to create child process" << std::endl;
        exit(-1);
    }

    wait(&status);
    if (WEXITSTATUS(status) != 0) {
        err(1, "[ERROR] Nanos++ instrumented execution exited with error");
    }
    std::cout << "[OMPSS] Run-time trace generated" << std::endl;
}

int double_dash_position(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--") == 0) {
            return i;
        }
    }
    return -1;
}
void usage() {

}

int main(int argc, char* argv[])
{
    int rank = 0, size = 0;
    Log::debug() << "MPI_Init.";
    PMPI_Init(&argc, &argv);
    Log::debug() << "MPI_COMM_RANK";
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Log::debug() << "MPI_COMM_SIZE";
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    bool no_nanos = false;
    unsigned vector_length = 256;
    int res;
    std::string redux_file;
    while ((res = ::getopt(argc, argv, "nv:f:")) != -1) {
        char c = static_cast<char>(res);
        if (c == 'n') {
            no_nanos = true;
        } else if (c == 'f') {
            redux_file = std::string(optarg);
        } else if (c == 'v') {
            vector_length = std::atoi(optarg);
        } else {
            std::cout << "[ERROR] Unknown symbol " << c << "." << std::endl;
            usage();
            exit(1);
        }
    }
    int double_dash_pos = double_dash_position(argc, argv);
    if (double_dash_pos == -1) {
        std::cout << "[ERROR] Benchmark command not found." << std::endl;
        exit(-1);
    }
    std::string benchmark_name(argv[double_dash_pos+1]);
    benchmark_name = benchmark_name.substr(benchmark_name.find_last_of("\\/") + 1);
    if (std::getenv("TS_ARMIE_PATH") == nullptr) {
        std::cout << "[ERROR] Environment variable TS_ARMIE_PATH is not set." << std::endl;
        exit(-1);
    }
    std::string armie_path(std::getenv("TS_ARMIE_PATH"));
    std::string drrun = armie_path + "/bin64/drrun";
    std::string armie_tool = armie_path + "/lib64/release/libmemtrace_sve_" + std::to_string(vector_length) + ".so";
    std::string client_flag("-client");
    if (std::getenv("TASKSIM_HOME") == nullptr) {
        Log::error() << "TASKSIM_HOME not set.";
        exit(-1);
    }
    std::string tasksim_home(std::getenv("TASKSIM_HOME"));
    std::string ts_tool(tasksim_home + "/lib/libompss-drtrace.so");
    std::string merge_tool(tasksim_home + "/bin/merge_sve_memtrace");

    std::string nanos_trace;
    if (std::getenv("MUSA_EXP_NAME") == nullptr) {
        Log::error() << "Could not read environment variable MUSA_EXP_NAME.";
    }

    std::string initial_trace;

    if (no_nanos) {
        nanos_trace = "empty.ts";
        initial_trace = "intermediate_" + benchmark_name + "_proc_" + std::to_string(rank) + "_" + std::to_string(vector_length) + ".ts";
    } else {
        std::string trace_suffix;
        // Here we get the Nanos++ intermediate name from the redux file.
        int l_index, l_size, l_procid;
        std::string l_nodename, l_suffix;
        std::cout << "Opening " << redux_file << std::endl;
        std::ifstream in(redux_file);
        while (in >> l_index >> l_size >> l_procid >> l_nodename >> l_suffix) {
            std::cout << l_index << " " << l_size << " " << l_procid << " " << l_nodename << " " << l_suffix << std::endl;
            if (l_index == rank+1) {
                // This is our line:
                trace_suffix = l_suffix;
            }
        }
        if (trace_suffix.size() == 0) {
            Log::error() << "Could not find the suffix for rank " << rank << " of " << size;
        }
        // In file name =  ${MUSA_EXP_NAME}/xxtemp//tmp/file${sufix}
        // Out file name = ${MUSA_EXP_NAME}/${APPNAME}_proc_${sufix}.ts

        nanos_trace = std::string(std::getenv("MUSA_EXP_NAME")) + "/xxtemp/tmp/file" + trace_suffix;
        initial_trace = std::string(std::getenv("MUSA_EXP_NAME")) + "/" + benchmark_name + "_proc_" + trace_suffix + ".ts";
    }

    std::string final_trace(benchmark_name + "_proc_" + std::to_string(rank) + "_" +  std::to_string(vector_length) + ".ts");
    std::string ts_tool_args;
    if (no_nanos) {
        ts_tool_args = std::string("-n -o " + initial_trace);
    } else {
        ts_tool_args = std::string("-i " + nanos_trace + " -o " + initial_trace);
    }
    // std::vector<std::string> dr_args({"-unsafe_build_ldstex", "-max_bb_instrs", "32", "-max_trace_bbs", "4"});

    std::vector<std::string> command({drrun,  "-max_bb_instrs", "32", "-max_trace_bbs", " 4",
            "-unsafe_build_ldstex", client_flag, armie_tool, "0", "\"\"", client_flag, ts_tool, "1", ts_tool_args});
    for (int i = double_dash_pos; i < argc; i++) {
        command.push_back(std::string(argv[i]));
    }

    // Get NX_ARGS from environment and add tracing specific flags
    char *nx_args = getenv("NX_ARGS");
    std::string nx_args_string;
    if (nx_args != NULL) {
        nx_args_string += nx_args;
    }
    nx_args_string += " --disable-ut --smp-workers=1 --throttle-upper=9999999";
    setenv("NX_ARGS", nx_args_string.c_str(), 1);

    execute(command);
    sim::tools::merge_traces<sim::trace::BinaryEventStream, sim::trace::SelectedEventStream,
                    sim::trace::selectedtrace::TraceInternal>(nanos_trace.c_str(), initial_trace.c_str());

    std::cout << "[INFO] SVE and TaskSim traces generated." << std::endl;
    /*
    std::string log_file("sve-memtrace." + benchmark_name + "." + std::to_string(pid) + ".log");
    std::vector<std::string> merge_command({merge_tool, "-i", initial_trace, "-o", final_trace, "-l", log_file});

    if (no_nanos) {
        merge_command.push_back("-n");
    }
    execute(merge_command);
    */
    MPI_Finalize();
}

