#include <map>
#include <string>

#include "core/engine/Config.h"
#include "core/engine/Module.h"
#include "core/engine/Simulator.h"
#include "core/logic/coherence/single/Message.h"
#include "core/modules/interconnect/Direct.h"

#include "gtest/gtest.h"

using sim::logic::coherence::single::Request;
using sim::logic::coherence::single::Ack;
typedef sim::interconnect::Direct<sim::logic::coherence::single::Message>::Input InputConnect;
typedef sim::interconnect::Direct<sim::logic::coherence::single::Message>::Output OutputConnect;

class TestModule : public sim::engine::Module {
  public:
    TestModule(sim::engine::Simulator &sim) : Module(sim) {}
    const std::string get_name() const override {return "TestModule";}
    bool reset() override {return true;}
    void start() override {}
    void end() override {}

};


class DirectConnectTest : public testing::Test {
protected:
    /* Simulator */
    sim::engine::Simulator *sim_;
    /* Port owner modules */
    TestModule *in_owner_, *out_owner_;
    /* Input port */
    InputConnect *in_;
    /* Output port */
    OutputConnect *out_;
    /* Cache configurations */

    virtual void SetUp() {
        sim::engine::Config sim_config ;
        sim_config.add_value("/tmp/trace", "trace_base_name", "Paraver");
        sim_config.add_value("/dev/null", "pcf_filename", "Paraver");
        sim_config.add_value(1000, "buffer_size", "Paraver");
        sim_config.add_value("", "modules", "Paraver");
        sim_config.add_value(-1, "hardware_sampling_interval", "Paraver");
        sim_config.add_value("DISABLED", "sampling_policy", "Paraver");
        sim_config.add_value(100000,"deadlock_detection_interval", "Global");
        sim_ = new sim::engine::Simulator(sim_config);
        in_owner_ = new TestModule(*sim_);
        out_owner_ = new TestModule(*sim_);
        in_ = new InputConnect(*in_owner_);
        out_ = new OutputConnect(*out_owner_);
        *out_ >> *in_;
    }

    virtual void TearDown() {
        delete sim_;
        delete out_;
        delete in_;
        delete out_owner_;
        delete in_owner_;
    }
};


TEST_F(DirectConnectTest, singleRequest)
{
    Request req(Request::op_t::READ, 0x7FFF, 1, 0);
    ASSERT_TRUE(out_->write_request(req));
    ASSERT_FALSE(out_->write_request(req));
    Request req2;
    ASSERT_TRUE(in_->read_request(req2));
    ASSERT_TRUE(req == req2);
    ASSERT_FALSE(in_->read_request(req2));
}

TEST_F(DirectConnectTest, singleAck)
{
    Ack ack(Ack::op_t::READ, 0x7FFF, 0, 1);
    ASSERT_TRUE(in_->write_ack(ack));
    ASSERT_FALSE(in_->write_ack(ack));
    Ack ack2;
    ASSERT_TRUE(out_->read_ack(ack2));
    ASSERT_TRUE(ack == ack2);
    ASSERT_FALSE(out_->read_ack(ack2));
}

