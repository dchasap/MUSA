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

#include <cstdio>
#include <cstring>
#include <string>
#include "gtest/gtest.h"
#include "core/trace/mem/types.h"
#include "core/trace/common/mitem.h"
#include "core/trace/common/minst.h"

#define REG_imm     75
#define REG_mem     76

/** Using a typed test we do the same tests with different stream types using the same code */
class InstrQueueTest : public testing::Test {
protected:
    sim::trace::mem::mitem_queue_t *instr_queue_;
    uint8_t imm, mem;
    virtual void SetUp() {
        imm = REG_imm;
        mem = REG_mem;
        instr_queue_ = new sim::trace::mem::mitem_queue_t();
    }

    virtual void TearDown() {
        delete instr_queue_;
    }
};

TEST_F(InstrQueueTest, QueueSingleLoad) {
    uint8_t size = 128;
    sim::trace::bbl_t bbl;
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::LD, size, mem, imm, imm, imm));
    instr_queue_->push(bbl);

    sim::trace::mitem_t mem = instr_queue_->front();
    ASSERT_TRUE(mem.op_ == sim::trace::mitem_t::op_t::LOAD);
    ASSERT_TRUE(mem.size_ == size);

    instr_queue_->pop();
    ASSERT_TRUE(instr_queue_->empty());
}

TEST_F(InstrQueueTest, QueueSingleStore) {
    uint8_t size = 128;
    sim::trace::bbl_t bbl;
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::ST, size, imm, imm, imm, mem));
    instr_queue_->push(bbl);

    sim::trace::mitem_t mem = instr_queue_->front();
    ASSERT_TRUE(mem.op_ == sim::trace::mitem_t::op_t::STORE);
    ASSERT_TRUE(mem.size_ == size);

    instr_queue_->pop();
    ASSERT_TRUE(instr_queue_->empty());
}


TEST_F(InstrQueueTest, QueueNoMem) {
    sim::trace::bbl_t bbl;
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    instr_queue_->push(bbl);

    ASSERT_TRUE(instr_queue_->empty());
}

TEST_F(InstrQueueTest, QueueMultipleMems) {
    sim::trace::bbl_t bbl;

    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::LD, 4, mem, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::ST, 2, imm, imm, imm, mem));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::LD, 8, mem, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::ST, 16, imm, imm, imm, mem));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::ST, 16, imm, imm, imm, mem));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::IALU, 0, imm, imm, imm, imm));
    bbl.ins_.emplace_back(sim::trace::minst_t(nullptr, sim::trace::op_t::LD, 4, mem, imm, imm, imm));

    instr_queue_->push(bbl);

    sim::trace::mitem_t mem = instr_queue_->front();
    ASSERT_TRUE(mem.op_ == sim::trace::mitem_t::op_t::LOAD);
    ASSERT_EQ(mem.size_, 4);
    ASSERT_EQ(mem.ins_, 4);
    ASSERT_FALSE(instr_queue_->empty());
    instr_queue_->pop();

    mem = instr_queue_->front();
    ASSERT_TRUE(mem.op_ == sim::trace::mitem_t::op_t::STORE);
    ASSERT_EQ(mem.size_, 2);
    ASSERT_EQ(mem.ins_, 3);
    ASSERT_FALSE(instr_queue_->empty());
    instr_queue_->pop();

    mem = instr_queue_->front();
    ASSERT_TRUE(mem.op_ == sim::trace::mitem_t::op_t::LOAD);
    ASSERT_EQ(mem.size_, 8);
    ASSERT_EQ(mem.ins_, 2);
    ASSERT_FALSE(instr_queue_->empty());
    instr_queue_->pop();

    mem = instr_queue_->front();
    ASSERT_TRUE(mem.op_ == sim::trace::mitem_t::op_t::STORE);
    ASSERT_EQ(mem.size_, 16);
    ASSERT_EQ(mem.ins_, 5);
    ASSERT_FALSE(instr_queue_->empty());
    instr_queue_->pop();

    mem = instr_queue_->front();
    ASSERT_TRUE(mem.op_ == sim::trace::mitem_t::op_t::STORE);
    ASSERT_EQ(mem.size_, 16);
    ASSERT_EQ(mem.ins_, 3);
    ASSERT_FALSE(instr_queue_->empty());
    instr_queue_->pop();

    mem = instr_queue_->front();
    ASSERT_TRUE(mem.op_ == sim::trace::mitem_t::op_t::LOAD);
    ASSERT_EQ(mem.size_, 4);
    ASSERT_EQ(mem.ins_, 4);
    ASSERT_FALSE(instr_queue_->empty());
    instr_queue_->pop();

    ASSERT_TRUE(instr_queue_->empty());
}
