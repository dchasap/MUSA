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


#include "SimSmpPlugin.h"

#include <cassert>
#include <vector>

#include "core/engine/Config.h"
#include "simulators/tasksim/runtime/smp/SimSmpProcessor.h"
#include "arch/smp/smpprocessor.hpp"
#include "arch/unix-os/osallocator_decl.hpp"
#include "arch/unix-os/os.hpp"
namespace nanos {
namespace ext {

void SimSmpPlugin::set_config_values(const sim::engine::Config& config)
{
    std::vector<unsigned> v = config.get_values<unsigned>("ncpus");
    _CPUsPerSocket = 0;
    for (unsigned cpus : v) {
      _CPUsPerSocket += cpus;
    }
    _numSockets = 1;
    _requestedWorkers = _requestedCPUs = _availableCPUs = _currentCPUs = _CPUsPerSocket * _numSockets;
}

void SimSmpPlugin::init()
{
    // At the moment we do this at the NanoxBridge
    // sys.setHostFactory( nanos::ext::tasksimProcessorFactory);
    sys.setSMPPlugin(this);

    for (int i = 0; i < _availableCPUs; i++) {
        _cpuSystemMask.set(i);
        _cpuProcessMask.set(i);
    }

    //! \note Fill _bindings vector with the active CPUs first, then the not active

    _bindings.reserve(_availableCPUs);
    for (int i = 0; i < _availableCPUs; i++) {
        if (_cpuProcessMask.isSet(i)) {
            _bindings.push_back(i);
        }
    }
    for (int i = 0; i < _availableCPUs; i++) {
        if (!_cpuProcessMask.isSet(i)) {
            _bindings.push_back(i);
        }
    }

    //! \note Load & check NUMA config (_cpus vectors must be created before)
    _cpus = NEW std::vector<SMPProcessor*>(_availableCPUs, reinterpret_cast<SMPProcessor*>(NULL));
    _cpusByCpuId = NEW std::map<int,SMPProcessor*>();

    loadNUMAInfo();

    memory_space_id_t mem_id = sys.getRootMemorySpaceId();

    //! \note Create the SMPProcessors in _cpus array
    int count = 0;
    for (std::vector<int>::iterator it = _bindings.begin(); it != _bindings.end(); it++) {
        SMPProcessor *cpu;
        bool active = ((count < _currentCPUs) && _cpuProcessMask.isSet(*it));
        unsigned numaNode;
        // If this PE can't be seen by hwloc (weird case in Altix 2, for instance)
        if (!sys._hwloc.isCpuAvailable(*it)) {
            /* There's a problem: we can't query it's numa
            node. Let's give it 0 (ticket #1090), consider throwing a warning */
            numaNode = 0;
        } else {
            numaNode = getNodeOfPE(*it);
        }
        unsigned socket = numaNode;   /* FIXME: socket */

        if (_smpPrivateMemory && count >= _smpHostCpus && !_memkindSupport) {
            OSAllocator a;
            memory_space_id_t id = sys.addSeparateMemoryAddressSpace(ext::getSMPDevice(),
                    _smpAllocWide, sys.getRegionCacheSlabSize());
            SeparateMemoryAddressSpace &numaMem = sys.getSeparateMemory(id);
            numaMem.setSpecificData(NEW SimpleAllocator((uintptr_t) a.allocate(_smpPrivateMemorySize),
                        _smpPrivateMemorySize));
            cpu = NEW SimSmpProcessor(*it, id, active, numaNode, socket);
        } else {
            cpu = NEW SimSmpProcessor(*it, mem_id, active, numaNode, socket);
        }
        if (active) {
            _cpuActiveMask.set(cpu->getBindingId());
        }
        // cpu->setNUMANode( getNodeOfPE( cpu->getId() ) );
        (*_cpus)[count] = cpu;
        (*_cpusByCpuId)[*it] = cpu;
        count += 1;
    }
}

}  // namespace ext
}  // namespace nanos
