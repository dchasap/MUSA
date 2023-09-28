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


#ifndef UTILS_GZSTREAM_H
#define UTILS_GZSTREAM_H

#include <iosfwd>
#include <memory>

#include "NonCopyable.h"
#include "GzFile.h"
#include "GzBuffer.h"

namespace sim {
namespace utils {

class GzBuffer;
class GzFile;

class GzIStream : public NonCopyable, public std::istream {

private:
    GzFile gz_file_;
    GzBuffer gz_buffer_;

public:
    GzIStream(const std::string &);

};

class GzOStream : public NonCopyable, public std::ostream {

private:
    GzFile gz_file_;
    GzBuffer gz_buffer_;

public:
    GzOStream(const std::string &);

};

/** GzFStream implements a compressed file stream. May be used instead
 * of fstream for compressed files.
 */
class GzFStream : public NonCopyable, public std::iostream {

private:
    /** Unique pointer to a gz_file instance */
    std::unique_ptr<GzFile> gz_file_;
    /** Unique pointer to a gz_buffer instance */
    std::unique_ptr<GzBuffer> gz_buffer_;

public:
    /** Creator that automatically opens a compressed file */
    GzFStream(const std::string &, const openmode&);
    /** Open function, same header as std::fstream */
    void open(const char* filename,
               std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    /** Function to check if there is an asociated GzFile opened. */
    bool is_open();
    /** Function to close the asociated file and buffer */
    void close();

};

static const std::string mode_transform(const std::ios::openmode mode);

}  // namespace utils
}  // namespace sim

#include "GzStream-impl.h"

#endif
