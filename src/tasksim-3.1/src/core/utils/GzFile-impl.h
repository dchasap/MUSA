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


#ifndef UTILS_GZFILE_IMPL_H
#define UTILS_GZFILE_IMPL_H

#include <string>
#include <zlib.h>

namespace sim {
namespace utils {

inline GzFile::GzFile(const std::string &name, const char *mode)
{
    gz_file_ = gzopen(name.c_str(), mode);
}

inline GzFile::~GzFile()
{
    if (gz_file_ != NULL) gzclose(gz_file_);
}

inline
bool GzFile::eof(void)
{
    return gzeof(gz_file_);
}

inline
void GzFile::gets(char *buf, size_t buflen)
{
    gzgets(gz_file_, buf, buflen);
}

inline ssize_t GzFile::read(void *buf, size_t buflen)
{
    ssize_t read_size = gzread(gz_file_, buf, buflen);
    return read_size;
}

inline ssize_t GzFile::write(void *buf, size_t buflen)
{
    return gzwrite(gz_file_, buf, buflen);
}

inline ssize_t GzFile::seek(ssize_t offset)
{
    return gzseek(gz_file_, offset, SEEK_SET);
}

inline ssize_t GzFile::tell()
{
    return gztell(gz_file_);
}

inline bool GzFile::is_open()
{
    return (gz_file_ != NULL);
}

inline void GzFile::flush()
{
    gzflush(gz_file_, 0);
}

}
}

#endif
