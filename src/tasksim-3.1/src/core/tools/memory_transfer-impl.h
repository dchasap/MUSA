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

#ifndef CORE_TOOLS_MEMORY_TRANSFER_IMPL_H_
#define CORE_TOOLS_MEMORY_TRANSFER_IMPL_H_

#include "core/tools/memory_transfer.h"

#include <iostream>
#include <string>

#include "core/trace/types.h"
#include "core/trace/common/mitem.h"
#include "core/trace/common/minst.h"
#include "core/trace/common/types.h"
#include "core/trace/mem/types.h"
#include "core/trace/rle/types.h"
#include "core/trace/ts/types.h"

using sim::trace::minst_t;

template<typename InputFormat, typename OutputFormat>
void uop_to_mem<InputFormat, OutputFormat>::convert_m(InputFormat &input, OutputFormat &output, unsigned subtrace_id) {
    sim::trace::ts::subtrace_t isubt = input.get_task_trace_by_id(subtrace_id);
    sim::trace::ts::subtrace_t osubt = output.get_task_trace_by_id(subtrace_id);
    sim::trace::bbl_t bbl = input.get_next_bbl(isubt);
    unsigned remaining = 0;
    while (bbl.id_.id_ != 0) {
        sim::trace::mem::mitem_queue_t mem_queue(bbl);
        while (not mem_queue.empty()) {
            sim::trace::mitem_t &item = mem_queue.front();
            if (item.op_ == sim::trace::mitem_t::NOMEM) {
                remaining += item.ins_;
            } else {
                if (remaining) {
                    item.ins_ += remaining;
                    remaining = 0;
                }
                item.addr_ = input.get_next_mem(isubt);
                output.add_mem_item_trace(osubt, item);
            }
            mem_queue.pop();
        }
        bbl = input.get_next_bbl(isubt);
    }
    // To get the zero out:
    input.get_next_mem(isubt);
    // We add the end of stream:
    output.add_mem_item_trace(osubt, sim::trace::mitem_t());
}

template<typename InputFormat, typename OutputFormat>
void uop_to_uop<InputFormat, OutputFormat>::transfer_bbl_dict(InputFormat &input, OutputFormat &output) {
    if (dict_transfered) {
        return;
    }
    // We add the 0 id basic block
    output.add_bbl(sim::trace::bbl_t());
    sim::trace::bbl_id_t id(1);
    sim::trace::bbl_t bbl = input.get_bbl(id);
    while (bbl.id_.id_ != 0) {
        output.add_bbl(bbl);
        id.id_++;
        bbl = input.get_bbl(id);
    }
    // We have got no way to fully read the bbl dictionary!
    dict_transfered = 1;
    return;
}

template<typename InputFormat, typename OutputFormat>
void uop_to_uop<InputFormat, OutputFormat>::convert_m(InputFormat &input, OutputFormat &output, unsigned subtrace_id) {
    transfer_bbl_dict(input, output);
    sim::trace::ts::subtrace_t isubt = input.get_task_trace_by_id(subtrace_id);
    sim::trace::ts::subtrace_t osubt = output.get_task_trace_by_id(subtrace_id);
    sim::trace::bbl_t item = input.get_next_bbl(isubt);
    while (true) {
        // If it is an invalid mitem_t:
        if (item.id_ == 0) {
            // First we add the invalid mitem: (Either end of segment or end of subtrace)
            output.add_bbl_trace(osubt, item.id_);
            break;
        }
        // Normal mitem_t, we add it:
        output.add_bbl_trace(osubt, item.id_);
        item = input.get_next_bbl(isubt);
    }
    sim::trace::mem_t mem = input.get_next_mem(isubt);
    while (true) {
        // If it is an invalid mitem_t:
        if (mem.addr_ == 0) {
            // First we add the invalid mitem: (Either end of segment or end of subtrace)
            output.add_mem_trace(osubt, mem);
            break;
       }
       // Normal mitem_t, we add it:
       output.add_mem_trace(osubt, mem);
       mem = input.get_next_mem(isubt);
    }
}

template<typename InputFormat, typename OutputFormat>
void mem_to_uop<InputFormat, OutputFormat>::convert_m(InputFormat &input, OutputFormat &output, unsigned subtrace_id) {
    if (mem_to_bbl_dict.size() == 0) {
        // Adding null bbl_t as id = 0;
        output.add_bbl(sim::trace::bbl_t());
        mem_to_bbl_dict[sim::trace::mitem_t()] = 0;
    }
    // This is the part that might be different for different internals:
    sim::trace::ts::subtrace_t isubt = input.get_task_trace_by_id(subtrace_id);
    sim::trace::ts::subtrace_t osubt = output.get_task_trace_by_id(subtrace_id);
    sim::trace::mitem_t item = input.get_next_mem_item(isubt);
    while (true) {
        // If it is an invalid mitem_t:
        if (item.op_ == sim::trace::mitem_t::INVALID) {
            // First we add the invalid mitem: (Either end of segment or end of subtrace)
            output.add_bbl_trace(osubt, sim::trace::bbl_id_t(0));
            output.add_mem_trace(osubt, sim::trace::mem_t(0));
            break;
        }

        sim::trace::bbl_id_t new_id;
        if (mem_to_bbl_dict.find(item) != mem_to_bbl_dict.end()) {
            new_id.id_ = mem_to_bbl_dict[item];
        } else {
            sim::trace::bbl_t new_bbl;
            new_bbl.id_.id_ = mem_to_bbl_dict.size();
            for (int i = 1; i < item.ins_; i++) {
                new_bbl.ins_.push_back(minst_t(sim::trace::op_t::OTHER, 0, 0, 0, 0));
            }
            if (item.op_ == sim::trace::mitem_t::LOAD) {
                new_bbl.ins_.push_back(minst_t(sim::trace::op_t::OTHER, 1, 0, item.size_, 0));
            } else if (item.op_ == sim::trace::mitem_t::STORE) {
                new_bbl.ins_.push_back(minst_t(sim::trace::op_t::OTHER, 0, 1, 0, item.size_));
            } else {
                new_bbl.ins_.push_back(minst_t(sim::trace::op_t::OTHER, 0, 0, 0, 0));
            }
            output.add_bbl(new_bbl);
            new_id = new_bbl.id_;
            mem_to_bbl_dict[item] = new_id.id_;
        }
        output.add_bbl_trace(osubt, new_id);
        output.add_mem_trace(osubt, item.addr_);
        item = input.get_next_mem_item(isubt);
    }
}

template<typename InputFormat, typename OutputFormat>
void mem_to_mem<InputFormat, OutputFormat>::convert_m(InputFormat &input, OutputFormat &output, unsigned subtrace_id) {
    sim::trace::ts::subtrace_t isubt = input.get_task_trace_by_id(subtrace_id);
    sim::trace::ts::subtrace_t osubt = output.get_task_trace_by_id(subtrace_id);
    sim::trace::mitem_t item = input.get_next_mem_item(isubt);
    while (true) {
        // If it is an invalid mitem_t:
        if (item.op_ == sim::trace::mitem_t::INVALID) {
            std::cout << "Invalid candidate: " << item << std::endl;
            // First we add the invalid mitem: (Either end of segment or end of subtrace)
            output.add_mem_item_trace(osubt, item);
            // We have finished this segment
            break;
        }
        // Normal mitem_t, we add it:
        output.add_mem_item_trace(osubt, item);
        item = input.get_next_mem_item(isubt);
    }
}


#endif  // CORE_TOOLS_MEMORY_TRANSFER_IMPL_H_
