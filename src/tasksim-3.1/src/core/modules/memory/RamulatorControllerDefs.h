/*
 * Copyright (c) 2016, Barcelona Supercomputing Center
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

#ifndef CORE_MODULES_MEMORY_RAMULATORCONTROLLERDEFS_H_
#define CORE_MODULES_MEMORY_RAMULATORCONTROLLERDEFS_H_

#include <string>
#include <utility>
#include <vector>

namespace sim {
namespace memory {

static const std::vector<std::pair<std::string, std::string>> dram_ini_options = {
        {"standard", "DDR4"},
        {"channels", "1"},
        {"ranks", "1"},
        {"speed", "DDR4_2400R"},
        {"org", "DDR4_4Gb_x8"},
        {"record_cmd_trace", "off"},
        {"print_cmd_trace", "off"},
        {"cpu_tick", "8"},
        {"mem_tick", "3"},
        {"early_exit", "on"},
        {"expected_limit_insts", "200000000"},
        {"cache", "no"},
        {"translation", "None"}};

}  // namespace memory
}  // namespace sim

#endif  // CORE_MODULES_MEMORY_RAMULATORCONTROLLERDEFS_H_
