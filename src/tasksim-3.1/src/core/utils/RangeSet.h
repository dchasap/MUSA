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


#ifndef STATS_RANGESET_H_
#define STATS_RANGESET_H_

#include <map>
#include <assert.h>
#include "nextsim/core/engine/types.h"

namespace sim {
namespace utils {

typedef std::pair<engine::addr_t, engine::addr_t> addr_range_t;
class RangeMap;

class RangeSet {
protected:
    std::map<engine::addr_t, engine::addr_t> map_;

public:
    RangeSet() : map_() {}

    inline
    RangeSet(addr_range_t initial_range) : RangeSet()
    {
        insert(initial_range);
    }

    std::pair<std::map<engine::addr_t, engine::addr_t>::iterator, bool> insert(addr_range_t range);

    void merge(RangeSet &other);
    bool contains(engine::addr_t) const;

    int64_t position(engine::addr_t) const;
    size_t measure() const;
    size_t cost() const { return map_.size(); }

    std::map<engine::addr_t, engine::addr_t>::iterator begin() { return map_.begin(); }
    std::map<engine::addr_t, engine::addr_t>::iterator end() { return map_.end(); }
    std::map<engine::addr_t, engine::addr_t>::const_iterator begin() const { return map_.begin(); }
    std::map<engine::addr_t, engine::addr_t>::const_iterator end() const { return map_.end(); }

    inline bool operator==(const std::map<engine::addr_t, engine::addr_t>& map) const { return map_ == map; }

    friend RangeMap;
};

}
}

#include "RangeSet-impl.h"

#endif // STATS_RANGESET_H_

