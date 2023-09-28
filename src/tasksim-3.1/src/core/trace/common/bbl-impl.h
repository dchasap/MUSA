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


#ifndef CORE_TRACE_COMMON_BBL_IMPL_H_
#define CORE_TRACE_COMMON_BBL_IMPL_H_

#include <vector>

namespace sim {
namespace trace {

inline
bbl_id_t::bbl_id_t(unsigned id) :
    id_(id)
{
}

inline
bbl_id_t &bbl_id_t::operator=(const unsigned &value)
{
    id_ = value;
    return *this;
}

inline
bool bbl_id_t::operator==(const unsigned &id) const
{
    return this->id_ == id;
}

inline
bool bbl_id_t::operator==(const bbl_id_t &bbl_id) const
{
    return this->id_ == bbl_id.id_;
}

inline
std::ostream &operator<<(std::ostream &os, const bbl_id_t &bbl_id)
{
    os << bbl_id.id_;
    return os;
}

inline
std::istream &operator>>(std::istream &is, bbl_id_t &bbl_id)
{
    is >> bbl_id.id_;
    return is;
}

inline
void bbl_t::write(std::ostream &os) const
{
    os.write(reinterpret_cast<const char *>(&id_), sizeof(id_));
    os.write(reinterpret_cast<const char *>(&instr_ptr_), sizeof(instr_ptr_));
    // os.write(reinterpret_cast<const char *>(&n_ops_), sizeof(n_ops_));
    os.write(reinterpret_cast<const char *>(&block_size_), sizeof(block_size_));
    uint16_t size = ins_.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size));
    for (uint64_t i = 0; i < size; i++) {
        ins_[i].write(os);
    }

    size = macroops_.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size));
    for (uint64_t i = 0; i < size; i++) {
        macroops_[i].write(os);
    }
}

inline
void bbl_t::read(std::istream &is)
{
    is.read(reinterpret_cast<char *>(&id_), sizeof(id_));
    is.read(reinterpret_cast<char *>(&instr_ptr_), sizeof(instr_ptr_));
    // is.read(reinterpret_cast<char *>(&n_ops_), sizeof(n_ops_));
    is.read(reinterpret_cast<char *>(&block_size_), sizeof(block_size_));

    uint16_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size));
    ins_ = std::vector<minst_t>(size);
    for (uint64_t i = 0; i < size; i++) {
        ins_[i].read(is);
    }

    is.read(reinterpret_cast<char *>(&size), sizeof(size));
    macroops_ = std::vector<minst_t>(size);
    for (uint64_t i = 0; i < size; i++) {
        macroops_[i].read(is);
    }
}

inline
void bbl_t::shallow_copy(const bbl_t &other)
{
    id_ = other.id_;
    instr_ptr_ = other.instr_ptr_;
    block_size_ = other.block_size_;
}

inline
std::ostream &operator<<(std::ostream &os, const bbl_t &bbl)
{
    os << bbl.id_ << ":" << bbl.instr_ptr_ << ":" << bbl.block_size_;  //  << ":" << bbl.n_ops_;

    os << ":" << bbl.ins_.size();
    for (unsigned i = 0; i < bbl.ins_.size(); i++) {
        os << ":" << bbl.ins_[i];
    }

    os << ":" << bbl.macroops_.size();
    for (unsigned i = 0; i < bbl.macroops_.size(); i++) {
        os << ":" << bbl.macroops_[i];
    }

    return os;
}

inline
std::istream &operator>>(std::istream &is, bbl_t &bbl)
{
    // Just in case someone reuses a previous bbl.
    bbl.ins_.clear();
    bbl.macroops_.clear();
    char sep;

    is >> bbl.id_;
    is >> sep;
    assert(sep == ':');
    is >> bbl.instr_ptr_;
    /*
    is >> sep;
    assert(sep == ':');
    is >> bbl.n_ops_;
    */
    is >> sep;
    assert(sep == ':');
    is >> bbl.block_size_;

    is >> sep;
    assert(sep == ':');

    unsigned num_elems;

    is >> num_elems;
    for (unsigned i = 0; i < num_elems; i++) {
        minst_t minst;
        is >> sep;
        assert(sep == ':');
        is >> minst;
        bbl.ins_.push_back(minst);
    }

    is >> sep;
    assert(sep == ':');
    is >> num_elems;
    for (unsigned i = 0; i < num_elems; i++) {
        minst_t minst;
        is >> sep;
        assert(sep == ':');
        is >> minst;
        bbl.macroops_.emplace_back(minst);
    }

    return is;
}

}  // namespace trace
}  // namespace sim

/** \class std::hash<sim::trace::bbl_id_t>
 * Specialization of std::hash for hashing a bbl_id_t
 */

#include <functional>

namespace std {

template <>
class hash<sim::trace::bbl_id_t> : public unary_function<sim::trace::bbl_id_t, size_t> {
public:
    size_t operator()(const sim::trace::bbl_id_t& bbl_id) const
    {
        return std::hash<unsigned>()(bbl_id.id_);
    }
};

}  // namespace std

#endif  // CORE_TRACE_COMMON_BBL_IMPL_H_
