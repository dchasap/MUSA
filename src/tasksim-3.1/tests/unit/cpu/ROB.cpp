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
#include "core/modules/cpu/models/ROA.h"

#include "gtest/gtest.h"

using sim::cpu::timing_models::ROB;
using sim::cpu::timing_models::OutputBuffer;
using sim::cpu::timing_models::LDQueue;
using sim::logic::coherence::single::Request;
using sim::trace::mitem_t;

// ------------------------------------------------------------------------
// Tests for the "ROA/ROB" classes.
// ------------------------------------------------------------------------
typedef Request MemoryAccess;
typedef OutputBuffer<MemoryAccess> OutputBuffer_t;
typedef ROB<OutputBuffer_t> ROB_t;

TEST(OutputBufferTest, OutputBufferPushPop)
{
    OutputBuffer_t out(1);

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

TEST(LDQueueTest,LDQueueInsertRetrieve)
{
    LDQueue<bool> queue;

    ASSERT_TRUE(queue.insert(0x80, 8, true));

    ASSERT_FALSE(queue.retrieve(0x100, 4));

    ASSERT_TRUE(queue.retrieve(0x80, 8));

    ASSERT_FALSE(queue.retrieve(0x80, 8));
}

TEST(LDQueueTest,LDQueueMultipleInsertRetrieve)
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

// Necessary to avoid warnings as memory access remain on the buffer.
static void clear(OutputBuffer_t& out)
{
    while (not out.empty()) {
        out.pop();
    }
}

TEST(ROBTest, ROBEmpty)
{
    OutputBuffer_t out(1);
    ROB_t rob(128, 64, out);

    ASSERT_TRUE(rob.empty());
}

TEST(ROBTest, ROBCapacity)
{
    OutputBuffer_t out(1);
    ROB_t rob(128, 64, out);

    ASSERT_TRUE(rob.capacity() == 128);
}

TEST(ROBTest, ROBLoadInsertion)
{
    OutputBuffer_t out(1);
    ROB_t rob(128, 64, out);
    mitem_t item(0x80, 0, mitem_t::LOAD, 0, 8, 1);

    ASSERT_TRUE(rob.insert(item) != NULL);
    ASSERT_FALSE(rob.empty());
    ASSERT_FALSE(out.empty());

    clear(out);
}

TEST(ROBTest, ROBStoreInsertion)
{
    OutputBuffer_t out(1);
    ROB_t rob(128, 64, out);
    mitem_t item(0x80, 0, mitem_t::STORE, 0, 8, 1);

    ASSERT_TRUE(rob.insert(item) == NULL);
    ASSERT_FALSE(rob.empty());
    ASSERT_TRUE(out.empty());

    clear(out);
}

TEST(ROBTest, ROBFreeEntries)
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

TEST(ROBTest, ROBInstrToFirst1)
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

TEST(ROBTest, ROBInstrToFirst2)
{
    OutputBuffer_t out(10);
    ROB_t rob(10, 64, out);

    mitem_t item(0x80, 0, mitem_t::STORE, 0, 8, 3);
    rob.insert(item);

    ASSERT_EQ(3u, rob.get_ins_to_first_waiting_item());

    clear(out);
}

TEST(ROBTest, ROBAck)
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

TEST(ROBTest, ROBRetire1)
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

TEST(ROBTest, ROBRetire2)
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
