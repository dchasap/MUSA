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


#ifndef CORE_TRACE_MEM_TYPES_IMPL_H_
#define CORE_TRACE_MEM_TYPES_IMPL_H_

#include <iostream>

namespace sim {
namespace trace {
namespace mem {

// this function does not necessarily have to be inline: there is not a
//  performance advantage, and it may increase code size.
// it is specified inline to avoid creating an additional library for the
//  'trace' directory just due to this function. Whenever additional .cpps
//  are created, this function may be moved to a .cpp as well
inline
void
mitem_queue_t::push_bbl(const bbl_t& bbl)
{
    for (unsigned int i = 0; i < bbl.ins_.size(); ++i) {
        unsigned nloads = bbl.ins_[i].ld_;
        unsigned nstores = bbl.ins_[i].st_;

        assert(bbl.ins_[i].isLoad() == (nloads > 0));
        assert(bbl.ins_[i].isStore() == (nstores > 0));

        if (bbl.ins_[i].isLoad()) {
            for (; nloads > 0; --nloads) {
                mitem_t mitem;
                mitem.op_ = mitem_t::LOAD;
                mitem.size_ = bbl.ins_[i].ld_size_;
                // assert(static_cast<int>(mitem.size_) != 0);
                if (mitem.size_ == 0) {
                    mitem.size_ = 256;
                }
                mitem.ins_ = trailing_ops_ + 1;  // +1: the instruction itself
                trailing_ops_ = 0;
                items_.push_back(mitem);
            }
        } else if (bbl.ins_[i].isStore()) {
            for (; nstores > 0; --nstores) {
                mitem_t mitem;
                mitem.op_ = mitem_t::STORE;
                mitem.size_ = bbl.ins_[i].st_size_;
                // assert(static_cast<int>(mitem.size_) != 0);
                if (mitem.size_ == 0) {
                    mitem.size_ = 256;
                }
                mitem.ins_ = trailing_ops_ + 1;  // +1: the instruction itself
                trailing_ops_ = 0;
                items_.push_back(mitem);
            }
        } else {
            ++trailing_ops_;
        }
    }
}


inline
mitem_queue_t::mitem_queue_t() :
    items_(), trailing_ops_(0)
{}


inline
mitem_queue_t::mitem_queue_t(const bbl_t& bbl) :
  items_(), trailing_ops_(0)
{
    push_bbl(bbl);
}


inline
void mitem_queue_t::push(const mitem_queue_t &queue)
{
    for (auto it = queue.items_.begin(); it != queue.items_.end(); it++) {
        items_.push_back(*it);
    }
    if (not items_.empty()) {
        items_.front().ins_ += trailing_ops_;
        trailing_ops_ = queue.trailing_ops_;
    } else {
        trailing_ops_ += queue.trailing_ops_;
    }
}

inline
mitem_queue_t& mitem_queue_t::operator=(const bbl_t& bbl)
{
    items_.clear();
    push_bbl(bbl);
    return *this;
}


inline
void mitem_queue_t::push(const bbl_t& bbl)
{
    push_bbl(bbl);
}


inline
void mitem_queue_t::push(const mitem_t &item)
{
    items_.push_back(item);
}


inline
void mitem_queue_t::push(const minst_t &item, unsigned mem_size)
{
    unsigned nloads = item.ld_;
    unsigned nstores = item.st_;

    assert(item.isLoad() == (nloads > 0));
    assert(item.isStore() == (nstores > 0));

    if (item.isLoad()) {
        for (; nloads > 0; --nloads) {
            mitem_t mitem;
            mitem.op_ = mitem_t::LOAD;
            if (mem_size != 0) {
                mitem.size_ = mem_size;
            } else {
                mitem.size_ = item.ld_size_;
                if (static_cast<int>(mitem.size_) == 0) {
                    mitem.size_ = 512;
                }
            }
            assert(static_cast<int>(mitem.size_) != 0);
            mitem.ins_ = trailing_ops_ + 1;  // +1: the instruction itself
            trailing_ops_ = 0;
            items_.push_back(mitem);
        }
    } else if (item.isStore()) {
        for (; nstores > 0; --nstores) {
            mitem_t mitem;
            mitem.op_ = mitem_t::STORE;
            if (mem_size != 0) {
                mitem.size_ = mem_size;
            } else {
                mitem.size_ = item.st_size_;
                if (static_cast<int>(mitem.size_) == 0) {
                    mitem.size_ = 512;
                }
            }
            assert(static_cast<int>(mitem.size_) != 0);
            mitem.ins_ = trailing_ops_ + 1;  // +1: the instruction itself
            trailing_ops_ = 0;
            items_.push_back(mitem);
        }
    } else {
        ++trailing_ops_;
    }
}


inline
mitem_t &mitem_queue_t::front()
{
    assert( not items_.empty());
    return items_.front();
}


inline
const mitem_t &mitem_queue_t::front() const
{
    assert( not items_.empty());
    return items_.front();
}


inline
void mitem_queue_t::pop()
{
    assert( not items_.empty());
    items_.pop_front();
}


inline
bool mitem_queue_t::empty() const
{
    return items_.empty();
}


}  // namespace mem
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_MEM_TYPES_IMPL_H_
