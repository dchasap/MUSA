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

#ifndef CORE_UTILS_TSMPI_LIBTSMPI_EXTERN_H_
#define CORE_UTILS_TSMPI_LIBTSMPI_EXTERN_H_

#ifdef __cplusplus
#   define API extern "C"
#else
#   define API
#endif

#include <libgen.h>


API void read_relation(int*, int*, int);
API void disable_extrae(void);


int relate_size = 2;
const char*  RELATION_FILENAME        = "MUSA_relation_filename";
const char*  PLAIN_RELATION_FILENAME  = "MUSA_relation_redux.txt";
const char*  MPIPHASES_TEMP           = "mpiphases_temp_rel_";

int64_t last_phase_id = -1;
int64_t last_phase_is_mem = -1;

unsigned tsmpi_initialized_ = 0;

typedef struct myId myId;
struct myId{
    int  ppid;
    int  rank;
    char hostname[256];
};

typedef struct {
    int64_t phase_id;
    int64_t master_id;
    bool      is_ph_nanos;
    bool      is_ph_mem;
} RT_WP_current_ph;


#define ARRAY_LENGTH 350

typedef struct {
    // process info  (once per process)
    bool      is_nanox_enabled;
    bool      is_extrae_enabled;
    int       mpi_rank;
    int       mpi_size;
    pid_t     pid;
    pid_t     ppid;
    FILE     *f_nanos_rel;
    FILE     *f_nanos_rel_b;
    FILE     *f_nanos_rel_redux_b;
    FILE     *f_mpi_rel_b;
    char      fname_nanos_rel_b[ARRAY_LENGTH];
    char      fname_nanos_rel_redux_b[ARRAY_LENGTH];
    char      fname_mpi_rel[ARRAY_LENGTH];
    char      fname_mpi_rel_b[ARRAY_LENGTH];
    char      fname_mpi_rel_redux_b[ARRAY_LENGTH];
    char      fname_extension[10];
    int64_t   counter_mem_phases;

    char      *exp_path_prefix;
    char      *exp_effective_relation;

    // current phase info (updated every phase)
    RT_WP_current_ph current;
} RT_WP_info;

typedef struct {
    unsigned int MASTER_OFFSET;
    unsigned int MASTER_MIN;
    unsigned int MASTER_MAX;
    unsigned int current_master;
} RT_PIN_info;

typedef struct {
    int64_t phase;
    int64_t rank;
} RT_PIN_filter;


RT_PIN_info      rt_pin_info;
RT_WP_info       rt_wp_info;
RT_PIN_filter    rt_pin_filter;


void rt_wp_get_extension(const char *appName)
{
    char *apptemp = (char*)malloc(sizeof(char)*(strlen(appName)+1));
    strcpy(apptemp, appName);
    strncpy(rt_wp_info.fname_extension, apptemp+(strlen(apptemp)-9), 6);
    free(apptemp);
    return;
}


void rt_wp_init_envar_info(void)
{
    // get MUSA_EXP_NAME...if it does not exist therefore path would be
    // current directory. It is important to maintain coherence between
    // the nanos plugin and the tasksim wrapper
    char *exp_path = getenv("MUSA_EXP_NAME");
    if (exp_path != NULL && exp_path[0] != '\0') {
        rt_wp_info.exp_path_prefix = (char*)malloc(sizeof(char) * (strlen(exp_path) + 2));
        if (rt_wp_info.exp_path_prefix == NULL) {
            printf("Error on malloc\n");
            exit(-1);
        }
        // memset(rt_wp_info.exp_path_prefix, '0', strlen(exp_path) + 2);
        strncpy(rt_wp_info.exp_path_prefix, exp_path, strlen(rt_wp_info.exp_path_prefix));
        // put a final slash on the string.
        rt_wp_info.exp_path_prefix[strlen(exp_path)] = '/';
    } else {
        rt_wp_info.exp_path_prefix = strdup("./");
    }

    // this can be done in rt_wp_init_envar_info
    memset(rt_wp_info.fname_mpi_rel_b, '\0', sizeof(char) * ARRAY_LENGTH);
    strcpy(rt_wp_info.fname_mpi_rel_b, rt_wp_info.exp_path_prefix);
    strcat(rt_wp_info.fname_mpi_rel_b, "/");
    strcat(rt_wp_info.fname_mpi_rel_b, RELATION_FILENAME);
    // strcat(rt_wp_info.fname_mpi_rel_b, '\0');

    memset(rt_wp_info.fname_mpi_rel_redux_b, '\0', sizeof(char) * ARRAY_LENGTH);
    strcpy(rt_wp_info.fname_mpi_rel_redux_b, rt_wp_info.exp_path_prefix);
    strcat(rt_wp_info.fname_mpi_rel_redux_b, "/");
    strcat(rt_wp_info.fname_mpi_rel_redux_b, PLAIN_RELATION_FILENAME);
    // strcat(rt_wp_info.fname_mpi_rel_redux_b, '\0');

    return;
}



void rt_wp_pin_init_envar_info(char * appName)
{
    // TEMPLATE:
    // [TRACE_bt-mz.A.2.200.iter_@@@@_000002_MEMO/bt-mz.A.2.200.iter_proc_000001.ts.mpiphases]
    // this can be done with basename and dirname instead

    char *full;
    char *prefix;
    int size;
    full = strdup(appName);

    dirname(full);
    size = strlen(full) + 2;

    prefix = (char*)malloc(strlen(full) + 2);
    memset(prefix, '\0', size);
    strcpy(prefix, full);
    prefix[size - 2] = '/';
    prefix[size - 1] = '\0';

    rt_wp_info.exp_path_prefix = strdup(prefix);

    return;
}

void rt_wp_init_info(void)
{
    rt_wp_info.is_nanox_enabled = false;
    rt_wp_info.is_extrae_enabled = false,
    rt_wp_info.mpi_rank = 0;
    rt_wp_info.mpi_size = 0;
    rt_wp_info.pid = 0;
    rt_wp_info.ppid = 0;
    rt_wp_info.f_nanos_rel_b = NULL;
    rt_wp_info.f_nanos_rel_redux_b = NULL;
    rt_wp_info.f_mpi_rel_b = NULL;
    rt_wp_info.current.phase_id = 0;
    rt_wp_info.current.master_id = 0;
    rt_wp_info.current.is_ph_nanos = false;
    rt_wp_info.current.is_ph_mem = false;
    rt_wp_info.counter_mem_phases = 0;

    return;
}


void rt_pin_init_info(void)
{
    rt_pin_info.MASTER_OFFSET = 20000000;
    rt_pin_info.current_master = 0;
    rt_pin_info.MASTER_MIN = 0;
    rt_pin_info.MASTER_MAX = 0;

    return;
}

#include "libtsmpi_extern-impl.h"

#endif  // CORE_UTILS_TSMPI_LIBTSMPI_EXTERN_H_
