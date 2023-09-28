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


#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "core/tools/ompss_util.h"

namespace sim {
namespace tools {

void cleanup_ompss_trace(const std::string &temp_file_name)
{
    std::string dir_path;
    std::string file_prefix;
    size_t dir_sep = temp_file_name.find_last_of("/\\");

    if (dir_sep != std::string::npos) {
        dir_path.assign(temp_file_name, 0, dir_sep);
        file_prefix.assign(temp_file_name, dir_sep + 1, std::string::npos);
    } else {
        dir_path.assign(".");
        file_prefix.assign(temp_file_name);
    }

    DIR *dir = opendir(dir_path.c_str());
    assert(dir != NULL);

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        if (strncmp(ent->d_name, file_prefix.c_str(), file_prefix.size()) != 0) {
            continue;
        }
        const std::string file_name(dir_path + "/" + std::string(ent->d_name));
        int error = unlink(file_name.c_str());
        if (error) {
            std::cerr << "[MERGE] Cannot remove intermediate file " << file_name << std::endl;
        } else {
            std::cerr << "[MERGE] Intermediate file " << file_name << " removed." << std::endl;
        }
    }
    std::cout << "[MERGE] Cleanup process done." << std::endl;
    closedir(dir);
    return;
}

bool file_exists(const std::string &file_name)
{
    std::ifstream is(file_name);
    return is.good();
}

void alloc_and_copy(char * argv[], unsigned i, const char *src)
{
    argv[i] = reinterpret_cast<char*>(malloc(strlen(src) + 1));
    strncpy(argv[i], src, strlen(argv[i]));
}

}  // namespace tools
}  // namespace sim
