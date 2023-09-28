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


#ifndef CORE_UTILS_COMPRESSEDSTREAM_IMPL_H_
#define CORE_UTILS_COMPRESSEDSTREAM_IMPL_H_

namespace sim {
namespace utils {

template<typename FileType>
CompressedStream<FileType>::CompressedStream(std::shared_ptr<FileType> &out) :
        compressor_(out)
{
    this->rdbuf(&compressor_);
}

template<typename FileType>
CompressedStream<FileType>& CompressedStream<FileType>::operator=(const CompressedStream<FileType> &r)
{
    compressor_ = r.compressor_;
    return *this;
}


template< typename FileType>
void CompressedStream<FileType>::end_stream()
{
    compressor_.end_stream();
}

template< typename FileType>
void CompressedStream<FileType>::add_data(size_t size)
{
    compressor_.add_data(size);
}

template< typename FileType>
size_t CompressedStream<FileType>::get_available()
{
    return compressor_.get_available();
}

}  // namespace utils
}  // namespace sim

#endif  // CORE_UTILS_COMPRESSEDSTREAM_IMPL_H_
