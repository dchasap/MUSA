//
// Copyright (C) 2009-2012
// Barcelona Supercomputing Center - Centro Nacional de Supercomputacion
// Universitat Politecnica de Catalunya
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//

#include "core/engine/types.h"
#include "core/engine/Config.h"
#include "core/engine/Simulator.h"
#include "core/modules/cpu/models/ROA.h"
#include "core/modules/cpu/Memory.h"

#include "gtest/gtest.h"

using sim::cpu::timing_models::ROB;
using sim::cpu::timing_models::OutputBuffer;
using sim::cpu::timing_models::LDQueue;
using sim::logic::coherence::single::Ack;
using sim::logic::coherence::single::Request;
using sim::trace::mitem_t;

// ------------------------------------------------------------------------
// Tests for the "ROA/ROB" classes.
// ------------------------------------------------------------------------
typedef sim::logic::coherence::single::Request MemoryAccess;
typedef OutputBuffer<MemoryAccess*> OutputBuffer_t;
typedef ROB<OutputBuffer<MemoryAccess*>> ROB_t;


class ROAInput {
private:
    std::queue<mitem_t> items_;
public:
    ROAInput(): items_() {}
    void push(const mitem_t& i) { items_.push(i); }
    void get_mem_item(sim::engine::EventBuffer<sim::trace::mitem_t> &mem_in_) {
        if (items_.empty()){
            return;
        }
        mem_in_.push_event(items_.front());
        items_.pop();
        return;
    }
};

class ROATestAlt : public testing::Test {
protected:
    sim::engine::Config config_;
    sim::engine::Simulator *sim_;
    ROAInput *roa_in_;
    sim::cpu::timing_models::ROA *roa_;
    sim::engine::EventBuffer<sim::trace::mitem_t> *mem_in_;

    virtual void SetUp() {
        roa_in_ = new ROAInput();
        mem_in_ = new sim::engine::EventBuffer<sim::trace::mitem_t>(std::bind(&ROAInput::get_mem_item, roa_in_, std::placeholders::_1), 1000, 1);
        config_.add_value(2, "commit_rate", "MemCPU");
        config_.add_value(10, "rob_size", "MemCPU");
        config_.add_value(10, "out_buff_size", "MemCPU");
        config_.add_value(64, "request-size", "MemCPU");
        config_.add_value("/tmp/trace", "trace_base_name", "Paraver");
        config_.add_value("/dev/null", "pcf_filename", "Paraver");
        config_.add_value(1000, "buffer_size", "Paraver");
        config_.add_value("", "modules", "Paraver");
        config_.add_value(-1, "hardware_sampling_interval", "Paraver");
        config_.add_value("DISABLED", "sampling_policy", "Paraver");
        config_.add_value(100000,"deadlock_detection_interval", "Global");
        sim_ = new sim::engine::Simulator(config_);
        roa_ = new sim::cpu::timing_models::ROA(0, *sim_, sim::engine::Config(config_, "MemCPU", 0), &mem_in_);
    }

    virtual void TearDown() {
        delete(roa_);
        delete(mem_in_);
        delete(sim_);
        delete(roa_in_);
    }
};



TEST_F(ROATestAlt, ROAIssueTwoItems)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 5));
    roa_in_->push(mitem_t(0xF8, 0, mitem_t::LOAD, 0, 8, 3));

    sim::engine::cycles_t warp;

    // Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp); //2nd LOAD fits, issue next cycle
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_size(), 8u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::READ);

    Ack ack1(ma1);
    roa_->ack_mem_response(ack1);

    // Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(3u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0xF8u);
    ASSERT_EQ(ma2.get_size(), 8u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack2(ma2);
    roa_->ack_mem_response(ack2);

    // Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);
}

TEST_F(ROATestAlt, ROAOutBuffFull)
{
    // This is done so we can have different configuration parameters
    // If this configuration is used more times it should be separated into another file without all the
    // reconfiguration.
    // We undo the SetUp:
    TearDown();
    // And redo it:
    roa_in_ = new ROAInput();
    mem_in_ = new sim::engine::EventBuffer<sim::trace::mitem_t>(std::bind(&ROAInput::get_mem_item, roa_in_, std::placeholders::_1), 1, 1);
    sim::engine::Config config_alt;
    config_alt.add_value(1, "commit_rate", "MemCPU");
    config_alt.add_value(10, "rob_size", "MemCPU");
    config_alt.add_value(1, "out_buff_size", "MemCPU");
    config_alt.add_value(64, "request-size", "MemCPU");
    config_alt.add_value("/tmp/trace", "trace_base_name", "Paraver");
    config_alt.add_value("/dev/null", "pcf_filename", "Paraver");
    config_alt.add_value(1000, "buffer_size", "Paraver");
    config_alt.add_value("", "modules", "Paraver");
    config_alt.add_value(-1, "hardware_sampling_interval", "Paraver");
    config_alt.add_value("DISABLED", "sampling_policy", "Paraver");
    config_alt.add_value(100000,"deadlock_detection_interval", "Global");
    sim_ = new sim::engine::Simulator(config_alt);
    roa_ = new sim::cpu::timing_models::ROA(0, *sim_, sim::engine::Config(config_alt, "MemCPU", 0), &mem_in_);

    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 1));
    roa_in_->push(mitem_t(0x20, 0, mitem_t::LOAD, 0, 8, 4));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(3u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    //don't release store (later)

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);

    sim_->testing_only_add_cycles(sim::engine::cycles_t(25));

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request(); //release

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_data_size(), 8u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::WRITE);

    //Cycle 30
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x20u);
    ASSERT_EQ(ma2.get_size(), 8u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack2(ma2);
    roa_->ack_mem_response(ack2);
};

