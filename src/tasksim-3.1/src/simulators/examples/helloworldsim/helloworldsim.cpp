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


#include <iostream>

#include <core/engine/Config.h>
#include <core/engine/Simulator.h>
#include <core/engine/Module.h>
#include <core/engine/InPort.h>
#include <core/engine/OutPort.h>

using namespace sim;

struct ExamplePacket {
    typedef int tag_t;
protected:
    tag_t tag_;
    
    ExamplePacket(tag_t tag);
public:
    tag_t get_tag();
    void touch(const sim::engine::Simulator &sim) {}
};

ExamplePacket::ExamplePacket(ExamplePacket::tag_t tag) :
    tag_(tag)
{}

ExamplePacket::tag_t ExamplePacket::get_tag() {
    return tag_;
}

struct ExampleRequest : public ExamplePacket {
    typedef enum { ADD = 0, SUB, MUL, DIV } operation_t;
protected:
    operation_t operation_;
    int operand1_, operand2_;

public:
    ExampleRequest();
    ExampleRequest(ExamplePacket::tag_t, operation_t, int, int);

    operation_t get_operation();
    int get_operand1();
    int get_operand2();
};

ExampleRequest::ExampleRequest() :
    ExamplePacket(0), operation_(ExampleRequest::operation_t::ADD), operand1_(0), operand2_(0)
{}

ExampleRequest::ExampleRequest(ExamplePacket::tag_t tag, operation_t operation, int operand1, int operand2) :
    ExamplePacket(tag), operation_(operation), operand1_(operand1), operand2_(operand2)
{}

ExampleRequest::operation_t ExampleRequest::get_operation()
{
    return operation_;
}

int ExampleRequest::get_operand1()
{
    return operand1_;
}

int ExampleRequest::get_operand2()
{
    return operand2_;
}


struct ExampleResponse : public ExamplePacket {
protected:
    int result_;

public:
    ExampleResponse();
    ExampleResponse(ExamplePacket::tag_t, int);

    int get_result();
};

ExampleResponse::ExampleResponse() :
    ExamplePacket(0), result_(0)
{}

ExampleResponse::ExampleResponse(ExamplePacket::tag_t tag, int result) :
    ExamplePacket(tag), result_(result)
{}

int ExampleResponse::get_result()
{
    return result_;
}


struct Message {
    typedef ExampleRequest Request;
    typedef ExampleResponse Response;
};


template<typename T>
class ExampleProducer : public engine::Module {
public:
    ExampleProducer(engine::Simulator &, const engine::Config &);

    bool reset();
    void start();
    void end();
    const std::string get_name() const;
    
    engine::OutPort<typename T::Request> out_;
    engine::InPort<typename T::Response> in_;
};

template<typename T>
inline
ExampleProducer<T>::ExampleProducer(engine::Simulator &sim, const engine::Config &conf):
    engine::Module(sim), out_(*this), in_(*this)
{
}

template<typename T>
inline
bool ExampleProducer<T>::reset()
{
    return true;
}

template<typename T>
inline
void ExampleProducer<T>::start()
{
    if (this->get_simulator().get_clock() == 50)
    {
        Message::Request req = Message::Request(0, ExampleRequest::operation_t::ADD, 2, 2);
        out_.write(req);
    }
}


template<typename T>
inline
void ExampleProducer<T>::end()
{
    if (this->get_simulator().get_clock() < 50)
    {
        this->set_module_activity(50 - this->get_simulator().get_clock());
    }
    else
    {
        this->set_module_activity(engine::NEVER);
    }
}

template<typename T>
inline
const std::string ExampleProducer<T>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

template<typename T>
class ExampleConsumer : public engine::Module {
public:
    ExampleConsumer(engine::Simulator &, const engine::Config &);

    bool reset();
    void start();
    void end();
    const std::string get_name() const;

    engine::OutPort<typename T::Response> out_;
    engine::InPort<typename T::Request> in_;
};

template<typename T>
inline
ExampleConsumer<T>::ExampleConsumer(engine::Simulator &sim, const engine::Config &conf):
    engine::Module(sim), out_(*this), in_(*this)
{
}

template<typename T>
inline
bool ExampleConsumer<T>::reset()
{
    return true;
}

template<typename T>
inline
void ExampleConsumer<T>::start() {}


template<typename T>
inline
void ExampleConsumer<T>::end()
{
    std::cout << "Consumer end: " << this->get_simulator().get_clock() << std::endl;
}

template<typename T>
inline
const std::string ExampleConsumer<T>::get_name() const
{
    return stats::demangle(typeid(*this).name());
}

int main(int argc, char *argv[])
{
    engine::Config conf(argv[1]);
	engine::Simulator simulator(conf);

    ExampleProducer<Message> producer(simulator, engine::Config(conf, "CPU"));
    ExampleConsumer<Message> consumer(simulator, engine::Config(conf, "CPU"));
	
	producer.out_ >> consumer.in_;
	
	simulator.run();
	
}
