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


#ifndef CORE_UTILS_COMPRESSEDSTREAM_H_
#define CORE_UTILS_COMPRESSEDSTREAM_H_

#include <iosfwd>
#include <memory>
#include "CompressedBuffer.h"

namespace sim {
namespace utils {

template< typename FileType>
class CompressedStream : public std::iostream {
private:
    CompressedBuffer<FileType> compressor_;

public:
    // Constructor:
    explicit CompressedStream(std::shared_ptr<FileType> &out);
    // Assignment operator
    CompressedStream& operator=(const CompressedStream &r);
    // We block the copy constructor:
    CompressedStream(const CompressedStream &r) = delete;
    // End of stream function (to flush the buffers)
    void end_stream();
    // To add the number of bytes available to read in the buffer (decompression):
    void add_data(size_t size);
    // To get an approximate number of bytes remaining (decompression).
    size_t get_available();
};

}  // namespace utils
}  // namespace sim

#include "CompressedStream-impl.h"

#endif  // CORE_UTILS_COMPRESSEDSTREAM_H_
