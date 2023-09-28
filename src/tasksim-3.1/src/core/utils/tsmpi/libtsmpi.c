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

// #define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <mpi.h>
#include <stdbool.h>

#include <time.h>

#include "libtsmpi.h"

#define TSMPI_REPLACE(name, ret, args, types) \
    ret result; \
    static ret(* real_ ## name) types = NULL; \
    real_ ## name = (dlsym(RTLD_NEXT, #name)); \
    if (!(real_ ## name)) \
      return (fprintf(stderr, "TSMPI wrapper failed to map symbol %s\n", #name)); \
    pre_execute(ev_ ## name) ; \
    result = real_ ## name args; \
    post_execute(ev_ ## name); \
    return result;
#define TSMPI_REPLACE_VOID(name, ret, args, types) \
    static void (* real_ ## name) types = NULL; \
    real_ ## name = (dlsym(RTLD_NEXT, #name)); \
    if (!(real_ ## name)) {\
        fprintf(stderr, "TSMPI wrapper failed to map symbol %s\n", #name); \
        return; \
    } \
    pre_execute(ev_ ## name) ; \
    real_ ## name args; \
    post_execute(ev_ ## name);

    // typedef void types; \
/************************************************************************/
/************************************************************************/
/*
// This wrapper intercepts MPI calls.
// 1) After an MPI event calls nanos_instrument_[enable|disable]), to
// inform nanos plugin tracer to create a new phase (splitting nanos trace)
// and a file recording nanos internal phase id, and mpi phases.
// '--> wrapper to nanos via nanos_instrummpiphases_relation_fileent_(disable|enable):
// '--> nanos to wrapper through temporary_rel_$(PID).
// 2) A MPI_relation_file is created, with the current rank and pid.
// making posible to relate nanos_trace(aware of pid) with MPI rank.
*/
void pre_execute(int event) {
    char line[256];
    nanos_instrument_disable();
    // if extrae is enabled.
    fgets(line, sizeof(line), mpiphases_relation_file);
    strtok(line, "\n");

    sprintf(line, "%s:%d:%s\n", line, last_event, TS_MPI_evNames[last_event]);

    // BUG 1818; start
    fputs(line, mpiphases_relation_file_redux);
    fflush(mpiphases_relation_file_redux);
    // BUG 1818; end
    last_event = event;
}


/************************************************************************/
void post_execute(int event)
{
    // After the real execution of a MPI function. Resume nanos instrumention
    nanos_instrument_enable();
}


/************************************************************************/
/* 1) Just after MPI_Init we can generate a file to relate Rank with PID*/
/* 2) If trace has generated temp_rel file, we assume nanos is enabled */
/* 3) EXTRAE_FEATURE: Report initial phase to extrae */

void post_initialize(int event)
{
    printf("[TSMPI] MPI Init finished, now starting TaskSim code.\n");
    // Initialize fields in the runtime wrapper info structure.
    // rt_wp_info.load_environment_info();
    char *exp_path_prefix;
    char *exp_path = getenv("MUSA_EXP_NAME");
    if (exp_path != NULL && exp_path[0] != '\0') {
        exp_path_prefix = (char*)malloc(sizeof(char) * (strlen(exp_path) + 2));
        if (exp_path_prefix == NULL) {
            printf("Error on malloc\n");
            exit(-1);
        }
        memset(exp_path_prefix, '\0', strlen(exp_path) + 2);
        strncpy(exp_path_prefix, exp_path, strlen(exp_path));
        // put a final slash on the string.
        exp_path_prefix[strlen(exp_path)] = '/';
    } else {
        exp_path_prefix = strdup("./");
    }

    // After initialization rank has been assigned
    int mpi_rank, mpi_size;
    PMPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    PMPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    unsigned ppid = getppid();

    // relation between rank_id and parent pid

    unsigned relate_size = 2;
    int *relate_vec = (int*)malloc(sizeof(int)*relate_size);
    relate_vec[0] = mpi_rank;
    relate_vec[1] = ppid;

    char *mpi_relation_file_name = (char*) malloc(350);
    memset(mpi_relation_file_name, 0, 350);
    strcpy(mpi_relation_file_name, exp_path_prefix);
    strcat(mpi_relation_file_name, "/");
    strcat(mpi_relation_file_name, "MUSA_relation_filename");
    write_all(relate_vec, relate_size, mpi_relation_file_name);
    free(relate_vec);
    free(mpi_relation_file_name);

    char hostname[256];
    memset(hostname, 0, 256);
    gethostname(hostname, 256);

    // At this point we already know rank, therefore, instead of writting to redux file...
    // write directly to mpiphases.

    char* name;
    name = getenv("MUSA_APP");
    if (name == NULL) {
        printf("[WRAPPER] Error obtaining applicaton name $(MUSA_APP)\n");
        // exit(-1);
    }

    // Based on the existence of temporary relation file, we can...
    // ... say that nextsim plugin from nanox is the underlying tracer.
    // BUG 1818 START
    // INPUT FILE FROM PLUGIN
    nanos_relation_file_name = (char*)(malloc(350));
    memset(nanos_relation_file_name, 0, 350);
    sprintf(nanos_relation_file_name, "%s/%s%d_%s",
            exp_path_prefix, "mpiphases_temp_rel_",
            ppid, hostname);

    // debug
    printf("[TSMPI] Opening phase file %s.\n", nanos_relation_file_name);
    mpiphases_relation_file = fopen(nanos_relation_file_name, "r");


    // OUTPUT FILE MPIPHASES
    printf("[TSMPI] Nanos++ instrumentation is enabled.\n");
    char *nanos_relation_redux_file_name = (char*) malloc(350);
    memset(nanos_relation_redux_file_name, 0, 350);
    sprintf(nanos_relation_redux_file_name, "%s/%s_proc_%06d.ts.mpiphases",
            exp_path_prefix, name, mpi_rank + 1);
    mpiphases_relation_file_redux = fopen(nanos_relation_redux_file_name, "w");
    free(nanos_relation_redux_file_name);
}

void post_finalize(int event) {
    fclose(mpiphases_relation_file);
    fclose(mpiphases_relation_file_redux);
    remove(nanos_relation_file_name);
}


/************************************************************************/
// This funtion writes rank and size in a single file, but using MPI views
// to write into the file in parallel

int write_all(int *local_array, int local_size, char *mpi_relation_file_name) {

    int rc;
    MPI_File fh;

    int size, rank;
    PMPI_Comm_rank(MPI_COMM_WORLD, &rank);
    PMPI_Comm_size(MPI_COMM_WORLD, &size);


    // Create an MPI type related to structure myId.
    int blocklengths[3] = {1, 1, 256};
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_CHAR};
    MPI_Aint offsets[3];
    MPI_Datatype mpi_myId;
    offsets[0] = offsetof(myId, ppid);
    offsets[1] = offsetof(myId, rank);
    offsets[2] = offsetof(myId, hostname);
    const unsigned nitems = 3;
    PMPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_myId);
    PMPI_Type_commit(&mpi_myId);

    myId ids[size];

    int i;

    //  Initializes the values on the proper offset per rank
    ids[rank].ppid = getppid();
    ids[rank].rank = rank+1;
    memset(ids[rank].hostname, 0, sizeof(ids[rank].hostname));
    gethostname(ids[rank].hostname, sizeof(ids[rank].hostname));

    // Parallel write of identifiers to a common file.
    rc = PMPI_File_open(MPI_COMM_WORLD, mpi_relation_file_name,
                        MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    if (rc != MPI_SUCCESS) {
          printf("Did not open file\n");
          return EXIT_FAILURE;
    }

    rc = PMPI_File_set_view(fh, rank*sizeof(myId) , mpi_myId, mpi_myId, "native", MPI_INFO_NULL);
    if (rc != MPI_SUCCESS) {
          printf("Problem setting process view\n");
          return EXIT_FAILURE;
    }

    rc = PMPI_File_write(fh, &ids[rank], 1, mpi_myId, MPI_STATUS_IGNORE);
    if (rc != MPI_SUCCESS) {
          printf("Problem writting file\n");
          return EXIT_FAILURE;
    }
    rc = PMPI_File_close(&fh);

    // Free the MPI Type
    PMPI_Type_free(&mpi_myId);

    return 0;
}

/************************************************************************/
// interposition functions
/************************************************************************/
int mpi_init_(MPI_Fint *ierror) {
    MPI_Init(0, NULL);
}

int mpi_finalize_() {
    MPI_Finalize();
}

int MPI_Init(int *argc, char ***argv) {
    printf("[TSMPI] MPI Init.\n");
    static int (*real_MPI_Init)(int *, char ***) = NULL;

    real_MPI_Init = (int (*)(int*, char***))(dlsym(RTLD_NEXT, "MPI_Init"));
    if (!real_MPI_Init) {
        return MPI_ERR_INTERN;
    }

    int result = real_MPI_Init(argc, argv);

    post_initialize(ev_MPI_Init);

    // REGISTER THIS EVENT
    pre_execute(ev_MPI_Init);
    post_execute(ev_MPI_Init);

    return result;
}


int MPI_Init_thread(int *argc, char ***argv, int required, int *provided) {
    static int (*real_MPI_Init_thread)(int *, char ***, int, int *) = NULL;
    int result;

    real_MPI_Init_thread = (int (*)(int*, char***, int, int*))(dlsym(RTLD_NEXT, "MPI_Init_thread"));
    if (!real_MPI_Init_thread) {
        return MPI_ERR_INTERN;
    }
    result = real_MPI_Init_thread(argc, argv, required, provided);

    post_initialize(ev_MPI_Init_thread);

    // REGISTER THIS EVENT
    pre_execute(ev_MPI_Init_thread);
    post_execute(ev_MPI_Init_thread);

    return result;
}

/************************************************************************/
int MPI_Finalize(void)
{
    static int (*real_MPI_Finalize)(void) = NULL;
    int result;

    real_MPI_Finalize = (int (*)(void))(dlsym(RTLD_NEXT, "MPI_Finalize"));
    if (!real_MPI_Finalize)
        return MPI_ERR_INTERN;

    pre_execute(ev_MPI_Finalize);

    result = real_MPI_Finalize();

    post_execute(ev_MPI_Finalize);

    pre_execute(ev_END);
    post_finalize(ev_END);

    return result;
}

/************************************************************************/
// templated interposition functions

int MPI_Send(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest,
            int tag, MPI_Comm comm) {
    TSMPI_REPLACE(MPI_Send, int, (buf, count, datatype, dest, tag, comm),
            (ARG_CONST void *, int, MPI_Datatype, int, int, MPI_Comm));
}

int MPI_Isend(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag,
            MPI_Comm comm, MPI_Request *request) {
    TSMPI_REPLACE(MPI_Isend, int, (buf, count, datatype, dest, tag, comm, request),
            (ARG_CONST void *, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
}

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source,
            int tag, MPI_Comm comm, MPI_Status *status) {
    TSMPI_REPLACE(MPI_Recv, int, (buf, count, datatype, source, tag, comm, status),
            (void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Status*));
}

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
            int tag, MPI_Comm comm, MPI_Request *request ) {
    TSMPI_REPLACE(MPI_Irecv, int, (buf, count, datatype, source, tag, comm, request),
            (void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
}

int MPI_Wait(MPI_Request *request, MPI_Status *status) {
    TSMPI_REPLACE(MPI_Wait, int, (request, status), (MPI_Request*, MPI_Status*));
}

int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm ) {
    TSMPI_REPLACE(MPI_Bcast, int, (buffer, count, datatype, root, comm),
            (void*, int, MPI_Datatype, int, MPI_Comm));
}

int MPI_Barrier(MPI_Comm comm) {
    TSMPI_REPLACE(MPI_Barrier, int, (comm), (MPI_Comm));
}

int MPI_Reduce(ARG_CONST void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
            MPI_Op op, int root, MPI_Comm comm) {
    TSMPI_REPLACE(MPI_Reduce, int, (sendbuf, recvbuf, count, datatype, op, root, comm),
            (ARG_CONST void*, void*, int, MPI_Datatype, MPI_Op, int, MPI_Comm));
}

int MPI_Waitall(int count, MPI_Request array_of_requests[],
            MPI_Status array_of_statuses[]) {
    TSMPI_REPLACE(MPI_Waitall, int, (count, array_of_requests, array_of_statuses),
            (int, MPI_Request*, MPI_Status*));
}

int MPI_Gather(ARG_CONST void *sendbuf, int sendcnt, MPI_Datatype sendtype,
            void *recvbuf, int recvcnt, MPI_Datatype recvtype,
            int root, MPI_Comm comm) {
    TSMPI_REPLACE(MPI_Gather, int, (sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm),
            (ARG_CONST void*, int, MPI_Datatype, void*, int, MPI_Datatype, int, MPI_Comm));
}

int MPI_Scatter(ARG_CONST void *sendbuf, int sendcnt, MPI_Datatype sendtype,
            void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm) {
    TSMPI_REPLACE(MPI_Scatter, int, (sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm),
            (ARG_CONST void*, int, MPI_Datatype, void*, int, MPI_Datatype, int, MPI_Comm));
}

int MPI_Allreduce(ARG_CONST void *sendbuf, void *recvbuf, int count,
            MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    TSMPI_REPLACE(MPI_Allreduce, int, (sendbuf, recvbuf, count, datatype, op, comm),
            (ARG_CONST void*, void*, int, MPI_Datatype, MPI_Op, MPI_Comm));
}

int  MPI_Cart_create(MPI_Comm comm_old, int ndims, ARG_CONST int dims[],
            ARG_CONST int periods[], int reorder, MPI_Comm *comm_cart) {
    TSMPI_REPLACE(MPI_Cart_create, int, (comm_old, ndims, dims, periods, reorder, comm_cart),
            (MPI_Comm, int, ARG_CONST int[], ARG_CONST int[], int, MPI_Comm*));
}

int MPI_Allgather(ARG_CONST void *sendbuf, int sendcount, MPI_Datatype sendtype,
            void *recvbuf, int recvcounts, MPI_Datatype recvtype, MPI_Comm comm) {
    TSMPI_REPLACE(MPI_Allgather, int, (sendbuf, sendcount, sendtype, recvbuf, recvcounts, recvtype, comm),
            (ARG_CONST void*, int, MPI_Datatype, void*, int, MPI_Datatype, MPI_Comm));
}

int MPI_Allgatherv(ARG_CONST void *sendbuf, int sendcount, MPI_Datatype sendtype,
            void *recvbuf, ARG_CONST int recvcounts[], ARG_CONST int displs[], MPI_Datatype recvtype, MPI_Comm comm) {
    TSMPI_REPLACE(MPI_Allgatherv, int, (sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm),
            (ARG_CONST void*, int, MPI_Datatype, void*, ARG_CONST int[], ARG_CONST int[], MPI_Datatype, MPI_Comm));
}

int MPI_Alltoall(ARG_CONST void *sendbuf, int sendcount, MPI_Datatype sendtype,
            void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm) {
     TSMPI_REPLACE(MPI_Alltoall, int, (sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm),
            (ARG_CONST void*, int, MPI_Datatype, void*, int, MPI_Datatype, MPI_Comm));
}

int MPI_Alltoallv(ARG_CONST void *sendbuf, ARG_CONST int sendcounts[], ARG_CONST int sdispls[],
            MPI_Datatype sendtype, void *recvbuf, ARG_CONST int recvcounts[], ARG_CONST int rdispls[],
            MPI_Datatype recvtype, MPI_Comm comm) {
     TSMPI_REPLACE(MPI_Alltoallv, int, (sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts,
            rdispls, recvtype, comm),
            (ARG_CONST void *, ARG_CONST int[], ARG_CONST int[], MPI_Datatype, void*, ARG_CONST int[],
            ARG_CONST int[], MPI_Datatype, MPI_Comm));
}

int MPI_Alltoallw(ARG_CONST void *sendbuf, ARG_CONST int sendcounts[], ARG_CONST int sdispls[],
            ARG_CONST MPI_Datatype sendtypes[], void *recvbuf, ARG_CONST int recvcounts[],
            ARG_CONST int rdispls[], ARG_CONST MPI_Datatype recvtypes[], MPI_Comm comm) {
     TSMPI_REPLACE(MPI_Alltoallw, int, (sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts,
            rdispls, recvtypes, comm),
            (ARG_CONST void*, ARG_CONST int[], ARG_CONST int[], ARG_CONST MPI_Datatype[], void*,
            ARG_CONST int[], ARG_CONST int[], ARG_CONST MPI_Datatype[], MPI_Comm));
}

int MPI_Bsend(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
     TSMPI_REPLACE(MPI_Bsend, int, (buf, count, datatype, dest, tag, comm),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm));
}

int MPI_Bsend_init(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm,
            MPI_Request *request) {
     TSMPI_REPLACE(MPI_Bsend_init, int, (buf, count, datatype, dest, tag, comm, request),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
}

int MPI_Ibsend(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm,
            MPI_Request *request) {
      TSMPI_REPLACE(MPI_Ibsend, int, (buf, count, datatype, dest, tag, comm, request),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
}

int MPI_Intercomm_create(MPI_Comm local_comm, int local_leader, MPI_Comm peer_comm, int remote_leader,
            int tag, MPI_Comm *newintercomm) {
      TSMPI_REPLACE(MPI_Intercomm_create, int, (local_comm, local_leader, peer_comm, remote_leader, tag, newintercomm),
            (MPI_Comm, int, MPI_Comm, int, int, MPI_Comm*));
}

int MPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintracomm) {
      TSMPI_REPLACE(MPI_Intercomm_merge, int, (intercomm, high, newintracomm), (MPI_Comm, int, MPI_Comm*));
}

int MPI_Irsend(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag,
            MPI_Comm comm, MPI_Request *request) {
      TSMPI_REPLACE(MPI_Irsend, int, (buf, count, datatype, dest, tag, comm, request),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *));
}

int MPI_Issend(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag,
            MPI_Comm comm, MPI_Request *request) {
      TSMPI_REPLACE(MPI_Issend, int, (buf, count, datatype, dest, tag, comm, request),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *));
}

int MPI_Reduce_scatter(ARG_CONST void *sendbuf, void *recvbuf, ARG_CONST int recvcounts[],
            MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
      TSMPI_REPLACE(MPI_Reduce_scatter, int, (sendbuf, recvbuf, recvcounts, datatype, op, comm),
            (ARG_CONST void*, void *, ARG_CONST int[], MPI_Datatype, MPI_Op, MPI_Comm));
}

int MPI_Rsend(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
      TSMPI_REPLACE(MPI_Rsend, int, (buf, count, datatype, dest, tag, comm),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm));
}

int MPI_Rsend_init(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag,
            MPI_Comm comm, MPI_Request *request) {
      TSMPI_REPLACE(MPI_Rsend_init, int, (buf, count, datatype, dest, tag, comm, request),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
}

int MPI_Scatterv(ARG_CONST void *sendbuf, ARG_CONST int sendcounts[], ARG_CONST int displs[],
            MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
      TSMPI_REPLACE(MPI_Scatterv, int, (sendbuf, sendcounts, displs, sendtype, recvbuf,
            recvcount, recvtype, root, comm),
            (ARG_CONST void *, ARG_CONST int[], ARG_CONST int[], MPI_Datatype, void*,
            int, MPI_Datatype, int, MPI_Comm));
}

int MPI_Sendrecv(ARG_CONST void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest,
            int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source,
            int recvtag, MPI_Comm comm, MPI_Status *status) {
      TSMPI_REPLACE(MPI_Sendrecv, int, (sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount,
            recvtype, source, recvtag, comm, status),
           (ARG_CONST void *, int, MPI_Datatype, int, int, void *, int,
            MPI_Datatype, int, int, MPI_Comm, MPI_Status *));
}

int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source,
            int recvtag, MPI_Comm comm, MPI_Status *status) {
      TSMPI_REPLACE(MPI_Sendrecv_replace, int, (buf, count, datatype, dest, sendtag, source, recvtag, comm, status),
            (void *, int, MPI_Datatype, int, int, int, int, MPI_Comm, MPI_Status *));
}

int MPI_Ssend(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
       TSMPI_REPLACE(MPI_Ssend, int, (buf, count, datatype, dest, tag, comm),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm));
}

int MPI_Ssend_init(ARG_CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag,
            MPI_Comm comm, MPI_Request *request) {
      TSMPI_REPLACE(MPI_Ssend_init, int, (buf, count, datatype, dest, tag, comm, request),
            (ARG_CONST void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
}

int MPI_Startall(int count, MPI_Request array_of_requests[]) {
      TSMPI_REPLACE(MPI_Startall, int, (count, array_of_requests), (int, MPI_Request[]));
}

int MPI_Gatherv(ARG_CONST void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf,
            ARG_CONST int recvcounts[], ARG_CONST int displs[], MPI_Datatype recvtype, int root, MPI_Comm comm) {
      TSMPI_REPLACE(MPI_Gatherv, int, (sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm),
            (ARG_CONST void*, int , MPI_Datatype , void*, ARG_CONST int[], ARG_CONST int[], MPI_Datatype , int , MPI_Comm));
}

int MPI_Scan(ARG_CONST void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
      TSMPI_REPLACE(MPI_Scan, int , (sendbuf, recvbuf, count, datatype, op, comm),
      (ARG_CONST void*, void*, int, MPI_Datatype, MPI_Op , MPI_Comm));
}

void mpi_send_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
        MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_send_, int, (buf, count, datatype, dest, tag, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_recv_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
        MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status,
        MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_recv_, int, (buf, count, datatype, source, tag, comm, status, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_isend_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
        MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
        MPI_Fint *ierror) {
    TSMPI_REPLACE_VOID(mpi_isend_, int, (buf, count, datatype, dest, tag, comm, request, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_irecv_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
        MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
        MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_irecv_, int, (buf, count, datatype, source, tag, comm, request, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_wait_ (MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_wait_, int, (request, status, ierror),
            (MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_bcast_ (void *buffer, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_bcast_, int, (buffer, count, datatype, root, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_barrier_ (MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_barrier_, int, (comm, ierror),
            (MPI_Fint*, MPI_Fint*));
}

void mpi_reduce_ (void *sendbuf, void *recvbuf, MPI_Fint *count,
    MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_reduce_, int, (sendbuf, recvbuf, count, datatype, op, root, comm, ierror),
            (void*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_waitall_ (MPI_Fint * count, MPI_Fint array_of_requests[],
        MPI_Fint array_of_statuses[][SIZEOF_MPI_STATUS], MPI_Fint * ierror) {
    TSMPI_REPLACE_VOID(mpi_waitall_, int, (count, array_of_requests, array_of_statuses, ierror),
            (MPI_Fint*,MPI_Fint*, MPI_Fint[][SIZEOF_MPI_STATUS], MPI_Fint*));
}

void mpi_gather_ (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
    MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_gather_, int, (sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_scatter_ (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
    MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_scatter_, int, (sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_allreduce_ (void *sendbuf, void *recvbuf, MPI_Fint *count,
    MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_allreduce_, int, (sendbuf, recvbuf, count, datatype, op, comm, ierror),
            (void*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_cart_create_ (MPI_Fint *comm_old, MPI_Fint *ndims,
    MPI_Fint *dims,  MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_cart_create_, int, (comm_old, ndims, dims, periods, reorder, comm_cart, ierror),
            (MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_allgather_ (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
    MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_allgather_, int, (sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_alltoall_ (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
    MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_alltoall_, int, (sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_alltoallv_ (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sdispls, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount,
    MPI_Fint *rdispls, MPI_Fint *recvtype,  MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_alltoallv_, int, (sendbuf, sendcount, sdispls, sendtype, recvbuf, recvcount, rdispls, recvtype, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_bsend_init_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_bsend_init_, int, (buf, count, datatype, dest, tag, comm, request, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_ibsend_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_ibsend_, int, (buf, count, datatype, dest, tag, comm, request, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void CtoF77(mpi_intercomm_create)(MPI_Fint *local_comm,
    MPI_Fint *local_leader, MPI_Fint *peer_comm, MPI_Fint *remote_leader,
    MPI_Fint *tag, MPI_Fint *new_intercomm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_intercomm_create_, int, (local_comm, local_leader, peer_comm, remote_leader, tag, new_intercomm, ierror),
            (MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_intercomm_merge_ (MPI_Fint *intercomm, MPI_Fint *high,
    MPI_Fint *newintracomm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_intercomm_merge_, int, (intercomm, high, newintracomm, ierror),
            (MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_irsend_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_irsend_, int, (buf, count, datatype, dest, tag, comm, request, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_issend_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_issend_, int, (buf, count, datatype, dest, tag, comm, request, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_reduce_scatter_ (void *sendbuf, void *recvbuf,
    MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_reduce_scatter_, int, (sendbuf, recvbuf, recvcounts, datatype, op, comm, ierror),
            (void*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_rsend_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_rsend_, int, (buf, count, datatype, dest, tag, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_rsend_init_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_rsend_init_, int, (buf, count, datatype, dest, tag, comm, request, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_scatterv_ (void *sendbuf,  MPI_Fint *sendcount,
    MPI_Fint *displs, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount,
    MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_scatterv_, int, (sendbuf, sendcount, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_sendrecv_ (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, void *recvbuf,
    MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag,
    MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_sendrecv_, int, (sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_sendrecv_replace_ (void *buf, MPI_Fint *count, MPI_Fint *type,
    MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag,
    MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_sendrecv_replace_, int, (buf, count, type, dest, sendtag, source, recvtag, comm, status, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_ssend_init_ (void *buf, MPI_Fint *count, MPI_Fint *datatype,
    MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request,
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_ssend_init_, int, (buf, count, datatype, dest, tag, comm, request, ierror),
            (void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_startall_ (MPI_Fint *count, MPI_Fint array_of_requests[],
    MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_startall_, int, (count, array_of_requests, ierror),
            (MPI_Fint*, MPI_Fint[], MPI_Fint*));
}

void mpi_gatherv_ (void *sendbuf, MPI_Fint *sendcount,
    MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount,  MPI_Fint *displs,
    MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_gatherv_, int, (sendbuf, sendcount, sendtype, recvbuf, recvcount, displs, recvtype, root, comm, ierror),
            (void*, MPI_Fint*, MPI_Fint*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

void mpi_scan_ (void *sendbuf, void *recvbuf, MPI_Fint *count,
    MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierror){
    TSMPI_REPLACE_VOID(mpi_scan_, int, (sendbuf, recvbuf, count, datatype, op, comm, ierror),
            (void*, void*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*, MPI_Fint*));
}

