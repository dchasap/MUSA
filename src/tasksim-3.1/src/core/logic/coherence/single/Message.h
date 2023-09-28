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


#ifndef CORE_LOGIC_COHERENCE_SINGLE_MESSAGE_H_
#define CORE_LOGIC_COHERENCE_SINGLE_MESSAGE_H_

#include <vector>
#include "core/engine/Simulator.h"
#include "core/engine/types.h"
#include "core/runtime/TaskDataDep.h"

namespace sim {
namespace logic {
namespace coherence {
namespace single {

enum class req_op_t : unsigned{READ = 0, WRITE, FLUSH, INVALIDATE, PREFETCH, CLEAR, LAST};
enum class ack_op_t : unsigned{READ = 0, WRITE, FLUSH, INVALIDATE, PREFETCH, CLEAR, MISS, LAST};

struct Packet {
    typedef sim::engine::addr_t tag_t;
    typedef std::vector<sim::engine::cycles_t> stamp_vector_t;
    typedef sim::runtime::task_dep_t task_dep_t;

protected:
    tag_t tag_;             /**< Address being requested/sent */
    size_t size_;           /**< Size of the data being requested */
    size_t data_size_;      /**< Size of the data being transmitted */
    size_t data_;           /**< Data being requested/sent */
    task_dep_t dep_type_;   /**< Dependency type of the variable that address belongs to in current task */
    std::size_t task_type_; /**< Type of the task that issued this request */
    uint16_t cpu_id_;       /**< ID of the cpu that issued the packet, with default value 0 */
    uint64_t user_code_id_; /**< Id of the task that generated the Packet */
    // This is mutable, should only be used for statistics:
    mutable int issuer_;    /**< Id of the cpu that issued the Packet */

    Packet();
    Packet(const tag_t &tag, size_t size, size_t data_size, size_t data,
           const task_dep_t &dep_type = task_dep_t::NON_IN_OUT, size_t task_type = 0,
           uint16_t cpu_id = 0, uint64_t user_code_id = 0);
    Packet(const Packet &original);
    /** Creates a Packet for a Cache Line Request. */
    Packet(const Packet &original, size_t cache_line_size);

public:
    tag_t get_tag() const;
    size_t get_size() const;
    size_t get_data_size() const;
    size_t get_data() const;
    task_dep_t get_dep_type() const;
    std::size_t get_task_type() const;
    uint16_t get_cpu_id() const;
    void set_issuer(int id) const;
    int get_issuer() const;
    uint64_t get_user_code_id() const;
    void set_user_code_id(uint64_t user_code_id);
    virtual void touch(const sim::engine::Simulator &sim);
    virtual void touch(const Packet &packet);
    ~Packet() {}
};

/** function to write an Ack to an output stream */
std::ostream &operator<<(std::ostream &os, const Packet &p);

struct Request : virtual public Packet {
private:
    req_op_t op_;
    bool fill_;
public:
    typedef req_op_t op_t;
    Request();
    Request(op_t, const tag_t &addr, size_t size, size_t data_size = 0, size_t data = 0,
        const task_dep_t &dep_type = task_dep_t::NON_IN_OUT, size_t task_type = 0,
        uint16_t cpu_id = 0, uint64_t user_code_id = 0, bool fill = false);
    Request(const Request &original);
    /** Creates a Request for a Cache Line. */
    Request(const Request &original, op_t op, size_t cache_line_size);
    op_t get_op() const;
    bool operator==(const Request &b) const;
    bool is_fill() const;
    void set_fill(bool fill);
    virtual ~Request() {}
    friend class Ack;
};

/** function to write a Request to an output stream */
std::ostream &operator<<(std::ostream &os, const Request &req);

struct TimePacket : virtual public Packet {
protected:
    stamp_vector_t time_log_;
public:
    TimePacket();
    TimePacket(const TimePacket &req);
    TimePacket(const tag_t &addr, std::size_t size, std::size_t data_size,
           std::size_t data, uint64_t user_code_id, const stamp_vector_t &time_log);
    TimePacket(const tag_t &addr, std::size_t size, std::size_t data_size,
           std::size_t data, uint64_t user_code_id, sim::engine::cycles_t time);
    const stamp_vector_t& get_time_log() const;
    engine::cycles_t get_stamp(unsigned i) const;
    void update_time_log(engine::cycles_t start);
    void write_time_log();
    void touch(const sim::engine::Simulator &sim) override;
    void touch(const Packet &packet) override;
    virtual ~TimePacket() {}
};

struct TimeRequest : public TimePacket, public Request {
public:
    TimeRequest();
    TimeRequest(op_t op, const tag_t &addr, size_t size, size_t data_size = 0);
    TimeRequest(const stamp_vector_t &time_log, op_t op, const tag_t &addr, size_t size, size_t data_size = 0);
    TimeRequest(sim::engine::cycles_t stamp, op_t op, const tag_t &addr, size_t size, size_t data_size = 0);
    virtual ~TimeRequest() {}
    friend class TimeAck;
};

struct Ack : virtual public Packet {
private:
    ack_op_t op_;
public:
    typedef ack_op_t op_t;
    Ack();
    explicit Ack(const Request &req);
    Ack(op_t op, const tag_t &tag, size_t size, size_t data_size= 0, uint64_t user_code_id = 0);
    op_t get_op() const;
    bool operator==(const Ack &b) const;
    virtual ~Ack() {}
};

/** function to write an Ack to an output stream */
std::ostream &operator<<(std::ostream &os, const Ack &ack);

struct TimeAck : public TimePacket, public Ack {
public:
    TimeAck();
    explicit TimeAck(const TimeRequest &req);
    TimeAck(op_t op, const tag_t &addr, size_t size, size_t data_size = 0);
    TimeAck(const stamp_vector_t &time_log, op_t op, const tag_t &addr, size_t size, size_t data_size = 0);
    TimeAck(sim::engine::cycles_t stamp, op_t op, const tag_t &addr, size_t size, size_t data_size = 0);
    virtual ~TimeAck() {}
    friend class TimeRequest;
};


struct Message {
    typedef typename single::Request Request;
    typedef typename single::Ack Ack;
};

struct TimeMessage {
    typedef typename single::TimeRequest Request;
    typedef typename single::TimeAck Ack;
};


}  // namespace single
}  // namespace coherence
}  // namespace logic
}  // namespace sim

#include "Message-impl.h"

#endif  // CORE_LOGIC_COHERENCE_SINGLE_MESSAGE_H_
