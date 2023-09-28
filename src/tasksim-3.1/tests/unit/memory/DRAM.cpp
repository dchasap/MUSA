#include "core/engine/types.h"
#include "core/logic/memory/DRAM.h"

#include "gtest/gtest.h"

using namespace sim;
using namespace sim::logic::memory;

class IndexTest : public DRAM {
public:
    inline
    IndexTest(const DRAM::range_t &range) :
        DRAM(range, range, range, range) {};

    inline
    unsigned get_index(engine::addr_t addr) {
        return DRAM::get_index(addr, this->bank_range_);
    }
};

TEST(DRAMTest, SingleIndex)
{
    unsigned long magic = 0xdab1cafe;
    for(unsigned j = 1; j < sizeof(unsigned long) * 4; j++) {
        unsigned long value = (1UL << j) - 1;
        unsigned long target = magic & value;

        for(unsigned i = 0; i < (sizeof(unsigned long) * 8) - j + 1; ++i) {
            IndexTest test(DRAM::range_t((value << i)));
            ASSERT_EQ(test.get_index(magic << i), target);
        }
    }
}


TEST(DRAMTest, DualIndex)
{
    unsigned long magic = 0xdab1cafe;
    for(unsigned long low = 9; low < sizeof(unsigned long) * 2; low++) {
        unsigned long low_value = (1UL << low) - 1;
        unsigned long low_target = magic & low_value;
        for(unsigned long high = 1; high < sizeof(unsigned long) * 2; high++) {
            unsigned long high_value = ((1UL << high) - 1) << 16;
            unsigned long high_target = magic & high_value;
            unsigned target = low_target | (high_target >> (16 - low));
            for(unsigned i = 0; i < sizeof(unsigned long) * 8 - 16 - high + 1; ++i) {
                IndexTest test(DRAM::range_t((high_value | low_value) << i));
                ASSERT_EQ(test.get_index(magic << i), target);
            }
        }
    }
}
