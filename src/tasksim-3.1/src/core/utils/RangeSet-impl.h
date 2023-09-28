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


#ifndef STATS_RANGESET_IMPL_H_
#define STATS_RANGESET_IMPL_H_

namespace sim {
namespace utils {

inline
std::pair<std::map<engine::addr_t, engine::addr_t>::iterator, bool> RangeSet::insert(addr_range_t range)
{
    // get smallest element after the low bound of range, so std::prev(after) is the highest element before
    std::map<engine::addr_t, engine::addr_t>::iterator after = map_.upper_bound(range.first), previous, insert_range;

    bool has_previous = after != map_.begin();
    if (has_previous) {
        previous = std::prev(after);
    }

    if (not has_previous or previous->second < range.first) {
        // no overlap with previous element (or no previous element), need to insert a new one
        insert_range = map_.insert(after, range);
    }
    else if (previous->second < range.second) {
        // partial overlap: extend range until the end of the inserted range
        previous->second = range.second;
        insert_range = previous;
    }
    else {
        // full overlap: do nothing
        return std::make_pair(insert_range, false);
    }

    // check for overlap with next element(s), could be several if inserting big element
    while (after != map_.end() and insert_range->second >= after->first) {
        insert_range->second = std::max(insert_range->second, after->second);
        after = map_.erase(after);
    }

    // semantics are true iff we added at least one value to the set
    return std::make_pair(insert_range, true);
}

inline
void RangeSet::merge(RangeSet &other)
{
    std::map<engine::addr_t, engine::addr_t>::iterator there = other.map_.begin(), here = map_.upper_bound(there->first);

    if (here != map_.begin()) {
        here = std::prev(here);
    }

    while (there != other.map_.end()) {
        if (here == map_.end()) {
            map_.insert(here, *there);
            there++;
        }
        // here != end(), there != other.end()
        else if (there->first > 0 && here->second < there->first) {
            // range in here ends before range in there starts, so just go to the next in here
            here++;
        }
        else if (here->first > 0 && there->second < here->first) {
            // range in there ends before range in here starts, so we can safely copy it over
            map_.insert(here, *there);
            there++;
        }
        else {
            // overlap: coalesce *here and *there in *here
            std::map<engine::addr_t, engine::addr_t>::iterator after_here;
            engine::addr_t range_end = std::max(there->second, here->second);

            if (there->first < here->first) {
                auto insert_pos = map_.insert(here, std::make_pair(there->first, range_end));
                after_here = map_.erase(here);
                assert(insert_pos == std::prev(after_here));
                here = insert_pos;
            }
            else {
                here->second = range_end;
                after_here = std::next(here);
            }
            there++;

            // now *here is bigger, so check for overlap with next elements
            while (after_here != map_.end() and here->second >= after_here->first) {
                here->second = std::max(here->second, after_here->second);
                after_here = map_.erase(after_here);
            }
        }
    }
}

inline
int64_t RangeSet::position(engine::addr_t addr) const
{
    std::map<engine::addr_t, engine::addr_t>::const_iterator after = map_.upper_bound(addr);

    if (after == map_.begin() or std::prev(after)->second < addr) {
        return -1;
    }

    std::map<engine::addr_t, engine::addr_t>::const_iterator it = std::prev(after);
    int64_t pos = addr - it->first;
    while (it != map_.begin()) {
        --it;
        pos += it->second + 1 - it->first;
    }

    return pos;
}

inline
bool RangeSet::contains(engine::addr_t addr) const
{
    std::map<engine::addr_t, engine::addr_t>::const_iterator after = map_.upper_bound(addr);

    return (after != map_.begin() and std::prev(after)->second > addr);
}

inline
size_t RangeSet::measure() const
{
    size_t addresses = 0;
    for (const auto &it: map_) {
        addresses += (it.second - it.first);
    }
    return addresses;
}

}
}

#endif // STATS_RANGESET_IMPL_H_

