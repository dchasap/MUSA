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

#include "GzBuffer.h"
#include <cassert>
#include "GzFile.h"

namespace sim {
namespace utils {

GzBuffer::GzBuffer(GzFile& gz_file, std::size_t buffer_size) :
        gz_file_(gz_file), buffer_size_(buffer_size), read_buffer_(new char[buffer_size_]), write_buffer_(
                new char[buffer_size_])
{
    setp(write_buffer_, write_buffer_ + buffer_size_);
}

GzBuffer::~GzBuffer()
{
    sync();
    delete[] read_buffer_;
    delete[] write_buffer_;
}

GzBuffer::int_type GzBuffer::underflow(void)
{
    assert(gptr() >= egptr());

    ssize_t count = gz_file_.read(read_buffer_, buffer_size_);
    if (count > 0) {
        setg(read_buffer_, read_buffer_, read_buffer_ + count);
        return traits_type::to_int_type(*gptr());
    }
    return traits_type::eof();
}

GzBuffer::int_type GzBuffer::overflow(int c)
{
    assert(pptr() >= epptr());
    /** sync() should reposition the pointer to the buffer */
    if (sync() == -1) return traits_type::eof();
    if (traits_type::eq_int_type(c, traits_type::eof()) == false) {
        traits_type::assign(*pptr(), c);
        pbump(1);
    }
    return traits_type::not_eof(c);
}

int GzBuffer::sync(void)
{
    ssize_t count = pptr() - pbase();
    if (gz_file_.write(pbase(), count) == count) {
        pbump(-count);
        return 0;
    }
    return -1;
}

std::streampos GzBuffer::seekpos(std::streampos pos, std::ios_base::openmode which)
{
    if (which & std::ios_base::in) {
        ssize_t current_buffer_size =  (egptr() - eback());
        ssize_t initial_buffer_position = gz_file_.tell() - current_buffer_size;
        ssize_t current_buffer_position = (gptr() - eback());
        ssize_t pointer = pos - initial_buffer_position;
        if (pointer != current_buffer_position) {
            if (pointer >= 0 && pointer < static_cast<ssize_t>(buffer_size_)) {
                setg(eback(), eback() + pointer, egptr());
            } else {
                gz_file_.seek(pos);
                setg(read_buffer_, read_buffer_ + buffer_size_, read_buffer_ + buffer_size_);
            }
        }
    }
    if (which & std::ios_base::out) {
        sync();
        gz_file_.seek(pos);
    }
    return pos;
}

std::streampos GzBuffer::seekoff(std::streamoff off, std::ios_base::seekdir way,
        std::ios_base::openmode which)
{
    if ((way == std::ios_base::cur) && (off == 0)) {
        // This is only a tell:
        ssize_t current_buffer_size =  (egptr() - eback());
        ssize_t initial_buffer_position = gz_file_.tell() - current_buffer_size;
        ssize_t current_file_pos = initial_buffer_position + (gptr() - eback());
        return current_file_pos;
    } else if (way == std::ios_base::cur) {
        // We want to do a relative move:
        ssize_t current_buffer_size =  (egptr() - eback());
        ssize_t initial_buffer_position = gz_file_.tell() - current_buffer_size;
        ssize_t current_file_pos = initial_buffer_position + (gptr() - eback());
        return seekpos(off + current_file_pos, which);
    } else if(way == std::ios_base::beg) {
        // Absolute move.
        return seekpos(off, which);
    } else {
        // Libz does not support std::ios_base::end seeks
        return -1;
    }
}

}  // namespace utils
}  // namespace sim
