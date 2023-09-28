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


#ifndef CORE_TRACE_NAMEEVENT_IMPL_H_
#define CORE_TRACE_NAMEEVENT_IMPL_H_

#include <cassert>
#include <cstring>
#include <string>

namespace sim {
namespace trace {

inline NameEvent::NameEvent() :
        buffer_(NULL)
{
}

inline NameEvent::NameEvent(const NameEvent &ne)
{
    if (ne.buffer_ == NULL) {
        buffer_ = NULL;
        return;
    }
    buffer_ = reinterpret_cast<buffer_t*>(malloc(sizeof(buffer_t) + ne.buffer_->name_len_));
    buffer_->id_ = ne.buffer_->id_;
    buffer_->name_len_ = ne.buffer_->name_len_;
    memcpy(&buffer_->first_byte_, &ne.buffer_->first_byte_, buffer_->name_len_ + 1);
    name_ = std::string(&buffer_->first_byte_);
    id_ = ne.id_;
}


inline
NameEvent::NameEvent(NameEvent &&ne)
{
    buffer_ = ne.buffer_;
    ne.buffer_ = NULL;
}


inline NameEvent::NameEvent(unsigned id, const char *name) :
        id_(id), name_(name)
{
    buffer_ = reinterpret_cast<buffer_t*>(malloc(sizeof(buffer_t) + strlen(name)));
    buffer_->id_ = id;
    buffer_->name_len_ = strlen(name);
    buffer_->first_byte_ = 0;
    memcpy(&buffer_->first_byte_, name, buffer_->name_len_);
    (&buffer_->first_byte_)[buffer_->name_len_] = 0;
}


inline NameEvent::~NameEvent()
{
    if (buffer_ != NULL) free(buffer_);
    buffer_ = NULL;
}


inline
NameEvent &NameEvent::operator=(const NameEvent &e)
{
    if (&e == this) {
        return *this;
    }
    if (this->buffer_ != NULL) {
        free(this->buffer_);
    }
    if (e.buffer_ != NULL) {
        buffer_ = reinterpret_cast<buffer_t*>(malloc(sizeof(buffer_t) + e.buffer_->name_len_));
        memcpy(buffer_, e.buffer_, sizeof(buffer_t) + e.buffer_->name_len_);
    }
    id_ = e.id_;
    name_ = e.name_;
    return *this;
}


inline
unsigned NameEvent::get_id() const
{
    assert(buffer_ != NULL);
    return buffer_->id_;
}


inline
unsigned NameEvent::get_name_length() const
{
    if (buffer_ == NULL) return 0;
    return strlen(&buffer_->first_byte_);
}


inline
const char *NameEvent::get_name() const
{
    assert(buffer_ != NULL);
    return &buffer_->first_byte_;
}


inline
void NameEvent::write(std::ostream &stream) const
{
    if (buffer_ == NULL) {
        return;
    }
    stream.write(reinterpret_cast<char *>(buffer_), sizeof(buffer_t) + buffer_->name_len_);
}


inline
void NameEvent::read(std::istream &stream)
{
    buffer_t header;
    stream.read(reinterpret_cast<char *>(&header), sizeof(buffer_t));
    if (buffer_ == NULL) {
        buffer_ = reinterpret_cast<buffer_t*>(malloc(sizeof(buffer_t) + header.name_len_));
    } else if (buffer_->name_len_ < header.name_len_) {
        buffer_ = reinterpret_cast<buffer_t*>(realloc(buffer_, sizeof(buffer_t) + header.name_len_));
    }
    *buffer_ = header;
    stream.read(&buffer_->first_byte_ + 1, buffer_->name_len_);
}


inline std::ostream &operator<<(std::ostream &os, const sim::trace::NameEvent &event)
{
    if (event.get_name() == NULL) {
        return os;
    }
    os << event.get_id() << ":" << event.get_name();
    return os;
}


inline std::istream &operator>>(std::istream &is, sim::trace::NameEvent &event)
{
    sim::trace::NameEvent::buffer_t header;
    const size_t kTmpBufferSize = 256;
    char tmp_buffer[kTmpBufferSize];

    tmp_buffer[kTmpBufferSize - 1] = 0; /* Ensure EOS */
    is.getline(tmp_buffer, kTmpBufferSize - 1, ':');
    header.id_ = atoi(tmp_buffer);
    is.getline(tmp_buffer, kTmpBufferSize - 1);

    header.name_len_ = strnlen(tmp_buffer, kTmpBufferSize - 1);
    if (event.buffer_ == NULL) event.buffer_ = (NameEvent::buffer_t *) malloc(
            sizeof(NameEvent::buffer_t) + header.name_len_);
    else if (event.buffer_->name_len_ < header.name_len_) event.buffer_ = (NameEvent::buffer_t *) realloc(event.buffer_,
            sizeof(NameEvent::buffer_t) + header.name_len_);
    event.buffer_->id_ = header.id_;
    event.buffer_->name_len_ = header.name_len_;
    memcpy(&event.buffer_->first_byte_, tmp_buffer, event.buffer_->name_len_ + 1);
    event.name_ = std::string(tmp_buffer);

    return is;
}

}  // namespace trace
}  // namespace sim

#endif  // CORE_TRACE_NAMEEVENT_IMPL_H_
