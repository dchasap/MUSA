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

#ifndef CORE_UTILS_TSMPI_LIBTSMPI_EXTERN_IMPL_H_
#define CORE_UTILS_TSMPI_LIBTSMPI_EXTERN_IMPL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "libtsmpi_extern.h"

void rt_wp_get_nanos_relation(const char *appName);
void rt_wp_read_file_phase(FILE *fp);
unsigned int rt_wp_read_offset(FILE *fp);
void rt_wp_get_rank_from_temporary(void);
void read_relation_b(int *, int *, int, char*);


/************************************************************************/

void rt_wp_get_nanos_relation(const char *appName)
{
    char pattern[] = "_proc_";
    char *app, *pos;

    app = (char*)malloc(strlen(appName) + 1);
    strcpy(app, appName);
    pos = strstr(app, pattern);
    *(pos+strlen(pattern))='\0';

    // if version MPI 1.8.1 , mpi++/
    rt_wp_info.mpi_rank++;
    sprintf(rt_wp_info.fname_nanos_rel_b, "%s%06d.ts.mpiphases", app, rt_wp_info.mpi_rank);

    return;
}


/************************************************************************/
// Obtain master offset from 2 line 2 field
unsigned int rt_wp_read_offset(FILE *fp)
{
    int   length = 200;
    char *my_string;
    char *key;
    unsigned int offset_val;

    my_string = (char*)malloc(length + 1);

    if (fgets (my_string, length, fp) == NULL) return rt_pin_info.MASTER_OFFSET;
    if (fgets (my_string, length, fp) == NULL) return rt_pin_info.MASTER_OFFSET;

    strtok_r(my_string, ":", &key);
    offset_val   = atoll(strtok_r(NULL, ":", &key));

    return offset_val;
}


/************************************************************************/
void rt_wp_read_file_phase(FILE *fp)
{
    int   length = 200;
    char *my_string;
    char *key;

    my_string = (char*)malloc(length + 1);

    if (fgets(my_string, length, fp) == NULL) {
        printf("[TSMPI ERROR] Error reading line from relation file.\n");
        exit(-1);
    }
    printf("Just read line %s.\n", my_string);
    rt_wp_info.current.phase_id    = atoll(strtok_r(my_string, ":", &key));
    rt_wp_info.current.master_id   = atoll(strtok_r(NULL, ":", &key));
    rt_wp_info.current.is_ph_nanos = (atoi(strtok_r(NULL, ":", &key)) == 1) ? true : false;
    rt_wp_info.current.is_ph_mem   = (atoi(strtok_r(NULL, ":", &key)) == 1) ? true : false;

    free(my_string);
    return;
}


/************************************************************************/
/************************************************************************/

void read_relation_b(int *my_rank, int *my_size, int myPID, char *hostname)
{
    *my_rank = 0;
    *my_size = 0;
    myId n;

    FILE *fd = fopen(rt_wp_info.fname_mpi_rel_b, "rb");
    if ( fd == NULL ) {
      printf("[WRAPPER]#####################file [%s] does not exists. MPI NOT called \n", rt_wp_info.fname_mpi_rel_b);
      exit(-1);
    }

    // find rank for ppid and hostname!!!
    int found = 0;
    int counter = 0;
    while (!feof(fd)) {
        if (fread(&n, sizeof(myId), 1, fd) > 0) {
            counter++;
            if (n.ppid == myPID && strcmp(n.hostname, hostname) == 0) {
                found = 1;
                *my_rank = n.rank;
            }
        }
    }
    if (found) {
        *my_size = counter;
    }
    fclose(fd);

    #ifdef DBG_WRAPPER
    printf("[WRAPPER]#####################Succeed - MPI has been called \n");
    #endif

    return;
}



/************************************************************************/
/************************************************************************/
void rt_wp_get_rank_from_temporary(void)
{
    int array[3];
    int found = 0;
    char name[16];
    char hostname[256];

    int lenA = strlen(rt_wp_info.exp_path_prefix);
    int lenB = strlen(PLAIN_RELATION_FILENAME);
    int length = lenA + lenB + 1;
    char *myfile = (char*)malloc(sizeof(char) * (length+1));
    strncpy(myfile, rt_wp_info.exp_path_prefix, strlen(myfile));
    strcpy(myfile + lenA, PLAIN_RELATION_FILENAME);

    FILE *fp = fopen(myfile, "r");
    if (fp == NULL) {
        printf("[TSMPI]#### ERROR; File [%s] does not exists. MPI NOT called \n", myfile);
        exit(-1);
    }

    while (fscanf(fp, "%d %d %d %s %s\n", &array[0], &array[1], &array[2], hostname, name) != EOF) {
        if (strcmp(name, rt_wp_info.fname_extension) == 0) {
            found = 1;
            rt_wp_info.mpi_rank = array[0] - 1;
       }
    }
    fclose(fp);
    if (!found) {
        // as defined by environmental variables TMPDIR_TMP_TRACE and TMPDIR_OUT_TRACE correspondingly.
        printf("[TSMPI] Extension %s NOT FOUND ON FILE %s\n", rt_wp_info.fname_extension, myfile);
        exit(0);
    }

    free(myfile);

    #ifdef DBG_WRAPPER
    printf("[WRAPPER]##################### Succeded - MPI has been called \n");
    #endif

    return;
}

#endif  // CORE_UTILS_TSMPI_LIBTSMPI_EXTERN_IMPL_H_
