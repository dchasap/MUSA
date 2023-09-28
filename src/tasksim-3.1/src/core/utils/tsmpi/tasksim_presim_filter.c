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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct cad_list {
    struct cad_list *next;
    char *cad;
} cad_list;


/**
  * CAA: a way to obtain end position of second field
  * for the rank+phase identifier on simulated results.
  * Taking into consideration variable sizes. Format:
  *     xxxxxxxxxxxxxxx----------------
  *     000003:1802610:777:0:5585775834
  */


int second_pos(char const *str)
{
    char const *p = str;
    int count;
    p = strstr(p, ":");
    p++;
    if (!p) {
        return -1;
    }
    p = strstr(p, ":");
    if (!p) {
        return -1;
    }
    return p-str+1;
}


/********************************************/
int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: tasksim_presim_filter [greasy_commands_file] [partial_results_file] > [filtered_greasy_commands_file]\n");
        return -1;
    }

    FILE *fd_commands, *fd_partial;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    // list of all the partial results
    int presim_num = 0;
    int command_num = 0;
    char *cad;

    cad_list *presim_ini, *presim_end, *presim_p;
    cad_list *command_ini, *command_end, *command_p;
    presim_ini = presim_end = presim_p = NULL;
    command_ini = command_end = command_p = NULL;



    fd_commands = fopen(argv[1], "r");
    if (fd_commands == NULL) {
        exit(EXIT_FAILURE);
    }

    fd_partial = fopen(argv[2], "r");
    if (fd_partial == NULL) {
        exit(EXIT_FAILURE);
    }

    // Load Partial Presimulation results
    while ((read = getline(&line, &len, fd_partial)) != -1) {
        // Extract rank+phase id from line
        int offset = second_pos(line);
        if (offset == -1) {
            printf("Error on presimulation data\n");
            exit(-1);
        }
        presim_num++;
        // Initialize and load new struct for presim result
        presim_p = malloc ( sizeof(cad_list));
        presim_p->cad = malloc(offset * sizeof(char));
        memcpy(presim_p->cad, line, offset);
        // Add to linked list
        presim_p->next = NULL;
        if (presim_num == 1) {
            presim_ini = presim_p;
        } else {
            presim_end->next = presim_p;
        }
        presim_end = presim_p;
    }
    fclose(fd_partial);

    // Load Command lines
    while ((read = getline(&line, &len, fd_commands)) != -1) {
        command_num++;
        // Initialize and load new structu form command
        command_p = malloc ( sizeof(cad_list));
        command_p->cad = malloc(len * sizeof(char));
        // Do not know why two end lines in each command.
        // Removed by len-2 instead of len-1
        memcpy(command_p->cad, line, len - 2);
        command_p->next = NULL;
        if (command_num == 1) {
            command_ini = command_p;
        } else {
            command_end->next = command_p;
        }
        command_end = command_p;
    }
    fclose(fd_commands);

    cad_list *command_prev = NULL;
    command_p = command_ini;

    while (command_p != NULL) {
        int found = 0;
        presim_p = presim_ini;
        while (presim_p != NULL) {
            // Search presim_p->cad in line;
            if (strstr(command_p->cad, presim_p->cad) != NULL) {
                found = 1;
                presim_p = presim_end;
            }
            presim_p = presim_p->next;
        }

        if (found) {
            // the command line is deleted
            cad_list *temp = command_p;

            if (command_p == command_ini) {
                command_ini = command_ini->next;
                command_p = command_ini;
                command_prev = NULL;
            } else {
                command_prev->next = command_p->next;
                command_p = command_p->next;
            }
            free(temp);
        } else {
            // if not found..
            command_prev = command_p;
            command_p = command_p->next;
        }
    }

    // Show commands
    command_p = command_ini;
    while (command_p != NULL) {
        printf("%s \n", command_p->cad);
        command_p = command_p->next;
    }
    return 0;
}
