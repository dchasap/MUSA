#include "core/engine/Config.h"
#include "core/logic/memory/MSHR.h"
#include "core/logic/memory/policy/types.h"
#include "core/logic/coherence/single/Message.h"
#include "gtest/gtest.h"

using namespace sim;
using sim::logic::coherence::single::ack_op_t;

class MSHRTest : public testing::Test {
protected:

    /* Cache configurations */
    engine::Config mshr_conf;

    static const engine::addr_t start_addr_;
    static const std::size_t cache_line_size_;
    static const std::size_t word_size_;
    static const std::size_t mshr_size_;

    typedef sim::logic::coherence::single::Message::Request Request;
    typedef sim::logic::memory::MSHR<sim::logic::coherence::single::Message> MSHR;
    MSHR *mshr;


    virtual void SetUp() {
        /* Create the configuration for the MSHR */
        mshr_conf.add_value(mshr_size_, "size");
        mshr = new MSHR(mshr_conf, cache_line_size_);
    }

    virtual void TearDown() {
        if(mshr != NULL) {
            mshr->clear_entries();
            delete mshr;
        }
    }
};

const engine::addr_t MSHRTest::start_addr_ = 0x1000;
const std::size_t MSHRTest::cache_line_size_ = 128;
const std::size_t MSHRTest::word_size_ = 8;
const std::size_t MSHRTest::mshr_size_ = 32;

TEST_F(MSHRTest, Insertion)
{
    for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
        MSHR::entry_t *mshr_entry = mshr->allocate_entry(n, Request());
        ASSERT_NE((void *)NULL, mshr_entry);
        ASSERT_TRUE(mshr_entry->list_.empty());
        mshr_entry->list_.emplace_back(MSHR::subentry_t(0, Request(Request::op_t::READ, n, 0)));
    }

    for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
        MSHR::entry_t *mshr_entry =mshr->lookup_entry(n);
        ASSERT_NE((void *)NULL, mshr_entry);
        ASSERT_FALSE(mshr_entry->list_.empty());
        ASSERT_EQ(n, mshr_entry->tag_);
        ASSERT_EQ(Request::op_t::READ, mshr_entry->list_.front().request_.get_op());
        ASSERT_EQ(n, mshr_entry->list_.front().request_.get_tag());
    }
}

TEST_F(MSHRTest, Removal)
{
    for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
        MSHR::entry_t *mshr_entry =mshr->allocate_entry(n, Request());
        ASSERT_NE((void *)NULL, mshr_entry);
        ASSERT_TRUE(mshr_entry->list_.empty());
        mshr_entry->list_.emplace_back(MSHR::subentry_t(0, Request(Request::op_t::READ, n, 0)));
    }

    for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
        MSHR::entry_t *mshr_entry =mshr->lookup_entry(n);
        ASSERT_NE((void *)NULL, mshr_entry);
        ASSERT_FALSE(mshr_entry->list_.empty());
        ASSERT_EQ(n, mshr_entry->tag_);
        ASSERT_EQ(Request::op_t::READ, mshr_entry->list_.front().request_.get_op());
        ASSERT_EQ(n, mshr_entry->list_.front().request_.get_tag());
        mshr_entry->list_.clear();
        mshr->remove_entry(*mshr_entry);
    }

    for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
        MSHR::entry_t *mshr_entry =mshr->lookup_entry(n);
        ASSERT_EQ((void *)NULL, mshr_entry);
    }
}


TEST_F(MSHRTest, UnalignedAccess)
{

    for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
        MSHR::entry_t *mshr_entry =mshr->allocate_entry(n + word_size_, Request());
        ASSERT_NE((void *)NULL, mshr_entry);
        ASSERT_TRUE(mshr_entry->list_.empty());
        mshr_entry->list_.emplace_back(MSHR::subentry_t(0, Request(Request::op_t::READ, n + word_size_, 0)));
    }


    for(unsigned i = 0; i < (cache_line_size_ / word_size_); ++i) {
        for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
            MSHR::entry_t *mshr_entry =mshr->lookup_entry(n + i * word_size_);
            ASSERT_NE((void *)NULL, mshr_entry);
            ASSERT_FALSE(mshr_entry->list_.empty());
            ASSERT_EQ(n, mshr_entry->tag_);
            ASSERT_EQ(Request::op_t::READ, mshr_entry->list_.front().request_.get_op());
            ASSERT_EQ(n + word_size_, mshr_entry->list_.front().request_.get_tag());
        }
    }
}

TEST_F(MSHRTest, Overflow)
{
    for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
        MSHR::entry_t *mshr_entry =mshr->allocate_entry(n + word_size_, Request());
        ASSERT_NE((void *)NULL, mshr_entry);
        ASSERT_TRUE(mshr_entry->list_.empty());
        mshr_entry->list_.emplace_back(MSHR::subentry_t(0, Request(Request::op_t::READ, n + word_size_, 0)));
    }

    for(unsigned n = 0; n < mshr_size_ * cache_line_size_; n += cache_line_size_) {
        MSHR::entry_t *mshr_entry =mshr->allocate_entry(n + word_size_, Request());
        ASSERT_EQ((void *)NULL, mshr_entry);
    }
}
