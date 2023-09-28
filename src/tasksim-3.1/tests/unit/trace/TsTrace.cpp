#include <cstdio>
#include <cstring>
#include "core/trace/ts/Trace.h"
#include "core/trace/mem/Trace.h"
#include "core/trace/BinaryEventStream.h"
#include "core/trace/TextEventStream.h"

#include "gtest/gtest.h"

/**
 * The tests in this file are for the functions of sim::trace::ts::Trace
 * The test cover:
 * - WD info (task dependencies)
 * - Task names
 * - Task event streams
 * - Task events
 * - Task subtraces
 * - waiton dependencies
 */

using namespace sim::trace;

/** Using a typed test we do the same tests with different stream types using the same code */
template <typename TraceType>
class TsTraceTest : public testing::Test {
protected:
    static const std::string filename_;
    TraceType *trace_;

   const TraceType &reloadAndGetReader() {
       assert(trace_ != NULL);
       delete trace_;
       trace_ = new TraceType(filename_.c_str());
       return *trace_;
   }

    virtual void SetUp() {
        trace_ = new TraceType(filename_.c_str());
    }

    virtual void TearDown() {
        if (trace_ != NULL) {
            delete trace_;
        }
        if (remove((filename_+".default.trace").c_str()) != 0) {
            std::cerr << "Error deleting test temporary default.trace file" << std::endl;
        }
        if (remove((filename_+".streaminfo").c_str()) != 0) {
            std::cerr << "Error deleting test temporary streaminfo file" << std::endl;
        }
    }
};
template <typename Type>
const std::string TsTraceTest<Type>::filename_ = "TsTraceTest_tmpfile.tmp";

typedef ::testing::Types<ts::FileTrace<BinaryEventStream, mem::TraceInternal>,ts::FileTrace<TextEventStream, mem::TraceInternal>> IntegerTypes;
TYPED_TEST_CASE(TsTraceTest, IntegerTypes);

TYPED_TEST(TsTraceTest, WDInfo) {
    const unsigned nids = 5;
    const unsigned wids[nids] = {1, 31, 7, 445, 111};
    const unsigned ndeps = 3;


    for (unsigned i = 0; i < nids; i ++) {
        std::vector<ts::dep_t> deps;
        for (unsigned j = 0; j < ndeps; j++) {
            ts::dep_t dep;
            dep.address = reinterpret_cast<void*>(wids[i]+j);
            deps.push_back(dep);
        }
        this->trace_->add_wd_info(wids[i], deps);
    }

    const TypeParam &reader = this->reloadAndGetReader();

    for (unsigned i = 0; i < nids; i++) {
        std::vector<ts::dep_t> deps;
        deps = reader.get_task_dependencies({wids[i]});
        ASSERT_EQ(deps.size(), ndeps);
        for (unsigned int j = 0; j < ndeps; j++) {
            ASSERT_EQ(deps[j].address, reinterpret_cast<void*>(wids[i]+j));
        }
    }
}

TYPED_TEST(TsTraceTest, TaskNames) {
    const unsigned nnames = 3;
    const char* names[nnames] = {"task", "foo", "func"};

    for (unsigned i = 0; i < nnames; i++) {
        ASSERT_FALSE(this->trace_->has_task_name(i));
        this->trace_->add_task_name(i, names[i]);
    }

    ASSERT_EQ(this->trace_->get_number_of_task_names(), nnames);

    for (unsigned i = 0; i < nnames; i++) {
        ASSERT_TRUE(this->trace_->has_task_name(i));
        ASSERT_TRUE(std::strncmp(this->trace_->get_task_name(i), names[i], std::strlen(names[i])) == 0);
    }
}

TYPED_TEST(TsTraceTest, TaskEventStreams) {
    const unsigned nids = 3;
    const unsigned wids[nids] = {1, 152, 32};
    unsigned stream_ids[nids];

    for (unsigned i = 0; i < nids; i++) {
        stream_ids[i] = this->trace_->add_task(wids[i]);
    }

    for (unsigned i = 0; i < nids; i++) {
        ASSERT_EQ(this->trace_->get_task_stream(wids[i]), stream_ids[i]);
    }
}

TYPED_TEST(TsTraceTest, TaskEvents) {
    const unsigned nids = 3;
    const unsigned nevents_x_wd = 2;
    const unsigned wids[nids] = {1, 152, 32};
    ts::event_t events[nids*nevents_x_wd];
    events[0].type = ts::PHASE;
    events[0].u.phase = {31,32,33};
    events[1].type = ts::CREATE_TASK;
    events[1].u.create_task = {51};
    events[2].type = ts::TASK_WAIT;
    events[3].type = ts::WAIT_ON;
    events[3].u.wait_on = {7};
    events[4].type = ts::TASK_WAIT;
    events[5].type = ts::PHASE;
    events[5].u.phase = {51,52,53};

    ts::event_t tmp;
    ASSERT_FALSE(this->trace_->get_task_event(wids[0],tmp));

    ASSERT_TRUE(this->trace_->add_task_event(wids[0],static_cast<ts::type_t>(events[0].type),events[0].u.phase));
    ASSERT_TRUE(this->trace_->add_task_event(wids[0],static_cast<ts::type_t>(events[1].type),events[1].u.create_task));
    ASSERT_TRUE(this->trace_->add_task_event(wids[1],static_cast<ts::type_t>(events[2].type),events[2].u.task_wait));
    ASSERT_TRUE(this->trace_->add_task_event(wids[1],static_cast<ts::type_t>(events[3].type),events[3].u.wait_on));
    ASSERT_TRUE(this->trace_->add_task_event(wids[2],static_cast<ts::type_t>(events[4].type),events[4].u.task_wait));
    ASSERT_TRUE(this->trace_->add_task_event(wids[2],static_cast<ts::type_t>(events[5].type),events[5].u.phase));

    delete this->trace_;
    this->trace_ = NULL;
    TypeParam traceReader(this->filename_.c_str());

    for (unsigned i = 0; i < nids; i++) {
        for (unsigned j = 0; j < nevents_x_wd; j++) {
            unsigned nevent = i * nevents_x_wd + j;
            ASSERT_TRUE(traceReader.get_task_event(wids[i], tmp));
            ASSERT_EQ(tmp.type, events[nevent].type);
            switch (tmp.type) {
            case ts::PHASE:
                ASSERT_EQ(tmp.u.phase.burst_id, events[nevent].u.phase.burst_id);
                ASSERT_EQ(tmp.u.phase.duration, events[nevent].u.phase.duration);
                ASSERT_EQ(tmp.u.phase.subtrace_id, events[nevent].u.phase.subtrace_id);
                break;
            case ts::CREATE_TASK:
                ASSERT_EQ(tmp.u.create_task.wd_id, events[nevent].u.create_task.wd_id);
                break;
            case ts::TASK_WAIT:
                break;
            case ts::WAIT_ON:
                ASSERT_EQ(tmp.u.wait_on.subtrace_id, events[nevent].u.wait_on.subtrace_id);
                break;
            }
        }
    }
}

