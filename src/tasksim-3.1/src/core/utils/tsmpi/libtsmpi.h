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

#ifndef CORE_UTILS_TSMPI_LIBTSMPI_H_
#define CORE_UTILS_TSMPI_LIBTSMPI_H_

#include <assert.h>

#include "nanos.h"

// #include "libtsmpi_types.h"


#ifdef MPI3
#define ARG_CONST   const
#else
#define ARG_CONST
#endif

#define CtoF77(x) (x ## _)
#define SIZEOF_MPI_STATUS 5
typedef struct myId myId;
struct myId {
    int  ppid;
    int  rank;
    char hostname[256];
};

FILE *mpiphases_relation_file;
FILE *mpiphases_relation_file_redux;
char *nanos_relation_file_name;

void pre_execute(int);
void post_execute(int);
void post_initialize(int);
void post_finalize(int);
int  write_all(int*, int, char *mpi_relation_file_name);


// interposition function
int  MPI_Init_thread(int*, char***, int, int*);
int  MPI_Init(int*, char***);
int  MPI_Finalize();
int  MPI_Send(ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm);
int  MPI_Recv(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Status*);
int  MPI_Isend(ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*);
int  MPI_Irecv(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*);
int  MPI_Wait(MPI_Request*, MPI_Status*);
int  MPI_Bcast(void *, int, MPI_Datatype, int, MPI_Comm);
int  MPI_Barrier(MPI_Comm);
int  MPI_Reduce(ARG_CONST void*, void*, int, MPI_Datatype, MPI_Op, int, MPI_Comm);
int  MPI_Waitall(int, MPI_Request[], MPI_Status[]);
int  MPI_Gather(ARG_CONST void*, int, MPI_Datatype, void*, int, MPI_Datatype, int, MPI_Comm);
int  MPI_Scatter(ARG_CONST void*, int, MPI_Datatype, void*, int, MPI_Datatype, int, MPI_Comm);
int  MPI_Allreduce(ARG_CONST void*, void*, int, MPI_Datatype, MPI_Op, MPI_Comm);
int  MPI_Cart_create(MPI_Comm, int, ARG_CONST int[], ARG_CONST int[], int, MPI_Comm*);
// int  MPI_Cart_create(MPI_Comm, int, int[], int[], int, MPI_Comm*);
int  MPI_Allgatherv(ARG_CONST void*, int , MPI_Datatype , void*, ARG_CONST int[],
                    ARG_CONST int[], MPI_Datatype , MPI_Comm);
int  MPI_Alltoall(ARG_CONST void*, int, MPI_Datatype, void*, int, MPI_Datatype, MPI_Comm);
int  MPI_Alltoallv(ARG_CONST void*, ARG_CONST int[],  ARG_CONST int[], MPI_Datatype, void*,
                    ARG_CONST int[], ARG_CONST int[], MPI_Datatype, MPI_Comm);

int  MPI_Alltoallw(ARG_CONST void *, ARG_CONST int[],  ARG_CONST int[], ARG_CONST MPI_Datatype[],
                    void *, ARG_CONST int[], ARG_CONST int[], ARG_CONST MPI_Datatype[], MPI_Comm);
int  MPI_Bsend(ARG_CONST void *, int, MPI_Datatype, int, int, MPI_Comm);
int  MPI_Bsend_init(ARG_CONST void *, int, MPI_Datatype ,  int , int , MPI_Comm , MPI_Request *);
int  MPI_Ibsend(ARG_CONST void *, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *);
int  MPI_Intercomm_create(MPI_Comm , int , MPI_Comm , int , int , MPI_Comm *);
int  MPI_Intercomm_merge(MPI_Comm , int ,  MPI_Comm *);
int  MPI_Irsend(ARG_CONST void *, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *);
int  MPI_Issend(ARG_CONST void *, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *);
int  MPI_Reduce_scatter(ARG_CONST void *, void *, ARG_CONST int[], MPI_Datatype, MPI_Op, MPI_Comm);
int  MPI_Rsend(ARG_CONST void *, int , MPI_Datatype , int , int , MPI_Comm);
int  MPI_Rsend_init(ARG_CONST void *, int , MPI_Datatype , int , int , MPI_Comm , MPI_Request *);
int  MPI_Scatterv(ARG_CONST void *, ARG_CONST int[], ARG_CONST int[], MPI_Datatype,
                    void *, int, MPI_Datatype, int, MPI_Comm);
