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


#ifndef CORE_LOGIC_MEMORY_POLICY_TREEPLRU_IMPL_H_
#define CORE_LOGIC_MEMORY_POLICY_TREEPLRU_IMPL_H_

#include <iostream>
#include <string>
#include <utility>
#include <cmath>


namespace sim {
namespace logic {
namespace memory {
namespace policy {


inline
OrderedBinaryTree::OrderedBinaryTree(const std::size_t size) :
    nodes_(size - 1, Direction::LEFT)
{
}


inline
std::size_t OrderedBinaryTree::out_of_bounds() const
{
    // this is used as an invalid index that points after the end of the container
    // size == N - 1 (N - number of lines in cache set)
    // size + (size + 1) + 1 [internal nodes + leaves + 1]
    return 2 * nodes_.size() + 2;
}


inline
std::size_t OrderedBinaryTree::get_last() const
{
    std::size_t node;
    for (node = 0; !is_leaf(node); node = get_default_child(node)) {
    }
    return node - nodes_.size();
}


inline
std::size_t OrderedBinaryTree::get_previous(const std::size_t leaf_index) const
{
    const std::size_t adjusted_leaf_index = leaf_index + nodes_.size();
    std::size_t child = adjusted_leaf_index;
    std::size_t node = get_parent(child);

    // go up, while the node points to the side of current child
    while (node != 0 and child != get_default_child(node)) {
        child = node;
        node = get_parent(node);
    }

    if (node == 0 and child != get_default_child(node)) {
        return out_of_bounds();
    }

    // go to the sibling
    node = get_sibling(child);
    // go down, following node directions
    while (!is_leaf(node)) {
        node = get_default_child(node);
    }

    // adjust back
    return node - nodes_.size();
}


inline
void OrderedBinaryTree::promote(const std::size_t leaf_index)
{
    const std::size_t adjusted_leaf_index = leaf_index + nodes_.size();

    for (std::size_t child = adjusted_leaf_index, node = get_parent(adjusted_leaf_index);
            child != 0;
            child = node, node = get_parent(node)) {
        // point away from child
        if (child % 2 == 0) {  // child is right
            nodes_[node] = Direction::LEFT;
        } else {  // child is left
            nodes_[node] = Direction::RIGHT;
        }
    }
}


inline
void OrderedBinaryTree::print_tree() const
{
    const std::size_t levels = std::log2(nodes_.size() + 1);
    std::cout << "levels " << levels << std::endl;
    std::size_t padding = nodes_.size() / 2;
    for (std::size_t level = 0; level < levels; ++level) {
        for (std::size_t index = std::pow(2, level) - 1; index < std::pow(2, level + 1) - 1; ++index) {
            // print padding
            for (std::size_t i = 0; i < padding; ++i) std::cout << " ";
            // print number
            std::cout << ((nodes_[index] == Direction::LEFT) ? "<" : ">");
//            std::cout << index;
            // print padding
            for (std::size_t i = 0; i < padding; ++i) std::cout << " ";
            // print one space
            std::cout << " ";
        }
        std::cout << std::endl;
        padding = (padding - 1) / 2;
    }
    std::cout << std::endl;
}


inline
bool OrderedBinaryTree::is_leaf(const std::size_t index) const
{
    return (index >= nodes_.size() and index <= 2 * nodes_.size() + 1);
}


inline
std::size_t OrderedBinaryTree::get_parent(const std::size_t index) const
{
    return std::floor((index - 1) / 2);
}


inline
std::size_t OrderedBinaryTree::get_default_child(const std::size_t parent_index) const
{
    // direction of the node points to its default child
    return 2 * parent_index + 1 + nodes_[parent_index];
}


inline
std::size_t OrderedBinaryTree::get_sibling(const std::size_t index) const
{
    if (index % 2 == 0) {  // "index" is right sibling,
        return index - 1;  // return left
    } else {  // "index" is left sibling,
        return index + 1;  // return right
    }
}


template<typename CacheSet>
inline
TreePLRU<CacheSet>::TreePLRU(CacheSet *set, const Parameters &params) : Policy<CacheSet>(set, params),
        tree_(this->params_.size_)
{
}


template<typename CacheSet>
inline
TreePLRU<CacheSet>::~TreePLRU()
{
}


template<typename CacheSet>
inline
void TreePLRU<CacheSet>::insert(const Tag &tag, const CacheLine &line)
{
    const auto success_and_iterator = this->set_->insert_and_get_iterator(tag, line);
    assert(success_and_iterator.first == true);

    std::size_t free_index = this->storage_.insert(tag, success_and_iterator.second);
    tree_.promote(free_index);
}


template<typename CacheSet>
inline
void TreePLRU<CacheSet>::update(const Tag &tag)
{
    const std::size_t index = this->storage_.find(tag);
    tree_.promote(index);
}


template<typename CacheSet>
inline
std::string TreePLRU<CacheSet>::get_name() const
{
    return "TreePLRU";
}


template<typename CacheSet>
inline
std::size_t TreePLRU<CacheSet>::get_evict_index()
{
    std::size_t index;
    for (index = tree_.get_last() ; index != tree_.out_of_bounds(); index = tree_.get_previous(index)) {
        if (this->storage_.valid_[index] and !this->storage_.get_cache_line(index).locked()) {
            break;
        }
    }

    assert(index != tree_.out_of_bounds());
    return index;
}


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#endif  // CORE_LOGIC_MEMORY_POLICY_TREEPLRU_IMPL_H_
