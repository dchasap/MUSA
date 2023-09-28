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
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cctype>
#include <cassert>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <string>
#include <utility>

#include "core/trace/intel/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/ompss/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/types.h"



#define CAA

#ifdef CAA
#include <fcntl.h>
// wrapper to inform about a MPI initialization.
extern  "C"{
#include "core/utils/tsmpi/libtsmpi_extern-impl.h"
}

typedef struct {
  char *local_temporal;
  char *remote_temporal;
  char *application;
  char *extension;
  char *trace_filename;
}
TEMP_NAMES;

TEMP_NAMES names;
#endif

using namespace sim::trace;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
static void execute_ompss(int argc, char * const argv[], char *tmp_file_name)
{
    int status;
    pid_t pid;
    static char tmpbuffer[1024];

    std::cout << "[OMPSS] Launching instrumented application" << std::endl;

    if ((pid = fork()) == 0) {
        /* Child process */
        setenv("OMPSS_TRACE_FILE", tmp_file_name, 1);

        // Get NX_ARGS from environment and add tracing specific flags
        char *nx_args;
        nx_args = getenv("NX_ARGS");
        std::string nx_args_string;
        if (nx_args != NULL) {
            nx_args_string = std::string(nx_args);
        }
        nx_args_string += " --instrument-default=all --smp-workers=1 --throttle-upper=99 -instrumentation=nextsim";
        setenv("NX_ARGS", nx_args_string.c_str(), 1);


        snprintf(tmpbuffer, 1024, "%s_instr", argv[0]);
        std::cout << "[OMPSS] Executing:";
        std::cout << "OMPSS_TRACE_FILE=" << tmp_file_name << " ";
        std::cout << "NX_ARGS=\"" << nx_args_string << "\" " ;
        std::cout << tmpbuffer << std::endl;
        for (int i = 1; i < argc; i++) {
            std::cout << " " << argv[i];
        }
        std::cout << std::endl;

        char *preload;
        preload = getenv("LD_PRELOAD");
        std::string prev_preload = std::string(_TS_PATH) + std::string("/lib/libtsmpi.so");
        if (preload != NULL) {
            prev_preload += ":" + std::string(preload);
        }
        std::cout << "[OMPSS] LD_PRELOAD=" << prev_preload << std::endl;
        setenv("LD_PRELOAD", prev_preload.c_str(),1);

        execv(tmpbuffer, argv);
        std::cerr << "[OMPSS ERROR] Unable to execute: ";
        for (int i = 0; i < argc; i++) {
            std::cerr << argv[i] << " ";
        }
        std::cerr << std::endl;
        exit(-1);
    } else if (pid < 0) {
        std::cerr << "[OMPSS ERROR] Unable to create child process" << std::endl;
        exit(-1);
    }

    wait(&status);
    if (WEXITSTATUS(status) != 0) {
        std::cerr << "[OMPSS] Error on instrumented execution " << WEXITSTATUS(status) << std::endl;
        exit(-1);
    }
    std::cout << "[OMPSS] Run-time trace generated" << std::endl;

    return;
}



