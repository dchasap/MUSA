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


#ifndef UTILS_GZBUFFER_H
#define UTILS_GZBUFFER_H

#include <streambuf>
#include <iostream>

#include "NonCopyable.h"

namespace sim {
namespace utils {



class GzFile;

class GzBuffer: public NonCopyable, public std::streambuf {
private:
    /** File where data is read from/ written to */
    GzFile &gz_file_;

    /** Size of the buffer */
    std::size_t buffer_size_;

    /** Buffer for read data */
    char *read_buffer_;

    /** Buffer for write data */
    char *write_buffer_;

protected:
    static const int Default_Buffer_Size_ = 8*1024;
    /** Virtual functions overriden from streambuf: */
    virtual int underflow(void);
    virtual int overflow(int c);
    virtual int sync(void);
    virtual std::streampos seekpos(std::streampos pos,
            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
    virtual std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
public:
    explicit GzBuffer(GzFile &, std::size_t buffer_size = Default_Buffer_Size_);
    ~GzBuffer(void);
};

}
}

#endif
