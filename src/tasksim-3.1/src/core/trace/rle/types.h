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


#ifndef CORE_TRACE_RLE_TYPES_H_
#define CORE_TRACE_RLE_TYPES_H_

#include <vector>
#include <deque>
#include <unordered_map>
#include "core/trace/common/bbl.h"
#include "core/trace/common/mem.h"
#include "core/trace/common/mitem.h"
#include "core/engine/types.h"

namespace sim {
namespace trace {
namespace rle {

/** Alias of an address in tracing for the one used in the engine */
typedef engine::addr_t addr_t;
static const addr_t diff_limit = 0xFFFF;

#pragma pack(1)
struct channel_mask_t {
    unsigned char reset_:1;
    unsigned char sign_:1;
    unsigned char channel_:6;
    channel_mask_t();
    channel_mask_t(unsigned char reset, unsigned char sign, unsigned char channel);
    void write(std::ostream &) const;
    void read(std::istream &);
};
#pragma pack()

template< typename value, typename offset>
class channeled_value {
public:
    channel_mask_t mask_;
    value value_;
    channeled_value();
    channeled_value(const channel_mask_t &mask, const value &val);
    void write(std::ostream &) const;
    void read(std::istream &);
};
/** function to write a channeled_value to an output stream */
template< typename value, typename offset>
inline
std::ostream &operator<<(std::ostream &os, const channeled_value<value, offset> &val);
/** function to read a channeled value from an input stream */
template< typename value, typename offset>
inline
std::istream &operator>>(std::istream &is, channeled_value<value, offset> &val);


template< typename value, typename offset>
class channeled_history {
    std::vector<value> history_;
    size_t limit_;
    static const size_t channel_limit_ = 64;
    unsigned last_stream_;
public:
    channeled_history();
    value& get_value(const channeled_value<value, offset> &channeled);
    channeled_value<value, offset> get_channeled(const value &val, unsigned stream);
};

typedef channeled_value<unsigned, uint8_t> channeled_bbl_id_t;
typedef channeled_value<addr_t, uint16_t> channeled_mem_t;

typedef channeled_history<unsigned, uint8_t> bbl_history;
typedef channeled_history<addr_t, uint16_t> mem_history;

typedef std::unordered_map<unsigned, bbl_history> bbl_history_map;
typedef std::unordered_map<unsigned, mem_history> mem_history_map;

template< typename value, typename counter>
class repeated_value {
public:
    value value_;
    counter repeats_;
    counter repeat_limit = static_cast<counter>(-1);
    explicit repeated_value(value val = 0, counter repeats = 1);
    void set_value(value val = 0, counter repeats = 1);
    void repeat();
    bool limit();
    void consume();
    void write(std::ostream &) const;
    void read(std::istream &);
};

/** function to write a repeated_value to an output stream */
template< typename value, typename offset>
inline
std::ostream &operator<<(std::ostream &os, const channeled_value<value, offset> &val);
/** function to read a repeated value from an input stream */
template< typename value, typename offset>
inline
std::istream &operator>>(std::istream &is, channeled_value<value, offset> &val);

typedef repeated_value<unsigned, uint16_t> rle_bbl_id_t;
typedef std::unordered_map<unsigned, rle_bbl_id_t> bbl_id_by_stream_t;


}  // namespace rle
}  // namespace trace
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_TRACE_RLE_TYPES_H_
