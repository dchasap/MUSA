#include "core/logic/memory/CacheSet.h"
#include "core/logic/memory/policy/Policy.h"
#include "gtest/gtest.h"

#include <set>

class CacheLineTest : public sim::logic::memory::CacheLineBase {
public:
    bool value_;
    CacheLineTest() : CacheLineBase(), value_(false) {}
    explicit CacheLineTest(bool value) : CacheLineBase(), value_(value) {}
};


class CacheSetTest : public testing::Test {
public:
    typedef sim::logic::memory::CacheSet<unsigned, CacheLineTest> CacheSet;
    typedef typename sim::logic::memory::policy::Policy<CacheSet> Policy;
    typedef Policy::policy_ptr_t policy_ptr_t;

    std::size_t cache_size_;
    CacheSet *cache_set_;
    sim::engine::Config config_;
    policy_ptr_t policy_;

    virtual void SetUp() {
        cache_size_ = 1024;
        cache_set_ = new CacheSet(cache_size_);
        config_.add_value("LRU", "type");
        policy_ = Policy::Create(cache_set_, cache_size_, config_);
        cache_set_->set_policy(std::move(policy_));
    }

    virtual void TearDown() {
        delete cache_set_;
    }
};


TEST_F(CacheSetTest, Insertion)
{
    for(unsigned i = 0; i < cache_size_; ++i) {
        ASSERT_TRUE(cache_set_->insert(i, CacheLineTest(true)));
    }

    for(unsigned i = 0; i < cache_size_; i++) {
        ASSERT_FALSE(cache_set_->insert(i, CacheLineTest(false)));
    }
}


TEST_F(CacheSetTest, Lookup)
{
    std::set<unsigned> test_set;
    for(unsigned i = 0; i < cache_size_; ++i) {
        unsigned n = rand();
        test_set.insert(n);
        ASSERT_TRUE(cache_set_->insert(n, CacheLineTest(true)));
    }

    std::set<unsigned>::const_iterator v;
    for(v = test_set.begin(); v != test_set.end(); ++v) {
        ASSERT_TRUE(cache_set_->find(*v) != cache_set_->end());
    }

    for(unsigned i = 0; i < cache_size_; ++i) {
        unsigned n = rand();
        if(test_set.find(n) == test_set.end()) ASSERT_TRUE(cache_set_->find(n) == cache_set_->end());
        else ASSERT_TRUE(cache_set_->find(n) != cache_set_->end());
    }
}


TEST_F(CacheSetTest, Removal)
{
    for(unsigned i = 0; i < cache_size_; ++i) {
        ASSERT_TRUE(cache_set_->insert(i, CacheLineTest(true)));
    }

    for(unsigned i = 0; i < cache_size_; i++) {
        ASSERT_TRUE(cache_set_->find(i) != cache_set_->end());
    }

    for(unsigned i = 0; i < cache_size_; i++) {
        ASSERT_TRUE(cache_set_->remove(i));
    }

    for(unsigned i = 0; i < cache_size_; i++) {
        ASSERT_TRUE(cache_set_->find(i) == cache_set_->end());
    }
}


TEST_F(CacheSetTest, Fill)
{
    for(unsigned i = 0; i < cache_size_; ++i) {
        ASSERT_TRUE(cache_set_->insert(i, CacheLineTest(true)));
    }

    for(unsigned i = 0; i < cache_size_; ++i) {
        ASSERT_FALSE(cache_set_->insert(i + cache_size_, CacheLineTest(true)));
    }
}
