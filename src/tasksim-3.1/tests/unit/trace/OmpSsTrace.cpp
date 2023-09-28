#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include "gtest/gtest.h"
#include "core/trace/ompss/Trace.h"
#include "core/trace/BinaryEventStream.h"
#include "core/trace/TextEventStream.h"
#include "core/utils/Log.h"

/**
 * The tests in this file are for the functions of sim::trace::ompss::Trace
 * The test cover:
 * - WDs (with sequential and random IDs)
 * - Task names
 * - Dependencies (waiton dependencies)
 */

using namespace sim::trace;
using sim::utils::Log;
/** Using a typed test we do the same tests with different stream types using the same code */
template <typename TraceType>
class OmpSsTraceTest : public testing::Test {
protected:
    static const std::string filename_;
    TraceType *trace_;

    TraceType &reloadAndGetTrace() {
        assert(trace_ != NULL);
        delete trace_;
        trace_ = new TraceType(filename_.c_str());
        return *trace_;
    }

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
        assert (trace_ != NULL);
        delete trace_;
        std::vector<std::string> suffixes({".default.trace", ".wd_info.trace", ".deps.trace", ".streaminfo"});
        for (auto &sufix : suffixes) {
            remove((filename_ + sufix).c_str());
        }
    }
};
template <typename T>
const std::string OmpSsTraceTest<T>::filename_ = "OmpSsTraceTest_tmpfile.tmp";

typedef ::testing::Types<ompss::FileTrace<BinaryEventStream>,ompss::FileTrace<TextEventStream>> StreamTypes;
TYPED_TEST_CASE(OmpSsTraceTest, StreamTypes);

TYPED_TEST(OmpSsTraceTest, SequentialWds) {
    const unsigned numids = 10;
    ompss::dep_t dep;
    const unsigned num_addr = 3;
    void* addrs[num_addr] = {reinterpret_cast<void*>(37),
            reinterpret_cast<void*>(867),
            reinterpret_cast<void*>(2)};

    TypeParam &writer = *this->trace_;
    for (unsigned i = 0; i < numids; i++){
        ompss::wd_info_t &info = writer.add_wd_info(i);
        info.deps_.resize(num_addr);
        for (unsigned j = 0; j < num_addr; j++) {
            info.deps_[j].address = addrs[j];
        }
    }

    const TypeParam &reader = this->reloadAndGetReader();
    ASSERT_EQ(reader.get_number_of_wd_info(), numids);

    for (unsigned i = 0; i < numids; i++){
        const ompss::wd_info_t &info = reader.get_wd_info(i);
        ASSERT_EQ(info.wd_id_, i);
        for (unsigned j = 0; j < num_addr; j++) {
            ASSERT_EQ(info.deps_[j].address, addrs[j]);
        }
    }

    TypeParam &trace = this->reloadAndGetTrace();
    ASSERT_EQ(trace.get_number_of_wd_info(), numids);

    for (unsigned i = 0; i < numids; i++){
        ompss::wd_info_t &info = trace.get_wd_info(i);
        ASSERT_EQ(info.wd_id_, i);
        for (unsigned j = 0; j < num_addr; j++) {
            ASSERT_EQ(info.deps_[j].address, addrs[j]);
        }
    }
}

TYPED_TEST(OmpSsTraceTest, RandomWds) {
    const unsigned numids = 5;
    const unsigned ids[numids] = {1, 15, 1024, 1000001, 32};
    const unsigned num_addr = 3;
    void* addrs[num_addr] = {reinterpret_cast<void*>(37),
            reinterpret_cast<void*>(867),
            reinterpret_cast<void*>(2)};

    TypeParam &writer = *this->trace_;
    for (unsigned i = 0; i < numids; i++){
        ompss::wd_info_t &info = writer.add_wd_info(ids[i]);
        info.deps_.resize(num_addr);
        for (unsigned j = 0; j < num_addr; j++) {
            info.deps_[j].address = addrs[j];
        }
    }

    const TypeParam &reader = this->reloadAndGetReader();
    ASSERT_EQ(reader.get_number_of_wd_info(), numids);

    for (unsigned i = 0; i < numids; i++){
        const ompss::wd_info_t &info = reader.get_wd_info(ids[i]);
        ASSERT_EQ(info.wd_id_, ids[i]);
        for (unsigned j = 0; j < num_addr; j++) {
            ASSERT_EQ(info.deps_[j].address, addrs[j]);
        }
    }

    TypeParam &trace = this->reloadAndGetTrace();
    ASSERT_EQ(trace.get_number_of_wd_info(), numids);

    for (unsigned i = 0; i < numids; i++){
        ompss::wd_info_t &info = trace.get_wd_info(ids[i]);
        ASSERT_EQ(info.wd_id_, ids[i]);
        for (unsigned j = 0; j < num_addr; j++) {
            ASSERT_EQ(info.deps_[j].address, addrs[j]);
        }
    }
}

