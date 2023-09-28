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


#ifndef CORE_TRACE_NAMEEVENT_H_
#define CORE_TRACE_NAMEEVENT_H_

#include <iostream>
#include <string>

namespace sim {
namespace trace {

/**
 * NameEvent represents a name given to an event
 */
class NameEvent {
public:
    unsigned id_;
    std::string name_;

#pragma pack(1)
    struct buffer_t {
        unsigned id_;
        unsigned name_len_;
        char first_byte_;
    };
#pragma pack()
    buffer_t *buffer_;

    friend std::istream &operator>>(std::istream &, sim::trace::NameEvent &);

public:
    NameEvent();
    NameEvent(const NameEvent &);
    NameEvent(NameEvent &&);
    NameEvent(unsigned, const char *);
    virtual ~NameEvent();

    NameEvent &operator=(const NameEvent &);

    unsigned get_id() const;
    unsigned get_name_length() const;
    const char *get_name() const;

    void write(std::ostream &) const;
    void read(std::istream &);
};

std::ostream &operator<<(std::ostream &, const sim::trace::NameEvent &);
std::istream &operator>>(std::istream &, sim::trace::NameEvent &);

}  // namespace trace
}  // namespace sim

#include "NameEvent-impl.h"

#endif  // CORE_TRACE_NAMEEVENT_H_