TEST_F(ROATestAlt, ROAItemSameSizeAsROBCycleByCycle)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    roa_in_->push(mitem_t(0x90, 0, mitem_t::STORE, 0, 4, 10));

    sim::engine::cycles_t warp;

    // Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(2u, warp);
    sim_->testing_only_add_cycles(sim::engine::cycles_t(1));

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(sim::engine::cycles_t(1));

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 3
    warp = roa_->cycle();
    ASSERT_EQ(5u, warp);
    sim_->testing_only_add_cycles(sim::engine::cycles_t(1));

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_data_size(), 8u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::WRITE);

    // Cycle 4-7
    unsigned exp_warp = 4;
    do {

        warp = roa_->cycle();
        ASSERT_EQ(exp_warp, warp);

        sim_->testing_only_add_cycles(sim::engine::cycles_t(1));
        --exp_warp;
        ASSERT_FALSE(roa_->has_mem_request());
    } while(exp_warp > 0);

    // Cycle 8

    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(sim::engine::cycles_t(1));
    // commit: ROB retired 2 ins, total 0
    // issue: 10 ins from 2nd STORE issued
    // ROB ends up with 10 entries (full), 2nd STORE fits

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x90u);
    ASSERT_EQ(ma2.get_data_size(), 4u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::WRITE);
}

TEST_F(ROATestAlt, ROASameSizeAsROB)
{
    // This test proceeds as the simulation engine, jumping unnecessary cycles

    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    roa_in_->push(mitem_t(0x90, 0, mitem_t::STORE, 0, 4, 10));

    sim::engine::cycles_t warp;

    // Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(2u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 3
    warp = roa_->cycle();
    ASSERT_EQ(5u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_data_size(), 8u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::WRITE);

    // Cycle 8
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x90u);
    ASSERT_EQ(ma2.get_data_size(), 4u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::WRITE);
}

TEST_F(ROATestAlt, ROALongLatencyMiss)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 4));
    roa_in_->push(mitem_t(0x180, 0, mitem_t::STORE, 0, 4, 12));

    sim::engine::cycles_t warp;

    // Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(2u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_size(), 8u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::READ);

    // Cycle 3
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);
    // commit: ROB retired 3 ins, total 3 from LOAD
    // issue: 0 ins issued from STORE, does not fit

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 4
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    // commit: stalled, pending LOAD on top
    // issue: 9 ins from STORE issued (partial - 3 left), does not fit
    // ROB ends up full with 10 entries, 1 from LOAD 9 from 2nd STORE

    ASSERT_FALSE(roa_->has_mem_request());

    sim_->testing_only_add_cycles(sim::engine::cycles_t(300));

    Ack ack1(ma1);
    roa_->ack_mem_response(ack1);

    // Cycle 304

    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);
    // commit: 2 ins retired (1 from LOAD, 1 from STORE), total 8
    // issue: 0 ins from 2nd STORE issued (3 left), does not fit
    // ROB ends up with 8 entries

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 305
    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);
    // commit: 2 ins retired from STORE
    // issue: 3 ins from STORE (0 left), fits
    // ROB ends up with 9 ins

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 309
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);
    // commit: 8 ins from STORE
    // issue: nothing
    // ROB ends up with 1 ins from STORE

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 310
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    // commit: 1 ins from STORE
    // issue: nothing
    // ROB ends up empty

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x180u);
    ASSERT_EQ(ma2.get_data_size(), 4u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::WRITE);
}

TEST_F(ROATestAlt, ROAAllItemsLargerThanROB)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 23));
    roa_in_->push(mitem_t(0xF8, 0, mitem_t::LOAD, 0, 8, 11));

    sim::engine::cycles_t warp;

    // Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(5u, warp);
    sim_->testing_only_add_cycles(warp);
    // ROB includes 10 ins from LOAD (13 left)

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 6
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);
    // ROB includes 10 ins from LOAD (3 left)

    // Cycle 7
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);
    // ROB includes 8 ins from LOAD (3 left)

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 8
    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);
    // ROB includes 9 ins from LOAD, fits

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_size(), 8u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::READ);

    Ack ack1(ma1);
    roa_->ack_mem_response(ack1);

    // Cycle 12
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);
    // ROB includes 1 ins from 1st LOAD
    //  and 9 from 2nd LOAD (2 left)

    ASSERT_FALSE(roa_->has_mem_request());

    // Cycle 13
    warp = roa_->cycle();
    ASSERT_EQ(5u, warp);
    sim_->testing_only_add_cycles(warp);
    // ROB includes 10 ins from 2nd LOAD

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0xF8u);
    ASSERT_EQ(ma2.get_size(), 8u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack2(ma2);
    roa_->ack_mem_response(ack2);

    // Cycle 18
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    // ROB empty

    ASSERT_FALSE(roa_->has_mem_request());
}