/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void create_names (int argc, char * const argv[]){

  const char *remote_trace = getenv("TMPDIR_TMP_TRACE");
  const char *local_trace = getenv("TMPDIR_OUT_TRACE");

  std::string s_postfix    = "XXXXXX";
  std::string s_prefix     = "file";
  std::string s_folder_l   (local_trace);
  std::string s_folder_r   (remote_trace);
  std::string s_location   = s_folder_l  + s_prefix + s_postfix;
  std::string s_trace_filename ;

  // Compose template path and filenamepath to temporary file
  int var_size = s_location.size();
  /*
  names.local_temporal = (char*) malloc(sizeof(char) * ((var_size)+1));
  strcpy (names.local_temporal, s_location.c_str());
  */
  names.local_temporal = strdup(s_location.c_str());

  // Create the unique name for the temporary file
  // Local folder
  mkstemp (names.local_temporal);

  // Obtain Unique extension
  names.extension = (char*) malloc (sizeof(char) * (s_postfix.size() + 1));
  var_size = std::strlen(names.local_temporal);
  strncpy (names.extension, names.local_temporal + (var_size - s_postfix.size()), s_postfix.size());

  // Remote folder
  std::string s_location_r = s_folder_r + s_prefix + std::string(names.extension);
  /*
  var_size = s_location_r.size();
  names.remote_temporal = (char*) malloc (sizeof(char) * var_size);
  strcpy(names.remote_temporal, s_location_r.c_str());
  */
  names.remote_temporal = strdup(s_location_r.c_str());

  // Application name
/*
  names.application = (char*) malloc (sizeof(char) * strlen(argv[1]));
  strcpy (names.application, argv[1]);
*/
  names.application = strdup(argv[1]);

  // Trace filename
  s_trace_filename = std::string(names.application) + "_proc_" + std::string(names.extension) + ".ts";
/*
  var_size = s_trace_filename.size();
  names.trace_filename = (char*) malloc (sizeof(char) * var_size);
  strcpy (names.trace_filename, s_trace_filename.c_str());
*/
  names.trace_filename = strdup(s_trace_filename.c_str());



  printf("[TOOL] ----------------------------------\n");
  printf("[TOOL] local_temporal    [%s]\n", names.local_temporal);
  printf("[TOOL] remote_temporal   [%s]\n", names.remote_temporal);
  printf("[TOOL] application       [%s]\n", names.application);
  printf("[TOOL] extension         [%s]\n", names.extension);
  printf("[TOOL] trace_filename    [%s]\n", names.trace_filename);
  printf("[TOOL] ----------------------------------\n");


  return;
}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char * const argv[])
{
    // Initialize the file names.
    create_names (argc, &argv[0]);

    // Remove plain relation file, just in case.
    // UPDATE: no need....everything is on a subfolder.
    remove(PLAIN_RELATION_FILENAME);

    // Launch instrumented version to gather run-time traces-
    // After the parallel execution;
    // 1.- The interposition library has creeated a relation file
    //        of PPID+hostname associated with MPI_rank.
    //        PPID refers to this parent process PID before the fork.
    // 2.- Every child process instance of nanos creates a temporary
    //        temporary mpiphase for every enable/disable event in
    //        the interpostion library. Which, updates the
    //        information on a final mpihpases file, with information
    //        of the specific MPI_event  (MPI_code:MPI_description).

    execute_ompss(argc - 1, &argv[1], names.remote_temporal);



    // Obtaining the rank id for the previous execution
    // Because MPI is already defunct, we obtain the rank id
    // by the relation of PID(current pid)+hostname from
    // the unique relation file.
    int rank, size, pid;
    char hostname[256];
    memset(hostname,0,256);
    gethostname(hostname, 256);
    pid=getpid();
    std::cout << "[DEBUG] PID = " << pid << " hostname = " << hostname << std::endl;

    // initialize environment.
    rt_wp_init_envar_info();
    rt_wp_init_info();

    // Read information from RELATION_FILENAME
    read_relation_b(&rank, &size, pid, hostname );
    std::cout << "Rank = " << rank << ", size = " << size << std::endl;

// old version, safe version on openmpi/1.8.5
    //std::cout << "[TOOL] name " << rt_wp_info.fname_mpi_rel_redux_b << std::endl;
    // Every instance of this program enhances the relation file by
    // adding the file extension of the temporary file.
    // Also, in a readable format.
    std::ofstream plainRelation;
    std::cout << "[DEBUG] Printing process reduction file " << rt_wp_info.fname_mpi_rel_redux_b << std::endl;
    plainRelation.open(rt_wp_info.fname_mpi_rel_redux_b, std::ios::out | std::ios::app);
    plainRelation << rank << " " << size << " " << pid << " " << hostname << " " << names.extension << std::endl;
    plainRelation.close();

    return 0;
}



