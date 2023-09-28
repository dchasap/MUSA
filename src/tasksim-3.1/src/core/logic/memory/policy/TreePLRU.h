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


#ifndef CORE_LOGIC_MEMORY_POLICY_TREEPLRU_H_
#define CORE_LOGIC_MEMORY_POLICY_TREEPLRU_H_

#include <string>
#include <utility>
#include <vector>
#include "Policy.h"


namespace sim {
namespace logic {
namespace memory {
namespace policy {

/*
 *        O
 *       / \
 *      /   \
 *     /     \
 *    O       O
 *   / \     / \
 * [ ] [ ] [ ] [ ]
 *
 * O - kept in Tree, used for ordering of []
 * [] - kept in Policy, actual contents
 *
 * Indexing done on the assumtion that the nodes are logically positioned in memory as follows
 * (NOTE: they are not actually positioned like this, as they are of different types (therefore, sizes))
 * OOO[][][][]
 */

class OrderedBinaryTree
{
public:
    typedef enum { LEFT, RIGHT } Direction;  // points to the subtree that contains LRU

    explicit OrderedBinaryTree(const std::size_t size);

    std::size_t out_of_bounds() const;

    // gives the pseudo-LRU position
    std::size_t get_last() const;

    // from LRU, towards the MRU
    std::size_t get_previous(const std::size_t leaf_index) const;

    // updates all nodes up to the root so that they point away from the given leaf
    void promote(const std::size_t leaf_index);

    void print_tree() const;
private:
    std::vector<Direction> nodes_;

    bool is_leaf(const std::size_t index) const;
    std::size_t get_parent(const std::size_t index) const;
    std::size_t get_default_child(const std::size_t parent_index) const;
    std::size_t get_sibling(const std::size_t index) const;
};


template<typename CacheSet>
class TreePLRU : public Policy<CacheSet> {
public:
    using Tag = typename Policy<CacheSet>::Tag;
    using CacheLine = typename Policy<CacheSet>::CacheLine;

    TreePLRU(CacheSet *set, const Parameters &params);
    ~TreePLRU();

    void insert(const Tag &tag, const CacheLine &line);
    void update(const Tag &tag);

    std::string get_name() const;
private:
    OrderedBinaryTree tree_;

    std::size_t get_evict_index();
};

}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "TreePLRU-impl.h"

#endif  // CORE_LOGIC_MEMORY_POLICY_TREEPLRU_H_
