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

#ifndef CORE_LOGIC_COHERENCE_SINGLE_DIRECTORY_IMPL_H_
#define CORE_LOGIC_COHERENCE_SINGLE_DIRECTORY_IMPL_H_

#include "core/utils/Log.h"

using sim::utils::Log;

namespace sim {
namespace logic {
namespace coherence {
namespace single {

template<typename Message>
inline
Directory<Message>::Directory(uint32_t num_cores, unsigned shared_level) :
    num_cores_(num_cores),
    shared_level_(shared_level)
{
}

template<typename Message>
inline
void Directory<Message>::register_cache(cache_t &cache){
    cache_id_t cache_id = cache.get_module_id();
    unsigned core_id = cache.get_id();
    unsigned level = cache.get_level();
    cache_dictionary_[cache_id] = &cache;
    cache_info_[cache_id] = cache_info_t(cache_id, level, level >= shared_level_);
    Log::debug() << "[Directory] Registering Cache ID " << cache_id << " which belongs to core " << core_id
                 << " and level " << level << " and is " << (level >= shared_level_ ? "SHARED" : "PRIVATE");
}

}  // single
}  // coherence
}  // logic
}  // sim

#endif  // CORE_LOGIC_COHERENCE_SINGLE_DIRECTORY_IMPL_H_
