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

#ifndef CORE_MODULES_MEMORY_DRAMSIM2CONTROLLERDEFS_H_
#define CORE_MODULES_MEMORY_DRAMSIM2CONTROLLERDEFS_H_

#include <string>
#include <utility>
#include <vector>

namespace sim {
namespace memory {

static const std::vector<std::pair<std::string, std::string>> system_ini_options = {
        {"NUM_CHANS", "4"},
        {"JEDEC_DATA_BUS_BITS", "64"},
        {"TRANS_QUEUE_DEPTH", "32"},
        {"CMD_QUEUE_DEPTH", "32"},
        {"EPOCH_LENGTH", "100000"},
        {"ROW_BUFFER_POLICY", "open_page"},
        {"ADDRESS_MAPPING_SCHEME", "scheme7"},
        {"SCHEDULING_POLICY", "rank_then_bank_round_robin"},
        {"QUEUING_STRUCTURE", "per_rank"},
        {"DEBUG_TRANS_Q", "false"},
        {"DEBUG_CMD_Q", "false"},
        {"DEBUG_ADDR_MAP", "false"},
        {"DEBUG_BUS", "false"},
        {"DEBUG_BANKSTATE", "false"},
        {"DEBUG_BANKS", "false"},
        {"DEBUG_POWER", "false"},
        {"VIS_FILE_OUTPUT", "false"},
        {"USE_LOW_POWER", "true"},
        {"VERIFICATION_OUTPUT", "false"},
        {"TOTAL_ROW_ACCESSES", "4"}};

static const std::vector<std::pair<std::string, std::string>> dram_ini_options = {
        {"NUM_BANKS", "8"},
        {"NUM_ROWS", "32768"},
        {"NUM_COLS", "1024"},
        {"DEVICE_WIDTH", "8"},
        {"REFRESH_PERIOD", "7800"},
        {"tCK", "5"},
        {"CL", "10"},
        {"AL", "0"},
        {"BL", "8"},
        {"tRAS", "24"},
        {"tRCD", "10"},
        {"tRRD", "4"},
        {"tRC", "34"},
        {"tRP", "10"},
        {"tCCD", "4"},
        {"tRTP", "5"},
        {"tWTR", "5"},
        {"tWR", "10"},
        {"tRTRS", "1"},
        {"tRFC", "107"},
        {"tFAW", "20"},
        {"tCKE", "4"},
        {"tXP", "4"},
        {"tCMD", "1"},
        {"IDD0", "130"},
        {"IDD1", "155"},
        {"IDD2P", "10"},
        {"IDD2Q", "70"},
        {"IDD2N", "70"},
        {"IDD3Pf", "60"},
        {"IDD3Ps", "60"},
        {"IDD3N", "90"},
        {"IDD4W", "300"},
        {"IDD4R", "255"},
        {"IDD5", "305"},
        {"IDD6", "9"},
        {"IDD6L", "12"},
        {"IDD7", "460"},
        {"Vdd", "1.5"}};

}  // namespace memory
}  // namespace sim

#endif  // CORE_MODULES_MEMORY_DRAMSIM2CONTROLLERDEFS_H_
