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

#ifndef CORE_TRACE_DR_TYPES_H_
#define CORE_TRACE_DR_TYPES_H_

#include <string>
#include <vector>

namespace sim {
namespace trace {
namespace dr {

/** struct including the number of loads and stores of a complex instruction */
struct minfo_t{
    unsigned ld_;
    unsigned st_;
    minfo_t(): ld_(0), st_(0) {}
    minfo_t(unsigned ld, unsigned st): ld_(ld), st_(st) {}
};

/** struct to hold statistics of instruction translation */
struct TransInfo {
    unsigned num_x86_trans_;
    unsigned num_invalid_;
    unsigned num_fixed_;
    std::vector<minfo_t> minfo_;
    TransInfo(): num_x86_trans_(0), num_invalid_(0), minfo_() {}
};

class mpi_info_t {
public:
    unsigned index;
    unsigned wd_id;
    bool is_nanos;
    bool is_mem;
    unsigned mpi_id;
    std::string mpi_name;
    mpi_info_t() :
        index(0),
        wd_id(1),
        is_nanos(false),
        is_mem(false),
        mpi_id(0),
        mpi_name("INIT")
    {}
};

std::ostream &operator<<(std::ostream &os, const mpi_info_t& mpi_info);
std::istream &operator>>(std::istream &is, mpi_info_t& mpi_info);

static const std::vector<std::string> mpi_name_list({
    "MPI_Init_thread",
    "MPI_Init",
    "MPI_Finalize",
    "MPI_Send",
    "MPI_Recv",
    "MPI_Isend",
    "MPI_Irecv",
    "MPI_Wait",
    "MPI_Bcast",
    "MPI_Barrier",
    "MPI_Reduce",
    "MPI_Waitall",
    "MPI_Gather",
    "MPI_Scatter",
    "MPI_Allreduce",
    "MPI_Cart_create",
    "MPI_Allgather",
    "MPI_Allgatherv",
    "MPI_Alltoall",
    "MPI_Alltoallv",
    "MPI_Alltoallw",
    "MPI_Bsend",
    "MPI_Bsend_init",
    "MPI_Ibsend",
    "MPI_Intercomm_create",
    "MPI_Intercomm_merge",
    "MPI_Irsend",
    "MPI_Issend",
    "MPI_Reduce_scatter",
    "MPI_Rsend",
    "MPI_Rsend_init",
    "MPI_Scatterv",
    "MPI_Sendrecv",
    "MPI_Sendrecv_replace",
    "MPI_Ssend",
    "MPI_Ssend_init",
    "MPI_Startall",
//    "MPI_Comm_split",
    "MPI_Gatherv",
    "MPI_Scan"
    /*,
    "MPI_Testall",
    "MPI_Test"*/
});

}  // namespace dr
}  // namespace trace
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_TRACE_DR_TYPES_H_
