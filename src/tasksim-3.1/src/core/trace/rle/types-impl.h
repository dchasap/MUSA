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


#ifndef CORE_TRACE_RLE_TYPES_IMPL_H_
#define CORE_TRACE_RLE_TYPES_IMPL_H_

#include <cassert>
#include <iostream>
#include <vector>

namespace sim {
namespace trace {
namespace rle {


inline
channel_mask_t::channel_mask_t() :
    reset_(0), sign_(0), channel_(0)
{}

inline
channel_mask_t::channel_mask_t(unsigned char reset, unsigned char sign, unsigned char channel):
    reset_(reset), sign_(sign), channel_(channel)
{}

inline
void channel_mask_t::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char *>(this), sizeof(*this));
}

inline
void channel_mask_t::read(std::istream &is)
{
    is.read(reinterpret_cast<char *>(this), sizeof(*this));
}

inline
std::ostream &operator<<(std::ostream &os, const channel_mask_t &mask)
{
    os << static_cast<unsigned>(mask.reset_) << ":"
       << static_cast<unsigned>(mask.sign_) << ":"
       << static_cast<unsigned>(mask.channel_);
    return os;
}

inline
std::istream &operator>>(std::istream &is, channel_mask_t &mask)
{
    char sep;
    unsigned reset, sign, channel;
    is >> reset; is >> sep;
    mask.reset_ = reset;
    is >> sign; is >> sep;
    mask.sign_ = sign;
    is >> channel;
    mask.channel_ = channel;
    return is;
}

template< typename value, typename offset>
inline
channeled_value<value, offset>::channeled_value() :
        mask_(),
        value_()
{
}

template< typename value, typename offset>
inline
channeled_value<value, offset>::channeled_value(const channel_mask_t &mask, const value &val) :
        mask_(mask),
        value_(val)
{
}

template< typename value, typename offset>
inline
void channeled_value<value, offset>::write(std::ostream &os) const
{
    mask_.write(os);
    if (mask_.reset_) {
        os.write(reinterpret_cast<const char *>(&value_), sizeof(value_));
    } else {
        offset diff = value_;
        os.write(reinterpret_cast<const char *>(&diff), sizeof(diff));
    }
}

template< typename value, typename offset>
inline
void channeled_value<value, offset>::read(std::istream &is)
{
    mask_.read(is);
    if (mask_.reset_) {
        value val;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        value_ = val;
    } else {
        offset diff;
        is.read(reinterpret_cast<char*>(&diff), sizeof(diff));
        value_ = diff;
    }
}

template< typename value, typename offset>
inline
std::ostream &operator<<(std::ostream &os, const channeled_value<value, offset> &val)
{
    os << val.mask_ << ":" << val.value_;
    return os;
}

template< typename value, typename offset>
inline
std::istream &operator>>(std::istream &is, channeled_value<value, offset> &val)
{
    char sep;
    std::ios::fmtflags fmt = is.flags();
    is >> val.mask_; is >> sep;
    is >> val.value_;
    is.flags(fmt);
    return is;
}

template< typename value, typename offset>
inline channeled_history<value, offset>::channeled_history() :
        history_(0),
        limit_(static_cast<offset>(-1)),
        last_stream_(0)
{
}

template< typename value, typename offset>
inline
value& channeled_history<value, offset>::get_value(const channeled_value<value, offset> &channeled)
{
    if (channeled.mask_.reset_ == 1) {
        if (channeled.value_ == 0) {
            // We can resize to 1 instead of to 0 because it will not change
            // The following results, if it would, the channeled_value was
            // badly enconded. Also, we can save on needing a 0-valued
            // member on channeled_history.
            history_.resize(1);
        } else if (history_.size() <= channeled.mask_.channel_) {
            history_.resize(channeled.mask_.channel_+1);
        }
        history_[channeled.mask_.channel_] = channeled.value_;
    } else {
        assert(history_.size() > static_cast<int>(channeled.mask_.channel_));
        if (channeled.mask_.sign_) {
            history_[channeled.mask_.channel_] = history_[channeled.mask_.channel_] - channeled.value_;
        } else {
            history_[channeled.mask_.channel_] = history_[channeled.mask_.channel_] + channeled.value_;
            return history_[channeled.mask_.channel_];
        }
    }
    return history_[channeled.mask_.channel_];
}

template< typename value, typename offset>
inline
channeled_value<value, offset> channeled_history<value, offset>::get_channeled(const value &val, unsigned stream)
{
    if (stream != last_stream_) {
        last_stream_ = stream;
        history_.resize(0);
    }
    if (val == 0) {
        history_.resize(0);
        return channeled_value<value, offset>(channel_mask_t(1, 0, 0), 0);
    }
    size_t i;
    for (i = 0; i < history_.size(); i++) {
        if (static_cast<size_t>(val - history_[i]) <= limit_) {
            channeled_value<value, offset> ret(channel_mask_t(0, 0, i), val - history_[i]);
            history_[i] = val;
            return ret;
        } else if (static_cast<size_t>(history_[i] - val) <= limit_) {
            channeled_value<value, offset> ret(channel_mask_t(0, 1, i), history_[i] - val);
            history_[i] = val;
            return ret;
        }
    }
    if (i >= channel_limit_) {
        // Clear all channels.
        i = 0;
    }
    history_.resize(i+1);
    history_[i] = val;
    return channeled_value<value, offset>(channel_mask_t(1, 0, i), val);
}


template< typename value, typename counter>
repeated_value<value, counter>::repeated_value(value val, counter repeats) :
        value_(val), repeats_(repeats)
{}

template< typename value, typename counter>
void repeated_value<value, counter>::set_value(value val, counter repeats)
{
    value_ = val;
    repeats_ = repeats;
}

template< typename value, typename counter>
void repeated_value<value, counter>::repeat()
{
    repeats_++;
}

template< typename value, typename counter>
bool repeated_value<value, counter>::limit()
{
    if (repeats_ == repeat_limit) {
        return true;
    } else {
        return false;
    }
}

template< typename value, typename counter>
void repeated_value<value, counter>::consume()
{
    repeats_--;
}


template< typename value, typename counter>
inline
void repeated_value<value, counter>::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char *>(&value_), sizeof(value_));
    os.write(reinterpret_cast<const char *>(&repeats_), sizeof(repeats_));
}

template< typename value, typename counter>
inline
void repeated_value<value, counter>::read(std::istream &is)
{
    value val;
    is.read(reinterpret_cast<char*>(&val), sizeof(val));
    value_ = val;
    counter rep;
    is.read(reinterpret_cast<char*>(&rep), sizeof(rep));
    repeats_ = rep;
}

template< typename value, typename counter>
inline
std::ostream &operator<<(std::ostream &os, const repeated_value<value, counter> &val)
{
    os << val.value_ << ":" << val.repeats_;
    return os;
}

template< typename value, typename counter>
inline
std::istream &operator>>(std::istream &is, repeated_value<value, counter> &val)
{
    char sep;
    // std::ios::fmtflags fmt = is.flags();
    is >> val.value_; is >> sep;
    is >> val.repeats_;
    // is.flags(fmt);
    return is;
}

}  // namespace rle
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_RLE_TYPES_IMPL_H_
