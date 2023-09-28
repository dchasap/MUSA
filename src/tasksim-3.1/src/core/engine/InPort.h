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


#ifndef CORE_ENGINE_INPORT_H_
#define CORE_ENGINE_INPORT_H_


namespace sim {
namespace engine {

class Module;
template<typename T> class OutPort;

template<typename T>
class InPort {
protected:
    /** Output port where this port is being connected */
    OutPort<T> *peer_;

    /** Module where the port is attached */
    Module &owner_;

    template<typename> friend class OutPort;

public:
    /** Default constructor */
    explicit InPort(Module &owner);

    /** Default destructor */
    virtual ~InPort();

    /** Check if the peer OutPort has got a Message
     * \return Whether a valid value can be read from the port
     */
    bool can_read();

    /** Read data from the port
     * \param value Reference where to store the read data
     * \return Whether a valid value was read from the port
     */
    bool read(T &value);

    /** Read data from the port and appends it to a container
     * \param container Container where the data is appended
     * \return Whether a valid value was read from the port
     */
    template<typename Container, typename... Args>
    bool read(Container &container, const Args &...);

    /** Connect port to an output port
     * \param peer Output port to connect
     */
    void operator <<(OutPort<T> &peer);

    /** Get the output port connected through this port */
    OutPort<T>* get_connected_port();
};

}  // namespace engine
}  // namespace sim

#include "InPort-impl.h"

#endif  // CORE_ENGINE_INPORT_H_