TYPED_TEST(TsTraceTest, ContiguousDependencies) {
   sim::trace::ts::dep_t dep;
   dep.address = (void *)0xdeadbeef;
   dep.flags.input = 1;
   dep.flags.output = 0;
   dep.flags.can_rename = 0;
   dep.flags.concurrent = 0;
   dep.flags.commutative = 0;

   dep.dimensions.resize(2);

   dep.dimensions[0].size = 8;
   dep.dimensions[0].lower_bound = 4;
   dep.dimensions[0].accessed_length = 2;

   dep.dimensions[1].size = 1L;
   dep.dimensions[1].lower_bound = 2;
   dep.dimensions[1].accessed_length = 1;


   ASSERT_EQ(2UL, dep.accessed_length());
   ASSERT_TRUE(dep.contiguous());
}

TYPED_TEST(TsTraceTest, TaskSubtraces) {
    const unsigned nnames = 5;
    const unsigned ncalls[nnames] = {10,20,15,5,10};
    unsigned  c = 1;
    for (unsigned i = 0; i< nnames; i++) {
        for (unsigned j = 0; j < ncalls[i]; j++) {
            const ts::subtrace_t &st = this->trace_->add_task_trace(c, i, reinterpret_cast<void*>(j));

            ASSERT_EQ(st.wd_id, c);
            ASSERT_EQ(st.name_id,i);
            ASSERT_EQ(st.stack,reinterpret_cast<void*>(j));
            c++;
        }
    }
    c = 1;
    for (unsigned i = 0; i< nnames; i++) {
        for (unsigned j = 0; j < ncalls[i]; j++) {
            const ts::subtrace_t &st = this->trace_->get_task_trace_by_wd_id(c);
            ASSERT_EQ(st.wd_id, c);
            ASSERT_EQ(st.name_id, i);
            ASSERT_EQ(st.stack,reinterpret_cast<void*>(j));

            unsigned id = this->trace_->get_task_trace_id_by_wd_id(c);
            const ts::subtrace_t &st2 = this->trace_->get_task_trace_by_id(id);
            ASSERT_EQ(st2.wd_id, c);
            ASSERT_EQ(st2.name_id, i);
            ASSERT_EQ(st2.stack,reinterpret_cast<void*>(j));
            c++;
        }
    }
}

TYPED_TEST(TsTraceTest, WaitOnDeps) {
    const unsigned nnames = 5;
    const unsigned ncalls[nnames] = {10,20,15,5,10};
    std::vector<ts::subtrace_t> sts;
    const unsigned ndeps = 3;
    unsigned subtrace = 0;
    for (unsigned i = 0; i < nnames; i++) {
        for (unsigned j = 0; j < ncalls[i]; j++) {
            sts.push_back(this->trace_->add_task_trace(subtrace, i, reinterpret_cast<void*>(j)));
            std::vector<ts::dep_t> deps(ndeps);
            for (unsigned k = 0; k < ndeps; k++) {
                deps[k].address = reinterpret_cast<void*>(i * j + j);
            }
            this->trace_->add_wait_on_dependencies(sts[subtrace], deps);
            const ts::subtrace_t &st = this->trace_->get_task_trace_by_wd_id(sts[subtrace].wd_id);
            ASSERT_TRUE(st.wait_trace_id > -1);
            ASSERT_EQ(st.wait_trace_id, sts[subtrace].wait_trace_id);
            ++subtrace;
        }
    }
    delete this->trace_;
    this->trace_ = NULL;
    TypeParam traceReader(this->filename_.c_str());

    subtrace = 0;
    for (unsigned i = 0; i < nnames; i++) {
        for (unsigned j = 0; j < ncalls[i]; j++) {
            std::vector<ts::dep_t> deps;
            ASSERT_TRUE(traceReader.get_wait_on_dependencies(sts[subtrace], deps));
            ASSERT_EQ(deps.size(), ndeps);
            for (unsigned k = 0; k < ndeps; k++) {
                ASSERT_EQ(deps[k].address, reinterpret_cast<void*>(i*j + j));
            }
            ++subtrace;
        }
    }
    ts::subtrace_t tmpst; tmpst.wait_trace_id = -1;
    std::vector<ts::dep_t> deps;
    ASSERT_FALSE(traceReader.get_wait_on_dependencies(tmpst, deps));
    ASSERT_TRUE(deps.empty());
}
