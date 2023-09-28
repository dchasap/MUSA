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


#ifndef CORE_TRACE_MEM_TYPES_H_
#define CORE_TRACE_MEM_TYPES_H_

#include <deque>

#include "core/trace/common/bbl.h"
#include "core/trace/common/mem.h"
#include "core/trace/common/mitem.h"

namespace sim {
namespace trace {
namespace mem {

/**
 * mitem_queue_t holds a queue of memory trace items. It provides
 * a set of functions to fill it with memory items from a bbl_t object.
 */
class mitem_queue_t {
private:
    std::deque<mitem_t> items_;
    void push_bbl(const bbl_t&);
    unsigned trailing_ops_;
public:
    mitem_queue_t();
    explicit mitem_queue_t(const bbl_t&);
    mitem_queue_t &operator=(const bbl_t&);
    void push(const bbl_t&);
    void push(const mitem_queue_t &queue);
    void push(const mitem_t &item);
    void push(const minst_t &item, unsigned mem_size = 0);
    mitem_t &front();
    const mitem_t &front() const;
    void pop();

    bool empty() const;
    std::size_t size() const { return items_.size(); }
};

}  // namespace mem
}  // namespace trace
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_TRACE_MEM_TYPES_H_
