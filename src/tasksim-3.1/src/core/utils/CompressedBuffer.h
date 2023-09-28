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


#ifndef CORE_UTILS_COMPRESSEDBUFFER_H_
#define CORE_UTILS_COMPRESSEDBUFFER_H_


#include <iosfwd>
#include "zlib.h"

namespace sim {
namespace utils {

template<typename FileType>
class CompressedBuffer : public std::streambuf {
private:
    // Pointer to the buffer that handles the compressed file (both for input and output operations).
    std::shared_ptr<FileType> compressed_buffer_;
    // zlib compression/decompression buffer:
    z_stream compressor_;
    // Compression level:
    int compression_level_;
    // Intermidiate buffer size:
    static const size_t Chunk_ = 1024;
    // Flags to signal if the buffers will need cleaning on sync()
    bool update_read_;
    bool update_write_;
    // Remaining readable bytes on input buffer (decompression).
    size_t readable_bytes_;
    // Intermediate buffers:
    char* comp_read_buffer_;
    char* comp_write_buffer_;
    // Function to catch zlib errors:
    void catch_error(int z_error_code);

protected:
    /** Virtual functions overriden from streambuf: */
    virtual int underflow(void);
    virtual int overflow(int c);
    virtual int sync(void);
    virtual std::streampos seekpos(std::streampos pos,
            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
    virtual std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);

public:
    // Constructor:
    CompressedBuffer(std::shared_ptr<FileType> &buffer,
                     int compression_level = Z_DEFAULT_COMPRESSION);
    // Copy constructor:
    CompressedBuffer(const CompressedBuffer& r);
    // We block the assignment operator:
    CompressedBuffer& operator=( const CompressedBuffer& r) = delete;
    // Destructor
    ~CompressedBuffer();
    // End of stream function (to flush the buffers)
    void end_stream();
    // To add the number of bytes available to read in the buffer (decompression):
    void add_data(size_t size);
    // To get an approximate number of bytes remaining (decompression).
    size_t get_available();
};

// Static Variable definition:
template<typename FileType>
const size_t CompressedBuffer<FileType>::Chunk_;

}  // namespace utils
}  // namespace sim

#include "CompressedBuffer-impl.h"

#endif  // CORE_UTILS_COMPRESSEDBUFFER_H_
