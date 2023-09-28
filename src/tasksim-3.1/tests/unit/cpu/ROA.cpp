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
typedef OutputBuffer<MemoryAccess> OutputBuffer_t;
typedef ROB<OutputBuffer_t> ROB_t;


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

class ROATest : public testing::Test {
protected:
    sim::engine::Config config_;
    sim::engine::Simulator *sim_;
    ROAInput *roa_in_;
    sim::cpu::timing_models::ROA *roa_;
    sim::engine::EventBuffer<sim::trace::mitem_t> *mem_in_;
    virtual void SetUp() {
        roa_in_ = new ROAInput();
        mem_in_ = new sim::engine::EventBuffer<sim::trace::mitem_t>(std::bind(&ROAInput::get_mem_item, roa_in_, std::placeholders::_1), 1000, 1);
        config_.add_value(1, "commit_rate", "MemCPU");
        config_.add_value(10, "rob_size", "MemCPU");
        config_.add_value(10, "out_buff_size", "MemCPU");
        config_.add_value(64, "request-size", "MemCPU");
        // This is necessary to run these tests with --enable-paraver
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

TEST_F(ROATest, OutputBufferPushPop)
{
    OutputBuffer_t out(1);

    // MemoryAccessLoad* ma = new MemoryAccessLoad(0x80,128,0x90,0xA0);
    // Source size target address
    MemoryAccess ma(Request::op_t::READ,0x80,128,0x90,0xA0);

    out.push(ma);

    ASSERT_EQ(static_cast<size_t>(1), out.size());
    ASSERT_TRUE(out.full());
    ASSERT_FALSE(out.empty());

    MemoryAccess& ref = out.front();

    ASSERT_EQ(0x80u, ref.get_tag());
    ASSERT_EQ(128u, ref.get_size());
    ASSERT_EQ(0x90u, ref.get_data_size());
    ASSERT_EQ(0xA0u, ref.get_data());
    ASSERT_EQ(Request::op_t::READ, ref.get_op());

    out.pop();

    ASSERT_EQ(0u, out.size());
    ASSERT_FALSE(out.full());
    ASSERT_TRUE(out.empty());

}

TEST_F(ROATest,LDQueueInsertRetrieve)
{
    LDQueue<bool> queue;

    ASSERT_TRUE(queue.insert(0x80,8,true));

    ASSERT_FALSE(queue.retrieve(0x100, 4));

    ASSERT_TRUE(queue.retrieve(0x80, 8));

    ASSERT_FALSE(queue.retrieve(0x80, 8));
}

TEST_F(ROATest,LDQueueMultipleInsertRetrieve)
{
    LDQueue<bool> queue;

    ASSERT_TRUE(queue.insert(0x80,8,true));
    ASSERT_TRUE(queue.insert(0x100,8,true));
    ASSERT_TRUE(queue.insert(0x80,16,true));

    ASSERT_FALSE(queue.retrieve(0x100, 4));

    ASSERT_TRUE(queue.retrieve(0x80, 16));

    ASSERT_TRUE(queue.retrieve(0x100, 8));

    ASSERT_TRUE(queue.retrieve(0x80, 8));

    ASSERT_FALSE(queue.retrieve(0x80, 16));
}


static void clear(OutputBuffer_t& out)
{
    while (not out.empty()) {
        out.pop();
    }
}

TEST_F(ROATest, ROBEmpty)
{
    OutputBuffer_t out(1);
    ROB_t rob(128, 64, out);

    ASSERT_TRUE(rob.empty());
}

TEST_F(ROATest, ROBCapacity)
{
    OutputBuffer_t out(1);
    ROB_t rob(128, 64, out);

    ASSERT_TRUE(rob.capacity() == 128);
}

TEST_F(ROATest, ROBLoadInsertion)
{
    OutputBuffer_t out(1);
    ROB_t rob(128, 64, out);
    mitem_t item(0x80, 0, mitem_t::LOAD, 0, 8, 1);

    ASSERT_TRUE(rob.insert(item) != NULL);
    ASSERT_FALSE(rob.empty());
    ASSERT_FALSE(out.empty());

    clear(out);
}

TEST_F(ROATest, ROBStoreInsertion)
{
    OutputBuffer_t out(1);
    ROB_t rob(128, 64, out);
    mitem_t item(0x80, 0, mitem_t::STORE, 0, 8, 1);

    ASSERT_TRUE(rob.insert(item) == NULL);
    ASSERT_FALSE(rob.empty());
    ASSERT_TRUE(out.empty());

    clear(out);
}

TEST_F(ROATest, ROBFreeEntries)
{
    OutputBuffer_t out(10);
    ROB_t rob(10, 64, out);

    mitem_t item(0x80, 0, mitem_t::LOAD, 0, 8, 5);
    rob.insert(item);

    ASSERT_EQ(5u, rob.free_entries());

    mitem_t item2(0x90, 0, mitem_t::LOAD, 0, 8, 5);
    rob.insert(item2);

    ASSERT_EQ(0u, rob.free_entries());

    clear(out);
}

TEST_F(ROATest, ROBInstrToFirst1)
{
    OutputBuffer_t out(10);
    ROB_t rob(10, 64, out);

    mitem_t item(0x80, 0, mitem_t::LOAD, 0, 8, 3);
    rob.insert(item);

    ASSERT_EQ(3u, rob.get_ins_to_first_waiting_item());

    mitem_t item2(0x90, 0, mitem_t::LOAD, 0, 8, 5);
    rob.insert(item);

    ASSERT_EQ(3u, rob.get_ins_to_first_waiting_item());

    clear(out);
}

TEST_F(ROATest, ROBInstrToFirst2)
{
    OutputBuffer_t out(10);
    ROB_t rob(10, 64, out);

    mitem_t item(0x80, 0, mitem_t::STORE, 0, 8, 3);
    rob.insert(item);

    ASSERT_EQ(3u, rob.get_ins_to_first_waiting_item());

    clear(out);
}

TEST_F(ROATest, ROBAck)
{
    OutputBuffer_t out(10);
    ROB_t rob(10, 64, out);

    mitem_t item(0x80, 0, mitem_t::LOAD, 0, 8, 3);
    ROB_t::elem_t& e = *rob.insert(item);

    mitem_t item2(0x90, 0, mitem_t::LOAD, 0, 8, 5);
    rob.insert(item);

    ASSERT_EQ(3u, rob.get_ins_to_first_waiting_item());

    rob.ack(e);

    clear(out);
}

TEST_F(ROATest, ROBRetire1)
{
    OutputBuffer_t out(10);
    ROB_t rob(10, 64, out);

    mitem_t item(0x80, 0, mitem_t::STORE, 0, 8, 3);
    rob.insert(item);

    mitem_t item2(0x90, 0, mitem_t::STORE, 0, 8, 7);
    rob.insert(item2);

    ASSERT_EQ(rob.free_entries(), 0u);

    rob.retire(5);

    ASSERT_EQ(rob.free_entries(), 5u);

    rob.retire(4);

    ASSERT_EQ(rob.free_entries(), 9u);

    rob.retire(1);

    ASSERT_EQ(rob.free_entries(), 10u);
    ASSERT_TRUE(rob.empty());

    clear(out);
}

TEST_F(ROATest, ROBRetire2)
{
    OutputBuffer_t out(10);
    ROB_t rob(10, 64, out);

    mitem_t item(0x80, 0, mitem_t::LOAD, 0, 8, 4);
    ROB_t::elem_t& e = *rob.insert(item);

    ASSERT_FALSE(rob.empty());
    ASSERT_EQ(rob.free_entries(), 6u);

    rob.retire(5);

    ASSERT_EQ(rob.free_entries(), 9u);

    mitem_t item2(0x90, 0, mitem_t::STORE, 0, 8, 3);
    rob.insert(item2);

    ASSERT_EQ(rob.free_entries(), 6u);

    rob.retire(5);

    ASSERT_EQ(rob.free_entries(), 6u);

    rob.ack(e);
    rob.retire(5);

    ASSERT_EQ(rob.free_entries(), 10u);

    rob.retire(5);

    ASSERT_EQ(rob.free_entries(), 10u);
    ASSERT_TRUE(rob.empty());

    clear(out);
}



TEST_F(ROATest, ROALoadInsertion)
{
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 4));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_size(), 8u);
    ASSERT_EQ(ma.get_op(), Request::op_t::READ);

    Ack ack(ma);
    roa_->ack_mem_response(ack);
};

