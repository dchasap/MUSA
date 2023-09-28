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

#include "tasksim_presim.h"

#include <dirent.h>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>


void display_usage()
{
    printf("[*]\tmpirun -np $procs tasksim_presim -d $input_trace.folder -o $output_presim.file  -c $input_config.file [-t $output_prv.folder]\n");
}

bool check_parameters(int argc, char **argv, presim_parameters &params)
{
    char c;
    if (argc == 1) {
        display_usage();
        return false;
    }

    while ((c = getopt(argc, argv, "d:o:c:t:gsfzq:")) != static_cast<char>(-1)) {
       switch (c) {
           case 'd':
               params.d_flag = true;
               params.folder = std::string(optarg);
               break;
           case 'o':
               params.o_flag = 1;
               params.output = std::string(optarg);
               break;
           case 'q':
               params.c_flag = 1;
               load_config_files(std::string(optarg), params.config);
               break;
           case 'c':
               params.c_flag = 1;
               params.config.push_back(std::string(optarg));
               break;
           case 't':
               params.t_flag = 1;
               params.out_folder = std::string(optarg);
               break;
           case 'g':
               params.g_flag = 1;
               break;
           case 's':
               params.s_flag = 1;
               break;
           case 'f':
               params.f_flag = 1;
               break;
           case 'z':
               params.z_flag = 1;
               break;
           case '?':
               display_usage();
               return false;
           default:
               std::cout << "[ERROR] Unknown parameter " << c << std::endl;
               display_usage();
               return false;
         }
    }
    // Check Values
    if (!params.d_flag || !params.o_flag || !params.c_flag) {
        display_usage();
        return false;
    } else {
        return true;
    }
}

bool check_file_exists(const std::string &filename) {
    std::ifstream f(filename);
    return f.good();
}

bool check_folder(const presim_parameters &params)
{
    struct stat s;
    // printf("Check if folder exists [%s]", params.folder.c_str());
    int err = stat(params.folder.c_str(), &s);
    if (err == -1) {
        if (ENOENT == errno) {
            std::cout << "[ERROR] Folder " << params.folder
                      << " does not exist." << std::endl;
            return false;
        } else {
            std::cout << "[ERROR] Stat." << std::endl;
            return false;
        }
    } else {
        if (!S_ISDIR(s.st_mode)) {
            std::cout << "[ERROR] " << params.folder << " is not a folder." << std::endl;
            return false;
        }
    }
    return true;
}

void load_config_files(const std::string &folder, std::vector<std::string> &config_files)
{
    struct dirent *ent;
    DIR *dir = opendir(folder.c_str());
    if (dir == NULL) {
        std::cout << "[ERROR] Could not open directory " << folder << std::endl;
        return;
    }
    while ((ent = readdir(dir)) != NULL) {
        const std::string name(ent->d_name);
        if (ent->d_type == DT_DIR || (name.find(".conf") == std::string::npos)) {
            // Nothing to do
            continue;
        }
        config_files.push_back(folder + "/" + name);
    }
    return;
}

bool get_file_list(const presim_parameters &params, const enviroment_vars &env)
{
    struct dirent *ent;

    DIR *dir = opendir(params.folder.c_str());
    if (dir == NULL) {
        std::cout << "[ERROR] Could not open directory " << params.folder << std::endl;
        return false;
    }
    while ((ent = readdir(dir)) != NULL) {
        const std::string name(ent->d_name);
        if (ent->d_type == DT_DIR || (name.find(".ts.mpiphases") == std::string::npos)) {
            // Nothing to do
            continue;
        }
        // Shortcut to avoid parsing traces belonging to Burst-only MPI Ranks.
        if (env.only_memory) {
            std::string mem_file = params.folder + "/" + name.substr(0, name.find("mpiphases")) + "mem.trace";
            if (check_file_exists(mem_file) == false) {
                // std::cerr << "Ignoring trace " << (params.folder + "/" + name) << " because we want only memory mode and "
                //           << (params.folder + "/" + mem_file) << " does not exist." << std::endl;
                continue;
            }
        }
        // get proc name
        int ini = name.find("_proc_") + 6;  // 6 := sizeof("_proc_")
        int fin = name.find(".ts.mpiphases");
        // Add elements to structure
        sim_files.push_back(sim_file());
        sim_file &last_file = sim_files.back();
        sim_file_desc &file_desc = last_file.file_desc;

        file_desc.filename = name;
        file_desc.path = params.folder + "/" + ent->d_name;
        file_desc.proc = name.substr(ini, fin - ini);

        // Obtain the name of the trace to be simualted
        // Used as a parameter for tasksim
        fin = name.find(".mpiphases");
        file_desc.sim_path = params.folder + "/" + name.substr(0, fin);
        file_desc.out_prefix = name.substr(0, fin);
        file_desc.out_folder = params.out_folder;
        // Initialize values.
        last_file.num_mem_phases = 0;
        last_file.rank = std::stoi(file_desc.proc);
    }
    closedir(dir);
    return true;
}


bool get_phase_list(const enviroment_vars &env)
{
    for (unsigned i = 0 ; i < sim_files.size() ; i++) {
        sim_file &elem = sim_files[i];

        std::ifstream in_file(elem.file_desc.path, std::ifstream::in);
        if (in_file.is_open() == false) {
            std::cout << "[ERROR] Opening file " << elem.file_desc.path << std::endl;
            return false;
        }
        sim_phase line;
        while (in_file >> line) {
            if (line.is_ompss && (!env.only_memory || line.mem_trace)) {
                if (env.no_memory == 0) {
                    line.mem_trace = 0;
                }
                elem.sim_phases.push_back(line);
                if (line.mem_trace) {
                    elem.num_mem_phases++;
                    total_mem_phases++;
                }
                total_phases++;
            }
        }
        in_file.close();
    }
    return true;
}

