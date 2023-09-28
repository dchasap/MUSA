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


#ifndef CORE_LOGIC_MEMORY_POLICY_TYPES_H_
#define CORE_LOGIC_MEMORY_POLICY_TYPES_H_

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <utility>
#include <numeric>

namespace sim {
namespace logic {
namespace memory {
namespace policy {


class Parameters {
public:
    std::size_t size_;
    Parameters(std::size_t size);
    virtual ~Parameters();
};


typedef enum class type_t {
    INVALID,
    LRU,
    NRU,
    TREE_PLRU,
    LAST
} type_t;


static const std::unordered_map<std::string, type_t> string_to_policy_type({
    {"INVALID", type_t::INVALID},
    {"LRU",     type_t::LRU},
    {"NRU",     type_t::NRU},
    {"TREE_PLRU",    type_t::TREE_PLRU},
});


// VD: TODO: Make initialization of this vector automatic, so new values do not have to be added manually
static const std::vector<std::string> policy_type_to_string({
    "INVALID",
    "LRU",
    "NRU",
    "TREE_PLRU",
});


std::string type_t_to_string(type_t type);
type_t string_to_type_t(const std::string &name);


template<typename Tag, typename CacheSet>
class Storage {
public:
    typedef typename std::pair<Tag, typename CacheSet::iterator> Element;
    typedef typename std::vector<Element> Container;
    typedef typename Container::iterator iterator;

    Container container_;
    std::vector<bool> valid_;
    std::vector<std::size_t> free_indices_;
    std::unordered_map<Tag, iterator> map_;

    Storage(const std::size_t size);

    std::size_t insert(const Tag &tag, const typename CacheSet::iterator &iter);
    std::size_t find(const Tag &tag) const;
    Tag get_tag(const std::size_t index) const;
    const typename CacheSet::line_t& get_cache_line(const std::size_t index) const;
    void remove(std::size_t index);

    /* This should be called only after creating the storage from another storage object.
     * In that case, iterators in the map of previous object get invalidated,
     * so map needs to be re-populated with iterators from the new storage object.
     * Calling it on a valid Storage object works fine, but does not make sense. */
    void refresh_map();
};

}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "types-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_TYPES_H_
