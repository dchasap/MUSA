#include <map>
#include <string>

#include "core/engine/Config.h"
#include "core/logic/coherence/single/Controller.h"
#include "core/logic/memory/policy/types.h"
#include "core/modules/memory/CacheStats.h"
#include "core/logic/coherence/single/Directory.h"
#include "core/logic/coherence/single/Message.h"
#include "core/modules/memory/Cache.h"

#include "gtest/gtest.h"

using namespace sim;
using namespace sim::logic::coherence::single;


template<typename T>
struct Port {
    T buffer_;
    engine::addr_t addr_;
    bool valid_;

    inline
    bool write_request(const T &msg) {
        buffer_ = msg;
        valid_ = true;
        return true;
    }

    inline
    void reset() {
        valid_ = false;
        buffer_ = T();
    }

    inline
    bool write_ack(const T &msg) {
        buffer_ = msg;
        valid_ = true;
        return true;
    }

    inline
    void set_wake_up_on_read(bool success) {

    }

    inline
    Port() : addr_(0), valid_(false) {}
};


template<typename T>
struct Interface {
    typedef struct Port<typename T::Ack> Input;
    typedef struct Port<typename T::Request> Output;
};


class SingleTest : public testing::Test {
protected:
    /* Output for the tested cache */
    Port<Request> out;
    /* Cache configurations */
    static const engine::addr_t start_addr_;
    static const std::size_t word_size_;
    static const std::size_t cache_size_;
    static const std::size_t line_size_;
    static const std::size_t assoc_;
    static const std::size_t mshr_size_;
    static const std::size_t cache_level_;
    static const std::size_t victim_cache_lines_;
    static const std::size_t num_ports_;

    Controller<sim::logic::coherence::single::Message> *controller;
    sim::logic::coherence::single::Directory<sim::logic::coherence::single::Message> *directory_;
    sim::memory::CacheStats *stats_;
    sim::engine::Simulator *sim_;
    virtual void SetUp() {
        sim::engine::Config cache_conf;
        sim::engine::Config config_;
        /* Create the configuration for the cache */
        cache_conf.add_value(cache_size_, "size");
        cache_conf.add_value(line_size_, "line-size");
        cache_conf.add_value(word_size_, "word-size");
        cache_conf.add_value(assoc_, "assoc");
        cache_conf.add_value(cache_level_, "level");
        cache_conf.add_value(victim_cache_lines_, "victim-lines");
        cache_conf.add_value(num_ports_, "num-ports");
        cache_conf.add_value("false", "perform-reduction");
        cache_conf.add_value("MSHR", "mshr");
        cache_conf.add_value("LRU","policy");

        // This is necessary to run these tests with --enable-paraver
        config_.add_value("/tmp/trace", "trace_base_name", "Paraver");
        config_.add_value("/dev/null", "pcf_filename", "Paraver");
        config_.add_value(1000, "buffer_size", "Paraver");
        config_.add_value("", "modules", "Paraver");
        config_.add_value(-1, "hardware_sampling_interval", "Paraver");
        config_.add_value("DISABLED", "sampling_policy", "Paraver");
        config_.add_value(100000,"deadlock_detection_interval", "Global");
        config_.add_value(mshr_size_, "size", "MSHR");
        config_.add_value("LRU", "type", "LRU");
        config_.add_value(assoc_, "size", "LRU");
        sim_ = new sim::engine::Simulator(config_);
        /* Create the configuration for the MSHR */
        stats_ = new sim::memory::CacheStats(*this);
        directory_ = new sim::logic::coherence::single::Directory<sim::logic::coherence::single::Message>(1, 1);
        controller = new Controller<sim::logic::coherence::single::Message>(0, *stats_, *sim_, *directory_, cache_conf);
        out = Port<Request>();
    }

    virtual void TearDown() {
        if (controller != NULL) {
            controller->clear_buffers();
            delete controller;
        }
        delete(directory_);
        delete(stats_);
        delete(sim_);
    }
public:
    std::string get_name() const {
        return std::string("Test");
    }
    unsigned get_id() const {
        return 0;
    }
};

const engine::addr_t SingleTest::start_addr_ = 0x1000;
const std::size_t SingleTest::word_size_ = 4;
const std::size_t SingleTest::cache_size_ = 8192;
const std::size_t SingleTest::line_size_ = 32;
const std::size_t SingleTest::assoc_ = 4;
const std::size_t SingleTest::mshr_size_ = 8;
const std::size_t SingleTest::cache_level_ = 1;
const std::size_t SingleTest::victim_cache_lines_ = 0;
const std::size_t SingleTest::num_ports_ = 1;