TYPED_TEST(OmpSsTraceTest, TaskNames) {
    const unsigned nnames = 3;
    const char* names[nnames] = {"task", "foo", "func"};

    TypeParam &writer = *this->trace_;
    for (unsigned i = 0; i < nnames; i++) {
        ASSERT_FALSE(writer.has_task_name(names[i]));
        unsigned ind;
        writer.add_task_name(names[i], ind);
    }

    const TypeParam &reader = this->reloadAndGetReader();
    ASSERT_EQ(reader.get_number_of_task_name(), nnames);

    for (unsigned i = 0; i < nnames; i++) {
        ASSERT_TRUE(reader.has_task_name(names[i]));
        ASSERT_TRUE(std::strcmp(reader.get_task_name(i).c_str(), names[i]) == 0);
        ASSERT_EQ(reader.get_task_id(names[i]), i);
    }
}

TYPED_TEST(OmpSsTraceTest, Deps) {
    const unsigned ndeps = 3;
    const unsigned nwds = 10;
    const unsigned wds[nwds] = { 11, 5, 37 };

    TypeParam &writer = *this->trace_;
    for (unsigned j = 0; j < ndeps; j++) {
        for (unsigned i = 0; i < nwds; i++) {
            if (i % 2 == 0) {
                ompss::dep_t dep;
                dep.wd_id_ = wds[i];
                dep.dep_.address = reinterpret_cast<void*>(j);
                this->trace_->add_dep(dep);
            } else {
                ts::dep_t tsdep;
                tsdep.address = reinterpret_cast<void*>(j);
                ompss::dep_t dep(wds[i], tsdep);
                writer.add_dep(dep);
            }
        }
    }
    const TypeParam &reader = this->reloadAndGetReader();
    for (unsigned j = 0; j < ndeps; j++) {
        for (unsigned i = 0; i < nwds; i++) {
            ompss::dep_t dep;
            ASSERT_TRUE(reader.get_dep(dep));
            ASSERT_EQ(dep.wd_id_, wds[i]);
            ASSERT_EQ(dep.dep_.address, reinterpret_cast<void*>(j));
        }
    }
    ompss::dep_t dep;
    ASSERT_FALSE(reader.get_dep(dep));
}



TYPED_TEST(OmpSsTraceTest, LotsOfWds) {
    int num_iter = 220;
    int num_wd = 1000;
    int wd = 2000001;
    void* addresses[10];
    for(int i = 0; i < 10; i++) { addresses[i] = reinterpret_cast<void*>(i);}
    std::chrono::system_clock::time_point ini, fin;
    TypeParam &writer = *this->trace_;
    for (int i = 0; i < num_iter; i++) {
        ini = std::chrono::system_clock::now();
        for (int j = 0; j < num_wd; j++) {
            sim::trace::ompss::wd_info_t &wd_info = writer.add_wd_info(wd++);
            wd_info.deps_.resize(10);
            for ( int k = 0; k < 10; k++) {
                wd_info.deps_[k].address = addresses[k];
            }
            unsigned int name_id;
            writer.add_task_name(std::string("test"), name_id);
            writer.add_event(sim::trace::ompss::event_t(wd, sim::trace::ompss::TASK_NAME, name_id, wd));
        }
        fin = std::chrono::system_clock::now();
        Log::debug() << "Iteration " << i  << " took = " << std::chrono::duration_cast<std::chrono::microseconds>(fin - ini).count() << " microseconds.";
    }
}
