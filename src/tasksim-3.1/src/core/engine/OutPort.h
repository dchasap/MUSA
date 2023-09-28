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


#ifndef CORE_ENGINE_OUTPORT_H_
#define CORE_ENGINE_OUTPORT_H_

#include <list>

namespace sim {
namespace engine {

template<typename> class InPort;
class Module;

/** \brief A write-only port
 *
 * This class implements an output connector for a Module
 */
template<typename T>
class OutPort {
protected:
    /** Port message width */
    unsigned max_size_;
    /** Output value */
    std::list<T> value_;

    /** Input port where this port has been connected */
    InPort<T> *peer_;

    /** Module where the port is attached */
    Module &owner_;

    /** Control variable to know if the port has been connected */
    bool connected_;

    /** Wake up on read **/
    bool wake_up_on_read_;

    friend class InPort<T>;
public:
    /** Default constructor
     * \param owner Module where the port is attached
     */
    explicit OutPort(Module &owner, unsigned size = 1);

    /** Default destructor */
    virtual ~OutPort();

    /** Check if the port is empty */
    bool is_empty() const;

    /** Check if the port is full */
    bool is_full() const;

    /** Returns and rvalue T */
    T&& get();

    /** Write a value to the output buffer with move semantics
     * \param value Value to be written to the output buffer
     * \return Whether the value was written to the output buffer or not
     */
    bool write(T value);

    /** Removes one element from the port */
    void consume();

    /** Connect to an input port
     * \param in_port Input port to connect
     */
    void operator >>(InPort<T> &in_port);

    /** Check if the port is connected
     * \return Whether the port has been connected or not
     */
    bool is_connected() const;

    /** Get the peer module connected through this port */
    engine::Module &get_peer();

    /** Sets if a module connected through this port should wake up on read */
    void set_wake_up_on_read(bool wake_up_on_read);

    unsigned get_max_size() const;
    void set_max_size(unsigned size);
};

}  // namespace engine
}  // namespace sim

#include "OutPort-impl.h"

#endif  // CORE_ENGINE_OUTPORT_H_