TEST_F(SingleTest, FullMiss)
{
    Port<Ack> in;

    engine::addr_t addr = start_addr_;
    for (unsigned n = 0; n < mshr_size_; ++n) {
        controller->process_request(0, Request(Request::op_t::READ, addr, 1, 8));
        controller->send_ack(0, in);
        ASSERT_FALSE(in.valid_);

        controller->send_request(out);
        ASSERT_TRUE(out.valid_);
        ASSERT_EQ(Request::op_t::READ, out.buffer_.get_op());
        ASSERT_EQ(addr, out.buffer_.get_tag());

        addr += line_size_;

        /* Clean up the output port */
        out.reset();
        in.reset();
    }
}


TEST_F(SingleTest, FullHalfMiss)
{
    Port<Ack> in;

    engine::addr_t addr = start_addr_;
    std::size_t max_requests = mshr_size_ * line_size_ / word_size_;
    for (unsigned n = 0; n < max_requests; ++n) {
        controller->process_request(0, Request(Request::op_t::READ, addr, addr, 1, 8));
        controller->send_ack(0, in);
        ASSERT_FALSE(in.valid_);

        controller->send_request(out);
        if (addr % line_size_ == 0) {
            ASSERT_TRUE(out.valid_);
            ASSERT_EQ(Request::op_t::READ, out.buffer_.get_op());
            ASSERT_EQ(addr, out.buffer_.get_tag());
        } else {
            ASSERT_FALSE(out.valid_);
        }

        addr += word_size_;

        /* Clean up the output port */
        out.reset();
        in.reset();
    }
}


TEST_F(SingleTest, ReadWriteMiss)
{
    Port<Ack> in;

    /* Send READ request */
    controller->process_request(0, Request(Request::op_t::READ, start_addr_, start_addr_, 1, 8));
    controller->send_ack(0, in);
    ASSERT_FALSE(in.valid_);

    /* Previous READ should produce a miss and send a READ request to upper level */
    controller->send_request(out);
    ASSERT_TRUE(out.valid_);
    ASSERT_EQ(Request::op_t::READ, out.buffer_.get_op());
    ASSERT_EQ(start_addr_, out.buffer_.get_tag());

    /* Clean up the output port */
    out.reset();
    in.reset();

    /* Send WRITE request */
    controller->process_request(0, Request(Request::op_t::WRITE, start_addr_, start_addr_, 1, 8));
    controller->send_ack(0, in);
    ASSERT_FALSE(in.valid_);

    /* Previous WRITE request should be a half miss, i.e., an INVALIDATION request */
    controller->send_request(out);
    ASSERT_FALSE(out.valid_);
}


TEST_F(SingleTest, WriteReadMiss)
{
    Port<Ack> in;

    /* Send READ request */
    controller->process_request(0, Request(Request::op_t::WRITE, start_addr_, start_addr_, 1, 8));
    controller->send_ack(0, in);
    ASSERT_FALSE(in.valid_);

    /* Previous READ should produce a miss and send a READ request to upper level */
    controller->send_request(out);
    ASSERT_TRUE(out.valid_);
    ASSERT_EQ(Request::op_t::READ, out.buffer_.get_op());
    ASSERT_EQ(start_addr_, out.buffer_.get_tag());

    /* Clean up the output and input port */
    out.reset();
    in.reset();

    /* Send WRITE request */
    controller->process_request(0, Request(Request::op_t::WRITE, start_addr_, start_addr_, 1, 8));
    controller->send_ack(0, in);
    ASSERT_FALSE(in.valid_);

    /* Previous WRITE request should be a half miss, i.e., no request */
    controller->send_request(out);
    ASSERT_FALSE(out.valid_);
}


