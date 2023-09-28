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


#ifndef CORE_LOGIC_MEMORY_DRAM_IMPL_H_
#define CORE_LOGIC_MEMORY_DRAM_IMPL_H_

#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

namespace sim {
namespace logic {
namespace memory {

inline
DRAM::Bank::Bank() :
    valid_(false),
    row_buffer_(0),
    precharge_(false),
    busy_(false)
{}


inline
bool DRAM::Bank::is_row_valid() const
{
    return valid_;
}


inline
unsigned DRAM::Bank::get_row_buffer() const
{
    assert(valid_ == true);
    return row_buffer_;
}


inline
void DRAM::Bank::set_row_buffer(unsigned row)
{
    assert(precharge_ == true);
    row_buffer_ = row;
    precharge_ = false;
    valid_ = true;
}


inline
void DRAM::Bank::invalidate_row_buffer()
{
    valid_ = false;
}


inline
bool DRAM::Bank::precharge()
{
    if (precharge_ == true) {
        return false;
    }
    assert(valid_ == false);
    precharge_ = true;
    return true;
}

inline
bool DRAM::Bank::is_busy() const
{
    return busy_;
}


inline
void DRAM::Bank::set_busy()
{
    assert(busy_ == false);
    busy_ = true;
}


inline
void DRAM::Bank::unset_busy()
{
    assert(busy_ == true);
    busy_ = false;
}


inline
unsigned DRAM::get_index(const engine::addr_t &addr, uint64_t init_range) const
{
    uint64_t mask = init_range;
    uint64_t ret = 0, first = 0, filled = 1;
    while ((first = ffsl(mask)) != 0) {
        unsigned last = first - 1 + ffsl(~(mask >> (first -1))) - 1;
        uint64_t range_mask = (last == 64) ? -1L : ((1UL << last) - 1);
        range_mask &= mask;
        ret |= (addr & range_mask) >> (first - filled);
        mask &= ~range_mask;
        filled += last - first + 1;
    }
    return ret;
}


inline
unsigned DRAM::get_rank_index(const engine::addr_t &addr) const
{
    return get_index(addr, rank_range_);
}


inline
unsigned DRAM::get_bank_index(const engine::addr_t &addr) const
{
    return get_index(addr, bank_range_);
}


inline
unsigned DRAM::get_column_index(const engine::addr_t &addr) const
{
    return get_index(addr, column_range_);
}


inline
unsigned DRAM::get_row_index(const engine::addr_t &addr) const
{
    return get_index(addr, row_range_);
}


template<typename T>
inline
T &DRAM::access(std::vector<T *> &vector, unsigned index)
{
    if (vector.size() <= index) {
        vector.resize(index + 1);
    }
    T *ptr = vector[index];
    if (ptr == nullptr) {
        ptr = new T();
        vector[index] = ptr;
    }
    return *ptr;
}


inline
DRAM::Bank &DRAM::get_bank(const engine::addr_t &addr)
{
    Rank &rank = this->access(dimms_, get_rank_index(addr));
    return this->access(rank, get_bank_index(addr));
}


inline
void DRAM::set(const engine::addr_t &addr)
{
    this->get_bank(addr).set_row_buffer(this->get_row_index(addr));
}


inline
bool DRAM::get(const engine::addr_t &addr)
{
    Bank &bank = this->get_bank(addr);
    if (bank.is_row_valid() and bank.get_row_buffer() == this->get_row_index(addr)) {
        return true;
    }
    return false;
}


inline
bool DRAM::precharge(const engine::addr_t &addr)
{
    return this->get_bank(addr).precharge();
}


inline
void DRAM::invalidate(const engine::addr_t &addr)
{
    this->get_bank(addr).invalidate_row_buffer();
}


inline
bool DRAM::is_busy(const engine::addr_t &addr)
{
    return this->get_bank(addr).is_busy();
}


inline
void DRAM::set_busy(const engine::addr_t &addr)
{
    this->get_bank(addr).set_busy();
}


inline
void DRAM::unset_busy(const engine::addr_t &addr)
{
    this->get_bank(addr).unset_busy();
}


}  // namespace memory
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_MEMORY_DRAM_IMPL_H_