int  MPI_Sendrecv(ARG_CONST void *, int , MPI_Datatype , int , int , void *, int,
                    MPI_Datatype , int , int , MPI_Comm , MPI_Status *);
int  MPI_Sendrecv_replace(void *, int , MPI_Datatype , int , int , int , int , MPI_Comm , MPI_Status *);
int  MPI_Ssend(ARG_CONST void *, int , MPI_Datatype , int , int , MPI_Comm);
int  MPI_Ssend_init(ARG_CONST void *, int , MPI_Datatype , int , int , MPI_Comm , MPI_Request *);
int  MPI_Startall(int, MPI_Request[]);
// BUG:15,120
// int  MPI_Comm_split(MPI_Comm , int , int , MPI_Comm *);
// BUG:15,120
int MPI_Gatherv(ARG_CONST void*, int , MPI_Datatype , void*, ARG_CONST int[],
                    ARG_CONST int[], MPI_Datatype , int , MPI_Comm);
int MPI_Scan(ARG_CONST void*, void*, int, MPI_Datatype, MPI_Op , MPI_Comm);
// BUG:amg2013
// int MPI_Testall(int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[]);
// int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status);


// Fortran aliases
int mpi_init_(MPI_Fint *ierror);
int mpi_finalize_(void);
void CtoF77 (mpi_send) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_recv) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
        MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status,
        MPI_Fint *ierror);

void CtoF77 (mpi_isend) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
        MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
        MPI_Fint *ierror);

void CtoF77 (mpi_irecv) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
        MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
        MPI_Fint *ierror);

void CtoF77 (mpi_wait) (MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierror);

void CtoF77 (mpi_bcast) (void *buffer, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_barrier) (MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_reduce) (void *sendbuf, void *recvbuf, MPI_Fint *count,
    MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm,
    MPI_Fint *ierror);

void CtoF77 (mpi_waitall) (MPI_Fint * count, MPI_Fint array_of_requests[],
  MPI_Fint array_of_statuses[][SIZEOF_MPI_STATUS], MPI_Fint * ierror);

void CtoF77 (mpi_gather) (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
    MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_scatter) (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
    MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_allreduce) (void *sendbuf, void *recvbuf, MPI_Fint *count,
    MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_cart_create) (MPI_Fint *comm_old, MPI_Fint *ndims,
    MPI_Fint *dims,  MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart,
    MPI_Fint *ierror);

void CtoF77 (mpi_allgather) (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
    MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_alltoall) (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
    MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_alltoallv) (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sdispls, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount,
    MPI_Fint *rdispls, MPI_Fint *recvtype,  MPI_Fint *comm, MPI_Fint *ierror);


void CtoF77 (mpi_bsend_init) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror);

void CtoF77 (mpi_ibsend) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror);

void CtoF77(mpi_intercomm_create)(MPI_Fint *local_comm,
    MPI_Fint *local_leader, MPI_Fint *peer_comm, MPI_Fint *remote_leader,
    MPI_Fint *tag, MPI_Fint *new_intercomm, MPI_Fint *ierror);

void CtoF77 (mpi_intercomm_merge) (MPI_Fint *intercomm, MPI_Fint *high,
    MPI_Fint *newintracomm, MPI_Fint *ierror);

void CtoF77 (mpi_irsend) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror);

void CtoF77 (mpi_issend) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror);

void CtoF77 (mpi_reduce_scatter) (void *sendbuf, void *recvbuf,
    MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
    MPI_Fint *ierror);

void CtoF77 (mpi_rsend) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_rsend_init) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror);

void CtoF77 (mpi_scatterv) (void *sendbuf,  MPI_Fint *sendcount,
    MPI_Fint *displs, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount,
    MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_sendrecv) (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, void *recvbuf,
    MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag,
    MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);


void CtoF77 (mpi_sendrecv_replace) (void *buf, MPI_Fint *count, MPI_Fint *type,
    MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag,
    MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);

void CtoF77 (mpi_ssend_init) (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror);

void CtoF77 (mpi_startall) (MPI_Fint *count, MPI_Fint array_of_requests[],
    MPI_Fint *ierror);

void CtoF77 (mpi_gatherv) (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount,  MPI_Fint *displs,
    MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror);

