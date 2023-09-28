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


#ifndef CORE_LOGIC_MEMORY_MMU_H_
#define CORE_LOGIC_MEMORY_MMU_H_

#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/engine/types.h"
#include "core/engine/Config.h"
#include "core/engine/Simulator.h"

namespace sim {
namespace logic {
namespace memory {

class ITLB {
public:
    virtual ~ITLB() {};
    virtual void clear_page(engine::addr_t &) = 0;
    virtual void notify_translation() = 0;
};

class IDMA {
public:
    virtual ~IDMA() {};
    virtual void memcpy(const engine::addr_t &, const engine::addr_t &, const std::size_t &) = 0;
};

class MMU {
protected:
    engine::Config config_;
    engine::Simulator* sim_;

    typedef std::pair<unsigned, engine::addr_t> mmu_key_t;

    std::string allocation_policy_;

    typedef enum { READ = 0, WRITE, TOTAL } access_priority_t;
    access_priority_t access_priority_;

    unsigned empty_page_threshold_;
    typedef std::list<engine::addr_t> page_list_t;
    std::vector<page_list_t> empty_pages_;

    struct allocated_page_t;
    typedef std::list<allocated_page_t> allocated_page_list_t;
    std::vector<allocated_page_list_t> allocated_pages_;
    uint64_t allocated_pages_count_;

    typedef std::list<mmu_key_t> pending_translations_list_t;
    std::vector<pending_translations_list_t> pending_translations_;

    struct page_table_entry_t {
        typedef enum { ALLOCATED = 0, CLEARING, MIGRATING } status_t;

        engine::addr_t physical_address_;
        unsigned read_access_count_;
        unsigned write_access_count_;
        status_t status_;
        // engine::cycles_t last_access_;

        engine::cycles_t timestamp_;

        allocated_page_list_t::iterator page_list_iterator_;

        explicit page_table_entry_t(engine::addr_t);
    };

    struct allocated_page_t {
        engine::addr_t physical_page_;
        page_table_entry_t* active_page_;
        page_table_entry_t* profile_page_;
        explicit allocated_page_t(engine::addr_t physical_page) {
            physical_page_ = physical_page;
            active_page_ = NULL;
            profile_page_ = NULL;
        }
    };

    typedef std::unordered_map<engine::addr_t, page_table_entry_t> page_table_t;
    typedef std::unordered_map<engine::addr_t, mmu_key_t> reverse_page_table_t;
    std::vector<page_table_t> page_tables_;
    reverse_page_table_t reverse_page_table_;

    std::vector<page_table_t> profile_tables_;

    std::size_t page_size_;
    engine::addr_t* next_free_physical_page_;

    engine::addr_t color_mask_;
    unsigned color_offset_;
    unsigned color_count_;

    uint64_t total_footprint_;

    std::vector<ITLB *> tlbs_;
    IDMA* dma_;

    engine::addr_t* allocate_page(unsigned, engine::addr_t);
    void victimize_page(unsigned);

    unsigned forward_migration_count_;
    unsigned backward_migration_count_;
    unsigned clear_count_;
    engine::cycles_t cumulative_clear_time_;
    engine::cycles_t cumulative_migration_time_;

    uint64_t fast_memory_read_count_;
    uint64_t fast_memory_write_count_;
    float backward_migration_factor_;

    bool dram_size_registered_;
    engine::addr_t fast_mem_size_;
    bool is_in_fast_memory(engine::addr_t);

public:
    MMU(engine::Simulator *, const engine::Config &, const engine::Config &);
    ~MMU();
    engine::addr_t* translate(unsigned, engine::addr_t, bool);
    engine::addr_t reverse_translate(engine::addr_t);
    void notify_access(sim::logic::coherence::single::Message::Request &);
    void dump_access_table(const std::string &);
    std::size_t get_page_size();
    void register_tlb(ITLB *tlb);
    void register_dma(IDMA *dma);
    void register_dram_size(std::size_t);

    void load_access_profile();
    void notify_clear_complete(unsigned, engine::addr_t &);
    void notify_migration_complete(engine::addr_t &, engine::addr_t &, std::size_t &);
};


}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "MMU-impl.h"

#endif  // CORE_LOGIC_MEMORY_MMU_H_
