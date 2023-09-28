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


#ifndef CORE_LOGIC_MEMORY_CACHESET_H_
#define CORE_LOGIC_MEMORY_CACHESET_H_

#include <list>
#include <unordered_map>
#include <utility>
#include <memory>
#include <core/logic/memory/policy/Policy.h>

namespace sim {
namespace logic {
namespace memory {

struct CacheLineBase {
    std::size_t task_type_;
    bool locked() const { return false; }
    sim::runtime::task_dep_t get_dep_type() const
    {
        return sim::runtime::task_dep_t::NON_IN_OUT;
    }
};

template<typename Tag, typename CacheLine, template<typename...> class Array = std::unordered_map>
class CacheSet {
public:
    typedef Tag tag_t;
    typedef CacheLine line_t;

    typedef std::list<std::pair<Tag, CacheLine>> Container;
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;
    typedef typename Container::reverse_iterator reverse_iterator;
    typedef sim::logic::memory::policy::Policy<CacheSet> Policy;

protected:
    std::size_t size_;
    Container contents_;
    Container free_entries_;
    Array<Tag, iterator> array_;
    typename Policy::policy_ptr_t policy_;

public:
    explicit CacheSet(std::size_t);
    explicit CacheSet(CacheSet<Tag, CacheLine> &&);
    CacheSet(const CacheSet&) = delete;
    ~CacheSet();

    std::size_t get_size() const;
    std::size_t get_free_size() const;
    bool is_full() const;

    bool insert(const Tag &, const CacheLine &);

    std::pair<bool, iterator> insert_and_get_iterator(const Tag &, const CacheLine &);
    std::pair<bool, iterator> insert_and_get_iterator(iterator, const Tag &, const CacheLine &);
    std::pair<Tag, CacheLine> evict();
    bool remove(const Tag &, bool call_policy_remove = true);
    void update(const Tag &);
    iterator find(const Tag &);
    const_iterator find(const Tag &) const;

    iterator end();
    const_iterator end() const;

//    void set_policy(const Policy &);
    Policy *get_policy() const;
    void set_policy(typename Policy::policy_ptr_t policy);

    Container &get_contents();
    const Container &get_contents() const;
    void print_status() const;
};

}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "CacheSet-impl.h"


#endif  // CORE_LOGIC_MEMORY_CACHESET_H_