void CtoF77 (mpi_scan) (void *sendbuf, void *recvbuf, MPI_Fint *count,
    MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierror);

#define TS_MPI_OPS      44
enum TS_MPI_events {
    ev_MPI_Init_thread      = 0,
    ev_MPI_Init             = 1,
    ev_mpi_init_            = 1,
    ev_MPI_Finalize         = 2,
    ev_mpi_finalize_        = 2,
    ev_MPI_Send             = 3,
    ev_mpi_send_            = 3,
    ev_MPI_Recv             = 4,
    ev_mpi_recv_            = 4,
    ev_MPI_Isend            = 5,
    ev_mpi_isend_           = 5,
    ev_MPI_Irecv            = 6,
    ev_mpi_irecv_           = 6,
    ev_MPI_Wait             = 7,
    ev_mpi_wait_            = 7,
    ev_MPI_Bcast            = 8,
    ev_mpi_bcast_           = 8,
    ev_MPI_Barrier          = 9,
    ev_mpi_barrier_         = 9,
    ev_MPI_Reduce           = 10,
    ev_mpi_reduce_          = 10,
    ev_MPI_Waitall          = 11,
    ev_mpi_waitall_         = 11,
    ev_MPI_Gather           = 12,
    ev_mpi_gather_          = 12,
    ev_MPI_Scatter          = 13,
    ev_mpi_scatter_         = 13,
    ev_MPI_Allreduce        = 14,
    ev_mpi_allreduce_       = 14,
    ev_MPI_Cart_create      = 15,
    ev_mpi_cart_create_     = 15,
    ev_MPI_Allgather        = 16,
    ev_mpi_allgather_       = 16,
    ev_MPI_Allgatherv       = 17,
    ev_mpi_allgatherv_      = 17,
    ev_MPI_Alltoall         = 18,
    ev_mpi_alltoall_        = 18,
    ev_MPI_Alltoallv        = 19,
    ev_mpi_alltoallv_       = 19,
    ev_MPI_Alltoallw        = 20,
    ev_mpi_alltoallw_       = 20,
    ev_MPI_Bsend            = 21,
    ev_mpi_bsend_           = 21,
    ev_MPI_Bsend_init       = 22,
    ev_mpi_bsend_init_      = 22,
    ev_MPI_Ibsend           = 23,
    ev_mpi_ibsend_          = 23,
    ev_MPI_Intercomm_create = 24,
    ev_mpi_intercomm_create_= 24,
    ev_MPI_Intercomm_merge  = 25,
    ev_mpi_intercomm_merge_ = 25,
    ev_MPI_Irsend           = 26,
    ev_mpi_irsend_          = 26,
    ev_MPI_Issend           = 27,
    ev_mpi_issend_          = 27,
    ev_MPI_Reduce_scatter   = 28,
    ev_mpi_reduce_scatter_  = 28,
    ev_MPI_Rsend            = 29,
    ev_mpi_rsend_           = 29,
    ev_MPI_Rsend_init       = 30,
    ev_mpi_rsend_init_      = 30,
    ev_MPI_Scatterv         = 31,
    ev_mpi_scatterv_        = 31,
    ev_MPI_Sendrecv         = 32,
    ev_mpi_sendrecv_        = 32,
    ev_MPI_Sendrecv_replace = 33,
    ev_mpi_sendrecv_replace_= 33,
    ev_MPI_Ssend            = 34,
    ev_mpi_ssend_           = 34,
    ev_MPI_Ssend_init       = 35,
    ev_mpi_ssend_init_      = 35,
    ev_MPI_Startall         = 36,
    ev_mpi_startall_        = 36,
    ev_MPI_Comm_split       = 37,
    ev_mpi_comm_split_      = 37,
    ev_MPI_Gatherv          = 38,
    ev_mpi_gatherv_         = 38,
    ev_MPI_Scan             = 39,
    ev_mpi_scan_            = 39,
    ev_MPI_Testall          = 40,
    ev_MPI_Test             = 41,
    ev_END                  = 42,
    ev_INIT                 = 43
};

static const char *TS_MPI_evNames[TS_MPI_OPS] =
{
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
    "MPI_Comm_split",
    "MPI_Gatherv",
    "MPI_Scan",
    "MPI_Testall",
    "MPI_Test",
    "END",
    "INIT",
};

int last_event = ev_INIT;

#endif  // CORE_UTILS_TSMPI_LIBTSMPI_H_