TEST_F(SingleTest, CompleteRead)
{
    Port<Ack> in;

    /* Send read request to the input port and expect a miss */
    unsigned read_size = 8;
    unsigned read_data_size = 0;
    controller->process_request(0, Request(Request::op_t::READ, start_addr_, read_size, read_data_size));
    controller->send_ack(0, in);
    ASSERT_FALSE(in.valid_);

    /* Previous READ should produce a miss and send a READ request to upper level */
    controller->send_request(out);
    ASSERT_TRUE(out.valid_);
    ASSERT_EQ(Request::op_t::READ, out.buffer_.get_op());
    ASSERT_EQ(start_addr_, out.buffer_.get_tag());
    ASSERT_EQ(line_size_, out.buffer_.get_size());
    ASSERT_EQ(read_data_size, out.buffer_.get_data_size());

    /* Clean up ports */
    out.reset();
    in.reset();

    /* Send read response to the output port */
    controller->process_ack(Ack(Ack::op_t::READ, start_addr_, read_data_size, read_data_size));
    controller->send_ack(0, in);
    ASSERT_TRUE(in.valid_);
    ASSERT_EQ(Ack::op_t::READ, in.buffer_.get_op());
    ASSERT_EQ(start_addr_, in.buffer_.get_tag());

    /* Clean up ports */
    out.reset();
    in.reset();

    /* Send read request again */
    controller->process_request(0, Request(Request::op_t::READ, start_addr_, read_size, read_data_size));
    controller->send_ack(0, in);
    ASSERT_TRUE(in.valid_);
    ASSERT_EQ(Ack::op_t::READ, in.buffer_.get_op());
    ASSERT_EQ(start_addr_, in.buffer_.get_tag());

    controller->send_request(out);
    ASSERT_FALSE(out.valid_);
}


TEST_F(SingleTest, CompleteWrite)
{
    Port<Ack> in;

    unsigned write_size = 0;
    unsigned write_data_size = 8;
    /* Send WRITE request to the input port and expect a miss */
    controller->process_request(0, Request(Request::op_t::WRITE, start_addr_, write_size, write_data_size));
    controller->send_ack(0, in);
    ASSERT_FALSE(in.valid_);

    /* Previous WRITE should produce a miss and send a READWRITE request to upper level */
    controller->send_request(out);
    ASSERT_TRUE(out.valid_);
    ASSERT_EQ(Request::op_t::READ, out.buffer_.get_op());
    ASSERT_EQ(start_addr_, out.buffer_.get_tag());

    /* Clean up ports */
    out.reset();
    in.reset();

    /* Send READWRITE response to the output port */
    controller->process_ack(Ack(Ack::op_t::READ, start_addr_, 8, 8));
    controller->send_ack(0, in);
    ASSERT_TRUE(in.valid_);
    ASSERT_EQ(Ack::op_t::WRITE, in.buffer_.get_op());
    ASSERT_EQ(start_addr_, in.buffer_.get_tag());

    /* Clean up ports */
    out.reset();
    in.reset();

    /* Send READ request again */
    controller->process_request(0, Request(Request::op_t::READ, start_addr_, 8, 0));
    controller->send_ack(0, in);
    ASSERT_TRUE(in.valid_);
    ASSERT_EQ(Ack::op_t::READ, in.buffer_.get_op());
    ASSERT_EQ(start_addr_, in.buffer_.get_tag());

    controller->send_request(out);
    ASSERT_FALSE(out.valid_);
}


TEST_F(SingleTest, Invalidate)
{
    Port<Ack> in;

    /* Send read request to the input port and expect a miss */
    controller->process_request(0, Request(Request::op_t::READ, start_addr_, 8, 0));
    controller->send_ack(0, in);
    ASSERT_FALSE(in.valid_);

    /* Previous READ should produce a miss and send a READ request to upper level */
    controller->send_request(out);
    ASSERT_TRUE(out.valid_);
    ASSERT_EQ(Request::op_t::READ, out.buffer_.get_op());
    ASSERT_EQ(start_addr_, out.buffer_.get_tag());

    /* Clean up ports */
    out.reset();
    in.reset();

    /* Send read response to the output port */
    controller->process_ack(Ack(Ack::op_t::READ, start_addr_, 1, 8));
    controller->send_ack(0, in);
    ASSERT_TRUE(in.valid_);
    ASSERT_EQ(Ack::op_t::READ, in.buffer_.get_op());
    ASSERT_EQ(start_addr_, in.buffer_.get_tag());

    /* Clean up ports */
    out.reset();
    in.reset();

    /* Send INVALIDATE request */
    controller->process_request(0, Request(Request::op_t::INVALIDATE, start_addr_, 0, 0));
    controller->send_ack(0, in);
    ASSERT_TRUE(in.valid_);
    ASSERT_EQ(Ack::op_t::INVALIDATE, in.buffer_.get_op());

    /* Clean up ports */
    out.reset();
    in.reset();

    /* Next READ request should produce a miss */
    controller->process_request(0, Request(Request::op_t::READ, start_addr_, 8, 0));
    controller->send_ack(0, in);
    ASSERT_FALSE(in.valid_);
}

