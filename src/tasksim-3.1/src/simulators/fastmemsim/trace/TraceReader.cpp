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


#include "TraceReader.h"

namespace sim {
namespace trace {
namespace fastmem {

TraceReader::TraceReader(const std::string &trace_file) : in_stream_(trace_file) {}

fastmem::mem_t TraceReader::getNextMem()
{
    fastmem::mem_t mem;
    addr_t addr_temp;
    unsigned char byte;

    in_stream_ >> std::noskipws >> byte;

    if (in_stream_.eof())
        return mem;

    mem.eob_ = ((byte & (0x01 << 7)) != 0);

    if (!mem.eob_)
    {
        if ((byte & (0x01 << 6)) == 0)
            mem.op_ = fastmem::mem_t::op_t::READ;
        else
            mem.op_ = fastmem::mem_t::op_t::WRITE;

        int addr_length = (byte & (0x07 << 3)) >> 3;
        int ins_length = byte & 0x07;

        mem.addr_ = 0;
        for (int i = 0; i <= addr_length; i++)
        {
            in_stream_ >> std::noskipws >> byte;
            addr_temp = byte;
            mem.addr_ = mem.addr_ | (addr_temp << (i * 8));
        }

        mem.size_ = 8;

        mem.delay_ = 0;
        for (int i = 0; i <= ins_length; i++)
        {
            in_stream_ >> std::noskipws >> byte;
            addr_temp = byte;
            mem.delay_ = mem.delay_ | (addr_temp << (i * 8));
        }
    }
    return mem;
}

bool TraceReader::eof()
{
    return in_stream_.eof();
}

}
}
}
