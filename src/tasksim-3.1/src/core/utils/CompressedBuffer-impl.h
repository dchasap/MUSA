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


#ifndef CORE_UTILS_COMPRESSEDBUFFER_IMPL_H_
#define CORE_UTILS_COMPRESSEDBUFFER_IMPL_H_

#include <algorithm>

namespace sim {
namespace utils {

template< typename FileType>
inline
CompressedBuffer<FileType>::CompressedBuffer(std::shared_ptr<FileType> &buffer, int compression_level) :
        compressed_buffer_(buffer), compressor_(), compression_level_(compression_level),
        update_read_(false), update_write_(false), readable_bytes_(0),
        comp_read_buffer_(new char[Chunk_+1]), comp_write_buffer_(new char[Chunk_+1])
{
    compressor_.next_in = reinterpret_cast<unsigned char*>(comp_read_buffer_);
    compressor_.next_out = reinterpret_cast<unsigned char*>(comp_write_buffer_);
    compressor_.avail_in = 0;
    compressor_.avail_out = Chunk_;
    compressor_.zalloc = Z_NULL;
    compressor_.zfree = Z_NULL;
    compressor_.opaque = Z_NULL;
    // Setting input buffer:
    setp(comp_read_buffer_, comp_read_buffer_ + Chunk_);
}

template< typename FileType>
inline
CompressedBuffer<FileType>::CompressedBuffer(const CompressedBuffer<FileType>& r) :
        compressed_buffer_(r.compressed_buffer_), compressor_(),
        compression_level_(r.compression_level_),
        update_read_(r.update_read_), update_write_(r.update_write_),
        readable_bytes_(r.readable_bytes_),
        comp_read_buffer_(new char[Chunk_+1]),
        comp_write_buffer_(new char[Chunk_+1])
{
    compressor_.next_in = reinterpret_cast<unsigned char*>(comp_read_buffer_);
    compressor_.next_out = reinterpret_cast<unsigned char*>(comp_write_buffer_);
    compressor_.avail_in = 0;
    compressor_.avail_out = Chunk_;
    compressor_.zalloc = Z_NULL;
    compressor_.zfree = Z_NULL;
    compressor_.opaque = Z_NULL;
    // Copy the buffers contents?
    setp(comp_read_buffer_, comp_read_buffer_ + Chunk_);
}

template< typename FileType>
inline
CompressedBuffer<FileType>::~CompressedBuffer()
{
    end_stream();
    delete[] comp_read_buffer_;
    delete[] comp_write_buffer_;
}

template<>
inline
void CompressedBuffer<std::fstream>::end_stream()
{
    if (update_read_) {
        // We close the inflate buffer:
        inflateEnd(&compressor_);
        update_read_ = false;
        readable_bytes_ = 0;
    }
    if (pbase() != pptr()) {
        // In case there is some data left to deflate:
        overflow(traits_type::eof());
    }
    if (update_write_) {
        // We compress all that is left:
        size_t total_out = 0;
        int deflate_res = Z_OK;
        // While there is something left to compress:
        while (deflate_res == Z_OK) {
            // We only write if the buffer is full (to reduce calls)
            if (compressor_.avail_out == 0) {
                // We write it out to the file:
                compressed_buffer_->write(reinterpret_cast<const char*>(
                                          &comp_write_buffer_[0]), Chunk_);
                // We mark the compression output buffer as empty
                total_out += Chunk_;
                compressor_.next_out = reinterpret_cast<unsigned char*>(comp_write_buffer_);
                compressor_.avail_out = Chunk_;
            }
            // And continue compressing.
            deflate_res = deflate(&compressor_, Z_FINISH);
        }
        // We check that all ended properly:
        if (deflate_res != Z_STREAM_END) {
            std::cout << "Error while flushing the compression buffer." << std::endl;
            std::abort();
        }
        // We check if there is something left to write out.
        if (Chunk_ > compressor_.avail_out) {
            compressed_buffer_->write(reinterpret_cast<const char *>(&comp_write_buffer_[0]),
                                      Chunk_ - compressor_.avail_out);
            total_out += Chunk_ - compressor_.avail_out;
            compressor_.next_out = reinterpret_cast<unsigned char*>(comp_write_buffer_);
            compressor_.avail_out = Chunk_;
        }
        // We close the deflate buffer:
        deflateEnd(&compressor_);
        update_write_ = false;
    }
}

template<>
inline
void CompressedBuffer<std::ifstream>::end_stream()
{
    if (update_read_) {
        // We do not need to finish reading the buffer.
        inflateEnd(&compressor_);
        update_read_ = false;
    }
}

template< typename FileType>
inline
void CompressedBuffer<FileType>::end_stream()
{
    // This should never be called.
    std::cout << "End unknown type." << std::endl;
    return;
}

template< typename FileType>
inline
void CompressedBuffer<FileType>::add_data(size_t size)
{
    if (readable_bytes_ == 0 && size != 0) {
        readable_bytes_ = size;
        if (update_read_) {
            // We End the inflating process of the previous chunk.
            inflateEnd(&compressor_);
            update_read_ = false;
        }
        // And in case that we had some previous request that had left eof on:
        // underflow();
    } else {
        readable_bytes_ = size;
    }

    return;
}

template< typename FileType>
inline
size_t CompressedBuffer<FileType>::get_available()
{
    // We calculate unread bytes + compressed bytes + decompressed bytes left to be read:
    // This is not exact but will behave as empty()
    size_t total = readable_bytes_ + compressor_.avail_in + (egptr() - gptr());
    return total;
}

template<typename FileType>
inline
typename CompressedBuffer<FileType>::int_type CompressedBuffer<FileType>::underflow()
{
    assert(gptr() >= egptr());
    // We check if the buffer is in an uninitialized state.
    // It might not be just after class creation as streams can have several
    // segments, so we have to check.
    if (!update_read_) {
        compressor_.avail_out = Chunk_;
        compressor_.next_out = reinterpret_cast<unsigned char*>(comp_write_buffer_);
        compressor_.avail_in = 0;
        compressor_.next_in = reinterpret_cast<unsigned char*>(comp_read_buffer_);
        int res = inflateInit(&compressor_);
        catch_error(res);
        update_read_ = true;
    }
    if (egptr() == gptr()) {
        compressor_.avail_out = Chunk_;
        compressor_.next_out = reinterpret_cast<unsigned char*>(comp_write_buffer_);
    } else {
        // This should never happen, NEVER.
        std::cout << "Error: still some data left to be read." << std::endl;
        std::cout << (egptr() - gptr() ) << " bytes decompressed and "
                  << readable_bytes_ << " on buffer." << std::endl;
        return traits_type::to_int_type(*gptr());
    }

    if (compressor_.avail_in != 0) {
        // Inflating remaining data
        int res = inflate(&compressor_, Z_NO_FLUSH);
        catch_error(res);
    }
    // Read as much as you can until you fill the buffer or run out of data.
    while (compressor_.avail_in == 0 && readable_bytes_ != 0 && compressor_.avail_out != 0) {
        size_t read_size = std::min(Chunk_, readable_bytes_);
        compressor_.avail_in = read_size;
        compressor_.next_in = reinterpret_cast<unsigned char*>(comp_read_buffer_);
        compressed_buffer_->read(comp_read_buffer_, read_size);
        readable_bytes_ -= read_size;
        int res = inflate(&compressor_, Z_NO_FLUSH);
        // Error check:
        catch_error(res);
    }
    if (compressor_.avail_in == 0 && readable_bytes_ == 0 && compressor_.avail_out == Chunk_) {
        // No input nor output left.
        return traits_type::eof();
    }

    // We make sure that we have some data:
    if (compressor_.avail_out != Chunk_) {
        setg(comp_write_buffer_, comp_write_buffer_, reinterpret_cast<char*>(compressor_.next_out));
        return traits_type::to_int_type(*gptr());
    } else {
        // Extra check, we should never end up here:
        std::cout << "Error: could not inflate data." << std::endl;
        std::cout << readable_bytes_ << " " << compressor_.avail_in << " "
                  << compressor_.avail_out << std::endl;
        std::abort();
    }
    return traits_type::eof();
}

template<typename FileType>
inline
typename CompressedBuffer<FileType>::int_type CompressedBuffer<FileType>::overflow(int c)
{
    if (sync() == -1) return traits_type::eof();
    if (traits_type::eq_int_type(c, traits_type::eof()) == false) {
        traits_type::assign(*pptr(), c);
        pbump(1);
    }
    return traits_type::not_eof(c);
}

template<>
inline
int CompressedBuffer<std::fstream>::sync(void)
{
    // Which is basically write the output buffer to the compressed file:
    ssize_t total_compressed = 0;
    ssize_t count = pptr() - pbase();
    if (compressor_.avail_in != 0) {
        std::cout << "Error: had some residue in the buffer." << std::endl;
        std::cout << update_write_ << std::endl;
        std::abort();
    }
    if (!update_write_) {
        // Initializing compressor.
        compressor_.next_out = reinterpret_cast<unsigned char*>(comp_write_buffer_);
        compressor_.avail_out = Chunk_;
        compressor_.next_in = reinterpret_cast<unsigned char*>(comp_read_buffer_);
        compressor_.avail_in = 0;
        deflateInit(&compressor_, compression_level_);
        update_write_ = true;
    }

    compressor_.next_in = reinterpret_cast<unsigned char*>(pbase());
    compressor_.avail_in = count;

    while (compressor_.avail_in != 0) {
        // Deflating.
        int res = deflate(&compressor_, Z_NO_FLUSH);
        assert(res == Z_OK);
        catch_error(res);
        if (compressor_.avail_out == 0) {
            // Buffer full.
            compressed_buffer_->write(reinterpret_cast<const char *>(
                                      &comp_write_buffer_[0]), Chunk_);
            total_compressed += Chunk_;
            compressor_.next_out = reinterpret_cast<unsigned char*>(comp_write_buffer_);
            compressor_.avail_out = Chunk_;
            // Emptied.
        }
    }

    pbump(-count);
    return 0;
}

template<>
inline
int CompressedBuffer<std::ifstream>::sync(void) {
    // Sync on a ifstream.
    return -1;
}

template<>
inline
int CompressedBuffer<std::ofstream>::sync(void) {
    // Sync on a ofstream.
    return -1;
}

template< typename FileType>
inline
int CompressedBuffer<FileType>::sync(void) {
    // Sync on an unknown type.
    return -1;
}

template<>
inline
std::streampos CompressedBuffer<std::ifstream>::seekpos(std::streampos pos,
        std::ios_base::openmode which)
{
    compressed_buffer_->seekg(pos);
    return compressed_buffer_->tellg();
}

template<>
inline
std::streampos CompressedBuffer<std::ofstream>::seekpos(std::streampos pos,
        std::ios_base::openmode which)
{
    compressed_buffer_->seekp(pos);
    return compressed_buffer_->tellp();
}


template< typename FileType>
inline
std::streampos CompressedBuffer<FileType>::seekpos(std::streampos pos,
        std::ios_base::openmode which)
{
    return pos;
}

template<>
inline
std::streampos CompressedBuffer<std::ifstream>::seekoff(std::streamoff off,
        std::ios_base::seekdir way,
        std::ios_base::openmode which)
{
    return compressed_buffer_->tellg();
}

template<>
inline
std::streampos CompressedBuffer<std::ofstream>::seekoff(std::streamoff off,
        std::ios_base::seekdir way,
        std::ios_base::openmode which)
{
    return compressed_buffer_->tellp();
}

template< typename FileType>
inline
std::streampos CompressedBuffer<FileType>::seekoff(std::streamoff off,
        std::ios_base::seekdir way,
        std::ios_base::openmode which)
{
    // We should not need this.
    std::abort();
    return -1;
}

template< typename FileType>
inline
void CompressedBuffer<FileType>::catch_error(int z_error_code)
{
    if (z_error_code == Z_OK || z_error_code == Z_STREAM_END) {
        // Nothing wrong happened.
        return;
    }
    std::cout << "Unexpected error while compressing/decompressing a file with zlib." << std::endl;
    switch (z_error_code) {
        case Z_NEED_DICT:
            std::cout << "Decompression dictionary not found." << std::endl;
            break;
        case Z_BUF_ERROR:
            std::cout << "Buffer errror" << std::endl;
            break;
        case Z_ERRNO:
            std::cout << "Error reading/writing the contents." << std::endl;
            break;
        case Z_STREAM_ERROR:
            std::cout << "Invalid compression level" << std::endl;
            break;
        case Z_DATA_ERROR:
            std::cout << "Invalid or incomplete deflate data" << std::endl;
            break;
        case Z_MEM_ERROR:
            std::cout << "Out of memory" << std::endl;
            break;
        case Z_VERSION_ERROR:
            std::cout << "Zlib version mismatch" << std::endl;
            break;
        default:
            std::cout << "Unexpected error. " << z_error_code << std::endl;
            break;
    }
    std::abort();
}

}  // namespace utils
}  // namespace sim

#endif  // CORE_UTILS_COMPRESSEDBUFFER_IMPL_H_
