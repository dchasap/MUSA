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

#include <cassert>
#include <string>
#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#include "core/trace/intel/Trace.h"
#include "core/tools/ompss_util.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/ts/Trace.h"
#include "core/trace/uop/Trace.h"
#include "core/trace/types.h"
#include "core/utils/Log.h"

using sim::utils::Log;

int main(int argc, char * argv[])
{
#ifdef ENABLE_MPI
    int rank, size, c = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    if (argc != 2) {
        std::cout << "[ERROR] We need the application name as a parameter." << std::endl;
        exit(-1);
    }

    const std::string benchmark_name(argv[1]);

#ifdef ENABLE_MPI
    std::cout << "Start Master-Worker -- Rank [ "<< rank << "]/["<< size << "] -- for Filename [" << benchmark_name << "]" << std::endl;
#endif

    char *tmpdir = getenv("TMPDIR_OUT_TRACE");
    if (tmpdir == NULL) {
        std::cerr << "Environment variable TMPDIR_OUT_TRACE is not set." << std::endl;
        exit(-1);
    }
    std::string tmpdir_folder(tmpdir);
    Log::debug() << "---- TMPDIR_OUT_TRACE" << tmpdir;

    char *exp_path = getenv("MUSA_EXP_NAME");
    if (tmpdir == NULL) {
        std::cerr << "Environment variable MUSA_EXP_NAME is not set." << std::endl;
        exit(-1);
    }
    std::string exp_path_folder(exp_path);
    std::string relation_file(exp_path_folder + "/MUSA_relation_redux.txt"); 
    std::ifstream fp(relation_file.c_str());
    if (fp.fail()) {
        std::cout << "[OMPSS_MERGE] Error accessing relation file data ["
                  << relation_file << "]" << std::endl;
        exit(-1);
    }

    std::string index, ranks, proc_id, hostname, random_sufix;
    while (fp >> index >> ranks >> proc_id >> hostname >> random_sufix) {
#ifdef ENABLE_MPI
        if ((c++ % size) != rank) {
            // We only do the ones we should
            continue;
        }
#endif
        // Use templated filename and replace it with effective FileExtension
        std::string input_filename(tmpdir_folder + "/file" + random_sufix);
        std::string output_filename(benchmark_name + "_proc_" + random_sufix + ".ts");

        const std::string tmp_name(input_filename);
        const std::string out_name(output_filename.substr(output_filename.find_last_of("/\\") + 1));
        const std::string full_out_name(exp_path_folder + "/" + output_filename);

        // Compose the final name
        // Leading zeros for padding
        const std::string num_padded(std::string(6 - index.size(), '0') + index);  // rank
        // Final name
        std::string final_name(benchmark_name + "_proc_" + num_padded + ".ts");

        std::cout << "----------------------------------------------------------------------" << std::endl;
        std::cout << "Merging " << tmp_name << " and " << out_name << std::endl;

        sim::tools::merge_traces<sim::trace::BinaryEventStream, sim::trace::SelectedEventStream,
               sim::trace::selectedtrace::TraceInternal>(tmp_name.c_str(), full_out_name.c_str());

        std::cout << "Cleaning OmpSs trace " << tmp_name << std::endl;

        sim::tools::cleanup_ompss_trace(tmp_name);

        sim::tools::rename_trace_files(out_name, final_name, exp_path_folder);

        std::cout << "----------------------------------------------------------------------" << std::endl;
    }
    fp.close();
#ifdef ENABLE_MPI
    MPI_Finalize();
    std::cout << "Process "<< rank << "/" << size << " ends" << std::endl;
#endif

    return 0;
}
