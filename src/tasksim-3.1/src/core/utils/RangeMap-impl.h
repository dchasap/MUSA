/*
 * Copyright - Barcelona Supercomputing Center.
 * All rights reserved.
 *
 * Redistribution in source and binary forms, with or without
 * modification, is not permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef STATS_RANGEMAP_IMPL_H_
#define STATS_RANGEMAP_IMPL_H_

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

namespace sim {
namespace utils {

inline
void RangeMap::setup_catch_all()
{
    if (default_value_ >= 0) {
        return;
    }
    RangeSet all_addresses({0, std::numeric_limits<engine::addr_t>::max()});
    default_value_ = range_names_.size();
    range_names_.push_back(map_.size() ? "noid" : "all");
}

inline
void RangeMap::merge_contiguous(engine::addr_t start, engine::addr_t end)
{
    auto pos = map_.upper_bound(start ? start - 1 : start);
    if (map_.begin() == pos) ++pos;

    for (auto prev = std::prev(pos); pos != map_.end() && pos->first <= end; prev = std::prev(pos)) {
        if (pos->second.second == prev->second.second && prev->second.first == pos->first) {
            prev->second = pos->second;
            pos = map_.erase(pos);
        } else {
            ++pos;
        }
    }
}

inline
void RangeMap::merge(const RangeSet &other, const size_t there_id)
{
    if (!other.map_.size()) {
        return;
    }

    auto there = other.map_.begin();
    auto here = map_.upper_bound(there->first);
    if (here != map_.begin()) --here;

    engine::addr_t there_from, there_until, here_from, here_until;
    size_t here_id;

    // for each range *there, we advance *here until it overlaps or is after there
    std::tie(there_from, there_until) = *there;
    while (here != map_.end() && there != other.map_.end()) {
        here_from = here->first;
        std::tie(here_until, here_id) = here->second;

        if (here_until < there_from) {
            // range *here ends before range *there starts, so just go to the next in here
            here++;
        }
        else if (there_until <= here_from) {
            // range *there ends before range *here starts, so we can safely copy it whole
            map_.insert(here, std::make_pair(there_from, std::make_pair(there_until, there_id)));
            if (++there != other.map_.end()) std::tie(there_from, there_until) = *there;
        }
        else if (here_from < there_from && here_until > there_until) {
            // Full inclusion of range *there, ignore it
            if (++there != other.map_.end()) std::tie(there_from, there_until) = *there;
        }
        else { // overlap
            // If *there starts before *here, insert
            if (there_from < here_from) {
                map_.insert(here, std::make_pair(there_from, std::make_pair(here_from, there_id)));
            }

            // Fully covered insertion of *there, move on to the next
            if (here_until >= there_until) {
                if (++there != other.map_.end()) std::tie(there_from, there_until) = *there;
            } else {
                there_from = here_until;
                ++here;
            }
        }
    }

    if (here == map_.end()) {
        while (there != other.map_.end()) {
            map_.insert(map_.end(), std::make_pair(there_from, std::make_pair(there_until, there_id)));
            if (++there != other.map_.end()) std::tie(there_from, there_until) = *there;
        }
    }

    // For simplicity, perform merging of contiguous same-id items separately
    merge_contiguous(other.map_.begin()->first, other.map_.rbegin()->second);
}

inline
void RangeMap::load_range_map(const std::string& filename, int num_bits_ignored)
{
    const engine::addr_t mask = ~((1ULL << num_bits_ignored) - 1);
    // reads a file with address mappings written in hex as follows:
    // start:end:name
    std::ifstream mapfile(filename);
    while(!mapfile.eof()) {
        char sep;
        engine::addr_t low, high;
        std::string name;
        mapfile >> std::hex >> low >> sep; assert(sep == ':');
        mapfile >> std::hex >> high >> sep; assert(sep == ':');
        mapfile >> name;
        name = name.substr(0, name.find(":"));

        low &= mask;
        if ((high & mask) < high) {
            high = (high & mask) + (1ULL << num_bits_ignored);
        }

        RangeSet new_range({low, high});

        auto pos = std::find(range_names_.begin(), range_names_.end(), name);
        if (pos == range_names_.end()) {
            range_names_.push_back(name);
            merge(new_range, range_names_.size() - 1);
        }
        else {
            merge(new_range, std::distance(range_names_.begin(), pos));
        }
    }
}

inline
size_t RangeMap::find_name(const std::string & name) const
{
    auto pos = std::find(range_names_.begin(), range_names_.end(), name);
    assert(pos != range_names_.end());
    return std::distance(range_names_.begin(), pos);
}

inline
void RangeMap::merge(const RangeSet &other, std::string s)
{
    merge(other, find_name(s));
}

inline
std::string RangeMap::operator[](size_t i) const
{
    return range_names_[i];
}

inline
int RangeMap::get_range_index(engine::addr_t address)
{
    // above is lowest range starting above address
    const auto above = map_.upper_bound(address);
    if (above == map_.begin()) {
        return default_value_;
    }

    engine::addr_t range_end;
    size_t range_id;
    std::tie(range_end, range_id) = std::prev(above)->second;

    if (range_end > address) {
        return range_id;
    } else {
        return default_value_;
    }
}


}
}

#endif // STATS_RANGEMAP_IMPL_H_