TEST_F(ROATest, ROAUnalignedLoadInsertion)
{
    roa_in_->push(mitem_t(0xA0, 0, mitem_t::LOAD, 0, 64, 4));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0xA0u);
    ASSERT_EQ(ma.get_size(), 32u);
    ASSERT_EQ(ma.get_op(), Request::op_t::READ);
    Ack ack(ma);
    roa_->ack_mem_response(ack);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0xC0u);
    ASSERT_EQ(ma2.get_size(), 32u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack2(ma2);
    roa_->ack_mem_response(ack2);
};

TEST_F(ROATest, ROAiMultiLineLoadInsertion)
{
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 128, 4));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_size(), 64u);
    ASSERT_EQ(ma.get_op(), Request::op_t::READ);

    Ack ack(ma);
    roa_->ack_mem_response(ack);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0xC0u);
    ASSERT_EQ(ma2.get_size(), 64u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack2(ma2);
    roa_->ack_mem_response(ack2);
};

TEST_F(ROATest, ROAStoreInsertion)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));
    roa_in_->push(mitem_t(0x90, 0, mitem_t::STORE, 0, 4, 4));
    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);
    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x90u);
    ASSERT_EQ(ma.get_data_size(), 4u);
    ASSERT_EQ(ma.get_op(), Request::op_t::WRITE);
};

