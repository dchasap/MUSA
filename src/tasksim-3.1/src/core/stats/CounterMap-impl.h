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


#ifndef CORE_STATS_COUNTERMAP_IMPL_H_
#define CORE_STATS_COUNTERMAP_IMPL_H_

#include <string>
#include <type_traits>
#include <utility>

namespace sim {
namespace stats {

template <typename KeyType, typename ValueType>
template <typename T>
inline
CounterMap<KeyType, ValueType>::CounterMap(const T &obj, const std::string &name) :
    Stat(obj, name),
    count_map_()
{
}

template <typename KeyType, typename ValueType>
template <typename T>
inline
CounterMap<KeyType, ValueType>::CounterMap(const T &obj, const std::string &name,
        const std::unordered_map<KeyType, std::string> names) :
    Stat(obj, name),
    count_map_(),
    names_(names)
{
}

template <typename KeyType, typename ValueType>
inline
CounterMap<KeyType, ValueType>::~CounterMap()
{
    print_values();
}

template <typename KeyType, typename ValueType>
inline
ValueType& CounterMap<KeyType, ValueType>::operator[](KeyType k)
{
    if (count_map_.find(k) == count_map_.end()) {
        count_map_.insert(std::pair<KeyType, ValueType>(k, ValueType()));
    }
    return count_map_[k];
}

template <typename KeyType, typename ValueType>
inline
void CounterMap<KeyType, ValueType>::extrapolate(uint64_t from, uint64_t to)
{
    extrap(from, to);
}

template <typename KeyType, typename ValueType>
inline
bool
CounterMap<KeyType, ValueType>::insert_id_name(KeyType key, const std::string name) {
    if (names_.find(key) == names_.end()) {
        names_.insert(key, name);
        return true;
    } else {
        std::cerr << "[Warning] CounterMap entry for key " << key << " already exists." << std::endl;
        return false;
    }
}

template <typename KeyType, typename ValueType>
inline
bool
CounterMap<KeyType, ValueType>::insert_id_names(const std::unordered_map<KeyType, std::string> &names)
{
    bool ret = true;
    for (const auto &elem : names) {
        ret = ret && insert_id_name(elem.first, elem.second);
    }
    return ret;
}

template<typename KeyType, typename ValueType>
template<typename K, typename V>
inline
typename std::enable_if<std::is_convertible<K, uint64_t>::value && std::is_arithmetic<V>::value>::type
CounterMap<KeyType, ValueType>::extrap(uint64_t from, uint64_t to)
{
    count_map_[static_cast<KeyType>(to)] = count_map_[static_cast<KeyType>(from)];
}

template<typename KeyType, typename ValueType>
template<typename K, typename V>
inline
typename std::enable_if<std::is_convertible<K, uint64_t>::value && !std::is_arithmetic<V>::value>::type
CounterMap<KeyType, ValueType>::extrap(uint64_t from, uint64_t to)
{
    for (auto &elem : count_map_) {
        auto &dict = elem.second;
        dict[to] = dict[from];
    }
}

template<typename KeyType, typename ValueType>
template<typename K, typename V>
inline
typename std::enable_if<!std::is_convertible<K, uint64_t>::value>::type
CounterMap<KeyType, ValueType>::extrap(uint64_t from, uint64_t to)
{
}

template<typename KeyType, typename ValueType>
template<typename V>
typename std::enable_if<std::is_arithmetic<V>::value>::type
CounterMap<KeyType, ValueType>::print_values()
{
    typename std::unordered_map<KeyType, ValueType>::iterator it;
    for (it = count_map_.begin(); it != count_map_.end(); it++) {
        if (it != count_map_.begin()) {
            Stat::out_ << ":";  // separate value, but not the last
        }
        if (names_.find(it->first) != names_.end()) {
            Stat::out_ << names_.at(it->first) << ":" << it->second;
        } else {
            Stat::out_ << it->first << ":" << it->second;
        }
    }
}

template<typename KeyType, typename ValueType>
template<typename V>
typename std::enable_if<!std::is_arithmetic<V>::value>::type
CounterMap<KeyType, ValueType>::print_values()
{
    ValueType total;
    for (const auto &entry : count_map_) {
        KeyType key = entry.first;
        ValueType dict = entry.second;
        bool first = true;
        typename ValueType::mapped_type total_val = 0;
        for (const auto &elem : dict) {
            if (first) {
                first = false;
            } else {
                Stat::out_ << ":";
            }
            total_val += elem.second;
            Stat::out_ << elem.first << ":" << elem.second;
            total[elem.first] += elem.second;
        }
        Stat::out_ << ":total:" << total_val;
        if (names_.find(key) != names_.end()) {
            Stat::out_ << ":" << this->name_ << "_" << names_.at(key) << ":"
                       << this->class_ << ":" << this->id_ << std::endl;
        } else {
            Stat::out_ << ":" << this->name_ << "_" << key << ":"
                       << this->class_ << ":" << this->id_ << std::endl;
        }
    }
    bool first = true;
    typename ValueType::mapped_type total_val = 0;
    for (const auto &elem : total) {
        if (first) {
            first = false;
        } else {
            Stat::out_ << ":";
        }
        total_val += elem.second;
        Stat::out_ << elem.first << ":" << elem.second;
        total[elem.first] += elem.second;
    }
    Stat::out_ << ":total:" << total_val;
    Stat::out_ << ":" << this->name_ << "_ANY" << ":"
               << this->class_ << ":" << this->id_ << std::endl;
    this->print_ = false;
}

}  // namespace stats
}  // namespace sim


#endif  // CORE_STATS_COUNTERMAP_IMPL_H_
