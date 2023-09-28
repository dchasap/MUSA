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


#ifndef CORE_STATS_COUNTERMAP_H_
#define CORE_STATS_COUNTERMAP_H_

#include <unordered_map>

#include <string>
#include <type_traits>
#include "core/stats/Stat.h"

namespace sim {
namespace stats {

template<typename KeyType, typename ValueType = uint64_t>
class CounterMap : public Stat {
public:
    /** Constructor with Module owner and Stat name. */
    template <typename T>
    CounterMap(const T &owner, const std::string &name);
    /** Constructor with Module owner, Stat name and Key names. */
    template <typename T>
    CounterMap(const T &owner, const std::string &name, const std::unordered_map<KeyType, std::string> names);
    /** Default destructor. */
    ~CounterMap();
    /** Index operator. */
    ValueType& operator[](KeyType);
    /** Extrapolate function (TaskPoint). */
    void extrapolate(uint64_t from, uint64_t to);

    /** Function to insert the name of a key. */
    bool insert_id_name(KeyType key, const std::string name);
    /** Function to insert a set of names for keys. */
    bool insert_id_names(const std::unordered_map<KeyType, std::string> &names);


protected:
    /** Dictionary to Store the Counted Value for each Key. */
    std::unordered_map<KeyType, ValueType> count_map_;
    /** Dictionary to store the name of each Key. */
    std::unordered_map<KeyType, std::string> names_;

    /** Extrapolate function enabled only if the Key is convertible to uint64_t. */
    template<typename K = KeyType, typename V = ValueType>
    typename std::enable_if<std::is_convertible<K, uint64_t>::value && std::is_arithmetic<V>::value>::type
    extrap(uint64_t from, uint64_t to);

    /** Extrapolate function enabled only if the Key is convertible to uint64_t. */
    template<typename K = KeyType, typename V = ValueType>
    typename std::enable_if<std::is_convertible<K, uint64_t>::value && !std::is_arithmetic<V>::value>::type
    extrap(uint64_t from, uint64_t to);

    /** Extrapolate function enabled only if the Key is NOT convertible to uint64_t. */
    template<typename K = KeyType, typename V = ValueType>
    typename std::enable_if<!std::is_convertible<K, uint64_t>::value>::type
    extrap(uint64_t from, uint64_t to);

    /** Print function enabled only if ValueType is an integral type. */
    template<typename V = ValueType>
    typename std::enable_if<std::is_arithmetic<V>::value>::type
    print_values();

    /** Print function enabled only if ValueType is a Dictionary type. */
    template<typename V = ValueType>
    typename std::enable_if<!std::is_arithmetic<V>::value>::type
    print_values();
};

}  // namespace stats
}  // namespace sim

#include "CounterMap-impl.h"

#endif  // CORE_STATS_COUNTERMAP_H_
