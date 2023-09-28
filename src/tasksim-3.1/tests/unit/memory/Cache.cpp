#include "core/engine/types.h"
#include "core/logic/memory/Cache.h"
#include "core/logic/memory/CacheSet.h"

#include "gtest/gtest.h"

using namespace sim::logic::memory;

class CacheLineTest : public CacheLineBase {
public:
    bool value_;
    CacheLineTest() : CacheLineBase(), value_(false) {}
    explicit CacheLineTest(bool value) : CacheLineBase(), value_(value) {}
};

class CacheTest : public testing::Test {
protected:
    sim::engine::Simulator *sim_;
    sim::logic::memory::Cache<CacheSet<unsigned, CacheLineTest>> *cache;
    virtual void SetUp() {
        sim::engine::Config policy_conf;
        sim::engine::Config config_;

        policy_conf.add_value("LRU", "type");
        policy_conf.add_value(4, "size");

        // This is necessary to run these tests with --enable-paraver
        config_.add_value("/tmp/trace", "trace_base_name", "Paraver");
        config_.add_value("/dev/null", "pcf_filename", "Paraver");
        config_.add_value(1000, "buffer_size", "Paraver");
        config_.add_value("", "modules", "Paraver");
        config_.add_value(-1, "hardware_sampling_interval", "Paraver");
        config_.add_value("DISABLED", "sampling_policy", "Paraver");
        config_.add_value(100000,"deadlock_detection_interval", "Global");
        sim_ = new sim::engine::Simulator(config_);
        cache = new Cache<CacheSet<unsigned, CacheLineTest>>(*sim_, 1024, 4, 128, false, policy_conf);
    }

    virtual void TearDown() {
        delete(cache);
        delete(sim_);
    }
};

TEST_F(CacheTest, Insertion)
{
    ASSERT_EQ(1024u / 4u, cache->get_num_sets());
    ASSERT_EQ(4u, cache->get_set_size());

    for(unsigned n = 0; n < 1024; ++n) {
        ASSERT_TRUE(cache->insert(128 * n, CacheLineTest()));
        ASSERT_TRUE(cache->access(128 * n) != NULL);
    }
}


TEST_F(CacheTest, Eviction)
{
    for(unsigned n = 0; n < 1024; ++n) {
        ASSERT_TRUE(cache->insert(128 * n, CacheLineTest(true)));
    }

    for(unsigned n = 1024; n < 2048; ++n) {
        ASSERT_FALSE(cache->insert(128 *n, CacheLineTest()));
        ASSERT_TRUE(cache->evict(128 * n).second.value_);
        ASSERT_TRUE(cache->insert(128 * n, CacheLineTest()));
    }

    for(unsigned n = 0; n < 1024; ++n) {
        ASSERT_TRUE(cache->access(128 * n) == NULL);
    }
}


TEST_F(CacheTest, Update)
{
    /* Fill the cache */
    for(unsigned n = 0; n < 1024; ++n) {
        ASSERT_TRUE(cache->insert(128 * n, CacheLineTest(true)));
    }

    /* Access the first quarter of the cache */
    for(unsigned n = 0; n < 1024 / 4; ++n) {
        ASSERT_TRUE(cache->access(128 * n) != NULL);
    }

    /* Insert 1/4 of the cache */
    for(unsigned n = 0; n < 1024 / 4; ++n) {
        ASSERT_FALSE(cache->insert(128 * (1024 + n), CacheLineTest()));
        ASSERT_TRUE(cache->evict(128 * (1024 + n)).second.value_);
        ASSERT_TRUE(cache->insert(128 * (1024 + n), CacheLineTest()));
    }

    /* Check that the first 1/4 is still in the cache */
    for(unsigned n = 0; n < 1024 / 4; ++n) {
        ASSERT_TRUE(cache->access(128 * n) != NULL);
    }
}