TEST_F(ROATest, ROAUnalignedStoreInsertion)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));
    roa_in_->push(mitem_t(0x90, 0, mitem_t::STORE, 0, 64, 4));
    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x90u);
    ASSERT_EQ(ma.get_data_size(), 48u);
    ASSERT_EQ(ma.get_op(), Request::op_t::WRITE);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0xC0u);
    ASSERT_EQ(ma2.get_data_size(), 16u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::WRITE);
};

TEST_F(ROATest, ROAiMultipleLineStoreInsertion)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));
    roa_in_->push(mitem_t(0x40, 0, mitem_t::STORE, 0, 128, 4));
    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x40u);
    ASSERT_EQ(ma.get_data_size(), 64u);
    ASSERT_EQ(ma.get_op(), Request::op_t::WRITE);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x80u);
    ASSERT_EQ(ma2.get_data_size(), 64u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::WRITE);
};

TEST_F(ROATest, ROALoadAliasingSameAddress)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 4, 2));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_size(), 8u);
    ASSERT_EQ(ma.get_op(), Request::op_t::READ);

    Ack ack(ma);
    roa_->ack_mem_response(ack);

    //Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(5u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());
};

TEST_F(ROATest, ROALoadAliasingContains)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 4));
    roa_in_->push(mitem_t(0x84, 0, mitem_t::LOAD, 0, 4, 2));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_size(), 8u);
    ASSERT_EQ(ma.get_op(), Request::op_t::READ);

    Ack ack(ma);
    roa_->ack_mem_response(ack);

    //Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(5u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());
};

TEST_F(ROATest, ROALoadAliasingLarger)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 4, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 2));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_size(), 4u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::READ);

    //Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(3u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x80u);
    ASSERT_EQ(ma2.get_size(), 8u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack1(ma1);
    roa_->ack_mem_response(ack1);
    Ack ack2(ma2);
    roa_->ack_mem_response(ack2);

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(2u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());
};

TEST_F(ROATest, ROAStoreAliasingSmaller)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 4, 2));

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

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(2u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_data_size(), 8u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::WRITE);

    //Cycle 7
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x80u);
    ASSERT_EQ(ma2.get_data_size(), 4u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::WRITE);
};

TEST_F(ROATest, ROAStoreAliasingSameSize)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 2));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(5u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 7
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_data_size(), 8u);
    ASSERT_EQ(ma.get_op(), Request::op_t::WRITE);
};

TEST_F(ROATest, ROAStoreAliasingLarger)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    // roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    // roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 16, 2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 4, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 2));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(5u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 7
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_data_size(), 8u);
    // ASSERT_EQ(ma.get_size(), 16u);
    ASSERT_EQ(ma.get_op(), Request::op_t::WRITE);
};

