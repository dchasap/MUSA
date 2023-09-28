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


#include "DRAM.h"

namespace sim {
namespace logic {
namespace memory {

DRAM::DRAM(const range_t &ranks, const range_t &banks, const range_t &rows,
            const range_t &columns) :
        rank_range_(ranks.to_ullong()),
        bank_range_(banks.to_ullong()),
        row_range_(rows.to_ullong()),
        column_range_(columns.to_ullong()),
        n_ranks_(1 << (ranks.count())),
        n_banks_(1 << (banks.count())),
        n_rows_(1 << (rows.count())),
        n_columns_(1 << (columns.count())),
        size_(0)
{
    unsigned last_bytes_bit = ffsl(rank_range_ | bank_range_ | row_range_ | column_range_);
    std::size_t column_size = 1 << (last_bytes_bit - 1);
    size_ = column_size * n_columns_ * n_rows_ * n_banks_ * n_ranks_;
}


DRAM::~DRAM()
{
    for (auto rank : dimms_) {
        if (rank == nullptr) {
            continue;
        }
        for (auto bank : *rank) {
            if (bank == nullptr) {
                continue;
            }
            delete bank;
        }
        delete rank;
    }
}


}  // namespace memory
}  // namespace logic
}  // namespace sim
