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

#ifndef CORE_LOGIC_COHERENCE_SINGLE_DIRECTORY_H_
#define CORE_LOGIC_COHERENCE_SINGLE_DIRECTORY_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "core/engine/types.h"
#include "core/engine/IModule.h"
#include "core/modules/interconnect/Direct.h"

// Cache module forward declaration:
namespace sim {
namespace memory {
template<typename Message, template<typename> class Controller, template<typename> class Upper, template<typename> class Lower> class Cache;
}
}

namespace sim {
namespace logic {
namespace coherence {
namespace single {

// Controller forward declaration:
template<typename Message> class Controller;

typedef uint32_t cache_id_t;
typedef sim::engine::addr_t addr_t;

class cache_info_t {
    public:
    uint32_t cpu_id;
    uint32_t level;
    bool shared;
    cache_info_t() : cpu_id(0), level(1), shared(false) {}
    cache_info_t(uint32_t id, uint32_t l, bool s) : cpu_id(id), level(l), shared(s) {}
};

template<typename Message/*, template<typename> Controller, template<typename> Direct*/>
class Directory
{
    typedef sim::memory::Cache<Message, sim::logic::coherence::single::Controller,
            sim::interconnect::Direct,  sim::interconnect::Direct> cache_t;
public:
    Directory(uint32_t num_cores, unsigned shared_level);
    void register_cache(cache_t &cache);
private:
    unsigned num_cores_;
    unsigned shared_level_;
    std::unordered_map<cache_id_t, cache_t*> cache_dictionary_;
    std::unordered_map<cache_id_t, cache_info_t> cache_info_;
};

}  // single
}  // coherence
}  // logic
}  // sim

#include "Directory-impl.h"

#endif  // CORE_LOGIC_COHERENCE_SINGLE_DIRECTORY_H_