TEST_F(ROATest, ROAStoreLoadStoreAliasing)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    // roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    // roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 16, 4));
    // roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 2));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 4, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 4, 2));

    sim::engine::cycles_t warp;

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 1
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 2
    warp = roa_->cycle();
    ASSERT_EQ(1u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 3
    warp = roa_->cycle();
    ASSERT_EQ(2u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(4u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    // ASSERT_EQ(ma1.get_size(), 8u);
    ASSERT_EQ(ma1.get_data_size(), 4u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::WRITE);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x80u);
    ASSERT_EQ(ma2.get_size(), 8u);
    // ASSERT_EQ(ma2.get_size(), 16u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack(ma2);
    roa_->ack_mem_response(ack);

    //Cycle 9
    warp = roa_->cycle();
    ASSERT_EQ(2u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 11
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma3 = roa_->get_mem_request();

    ASSERT_EQ(ma3.get_tag(), 0x80u);
    // ASSERT_EQ(ma3.get_size(), 8u);
    ASSERT_EQ(ma3.get_data_size(), 4u);
    ASSERT_EQ(ma3.get_op(), Request::op_t::WRITE);
};


TEST_F(ROATest, ROAStoreLoadForwardingSmaller)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 4, 3));

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

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(3u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_data_size(), 8u);
    ASSERT_EQ(ma.get_op(), Request::op_t::WRITE);

    //Cycle 8
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());
};

TEST_F(ROATest, ROAStoreLoadForwardingSameSize)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 3));

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

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(3u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_data_size(), 8u);
    ASSERT_EQ(ma.get_op(), Request::op_t::WRITE);

    //Cycle 8
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());
};

TEST_F(ROATest, ROAStoreLoadForwardingContains)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    roa_in_->push(mitem_t(0x84, 0, mitem_t::LOAD, 0, 4, 3));

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

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(3u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma = roa_->get_mem_request();

    ASSERT_EQ(ma.get_tag(), 0x80u);
    ASSERT_EQ(ma.get_data_size(), 8u);
    ASSERT_EQ(ma.get_op(), Request::op_t::WRITE);

    //Cycle 8
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());
};

TEST_F(ROATest, ROAStoreLoadForwardingLarger)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    // roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    // roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 16, 3));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 4, 4));
    roa_in_->push(mitem_t(0x80, 0, mitem_t::LOAD, 0, 8, 3));

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

    ASSERT_FALSE(roa_->has_mem_request());

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(3u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    // ASSERT_EQ(ma1.get_size(), 8u);
    ASSERT_EQ(ma1.get_data_size(), 4u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::WRITE);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x80u);
    // ASSERT_EQ(ma2.get_size(), 16u);
    ASSERT_EQ(ma2.get_size(), 8u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack2(ma2);
    roa_->ack_mem_response(ack2);

    //Cycle 8
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);
};

TEST_F(ROATest, ROAStoreLoadOOO)
{
    sim_->testing_only_add_cycles(sim::engine::cycles_t(2));

    roa_in_->push(mitem_t(0x80, 0, mitem_t::STORE, 0, 8, 4));
    roa_in_->push(mitem_t(0x90, 0, mitem_t::LOAD, 0, 8, 2));

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
    MemoryAccess ma2 = roa_->get_mem_request();

    ASSERT_EQ(ma2.get_tag(), 0x90u);
    ASSERT_EQ(ma2.get_size(), 8u);
    ASSERT_EQ(ma2.get_op(), Request::op_t::READ);

    Ack ack2(ma2);
    roa_->ack_mem_response(ack2);

    //Cycle 5
    warp = roa_->cycle();
    ASSERT_EQ(2u, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_TRUE(roa_->has_mem_request());
    MemoryAccess ma1 = roa_->get_mem_request();

    ASSERT_EQ(ma1.get_tag(), 0x80u);
    ASSERT_EQ(ma1.get_data_size(), 8u);
    ASSERT_EQ(ma1.get_op(), Request::op_t::WRITE);

    //Cycle 7
    warp = roa_->cycle();
    ASSERT_EQ(sim::engine::NEVER, warp);
    sim_->testing_only_add_cycles(warp);

    ASSERT_FALSE(roa_->has_mem_request());
};