TEST_F(DirectConnectTest, MultipleRequests)
{
    std::vector<Request> vreq({{Request::op_t::READ, 0x7FFF, 1, 0},
                              {Request::op_t::READ, 0x8000, 16, 0},
                              {Request::op_t::READ, 0x8080, 8, 0},
                              {Request::op_t::READ, 0x4000, 2, 0},
                              {Request::op_t::READ, 0x2010, 16, 0},
                              {Request::op_t::READ, 0x1004, 4, 0},
                              {Request::op_t::WRITE, 0x8000, 0, 16},
                              {Request::op_t::WRITE, 0x8080, 0, 8},
                              {Request::op_t::WRITE, 0x4000, 0, 2},
                              {Request::op_t::WRITE, 0x2010, 0, 16},
                              {Request::op_t::WRITE, 0x1004, 0, 4}
                              });
    for (const Request &req : vreq) {
        ASSERT_TRUE(out_->write_request(req));
        ASSERT_FALSE(out_->write_request(req));
        Request req2;
        ASSERT_TRUE(in_->read_request(req2));
        ASSERT_TRUE(req == req2);
        ASSERT_FALSE(in_->read_request(req2));
    }
}
TEST_F(DirectConnectTest, MultipleAcks)
{
    std::vector<Ack> vack({{Ack::op_t::READ, 0x7FFF, 0, 1},
                          {Ack::op_t::READ, 0x8000, 0, 16},
                          {Ack::op_t::READ, 0x8080, 0, 8},
                          {Ack::op_t::READ, 0x4000, 0, 2},
                          {Ack::op_t::READ, 0x2010, 0, 16},
                          {Ack::op_t::READ, 0x1004, 0, 4},
                          {Ack::op_t::WRITE, 0x8000, 16, 0},
                          {Ack::op_t::WRITE, 0x8080, 8, 0},
                          {Ack::op_t::WRITE, 0x4000, 2, 0},
                          {Ack::op_t::WRITE, 0x2010, 16, 0},
                          {Ack::op_t::WRITE, 0x1004, 4, 0}
                          });
    for (const Ack &ack : vack) {
        ASSERT_TRUE(in_->write_ack(ack));
        ASSERT_FALSE(in_->write_ack(ack));
        Ack ack2;
        ASSERT_TRUE(out_->read_ack(ack2));
        ASSERT_TRUE(ack == ack2);
        ASSERT_FALSE(out_->read_ack(ack2));
    }
}

TEST_F(DirectConnectTest, singleMessage)
{
    Request req(Request::op_t::READ, 0x7FFF, 1, 0);
    ASSERT_TRUE(out_->write_request(req));
    ASSERT_FALSE(out_->write_request(req));
    Request req2;
    ASSERT_TRUE(in_->read_request(req2));
    ASSERT_TRUE(req == req2);
    ASSERT_FALSE(in_->read_request(req2));

    Ack ack(req2);
    ASSERT_TRUE(in_->write_ack(ack));
    ASSERT_FALSE(in_->write_ack(ack));
    Ack ack2;
    ASSERT_TRUE(out_->read_ack(ack2));
    ASSERT_TRUE(ack == ack2);
    ASSERT_FALSE(out_->read_ack(ack2));
    ASSERT_TRUE(req.get_tag() == ack2.get_tag());
    ASSERT_TRUE(req.get_data() == ack2.get_data());
    ASSERT_TRUE(req.get_size() == ack2.get_data_size());
    ASSERT_TRUE(req.get_data_size() == ack2.get_size());
}

TEST_F(DirectConnectTest, MultipleMessages)
{
    std::vector<Request> vreq({{Request::op_t::READ, 0x7FFF, 1, 0},
                              {Request::op_t::READ, 0x8000, 16, 0},
                              {Request::op_t::READ, 0x8080, 8, 0},
                              {Request::op_t::READ, 0x4000, 2, 0},
                              {Request::op_t::READ, 0x2010, 16, 0},
                              {Request::op_t::READ, 0x1004, 4, 0},
                              {Request::op_t::WRITE, 0x8000, 0, 16},
                              {Request::op_t::WRITE, 0x8080, 0, 8},
                              {Request::op_t::WRITE, 0x4000, 0, 2},
                              {Request::op_t::WRITE, 0x2010, 0, 16},
                              {Request::op_t::WRITE, 0x1004, 0, 4}
                              });
    for (const Request &req : vreq) {
        ASSERT_TRUE(out_->write_request(req));
        ASSERT_FALSE(out_->write_request(req));
        Request req2;
        ASSERT_TRUE(in_->read_request(req2));
        ASSERT_TRUE(req == req2);
        ASSERT_FALSE(in_->read_request(req2));

        Ack ack(req2);
        ASSERT_TRUE(in_->write_ack(ack));
        ASSERT_FALSE(in_->write_ack(ack));
        Ack ack2;
        ASSERT_TRUE(out_->read_ack(ack2));
        ASSERT_TRUE(ack == ack2);
        ASSERT_FALSE(out_->read_ack(ack2));
        ASSERT_TRUE(req.get_tag() == ack2.get_tag());
        ASSERT_TRUE(req.get_data() == ack2.get_data());
        ASSERT_TRUE(req.get_size() == ack2.get_data_size());
        ASSERT_TRUE(req.get_data_size() == ack2.get_size());
    }
}