std::string create_pad_num(int num, size_t length, char padding)
{
    std::string str_num = std::to_string(num);
    if (length <= str_num.size()) {
        return str_num;
    } else {
        return std::string('0', length - str_num.size()) + str_num;
    }
}

void master_greasy(const presim_parameters &params)
{
    const std::string zip_cmd("&& tar zcf zipped_cmd_traces.tar.gz cmd-trace* && rm *.cmdtrace");
    for (const std::string &config_file : params.config) {
        for (const sim_file &file : sim_files) {
            const std::string &sim_path = file.file_desc.sim_path;
            for (const sim_phase &phase : file.sim_phases) {
                std::string pad_phase_id = create_pad_num(phase.phase_id, 6, '0');
                std::string burst_folder_name = params.out_folder + "/" + pad_phase_id;

                // First, lets create the out_folder if doesnt exists.
                struct stat s;
                int err = stat(params.out_folder.c_str(), &s);
                if (err == -1) {
                    int dir_err = mkdir(params.out_folder.c_str(), 0755);
                    if (dir_err == -1) {
                        std::cout << "[ERRROR] Cannot create folder " << params.out_folder << std::endl;
                        exit(-1);
                    }
                }

                // Then we check the output log.
                const std::string outlog = file.file_desc.out_prefix + "_" +
                                           pad_phase_id + "_" +
                                           (phase.mem_trace != 0 ? "MEMO" : "BRST") +
                                           ".simout";

                err = stat(burst_folder_name.c_str(), &s);
                if (err == -1) {
                    int dir_err = mkdir(burst_folder_name.c_str(), 0755);
                    if (dir_err == -1) {
                        std::cerr << "[ERROR] Cannot create output directory " << burst_folder_name
                                  << ", aborting." << std::endl;
                        exit(-1);
                    }
                } else {
                    const std::string outlog_path = burst_folder_name + "/" + outlog;
                    if (access(outlog_path.c_str(), F_OK) != -1) {
                        FILE *fp = fopen(outlog_path.c_str(), "r");
                        fseek(fp, 0L, SEEK_END);
                        size_t sz = ftell(fp);
                        fclose(fp);
                        if (sz) {
                            std::cerr << "[WARNING] Skipping simulation " << outlog_path << std::endl;
                            continue;
                        } else {
                            std::cerr << "[ERROR] Folder " << burst_folder_name
                                      << " is broken, cleaning it." << std::endl;
                            const std::string delete_folder_contents = "rm -r " + burst_folder_name + "/*";
                            system(delete_folder_contents.c_str());
                        }
                    }
                }

                std::ostringstream n_command;
                n_command << "cd " << burst_folder_name << " && "
                          << cmd_tasksim << " ../../../" << config_file
                          << " ../../../" << sim_path << " " << phase.phase_id;
                n_command << " | tee " << outlog << " "
                          << (params.z_flag ? zip_cmd : "") << " && cat " << outlog
                          << " | head -n 1 | cut -d' ' -f4 | awk '{print \""
                          << std::setfill('0') << std::setw(6) << file.rank << ":"
                          << phase.phase_id << ":777:" << phase.mem_trace << ":\"$var}' >> ../"
                          << params.output;


                std::cout << n_command.str() << std::endl;
            }
            // We copy the configuration file for safe keeping
            if (file.sim_phases.size()) {
                system(std::string("/bin/cp " + config_file + " " + params.out_folder).c_str());
            }
        }
    }
    return;
}


enviroment_vars get_environment_vars(void)
{
    enviroment_vars env(false, false, false, 1);
    char *envar;
    envar = getenv("TS_SIM_MEM");
    if (envar != NULL) {
        env.no_memory = std::string(envar) != "0";
    }
    envar = getenv("TS_SIM_LOG");
    if (envar != NULL) {
        env.keep_logs = std::string(envar) != "0";
    }
    envar = getenv("TS_SIM_CORES");
    if (envar != NULL) {
        env.num_cores = atoi(envar);
    }
    envar = getenv("TS_SIM_ONLY_MEM");
    if (envar != NULL) {
        env.only_memory = std::string(envar) != "0";
    }
    return env;
}


int main(int argc, char **argv)
{
    enviroment_vars env = get_environment_vars();
    presim_parameters params;
    if (!check_parameters(argc, argv, params)) {
        printf("Error on parameters.\n");
        exit(-1);
    }

    // Check if greasy file generation is enabled.
    if (params.g_flag) {
        if (!check_folder(params)) {
            printf("Tasksim Trace folder not found.\n");
            exit(-1);
        }

        if (!get_file_list(params, env)) {
            printf("Failure while retrieving file list.\n");
            exit(-1);
        }

        if (!get_phase_list(env)) {
            printf("Failure while retrieving phase list.\n");
            exit(-1);
        }

        std::sort(sim_files.begin(), sim_files.end());

        if (params.s_flag) {
            master_greasy(params);
        } else {
            printf("NON Greasy no longer supported.\n");
        }
    } else {
        printf("On MPI no longer supported\n");
        return -1;
    }
    return 0;
}
