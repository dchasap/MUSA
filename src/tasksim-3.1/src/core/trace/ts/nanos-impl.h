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


#ifndef CORE_TRACE_TS_NANOS_IMPL_H_
#define CORE_TRACE_TS_NANOS_IMPL_H_

#include <cassert>
#include <iostream>
#include <vector>

//#include <chrono>
//typedef std::chrono::high_resolution_clock Clock;


namespace sim {
namespace trace {
namespace ts {

inline
dep_t::dep_t() :
    address(NULL), offset(0)
{};

inline
dep_t::dep_t(const nanos_data_access_internal_t &dep) :
    address(dep.address), flags(dep.flags), offset(dep.offset)
{
//  std::chrono::system_clock::time_point bug_ini,bug_fini;
//  bug_ini = Clock::now();
// CAA TODO: maybe this is the cause....timer and entry message
//    std::cerr << "[BUG DEPS] dep_t size " << dep.dimension_count << std::endl;
    
    // CAA reserve ++  
    dimensions.reserve(dep.dimension_count);

    for (uint16_t i = 0; i < dep.dimension_count; ++i) {
        dimensions.push_back(dep.dimensions[i]);
    }
//  bug_fini = Clock::now();
//  if ((std::chrono::duration_cast<std::chrono::seconds>(bug_fini-bug_ini).count()) > 0)
//    std::cerr << "[BUG DEPS] dep_t size " << dep.dimension_count << " TIME spend is " << std::chrono::duration_cast<std::chrono::seconds>(bug_fini-bug_ini).count()<< std::endl;
    
}

inline
void dep_t::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char *>(&address), sizeof(address));
    os.write(reinterpret_cast<const char *>(&flags), sizeof(flags));
    os.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
    std::size_t num_dims = dimensions.size();
    os.write(reinterpret_cast<const char *>(&num_dims), sizeof(num_dims));
    for (std::vector<nanos_region_dimension_internal_t>::const_iterator d = dimensions.begin();
            d != dimensions.end(); ++d) {
        os.write(reinterpret_cast<const char *>(&(*d)), sizeof(*d));
    }
}


inline
void dep_t::read(std::istream &is)
{
    is.read(reinterpret_cast<char *>(&address), sizeof(address));
    is.read(reinterpret_cast<char *>(&flags), sizeof(flags));
    is.read(reinterpret_cast<char *>(&offset), sizeof(offset));
    std::size_t num_dims;
    is.read(reinterpret_cast<char *>(&num_dims), sizeof(num_dims));
    for (std::size_t d = 0; d < num_dims; ++d) {
        nanos_region_dimension_t dim;
        is.read(reinterpret_cast<char *>(&dim), sizeof(dim));
        dimensions.push_back(dim);
    }
}


inline
size_t dep_t::accessed_length() const
{
    size_t dep_size = 1;
    for (const auto &dim: dimensions) {
        dep_size *= dim.accessed_length;
    }
    return dep_size;
}


inline
bool dep_t::contiguous() const
{
    // Last dimension as bytes, all previous dimensions expressed in terms of the next one.
    // For contiguity:
    // - if all dimensions have size == accessed_length, ok
    // - if one dimension has size > accessed_length, all deps above must have accessed_length == 1
    bool contiguous = true, found_partial_dim = false;

    for (const auto &dim: dimensions) {
        if (found_partial_dim) {
            contiguous = contiguous && dim.accessed_length == 1;
        } else if (dim.size != dim.accessed_length) {
            found_partial_dim = true;
        }
    }

    return contiguous;
}

inline
std::ostream &operator<<(std::ostream &os, const dep_t &d)
{
    os << d.address << ":"
       << d.flags.input << ":"
       << d.flags.output << ":"
       << d.flags.can_rename << ":"
       << d.flags.concurrent << ":"
       << d.flags.commutative << ":"
       << d.offset << ":"
       << d.dimensions.size();
    for (std::vector<nanos_region_dimension_internal_t>::const_iterator i = d.dimensions.begin();
            i != d.dimensions.end(); ++i) {
        os << ":" << i->size
           << ":" << i->lower_bound
           << ":" << i->accessed_length;
    }
    return os;
}

inline
std::istream &operator>>(std::istream &is, dep_t &d)
{
    char sep;
    void *addr;
    is >> addr; d.address = static_cast<void *>(addr); is >> sep; assert(sep == ':');

    bool b;
    is >> b; d.flags.input = (b == true) ? 1 : 0; is >> sep; assert(sep == ':');
    is >> b; d.flags.output = (b == true) ? 1 : 0; is >> sep; assert(sep == ':');
    is >> b; d.flags.can_rename = (b == true) ? 1 : 0; is >> sep; assert(sep == ':');
    is >> b; d.flags.concurrent = (b == true) ? 1 : 0; is >> sep; assert(sep == ':');
    is >> b; d.flags.commutative = (b == true) ? 1 : 0; is >> sep; assert(sep == ':');
    is >> d.offset; is >> sep; assert(sep == ':');

    std::size_t num_dims;
    is >> num_dims;
    d.dimensions.clear(); /* We need to clear the vector because it might be being reused */
    for (std::size_t i = 0; i < num_dims; ++i) {
        is >> sep; assert(sep == ':');
        nanos_region_dimension_internal_t dim;
        is >> dim.size; is >> sep; assert(sep == ':');
        is >> dim.lower_bound; is >> sep; assert(sep == ':');
        is >> dim.accessed_length;
        d.dimensions.push_back(dim);
    }
    return is;
}

inline
void wd_info_t::write(std::ostream &os) const
{
    std::size_t num_deps = deps_.size();
    os.write(reinterpret_cast<const char *>(&num_deps), sizeof(num_deps));
    for (std::vector<dep_t>::const_iterator i = deps_.begin(); i != deps_.end(); ++i) {
        i->write(os);
    }
}

inline
void wd_info_t::read(std::istream &is)
{
    std::size_t num_deps;
    is.read(reinterpret_cast<char *>(&num_deps), sizeof(num_deps));
    deps_.resize(num_deps);
    for (std::vector<dep_t>::iterator i = deps_.begin(); i != deps_.end(); ++i) {
        i->read(is);
    }
}

inline
std::ostream &operator<<(std::ostream &os, const wd_info_t &wd_info)
{
    os << wd_info.deps_.size();
    for (unsigned int i = 0; i < wd_info.deps_.size(); i++) {
        os << ":" << wd_info.deps_[i];
    }
    return os;
}

inline
std::istream &operator>>(std::istream &is, wd_info_t &wd_info)
{
    char sep;
    unsigned num_deps = 0;
    is >> num_deps;
    wd_info.deps_.resize(num_deps);
    for (unsigned int i = 0; i < num_deps; i++) {
        is >> sep; assert(sep == ':');
        is >> wd_info.deps_[i];
    }
    return is;
}

inline
void wd_info_event_t::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char *>(&wd_id_), sizeof(wd_id_));
    info_.write(os);
}

inline
void wd_info_event_t::read(std::istream &is)
{
    is.read(reinterpret_cast<char *>(&wd_id_), sizeof(wd_id_));
    info_.read(is);
}

inline
std::ostream &operator<<(std::ostream &os, const wd_info_event_t &wd_info)
{
    os << wd_info.wd_id_ << ":";
    os << wd_info.info_;
    return os;
}

inline
std::istream &operator>>(std::istream &is, wd_info_event_t &wd_info)
{
    char sep;
    is >> wd_info.wd_id_;
    is >> sep; assert(sep == ':');
    is >> wd_info.info_;
    return is;
}

}  // namespace ts
}  // namespace trace
}  // namespace sim


#endif  // CORE_TRACE_TS_NANOS_IMPL_H_
