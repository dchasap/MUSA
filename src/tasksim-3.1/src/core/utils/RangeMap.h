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


#ifndef STATS_RANGEMAP_H_
#define STATS_RANGEMAP_H_

#include <assert.h>
#include <vector>
#include <limits>
#include "RangeSet.h"
#include "nextsim/core/engine/types.h"

namespace sim {
namespace utils {

class RangeMap {
protected:
    std::vector<std::string> range_names_;
    std::map<engine::addr_t, std::pair<engine::addr_t, size_t>> map_;

    size_t find_name(const std::string &) const;
    void merge_contiguous(engine::addr_t start = 0, engine::addr_t end = std::numeric_limits<engine::addr_t>::max());

    int default_value_;

public:
    inline
    RangeMap() : default_value_(-1)
    {}

    void setup_catch_all();
    void load_range_map(const std::string&, int);
    int get_range_index(engine::addr_t);

    // size and simple {g,s}etters
    size_t size() { return range_names_.size(); }

    const std::string & name_at(size_t i) const { return range_names_[i]; }
    std::string &  name_at(size_t i) { return range_names_[i]; }

    void merge(const RangeSet &other, size_t i);
    void merge(const RangeSet &other, std::string s);


    std::map<engine::addr_t, std::pair<engine::addr_t, size_t>>::iterator begin() { return map_.begin(); }
    std::map<engine::addr_t, std::pair<engine::addr_t, size_t>>::iterator end() { return map_.end(); }
    std::map<engine::addr_t, std::pair<engine::addr_t, size_t>>::const_iterator begin() const { return map_.begin(); }
    std::map<engine::addr_t, std::pair<engine::addr_t, size_t>>::const_iterator end() const { return map_.end(); }

    inline bool operator==(const std::map<engine::addr_t, std::pair<engine::addr_t, size_t>>& map) const { return map_ == map; }

    std::string operator[](size_t) const;
};

}
}

#include "RangeMap-impl.h"

#endif // STATS_RANGEMAP_H_
