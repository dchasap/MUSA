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


#ifndef CORE_LOGIC_MEMORY_MMU_IMPL_H_
#define CORE_LOGIC_MEMORY_MMU_IMPL_H_

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "core/utils/Log.h"

namespace sim {
namespace logic {
namespace memory {

using sim::utils::Log;

using Request = sim::logic::coherence::single::Message::Request;

MMU::page_table_entry_t::page_table_entry_t(engine::addr_t physical_address) :
    physical_address_(physical_address),
    read_access_count_(0),
    write_access_count_(0),
    status_(ALLOCATED),
    // last_access_(0),
    timestamp_(0)
{}


inline
MMU::MMU(engine::Simulator* sim, const engine::Config &config, const engine::Config &cache_config) :
    config_(config), sim_(sim),
    allocation_policy_(config.get_value<std::string>("allocation-policy")),
    access_priority_(TOTAL),
    empty_page_threshold_(0), allocated_pages_count_(0),
    page_size_(config.get_value<std::size_t>("page-size")),
    total_footprint_(0), dma_(NULL),
    forward_migration_count_(0), backward_migration_count_(0),
    clear_count_(0), cumulative_clear_time_(0),
    cumulative_migration_time_(0),
    fast_memory_read_count_(0), fast_memory_write_count_(0),
    backward_migration_factor_(1),
    dram_size_registered_(false),
    fast_mem_size_(0)
{
    color_mask_ = 0;
    color_offset_ = 0;
    color_count_ = 1;
    if ((cache_config.key_exists("size")) && (cache_config.key_exists("assoc"))) {
        std::size_t cache_size = cache_config.get_value<std::size_t>("size");
        std::size_t cache_assoc = cache_config.get_value<std::size_t>("assoc");
        color_mask_ = ((cache_size / cache_assoc) - 1) & (~(page_size_ - 1));
        if (0 != color_mask_) {
            color_count_ = cache_size / cache_assoc / page_size_;
            std::size_t i = page_size_;
            while (0 == (i & 1)) {
                i >>= 1;
                color_offset_++;
            }
        }
    }

    if (config_.key_exists("access-priority")) {
        std::string access_priority = config_.get_value<std::string>("access-priority");
        if ("read" == access_priority) access_priority_ = READ;
        if ("write" == access_priority) access_priority_ = WRITE;
        if ("total" == access_priority) access_priority_ = TOTAL;
    }

    if (config_.key_exists("empty-page-threshold"))
        empty_page_threshold_ = config_.get_value<unsigned>("empty-page-threshold");

    if (config_.key_exists("backward-migration-factor"))
        backward_migration_factor_ = config_.get_value<float>("backward-migration-factor");

    next_free_physical_page_ = new engine::addr_t[color_count_];
    if ((allocation_policy_ == "dynamic_1") ||
            (allocation_policy_ == "dynamic_2") ||
            (allocation_policy_ == "dynamic_3")) {
        for (unsigned i = 0; i < color_count_; i++) {
            page_list_t empty_page_list;
            empty_pages_.emplace_back(empty_page_list);

            allocated_page_list_t allocated_page_list;
            allocated_pages_.emplace_back(allocated_page_list);

            pending_translations_list_t pending_translations_list;
            pending_translations_.emplace_back(pending_translations_list);
        }
    }

    for (unsigned i = 0; i < color_count_; i++)
        next_free_physical_page_[i] = i << color_offset_;
}

inline
MMU::~MMU()
{
    std::cout << "Clear count: " << clear_count_ << std::endl;
    if (clear_count_ > 0) {
        std::cout << "Average clear time: " << cumulative_clear_time_ / clear_count_ << std::endl;
    }
    std::cout << "Forward migration count: " << forward_migration_count_ << std::endl;
    std::cout << "Backward migration count: " << backward_migration_count_ << std::endl;
    if (forward_migration_count_ + backward_migration_count_ > 0) {
        std::cout << "Average migration time: "
                  << cumulative_migration_time_ / (forward_migration_count_ + backward_migration_count_) << std::endl;
    }
    std::cout << "Total footprint: " << total_footprint_ << std::endl;
}

inline
engine::addr_t* MMU::translate(unsigned cpu_id, engine::addr_t logical_addr, bool do_allocate = true)
{
    engine::addr_t physical_page;
    engine::addr_t logical_offset = logical_addr & (page_size_ - 1);
    engine::addr_t logical_page = logical_addr & (~(page_size_ - 1));
    page_table_t::iterator i = page_tables_[cpu_id].find(logical_page);
    if (i == page_tables_[cpu_id].end()) {
        if (!do_allocate) return NULL;
        engine::addr_t* allocated_page = this->allocate_page(cpu_id, logical_addr);
        if (allocated_page == NULL)
            return NULL;
        physical_page = *allocated_page;
        delete allocated_page;
    } else {
        if (i->second.status_ == page_table_entry_t::status_t::ALLOCATED)
            physical_page = i->second.physical_address_;
        else
            return NULL;
    }

    return new engine::addr_t(physical_page + logical_offset);
}


inline
engine::addr_t MMU::reverse_translate(engine::addr_t physical_addr)
{
    engine::addr_t physical_offset = physical_addr & (page_size_ - 1);
    engine::addr_t physical_page = physical_addr & (~(page_size_ - 1));

    reverse_page_table_t::iterator i = reverse_page_table_.find(physical_page);

    MMU::mmu_key_t mmu_key = i->second;
    engine::addr_t logical_page = mmu_key.second;

    return logical_page + physical_offset;
}

inline
void MMU::victimize_page(unsigned page_color) {
    engine::addr_t victim_page;
    if ((allocation_policy_ == "dynamic_1") || (allocation_policy_ == "dynamic_3")) {
        victim_page = allocated_pages_[page_color].front().physical_page_;
        allocated_pages_[page_color].pop_front();
        allocated_pages_count_--;

        /*bool victim_candidate_found = false;
        typename page_list_t::iterator victim_page_candidate;
        unsigned lowest_order_key;
        unsigned index = 0;
        typename page_list_t::iterator it = allocated_pages_[page_color].begin();
        for (; it != allocated_pages_[page_color].end(); ++it) {
            if (index <= allocated_pages_[page_color].size() * 3 / 4) {
                engine::addr_t physical_page = *it;
                reverse_page_table_t::iterator rev_it = reverse_page_table_.find(physical_page);

                MMU::mmu_key_t mmu_key = rev_it->second;
                unsigned cpu_id = mmu_key.first;
                engine::addr_t logical_page = mmu_key.second;

                typename page_table_t::iterator pgt_it = page_tables_[cpu_id].find(logical_page);
                page_table_entry_t active_page = pgt_it->second;

                unsigned order_key;
                if (READ == access_priority_) {
                    order_key = active_page.read_access_count_;
                }
                if (WRITE == access_priority_) {
                    order_key = active_page.write_access_count_;
                }
                if (TOTAL == access_priority_) {
                    order_key = active_page.read_access_count_ + active_page.write_access_count_;
                }

                //order_key = order_key / (allocated_pages_[page_color].size() - 4 - index + 1);

                if ((false == victim_candidate_found) || (order_key < lowest_order_key)) {
                    victim_page_candidate = it;
                    lowest_order_key = order_key;
                    victim_candidate_found = true;
                }
            }

            index++;
        }

        victim_page = *victim_page_candidate;
        allocated_pages_[page_color].erase(victim_page_candidate);*/
    }

    if (allocation_policy_ == "dynamic_2") {
        bool victim_candidate_found = false;
        typename allocated_page_list_t::iterator victim_page_candidate;
        unsigned lowest_order_key;
        // unsigned lowest_order_index;
        typename allocated_page_list_t::iterator it = allocated_pages_[page_color].begin();
        for (; it != allocated_pages_[page_color].end(); ++it) {
            /*engine::addr_t physical_page = it->physical_page_;
            reverse_page_table_t::iterator rev_it = reverse_page_table_.find(physical_page);

            MMU::mmu_key_t mmu_key = rev_it->second;
            unsigned cpu_id = mmu_key.first;
            engine::addr_t logical_page = mmu_key.second;

            typename page_table_t::iterator pgt_it = page_tables_[cpu_id].find(logical_page);
            page_table_entry_t active_page = pgt_it->second;

            pgt_it = profile_tables_[cpu_id].find(logical_page);
            page_table_entry_t profile_page = pgt_it->second;*/

            page_table_entry_t active_page = *(it->active_page_);
            page_table_entry_t profile_page = *(it->profile_page_);

            unsigned order_key;
            if (READ == access_priority_) {
                order_key = profile_page.read_access_count_ - active_page.read_access_count_;
            }
            if (WRITE == access_priority_) {
                order_key = profile_page.write_access_count_ - active_page.write_access_count_;
            }
            if (TOTAL == access_priority_) {
                order_key = profile_page.read_access_count_ + profile_page.write_access_count_ -
                            active_page.read_access_count_ - active_page.write_access_count_;
            }

            if ((false == victim_candidate_found) || (order_key < lowest_order_key)) {
                victim_page_candidate = it;
                lowest_order_key = order_key;
                victim_candidate_found = true;
            }
        }

        victim_page = victim_page_candidate->physical_page_;
        allocated_pages_[page_color].erase(victim_page_candidate);
        allocated_pages_count_--;
    }

    reverse_page_table_t::iterator i = reverse_page_table_.find(victim_page);
    assert(i != reverse_page_table_.end());

    page_table_t::iterator j = page_tables_[(i->second).first].find((i->second).second);
    Log::debug3() << "victimizing page - cpu_id: " << (i->second).first << ", logical_addr: "
                  << (i->second).second << ", physical_addr: " << victim_page;
    j->second.status_ = page_table_entry_t::status_t::CLEARING;
    if (is_in_fast_memory(victim_page)) {
        fast_memory_read_count_ -= j->second.read_access_count_;
        fast_memory_write_count_ -= j->second.write_access_count_;
    }
    j->second.read_access_count_ = 0;
    j->second.write_access_count_ = 0;
    j->second.timestamp_ = sim_->get_clock();

    clear_count_++;
    tlbs_[(i->second).first]->clear_page(victim_page);
}


inline
engine::addr_t* MMU::allocate_page(unsigned cpu_id, engine::addr_t logical_addr)
{
    engine::addr_t page_color = (logical_addr & color_mask_) >> color_offset_;
    engine::addr_t logical_page = logical_addr & (~(page_size_ - 1));

    if ((allocation_policy_ == "dynamic_1") ||
            (allocation_policy_ == "dynamic_2") ||
            (allocation_policy_ == "dynamic_3")) {
        if (empty_pages_[page_color].size() == 0) {
            victimize_page(page_color);
            MMU::mmu_key_t mmu_key = std::make_pair(cpu_id, logical_addr);
            pending_translations_[page_color].emplace_back(mmu_key);

            return NULL;
        } else {
            engine::addr_t physical_page = empty_pages_[page_color].front();
            empty_pages_[page_color].pop_front();

            if (empty_pages_[page_color].size() < empty_page_threshold_)
                victimize_page(page_color);

            allocated_pages_[page_color].emplace_back(allocated_page_t(physical_page));
            allocated_pages_count_++;

            page_table_entry_t page_table_entry(physical_page);
            page_table_entry.page_list_iterator_ = --allocated_pages_[page_color].end();

            page_tables_[cpu_id].insert(std::pair<engine::addr_t, page_table_entry_t>(logical_page, page_table_entry));
            allocated_pages_[page_color].back().active_page_ = &(page_tables_[cpu_id].find(logical_page)->second);
            allocated_pages_[page_color].back().profile_page_ = &(profile_tables_[cpu_id].find(logical_page)->second);

            MMU::mmu_key_t mmu_key = std::make_pair(cpu_id, logical_page);
            reverse_page_table_.insert(std::pair<engine::addr_t, mmu_key_t>(physical_page, mmu_key));
            total_footprint_ += page_size_;

            Log::debug3() << "allocating page - cpu_id: " << cpu_id << ", logical_addr: " << logical_page
                          << ", physical_addr: " << physical_page;

            return new engine::addr_t(physical_page);
        }
    } else {
        engine::addr_t physical_page = next_free_physical_page_[page_color];
        next_free_physical_page_[page_color] += page_size_ * color_count_;

        MMU::mmu_key_t mmu_key = std::make_pair(cpu_id, logical_page);

        page_tables_[cpu_id].insert(std::pair<engine::addr_t,
                                    page_table_entry_t>(logical_page, page_table_entry_t(physical_page)));
        reverse_page_table_.insert(std::pair<engine::addr_t,
                                   mmu_key_t>(physical_page, mmu_key));
        // access_table_.insert(std::pair<mmu_key_t, unsigned>(mmu_key, 0));

        total_footprint_ += page_size_;

        return new engine::addr_t(physical_page);
    }
}

inline
void MMU::notify_access(Request &req)
{
    engine::addr_t accessed_physical_page = req.get_tag() & (~(page_size_ - 1));
    reverse_page_table_t::iterator i = reverse_page_table_.find(accessed_physical_page);
    if (i == reverse_page_table_.end()) {
        return;
    }

    // MMU::mmu_key_t mmu_key = i->second;
    page_table_t::iterator j = page_tables_[(i->second).first].find((i->second).second);

    if (j->second.status_ == page_table_entry_t::status_t::ALLOCATED) {
        if (req.get_op() == Request::op_t::READ) {
            j->second.read_access_count_++;
        }

        if (req.get_op() == Request::op_t::WRITE) {
            j->second.write_access_count_++;
        }

        // j->second.last_access_ = sim_->get_clock();

        engine::addr_t page_color = (req.get_tag() & color_mask_) >> color_offset_;
        if (is_in_fast_memory(accessed_physical_page)) {
            if ((allocation_policy_ == "dynamic_1") ||
                    (allocation_policy_ == "dynamic_2") ||
                    (allocation_policy_ == "dynamic_3")) {
                if (((req.get_op() == Request::op_t::READ) && (access_priority_ != WRITE)) ||
                        ((req.get_op() == Request::op_t::WRITE) && (access_priority_ != READ))) {
                    allocated_pages_[page_color].splice(allocated_pages_[page_color].end(),
                                                        allocated_pages_[page_color],
                                                        j->second.page_list_iterator_);
                }
            }

            if (req.get_op() == Request::op_t::READ)
                fast_memory_read_count_++;
            if (req.get_op() == Request::op_t::WRITE)
                fast_memory_write_count_++;
        } else {
            uint64_t page_access_count = j->second.read_access_count_ + j->second.write_access_count_;
            uint64_t fast_memory_access_count = fast_memory_read_count_ + fast_memory_write_count_;
            if (access_priority_ == WRITE) {
                page_access_count -= j->second.read_access_count_;
                fast_memory_access_count -= fast_memory_read_count_;
            }
            if (access_priority_ == READ) {
                page_access_count -= j->second.write_access_count_;
                fast_memory_access_count -= fast_memory_write_count_;
            }

            Log::debug3() << page_access_count << ":" << fast_memory_access_count << ":" << allocated_pages_count_
                          << ":" << sim_->get_clock();

            if ((allocation_policy_ == "dynamic_3") &&
                    (page_access_count > backward_migration_factor_ *
                        (fast_memory_access_count / allocated_pages_count_))) {
                j->second.status_ = page_table_entry_t::CLEARING;
                j->second.timestamp_ = sim_->get_clock();
                clear_count_++;
                tlbs_[(i->second).first]->clear_page(j->second.physical_address_);
            }
        }
    }
}

inline
void MMU::dump_access_table(const std::string &output_filename)
{
    std::fstream outputFile(output_filename, std::ios::out | std::ios::binary);
    for (unsigned cpu_id = 0; cpu_id < page_tables_.size(); cpu_id++)
        for (page_table_t::iterator i = page_tables_[cpu_id].begin(); i != page_tables_[cpu_id].end(); ++i) {
            outputFile.write(reinterpret_cast<char*>(&cpu_id), sizeof(unsigned));
            outputFile.write(reinterpret_cast<const char*>(&(i->first)), sizeof(engine::addr_t));
            outputFile.write(reinterpret_cast<char*>(&(i->second).read_access_count_), sizeof(unsigned));
            outputFile.write(reinterpret_cast<char*>(&(i->second).write_access_count_), sizeof(unsigned));
        }
    outputFile.close();
}

inline
std::size_t MMU::get_page_size()
{
    return page_size_;
}

inline
void MMU::register_tlb(ITLB* tlb)
{
    page_table_t page_table;
    tlbs_.push_back(tlb);
    page_tables_.push_back(page_table);

    page_table_t profile_table;
    profile_tables_.push_back(profile_table);
}

inline
void MMU::register_dma(IDMA* dma)
{
    dma_ = dma;
}

inline
void MMU::register_dram_size(std::size_t size)
{
    if (!dram_size_registered_) {
        dram_size_registered_ = true;
        fast_mem_size_ = size;

        if ((allocation_policy_ == "dynamic_1") ||
                (allocation_policy_ == "dynamic_2") ||
                (allocation_policy_ == "dynamic_3")) {
            engine::addr_t page_addr = 0;
            while (page_addr < fast_mem_size_) {
                engine::addr_t page_color = (page_addr & color_mask_) >> color_offset_;
                empty_pages_[page_color].emplace_back(page_addr);
                page_addr += page_size_;
            }
            for (unsigned i = 0; i < color_count_; i++)
                next_free_physical_page_[i] = fast_mem_size_ + (i << color_offset_);
        }
    }
}

inline
bool MMU::is_in_fast_memory(engine::addr_t physical_addr)
{
    return (physical_addr < fast_mem_size_);
}

inline
void MMU::load_access_profile()
{
    bool do_allocate = ((allocation_policy_ == "profile_total") || (allocation_policy_ == "profile_write"));
    if (config_.key_exists("input-access-profile")) {
        std::fstream inputFile(config_.get_value<std::string>("input-access-profile"), std::ios::in | std::ios::binary);

        typedef std::multimap<unsigned, mmu_key_t> reverse_access_table_t;
        reverse_access_table_t reverse_access_table;

        unsigned cpu_id;
        engine::addr_t logical_page;
        unsigned read_access_count;
        unsigned write_access_count;
        while (true) {
            inputFile.read(reinterpret_cast<char*>(&cpu_id), sizeof(unsigned));
            if (inputFile.eof()) {
                break;
            }
            inputFile.read(reinterpret_cast<char*>(&logical_page), sizeof(engine::addr_t));
            inputFile.read(reinterpret_cast<char*>(&read_access_count), sizeof(unsigned));
            inputFile.read(reinterpret_cast<char*>(&write_access_count), sizeof(unsigned));

            page_table_entry_t profile_table_entry(logical_page);
            profile_table_entry.read_access_count_ = read_access_count;
            profile_table_entry.write_access_count_ = write_access_count;
            profile_tables_[cpu_id].insert(std::pair<engine::addr_t, page_table_entry_t>(logical_page,
                                                                                         profile_table_entry));

            if (do_allocate) {
                MMU::mmu_key_t mmu_key = std::make_pair(cpu_id, logical_page);
                unsigned order_key;
                if (READ == access_priority_) order_key = read_access_count;
                if (WRITE == access_priority_) order_key = write_access_count;
                if (TOTAL == access_priority_) order_key = read_access_count + write_access_count;

                reverse_access_table.insert(std::pair<unsigned, mmu_key_t>(order_key, mmu_key));
            }
        }

        if (do_allocate) {
            reverse_access_table_t::reverse_iterator i = reverse_access_table.rbegin();
            for (; i != reverse_access_table.rend(); ++i) {
                this->allocate_page((i->second).first, (i->second).second);
            }
        }
    }
}

inline
void MMU::notify_clear_complete(unsigned cpu_id, engine::addr_t &physical_page, uint64_t ip)
{
    engine::addr_t page_color = (physical_page & color_mask_) >> color_offset_;

    reverse_page_table_t::iterator i = reverse_page_table_.find(physical_page);
    MMU::mmu_key_t mmu_key = i->second;
    page_table_t::iterator j = page_tables_[mmu_key.first].find(mmu_key.second);

    cumulative_clear_time_ += sim_->get_clock() - j->second.timestamp_;

    if (is_in_fast_memory(physical_page)) {
        engine::addr_t free_physical_page = next_free_physical_page_[page_color];
        next_free_physical_page_[page_color] += page_size_ * color_count_;
        j->second.status_ = page_table_entry_t::MIGRATING;
        j->second.timestamp_ = sim_->get_clock();
        forward_migration_count_++;
        dma_->memcpy(physical_page, free_physical_page, page_size_, ip);
    } else {
        if (empty_pages_[page_color].size() > 0) {
            engine::addr_t free_physical_page = empty_pages_[page_color].front();
            empty_pages_[page_color].pop_front();
            if (empty_pages_[page_color].size() < empty_page_threshold_)
                victimize_page(page_color);

            j->second.status_ = page_table_entry_t::MIGRATING;
            j->second.timestamp_ = sim_->get_clock();
            backward_migration_count_++;
            dma_->memcpy(physical_page, free_physical_page, page_size_, ip);
        } else {
            victimize_page(page_color);
            pending_translations_[page_color].emplace_back(mmu_key);
        }
    }
}

inline
void MMU::notify_migration_complete(engine::addr_t &source_addr, engine::addr_t &destination_addr, std::size_t &size, uint64_t ip)
{
    engine::addr_t page_color = (source_addr & color_mask_) >> color_offset_;

    reverse_page_table_t::iterator i = reverse_page_table_.find(source_addr);
    MMU::mmu_key_t mmu_key = i->second;
    page_table_t::iterator j = page_tables_[mmu_key.first].find(mmu_key.second);
    j->second.physical_address_ = destination_addr;

    cumulative_migration_time_ += sim_->get_clock() - j->second.timestamp_;
    j->second.status_ = page_table_entry_t::status_t::ALLOCATED;
    j->second.read_access_count_ = 0;
    j->second.write_access_count_ = 0;
    if (is_in_fast_memory(destination_addr)) {
        allocated_page_t allocated_page(destination_addr);
        allocated_page.active_page_ = &(j->second);
        allocated_page.profile_page_ = &(profile_tables_[mmu_key.first].find(mmu_key.second)->second);

        allocated_pages_[page_color].emplace_back(allocated_page);
        allocated_pages_count_++;
        j->second.page_list_iterator_ = --allocated_pages_[page_color].end();
    }

    reverse_page_table_.erase(i);
    reverse_page_table_.insert(std::pair<engine::addr_t, mmu_key_t>(destination_addr, mmu_key));

    typename std::list<mmu_key_t>::iterator it = pending_translations_[page_color].begin();
    while (it != pending_translations_[page_color].end()) {
        engine::addr_t logical_page = it->second & (~(page_size_ - 1));
        if ((it->first == mmu_key.first) && (logical_page == mmu_key.second)) {
            pending_translations_[page_color].erase(it++);
        } else {
            it++;
        }
    }

    tlbs_[mmu_key.first]->notify_translation();

    if (is_in_fast_memory(source_addr)) {
        if (pending_translations_[page_color].size() == 0) {
            empty_pages_[page_color].emplace_back(source_addr);
        } else {
            unsigned cpu_id = pending_translations_[page_color].front().first;
            engine::addr_t logical_addr = pending_translations_[page_color].front().second;
            engine::addr_t logical_page = logical_addr & (~(page_size_ - 1));
            pending_translations_[page_color].pop_front();

            typename page_table_t::iterator pgt_it = page_tables_[cpu_id].find(logical_page);
            if (pgt_it == page_tables_[cpu_id].end()) {
                // This is a migration from fast to slow memory
                // caused by a newly allocated page in fast memory

                engine::addr_t physical_page = source_addr;

                allocated_pages_[page_color].emplace_back(allocated_page_t(physical_page));
                allocated_pages_count_++;

                page_table_entry_t page_table_entry(physical_page);
                page_table_entry.page_list_iterator_ = --allocated_pages_[page_color].end();

                page_tables_[cpu_id].insert(std::pair<engine::addr_t, page_table_entry_t>(logical_page,
                                                                                          page_table_entry));
                allocated_page_t &page = allocated_pages_[page_color].back();
                page.active_page_ = &(page_tables_[cpu_id].find(logical_page)->second);
                page.profile_page_ = &(profile_tables_[cpu_id].find(logical_page)->second);

                MMU::mmu_key_t mmu_key = std::make_pair(cpu_id, logical_page);
                reverse_page_table_.insert(std::pair<engine::addr_t, mmu_key_t>(physical_page, mmu_key));
                total_footprint_ += page_size_;

                tlbs_[cpu_id]->notify_translation();
            } else {
                // This is a migration from fast to slow memory in order to
                // make space for other page that requested to be migrated
                // from slow memory to fast memory. Now this second migration
                // can begin.

                engine::addr_t old_physical_address = pgt_it->second.physical_address_;
                pgt_it->second.physical_address_ = source_addr;
                // pgt_it->second.page_list_iterator_ == --allocated_pages_[page_color].end();
                pgt_it->second.status_ = page_table_entry_t::MIGRATING;
                pgt_it->second.timestamp_ = sim_->get_clock();
                backward_migration_count_++;
                dma_->memcpy(old_physical_address, source_addr, page_size_, ip);
            }
        }
    }
}

}  // namespace memory
}  // namespace logic
}  // namespace sim


#endif  // CORE_LOGIC_MEMORY_MMU_IMPL_H_
