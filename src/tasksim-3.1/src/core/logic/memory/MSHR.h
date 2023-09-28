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


#ifndef CORE_LOGIC_MEMORY_MSHR_H_
#define CORE_LOGIC_MEMORY_MSHR_H_

#include <list>
#include <unordered_map>
#include <vector>

#include "core/engine/types.h"
#include "core/engine/Config.h"
#include "core/logic/coherence/single/Message.h"

namespace sim {
namespace logic {
namespace memory {


template<typename Message>
class MSHR {
public:
    /** The State of the MSHR Request */
    struct MSHRState {
        typedef enum {
            // Pending: pending an evict.
            // Sending: scheduled to be sent
            // Sent: actually sent through the buffer.
            // Ready: got the answer already.
            INVALID, PENDING, SENDING, SENT, INSPECTED, READY
        } state_t;
        /** Constructor: */
        MSHRState(state_t);
        /** Current state of the MSHR Request: */
        state_t value_;
        /** Request receive time: */
        engine::cycles_t time_;
    };

    /* An subentry on a MSHR entry */
    struct subentry_t {
        /** Constructor: */
        subentry_t(unsigned port, const typename Message::Request &request);
        /** Request type. */
        typename Message::Request request_;
        /** Input port from which the Request arrived. */
        unsigned port_;
    };

    /* An entry in the MSHR */
    struct entry_t {
        /** Constructor: */
        explicit entry_t(sim::engine::addr_t tag = 0);
        entry_t(sim::engine::addr_t tag, const typename Message::Request &request);
        /** Destructor: */
        ~entry_t();
        /** Indexing tag: */
        sim::engine::addr_t tag_;
        /** The state of the MSHR Request. */
        MSHRState state_;
        /** Associated request: */
        typename Message::Request request_;
        /** List of all pending Requests to the cache line. */
        std::list<subentry_t> list_;
        /** Marks if any of the Requests where write Requests. */
        bool dirty_;
        bool fill_;
    };

protected:
    /** Cache hosting the miss state */
    std::unordered_map<sim::engine::addr_t, entry_t> data_;
    /** MSHR size. */
    size_t data_size_;
    /** Address mask used to access the cache */
    sim::engine::addr_t mask_;

public:
    /** Constructor:*/
    MSHR(const engine::Config &mshr_config, std::size_t cache_line_size);
    /** Destructor: */
    ~MSHR();
    /** Returns the MSHR size, */
    std::size_t get_size() const;
    /** Returns the current empty entries on the MSHR. */
    std::size_t get_free_size() const;

    /** Allocates an entry for addr, returns NULL if MSHR is full. aborts if the line is already present. */
    entry_t *allocate_entry(const sim::engine::addr_t &addr, const typename Message::Request &request);
    /** Returns a pointer to the entry for addr, returns NULL if there is no entry for addr. */
    entry_t *lookup_entry(const sim::engine::addr_t &addr);
    /** Removes the entry for \entry. */
    void remove_entry(entry_t &entry);
    /** Debug function, to be used only on testing, erases al entries and subentries from data_. */
    void clear_entries();
    /** Prints the current status of the MSHR (size and tags for the entries). */
    void print_status() const;
};


}  // namespace memory
}  // namespace logic
}  // namespace sim

#include "MSHR-impl.h"

#endif  // CORE_LOGIC_MEMORY_MSHR_H_
