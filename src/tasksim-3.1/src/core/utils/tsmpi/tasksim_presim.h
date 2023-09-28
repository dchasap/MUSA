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

#ifndef CORE_UTILS_TSMPI_TASKSIM_PRESIM_H_
#define CORE_UTILS_TSMPI_TASKSIM_PRESIM_H_

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

class sim_phase {
    public:
    int id;
    int phase_id;
    int is_ompss;
    int mem_trace;
};

std::istream &operator>>(std::istream &is, sim_phase &line)
{
    char c;
    is >> line.id >> c;
    assert(c == ':');
    is >> line.phase_id >> c;
    assert(c == ':');
    is >> line.is_ompss >> c;
    assert(c == ':');
    is >> line.mem_trace >> c;
    assert(c == ':');
    int mpi_event;
    is >> mpi_event >> c;
    assert(c == ':');
    std::string mpi_name;
    is >> mpi_name;
    return is;
}

class sim_file_desc {
    public:
    std::string filename, path, proc, sim_path, out_prefix, out_folder;
    sim_file_desc(std::string fn, std::string p, std::string pr, std::string sim_p,
                  std::string sim_c, std::string out_p, std::string out_f) :
            filename(fn),
            path(p),
            proc(pr),
            sim_path(sim_p),
            out_prefix(out_p),
            out_folder(out_f)
    {}
    sim_file_desc(){}
    std::string serialize() {
        return filename + ":" + path + ":" + proc + ":" + sim_path + ":" +
               out_prefix +":" + out_folder;
    }
    void load(std::string message) {
        // Filename:
        int pos = message.find(":");
        filename = message.substr(0, pos);
        message = message.substr(pos+1);
        // Path:
        pos = message.find(":");
        path = message.substr(0, pos);
        message = message.substr(pos+1);
        // Proc:
        pos = message.find(":");
        proc = message.substr(0, pos);
        message = message.substr(pos+1);
        // Sim_path:
        pos = message.find(":");
        sim_path = message.substr(0, pos);
        message = message.substr(pos+1);
        // out_prefix:
        pos = message.find(":");
        out_prefix = message.substr(0, pos);
        message = message.substr(pos+1);
        // out_folder:
        out_folder = message;
    }
};


class sim_file {
    public:
    int num_mem_phases;
    sim_file_desc file_desc;
    std::vector<sim_phase> sim_phases;
    int rank;
    sim_file(){}
    friend bool operator<(const sim_file &l, const sim_file &r) {
        return l.rank < r.rank;
    }
};

class enviroment_vars {
    public:
    bool only_memory;
    bool no_memory;
    bool keep_logs;
    int num_cores;
    enviroment_vars(bool only_mem, bool no_mem, bool keep_l, int num_c) :
        only_memory(only_mem),
        no_memory(no_mem),
        keep_logs(keep_l),
        num_cores(num_c)
    {}
};

// MPI general variables, global to functions
int rank, size;

class presim_parameters {
    public:
    std::string output, folder, out_folder;
    std::vector<std::string> config;
    bool t_flag, d_flag, o_flag, c_flag, g_flag, s_flag, f_flag, z_flag;
};

static const std::string cmd_tasksim("tasksim");


// Information from traces
int total_phases = 0;
int total_mem_phases = 0;
std::vector<sim_file> sim_files;

enviroment_vars get_enviroment_vars();
bool check_parameters(int argc, char **argv, presim_parameters &params);
bool check_folder(const presim_parameters &params);
bool get_file_list(const presim_parameters &params, const enviroment_vars &env);
bool check_file_exists(const std::string &filename);
void load_config_files(const std::string &folder, std::vector<std::string> &config_files);

void display_usage();
bool get_phase_list(const enviroment_vars &env);
void master_greasy(const presim_parameters &params);

#endif  // CORE_UTILS_TSMPI_TASKSIM_PRESIM_H_
