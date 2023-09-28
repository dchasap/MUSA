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


#ifndef CORE_LOGIC_COHERENCE_CONTROLLER_IMPL_H_
#define CORE_LOGIC_COHERENCE_CONTROLLER_IMPL_H_

#include <iostream>

namespace sim {
namespace logic {
namespace coherence {

template<typename Tag,
         typename CacheLine>
inline
Controller<Tag, CacheLine>::Controller(const std::size_t cpu_id, sim::engine::Simulator &simulator,
                const engine::Config &cache_config) :
        size_(cache_config.get_value<std::size_t>("size")),
        line_size_(cache_config.get_value<std::size_t>("line-size")),
        assoc_(cache_config.get_value<std::size_t>("assoc")),
        last_level_(cache_config.key_exists("last-level") && (1 == cache_config.get_value<unsigned>("last-level"))),
        level_(cache_config.get_value<std::size_t>("level")),
        cpu_id_(cpu_id),
        cache_(simulator, size_ / line_size_, assoc_, line_size_, false,
                sim::engine::Config(simulator.get_config(), cache_config.get_value<std::string>("policy")))
{
}


template<typename Tag,
         typename CacheLine>
inline
Controller<Tag, CacheLine>::~Controller()
{
}

}  // namespace coherence
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_COHERENCE_CONTROLLER_IMPL_H_
