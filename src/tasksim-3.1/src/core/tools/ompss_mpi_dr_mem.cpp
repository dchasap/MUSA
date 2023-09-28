/*
 * Copyright - Barcelona Supercomputing Center.
 * All rights reserved.
 *
 * Redistribution in source and binary forms, with or without
 * modification, is not permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */



#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mpi.h>
#include <fcntl.h>

#include <cassert>
#include <vector>
#include <queue>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>
#include <string>

// Wrapper informing about MPI initialization.
extern "C" {
#include "core/utils/tsmpi/libtsmpi_extern-impl.h"
}
int ts_rank = 0;
int num_procs = 0;

#include "core/trace/ompss/Trace.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/intel/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/rle/Trace.h"

#include "core/utils/Log.h"


using sim::utils::Log;

inline
static void alloc_and_copy(char * argv[], unsigned i, const char *src)
{
    argv[i] = reinterpret_cast<char*>(malloc(std::strlen(src) + 1));
    strncpy(argv[i], src, std::strlen(argv[i]));
}


static void execute_pin_mpi(int argc, char * const argv[],
                            std::vector<std::string> &tmp_filenames,
                            std::vector<std::string> &out_filenames)
{
    int size = tmp_filenames.size();
    std::cout << "[OMPSS_DR] Size is = " << size << std::endl;
    char *line;
    std::string final_arg;
    const char *phase_wrapper       = getenv("TS_PHASE_WRAPPER");

    const char *dr_path = getenv("DR_PATH");
    if (dr_path == NULL) {
        std::cout << "[OMPSS_MPI_MEM] DR_PATH must be defined." << std::endl;
        exit(-1);
    }
    const char *tool_path = getenv("DRTOOL_PATH");
    if (tool_path == NULL) {
        std::cout << "[OMPSS_MPI_MEM] DRTOOL_PATH must be defined." << std::endl;
        exit(-1);
    }
    char *dr_args = getenv("DR_ARGS");
    std::string dr_args_string;
    std::vector<std::string> v_dr_args;
    int max_extra_words = 0;
    if (dr_args != NULL) {
        std::cout << "[OMPSS_DR] Got DR_ARGS " << std::string(dr_args) << std::endl;
        dr_args_string = std::string(dr_args);
        max_extra_words = 1 + std::count_if(dr_args_string.begin(), dr_args_string.end(),
                [] (char c) {return std::isspace(static_cast<int>(c));});
        char *dr_word = strtok(dr_args, " \t\n\v\f\r");
        while (dr_word != NULL) {
            v_dr_args.emplace_back(dr_word);
            dr_word = strtok(NULL, " \t\n\v\f\r");
        }
        if (max_extra_words == 1) {
            max_extra_words = 0;
        }
    }

    std::string env3(std::string("NX_ARGS=--smp-workers=1 --disable-ut --throttle-upper=9999999 \nLD_PRELOAD=")
            + phase_wrapper);

    const char *c_flag = "-c";
    const char *i_flag = "-i";
    const char *o_flag = "-o";
    const char *m_flag = "-m";
    const char *dash_flag = "--";
    const char *env_code = "env";

    std::cout << "[OMPSS MPI MEM] Launching instrumented application" << std::endl
              << "[OMPSS MPI MEM] DR_PATH = " << dr_path << std::endl
              << "[OMPSS MPI MEM] DRTOOL_PATH = " << tool_path << std::endl
              << "[OMPSS MPI MEM] NX_ARGS " << std::endl
              << "{" << std::endl
              << env3 << std::endl
              << "}" << std::endl;

    //  This requires the utilization of MPI_Comm_spawn_multiple instead of MPI_Comm_Spawn

    char **array_of_commands;
    char ***array_of_argv;

    int *arg_elems;
    int mandatory_parameters = 8 + max_extra_words;
    int *array_of_maxprocs;

    int root = 0;
    MPI_Info *array_of_info;
    MPI_Comm intercomm;

    array_of_commands = reinterpret_cast<char**>(malloc(size * sizeof(char*)));
    array_of_argv     = reinterpret_cast<char***>(malloc(size * sizeof(char**)));
    arg_elems         = reinterpret_cast<int*>(malloc(size * sizeof(int)));
    array_of_maxprocs = reinterpret_cast<int*>(malloc(size * sizeof(int)));
    array_of_info     = reinterpret_cast<MPI_Info*>(malloc(size * sizeof(MPI_Info)));
    std::cout << "[OPMPSS_DR] Mallocs done." << std::endl;

    for (int i = 0 ; i < size ; i ++) {
        arg_elems[i] = mandatory_parameters + argc + 1;
        std::cout << "[OMPSS_DR] arg_elems = " << arg_elems[i]
                  << " " << mandatory_parameters
                  << " " << argc << std::endl;
        // commands; the binary requires postfix "_instr"
        array_of_commands[i] = strdup(dr_path);
        array_of_argv[i] = reinterpret_cast<char**>(malloc(arg_elems[i] * sizeof(char*)));

        // Must point to the temporary directory.
        std::cout << "[OMPSS_DR] Starting alloc and copy." << std::endl;
        std::string tmp_out_file(std::string(rt_wp_info.exp_path_prefix) + out_filenames[i]);
        int nparams = 0;
        for (std::string drarg : v_dr_args) {
            alloc_and_copy(array_of_argv[i], nparams++, drarg.c_str());
        }
        alloc_and_copy(array_of_argv[i], nparams++, c_flag);
        alloc_and_copy(array_of_argv[i], nparams++, tool_path);
        alloc_and_copy(array_of_argv[i], nparams++, i_flag);
        alloc_and_copy(array_of_argv[i], nparams++, tmp_filenames[i].c_str());  //  < -- input
        alloc_and_copy(array_of_argv[i], nparams++, o_flag);
        alloc_and_copy(array_of_argv[i], nparams++, tmp_out_file.c_str());         //  < -- output
        alloc_and_copy(array_of_argv[i], nparams++, m_flag);
        alloc_and_copy(array_of_argv[i], nparams++, dash_flag);

        alloc_and_copy(array_of_argv[i], nparams++, argv[0]);
        // Application and parameters
        for (int j = 1; j < argc ; j++) {
            std::cout << "[OMPSS_DR] Now the argc " << j << " of " << argc << std::endl;
            alloc_and_copy(array_of_argv[i], nparams++, argv[j]);
        }
        if (nparams != (arg_elems[i]-1)) {
            std::cout << "[OMPSS_DR] Used " << nparams
                      << " parameters and I'm supossed to use " << (arg_elems[i]-1) << std::endl;
            for (int j = 0; j < nparams; j++) {
                std::cout << array_of_argv[i][j] << " ";
            }
        }
        array_of_argv[i][arg_elems[i] - 1] = nullptr;

        // list of arguments
        array_of_maxprocs[i] = 1;
        Log::debug() << "[OMPSS_DR] Invoking MPI_Create.";
        MPI_Info_create(&array_of_info[i]);

        final_arg = env3;
        line = reinterpret_cast<char*>(malloc((final_arg.length() + 1) * sizeof(char)));
        snprintf(line, final_arg.length() + 1, "%s", final_arg.c_str());
        // strcpy(line, final_arg.c_str());

        std::cout << "[OMPSS_DR] Invoking MPI_Info set " << env_code << " to " << line << std::endl;
        MPI_Info_set(array_of_info[i], env_code, line);
    }

    // SHOW INFO TO BE EXECUTED

    char value[MPI_MAX_INFO_VAL + 1];
    int useless_flag;

    for (int i = 0 ; i < size ; i ++) {
        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "[OMPSS PIN] RANK [" << i << "]  Commands: [" << array_of_commands[i] << " ";
        for (int j = 0; j < (arg_elems[i] - 1); j++) {
            std::cout << array_of_argv[i][j] << " ";
        }
        std::cout << "]" << std::endl;

        MPI_Info_get(array_of_info[i], env_code, MPI_MAX_INFO_VAL, value, &useless_flag);
        if (not useless_flag) {
            std::cout << "MPI Info for rank " << i << " not set." << std::endl;
        } else {
            std::cout << "MPI_Info_env  {" << std::endl;
            std::cout << value << std::endl
                      << "}" << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

    // Creation of the MPI environment.
    int retv = MPI_Comm_spawn_multiple(size,
                                       array_of_commands,
                                       array_of_argv,
                                       array_of_maxprocs,
                                       array_of_info,
                                       root,
                                       MPI_COMM_SELF,
                                       &intercomm,
                                       MPI_ERRCODES_IGNORE);
    switch (retv) {
      case MPI_SUCCESS:
        std::cout << "MPI_Comm_spawn_multiple finished without error." << std::endl;
        break;
      case MPI_ERR_COMM:
        std::cout << "MPI_Comm_spawn_multiple had an invalid communicator." << std::endl;
        break;
      case MPI_ERR_ARG:
        std::cout << "MPI_Comm_spawn_multiple had an invalid arguments." << std::endl;
        break;
      case MPI_ERR_INFO:
        std::cout << "MPI_Comm_spawn_multiple had an invalid info." << std::endl;
        break;
      case MPI_ERR_SPAWN:
        std::cout << "MPI_Comm_spawn_multiple had MPI_ERR_SPAWN." << std::endl;
        break;
      default:
        std::cout << "MPI_Comm_spawn_multiple had an unknown error." << std::endl;
        break;
    }
    // MPI_Barrier(intercomm);
    std::cout << "MPI_COmm spawn multiple finished." << std::endl;
    MPI_Comm_free(&intercomm);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char * argv[])
{
    Log::debug() << "MAIN";
    const char *local_trace = getenv("TMPDIR_OUT_TRACE");

    std::string s_tmp_file_prefix;
    if (local_trace != NULL) {
        s_tmp_file_prefix = (std::string(local_trace) + "file");
    } else {
        s_tmp_file_prefix = "./file";
    }

    // Initialize MPI stuff:
    // debug...try to initalize
    rt_wp_init_envar_info();
    rt_wp_init_info();

    int rank, size;
    std::string s_tmp_file_name = s_tmp_file_prefix + "XXXXXX";
    std::ifstream fp;

    // need to obtain parameter -np and procs
    // remove form list and update on spawining process
    Log::debug() << "[OMPSS MPI MEM] " << argc;;
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i];
    }
    std::cout << std::endl;

    if (strcmp(argv[1], "-np") != 0) {
        std::cout << "2nd and 3rd Argument must define -np and #processes" << std::endl;
        exit(-1);
    }

    num_procs = atoi(argv[2]);
    if (num_procs < 1) {
        std::cout << "Error on input parameters. Num procs must be > 0" << std::endl;
        exit(-1);
    }

    Log::debug() << "MPI_Init.";
    PMPI_Init(&argc, &argv);
    Log::debug() << "MPI_COMM_RANK";
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Log::debug() << "MPI_COMM_SIZE";
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    size = num_procs;

    if (rank == 0) {
        // We identify the core
        /*
        int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
        std::cout << std::endl
                  << "[OMPSS MPI MEM] MASTER IS GOING TO SPAWN --- Rank "
                  << rank << "/" << size << " at your command (PID: " << getpid()
                  << " PPID: " << getppid() << ") [" << sched_getcpu() << "/"
                  << num_cpus << "]" << std::endl;
        */
        std::string relation(std::string(rt_wp_info.exp_path_prefix)+PLAIN_RELATION_FILENAME);
        Log::debug() << "Opening file " << relation;
        std::vector<std::string> tmp_filenames, out_filenames;

        std::ifstream myfile(relation);
        if (not myfile.is_open()) {
            std::cout << "[OMPSS MPI MEM] Could not open relation file " << relation << std::endl;
            exit(-1);
        }

        std::string v1, v2, v3, v4, v5, token;
        std::string line;
        // We need to redo this:
        while (std::getline(myfile, line)) {
            Log::debug() << "[OMPSS_DR_MEM] line: " << line;
            if (line.size() != 0) {
                token = line;
                v1 = token.substr(0, token.find(" "));
                token = token.substr(token.find(" ") + 1);
                v2 = token.substr(0, token.find(" "));
                token = token.substr(token.find(" ") + 1);
                v3 = token.substr(0, token.find(" "));
                token = token.substr(token.find(" ") + 1);
                v4 = token.substr(0, token.find(" "));
                v5 = token.substr(token.find(" ")+1);

                int index = std::stoi(v1) - 1;
                std::ostringstream code;
                code << std::setw(6) << std::setfill('0') << v5;
                if (tmp_filenames.size() <= static_cast<unsigned>(index)) {
                    tmp_filenames.resize(index + 1);
                    out_filenames.resize(index + 1);
                }

                tmp_filenames[index] = std::string(s_tmp_file_prefix + v5);
                out_filenames[index] = std::string(std::string(argv[3]) + "_proc_" + code.str() +  ".ts");
                Log::debug() << "[OMPSS_MPI_MEM] DEBUG: line " << tmp_filenames.size()
                             << " equals index " << index;
            }
        }
        myfile.close();
        if (tmp_filenames.size() == 0) {
            std::cerr << "[OMPSS MPI MEM] FILE: " << relation << " does not contain information" << std::endl;
            exit(-1);
        }
        // Launch PIN tool (if environment variables have been set u
        std::cout << "[OMPSS MPI MEM] Launch memory mode on rank." << std::endl;
        execute_pin_mpi(argc - 3, &argv[3], tmp_filenames, out_filenames);
    } else {
        // If MPIRUN is invoked using more than 1 process, the slave ranks would reamin idle.
        int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
        printf("[OMPSS MPI MEM] IDLE - Rank %d/%d at your command (PID: %d PPID: %d) [%d/%d]\n",
                rank, size, getpid(), getppid(), sched_getcpu(), num_cpus);
    }
    PMPI_Finalize();
}
