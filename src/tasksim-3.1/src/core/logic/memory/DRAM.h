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


#ifndef CORE_LOGIC_MEMORY_DRAM_H_
#define CORE_LOGIC_MEMORY_DRAM_H_

#include <bitset>
#include <vector>

#include "core/engine/types.h"

namespace sim {
namespace logic {
namespace memory {

class DRAM {
public:
    typedef std::bitset<sizeof(engine::addr_t) * 8> range_t;

protected:
    uint64_t rank_range_;
    uint64_t bank_range_;
    uint64_t row_range_;
    uint64_t column_range_;

    class Bank {
    private:
        bool valid_;
        unsigned row_buffer_;
        bool precharge_;
        bool busy_;

    public:
        Bank();

        bool is_row_valid() const;
        unsigned get_row_buffer() const;
        void set_row_buffer(unsigned);
        void invalidate_row_buffer();

        bool precharge();

        bool is_busy() const;
        void set_busy();
        void unset_busy();
    };

    typedef std::vector<Bank *> Rank;
    typedef std::vector<Rank *> Dimm;

    unsigned n_ranks_, n_banks_, n_rows_, n_columns_;
    std::size_t size_;
    Dimm dimms_;

    unsigned get_index(const engine::addr_t &, uint64_t) const;
    unsigned get_rank_index(const engine::addr_t &) const;
    unsigned get_bank_index(const engine::addr_t &) const;
    unsigned get_row_index(const engine::addr_t &) const;
    unsigned get_column_index(const engine::addr_t &) const;

    template<typename T>
    T &access(std::vector<T *> &, unsigned);
    Bank &get_bank(const engine::addr_t &);

public:
    DRAM(const range_t &, const range_t &, const range_t &, const range_t &);
    virtual ~DRAM();

    void set(const engine::addr_t &);
    bool get(const engine::addr_t &);
    bool precharge(const engine::addr_t &);
    void invalidate(const engine::addr_t &);

    bool is_busy(const engine::addr_t &);
    void set_busy(const engine::addr_t &);
    void unset_busy(const engine::addr_t &);
};


}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "DRAM-impl.h"

#endif  // CORE_LOGIC_MEMORY_DRAM_H_
