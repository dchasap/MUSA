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

#ifndef CORE_TOOLS_MEMORY_TRANSFER_H_
#define CORE_TOOLS_MEMORY_TRANSFER_H_

#include <iostream>
#include <string>

#include "core/trace/types.h"
#include "core/trace/common/mitem.h"
#include "core/trace/common/types.h"
#include "core/trace/mem/types.h"
#include "core/trace/rle/types.h"
#include "core/trace/ts/types.h"

using sim::trace::minst_t;

struct mitem_keyhash {
public:
    std::size_t operator()(const sim::trace::mitem_t& item) const {
        return static_cast<int>(item.op_) * (1<<31) + static_cast<int>(item.size_) * (1<<15) +
                static_cast<int>(item.ins_);
    }
};

struct mitem_keyequal {
public:
    bool operator()(const sim::trace::mitem_t& lhs, const sim::trace::mitem_t& rhs) const {
        return (lhs.op_ == rhs.op_) and (lhs.size_ == rhs.size_) and (lhs.ins_ == rhs.ins_);
    }
};

template<typename InputFormat, typename OutputFormat>
class uop_to_mem {
public:
    static void convert_m(InputFormat &input, OutputFormat &output, unsigned subtrace_id);
};

template<typename InputFormat, typename OutputFormat>
class uop_to_uop {
public:
    static int dict_transfered;
    static void transfer_bbl_dict(InputFormat &input, OutputFormat &output);
    static void convert_m(InputFormat &input, OutputFormat &output, unsigned subtrace_id);
};

template<typename InputFormat, typename OutputFormat>
int uop_to_uop<InputFormat, OutputFormat>::dict_transfered = 0;

template<typename InputFormat, typename OutputFormat>
class mem_to_uop {
public:
    static std::unordered_map<sim::trace::mitem_t, int, mitem_keyhash, mitem_keyequal> mem_to_bbl_dict;

    static void convert_m(InputFormat &input, OutputFormat &output, unsigned subtrace_id);
};

template<typename InputFormat, typename OutputFormat>
std::unordered_map<sim::trace::mitem_t, int, mitem_keyhash, mitem_keyequal>
        mem_to_uop<InputFormat, OutputFormat>::mem_to_bbl_dict;


template<typename InputFormat, typename OutputFormat>
class mem_to_mem {
public:
    static void convert_m(InputFormat &input, OutputFormat &output, unsigned subtrace_id);
};

#include "memory_transfer-impl.h"

#endif  // CORE_TOOLS_MEMORY_TRANSFER_H_
