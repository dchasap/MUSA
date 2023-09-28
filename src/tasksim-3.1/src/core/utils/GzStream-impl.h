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


#ifndef UTILS_GZSTREAM_IMPL_H
#define UTILS_GZSTREAM_IMPL_H

#include <string>
#include "GzFile.h"

namespace sim {
namespace utils {

inline GzIStream::GzIStream(const std::string &path) :
        std::istream(NULL), gz_file_(path, "r"), gz_buffer_(gz_file_)
{
    rdbuf(&gz_buffer_);
}

inline GzOStream::GzOStream(const std::string &path) :
        std::ostream(NULL), gz_file_(path, "w"), gz_buffer_(gz_file_)
{
    rdbuf(&gz_buffer_);
}

inline
GzFStream::GzFStream(const std::string &path, const openmode &mode) :
    std::iostream(NULL),
    gz_file_(new GzFile(path, mode_transform(mode).c_str())),
    gz_buffer_(new GzBuffer(*gz_file_.get()))
{
    if (gz_file_->is_open()) {
        std::iostream::rdbuf(gz_buffer_.get());
    }
}

inline
void GzFStream::open(const char* filename, std::ios_base::openmode mode)
{
    if ( gz_file_.get() != NULL) {
      close();
    }
    gz_file_ = std::unique_ptr<GzFile>(new GzFile(filename, mode_transform(mode).c_str()));
    gz_buffer_ = std::unique_ptr<GzBuffer>(new GzBuffer(*gz_file_.get()));
    if (gz_file_->is_open()) {
        std::iostream::rdbuf(gz_buffer_.get());
    }
}

inline
bool GzFStream::is_open()
{
    if (gz_file_.get() != NULL) {
        return gz_file_->is_open();
    } else {
        return false;
    }
}

inline
void GzFStream::close()
{
    // Remmeber to destroy the buffer first.
    gz_buffer_.reset();
    gz_file_.reset();
}

inline
static const std::string mode_transform(const std::ios::openmode mode)
{
    std::string ret;
    if ((mode & std::ios::in) && (mode & std::ios::out)) {
        ret = "rw";
    } else if (mode & std::ios::in ) {
        ret = "r";
    } else if ((mode & std::ios::out) && (mode & std::ios::app)) {
        ret = "a";
    } else if ((mode & std::ios::out) && (mode & std::ios::trunc)) {
        ret = "w";
    } else if (mode & std::ios::out) {
        ret = "a";
    }
    return ret;
}

}  // namespace utils
}  // namespace sim

#endif
