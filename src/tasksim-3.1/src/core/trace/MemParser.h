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


#ifndef CORE_TRACE_MEMPARSER_H_
#define CORE_TRACE_MEMPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <queue>
#include <string>
#include <utility>

#include "core/engine/Config.h"
#include "core/trace/types.h"
#include "core/trace/common/mitem.h"
#include "core/trace/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/rle/Trace.h"
#include "core/trace/uop/Trace.h"

namespace sim {
namespace trace {

template<typename trace>
class MemParser {
private:
    trace &trace_;
    /** Contains the size of the vector registers. */
    unsigned vector_length_;
    unsigned trace_vector_length_;
    unsigned register_file_length_;
    uint64_t zero_;
    // We mantain a bbl_map and pending_items and pending_bbl_id for every task instance
    std::unordered_map<unsigned, sim::trace::mem::mitem_queue_t> pending_items_;
    std::unordered_map<unsigned, unsigned> pending_bbl_id_;
    std::unordered_map<unsigned, std::queue<uint64_t>> pending_addrs_;
    std::unordered_map<unsigned, unsigned> pending_removed_;
    /** Contains the dictionary of basic blocks parsed for vectorization. */
    std::unordered_map<unsigned, sim::trace::bbl_t> parsed_bbl_;

    std::unordered_map<unsigned, unsigned> max_accs_;
    std::unordered_map<unsigned, unsigned> num_accs_;

    sim::trace::mitem_t get_next_mem_uop(const sim::trace::ts::subtrace_t &subtrace);
    void init_info(const sim::trace::bbl_t &origin);
    sim::trace::bbl_t parse_bbl(const sim::trace::bbl_t &origin);
    void add_bbl(unsigned id, unsigned eliminated_bbls, sim::trace::mem::mitem_queue_t& instr_queue);
    sim::trace::mitem_t give_next_mitem(const sim::trace::ts::subtrace_t &subtrace);
    unsigned get_next_bbl_id(const sim::trace::ts::subtrace_t &subtrace);
    void fill_not_vectorizing(const sim::trace::ts::subtrace_t &subtrace);
    void fill_vectorizing(const sim::trace::ts::subtrace_t &subtrace);
public:
    explicit MemParser(trace &t, unsigned vector_length = 8,
            unsigned trace_vector_length = 8, unsigned register_file_length = 8);
    MemParser(trace &t, const sim::engine::Config &config);
    ~MemParser();
    sim::trace::mitem_t get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace);
};

template<>
inline
sim::trace::mitem_t
MemParser<trace::ts::FileTraceReader<trace::SelectedEventStream, trace::mem::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace);

template<>
inline
sim::trace::mitem_t
MemParser<trace::ts::FileTraceReader<trace::SelectedEventStream, trace::rle::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace);

template<>
inline
sim::trace::mitem_t
MemParser<trace::ts::FileTraceReader<trace::SelectedEventStream, trace::uop::TraceInternal>>::
get_next_mem_item(const sim::trace::ts::subtrace_t &subtrace);

}  // namespace trace
}  // namespace sim

#include "MemParser-impl.h"

#endif  // CORE_TRACE_MEMPARSER_H_
