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

#ifndef CORE_UTILS_TSMPI_LIBTSMPI_TYPES_H_
#define CORE_UTILS_TSMPI_LIBTSMPI_TYPES_H_


int relate_size = 2;
int64_t last_phase_id = -1;
int64_t last_phase_is_mem = -1;
unsigned tsmpi_initialized_ = 0;
const static std::string RELATION_FILENAME("MUSA_relation_filename");
const static std::string PLAIN_RELATION_FILENAME("MUSA_relation_redux.txt");
const static std::string MPIPHASES_TEMP("mpiphases_temp_rel_");


const static unsigned ARRAY_LENGTH = 350;
class myId {
public:
    int  ppid;
    int  rank;
    char hostname[256];
};

class current_phase_t {
public:
    int64_t phase_id;
    int64_t master_id;
    bool is_nanos;
    bool is_mem;
    current_phase_t() :
        phase_id(0),
        master_id(0),
        is_nanos(false),
        is_mem(false)
    {}
};

std::ostream &operator<<(std::ostream &os, const current_phase_t &phase_info);
std::istream &operator>>(std::istream &is, current_phase_t &phase_info);

inline std::ostream &operator<<(std::ostream &os, const current_phase_t &phase_info)
{
    os << phase_info.phase_id << ":"
        << phase_info.master_id << ":"
        << (phase_info.is_nanos ? "1" : "0") << ":"
        << (phase_info.is_mem ? "1" : "0");
    return os;
}

inline std::istream &operator>>(std::istream &is, current_phase_t &phase_info)
{
    char sep;
    unsigned val;
    is >> phase_info.phase_id >> sep;
    assert(sep == ':');
    is >> phase_info.master_id >> sep;
    assert(sep == ':');
    is >> val >> sep;
    assert(sep == ':');
    phase_info.is_nanos = (val==1);
    is >> val >> sep;
    phase_info.is_mem = (val==1);
    return is;
}

class RT_WP_info {
    // process info  (once per process)
public:
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
    current_phase_t current_phase;
    RT_WP_info() :
        is_nanox_enabled(false),
        is_extrae_enabled(false),
        mpi_rank(0),
        mpi_size(0),
        pid(0),
        ppid(0),
        f_nanos_rel(NULL),
        f_nanos_rel_b(NULL),
        f_nanos_rel_redux_b(NULL),
        f_mpi_rel_b(NULL),
        counter_mem_phases(0),
        current_phase()
    {}
    void load_environment_info() {
        // get MUSA_EXP_NAME...if it does not exist therefore path would be
        // current directory. It is important to maintain coherence between
        // the nanos plugin and the tasksim wrapper
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

        // this can be done in rt_wp_init_envar_info
        memset(fname_mpi_rel_b, '\0', sizeof(char) * ARRAY_LENGTH);
        strcpy(fname_mpi_rel_b, exp_path_prefix);
        strcat(fname_mpi_rel_b, "/");
        strcat(fname_mpi_rel_b, RELATION_FILENAME.c_str());

        memset(fname_mpi_rel_redux_b, '\0', sizeof(char) * ARRAY_LENGTH);
        strcpy(fname_mpi_rel_redux_b, exp_path_prefix);
        strcat(fname_mpi_rel_redux_b, "/");
        strcat(fname_mpi_rel_redux_b, PLAIN_RELATION_FILENAME.c_str());

        return;
    }
};

RT_WP_info       rt_wp_info;


#endif  // CORE_UTILS_TSMPI_LIBTSMPI_TYPES_H_
