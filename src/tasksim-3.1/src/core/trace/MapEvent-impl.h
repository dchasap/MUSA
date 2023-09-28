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

#ifndef CORE_TRACE_MAPEVENT_IMPL_H_
#define CORE_TRACE_MAPEVENT_IMPL_H_

namespace sim {
namespace trace {

template<typename EventType, template<typename> class StreamType, typename Store,
        typename T, T EventType::*Container, unsigned InitSize>
inline
MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::MapEventCache(unsigned id, Store &store) :
    VectorEventCache<EventType, StreamType, Store, InitSize>(id, store)
{
    typename VectorEventCache<EventType, StreamType, Store, InitSize>::vector_t::iterator i = this->vector_.begin();
    for (unsigned n = 0; i != this->vector_.end(); ++i, n++) {
        const EventType &e = *i;
        map_.insert(typename map_t::value_type(e.*Container, n));
    }
}


template<typename EventType, template<typename> class StreamType, typename Store,
        typename T, T EventType::*Container, unsigned InitSize>
inline
MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::~MapEventCache()
{
}


template<typename EventType, template<typename> class StreamType, typename Store,
        typename T, T EventType::*Container, unsigned InitSize>
inline
bool MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::append(const EventType &e)
{
    bool ret = VectorEventCache<EventType, StreamType, Store, InitSize>::append(e);
    map_.insert(typename map_t::value_type(e.*Container, this->vector_.size() - 1));
    return ret;
}


template<typename EventType, template<typename> class StreamType, typename Store,
        typename T, T EventType::*Container, unsigned InitSize>
inline
bool MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::has_index(const T &index) const
{
    return map_.find(index) != map_.end();
}


template<typename EventType, template<typename> class StreamType, typename Store,
        typename T, T EventType::*Container, unsigned InitSize>
inline
const EventType &MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::get_index(const T &index) const
{
    typename map_t::const_iterator i = map_.find(index);
    assert(i != map_.end());
    return this->vector_[i->second];
}


template<typename EventType, template<typename> class StreamType, typename Store,
        typename T, T EventType::*Container, unsigned InitSize>
inline
EventType &MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::get_reference(const T &index)
{
    typename map_t::const_iterator i = map_.find(index);
    assert(i != map_.end());
    return this->vector_[i->second];
}


template<typename EventType, template<typename> class StreamType, typename Store,
        typename T, T EventType::*Container, unsigned InitSize>
inline
unsigned MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::get_index_id(const T &index) const
{
    typename map_t::const_iterator i = map_.find(index);
    assert(i != map_.end());
    return i->second;
}

template<typename EventType, template<typename> class StreamType, typename Store, typename T, T EventType::*Container, unsigned InitSize>
inline
void MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::flush_contents() {
    // CAA
    map_.clear();
    for (auto i = this->vector_.begin(); i != this->vector_.end(); ++i) {
        this->store_.append(this->id_, *i);
    }
    this->vector_.clear();
}


template<typename EventType, template<typename> class StreamType, typename Store, typename T, T EventType::*Container, unsigned InitSize>
inline
void MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::print_contents() const{
    for (auto it : map_) {
        std::cout << it.first << " " << this->vector_.at(it.second) << std::endl;
    }
}

template<typename EventType, template<typename> class StreamType, typename Store, typename T, T EventType::*Container, unsigned InitSize>
inline
uint64_t MapEventCache<EventType, StreamType, Store, T, Container, InitSize>::get_size() const {
    return this->vector_.size();
}
}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_MAPEVENT_IMPL_H_
