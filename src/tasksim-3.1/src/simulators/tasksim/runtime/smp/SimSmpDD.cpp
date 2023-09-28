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

#include "SimSmpDD.h"

#include "simulators/tasksim/runtime/NanoxBridge.h"

#include "schedule.hpp"
#include "debug.hpp"
#include "system.hpp"

namespace nanos {
namespace ext {

SimSmpDD::SimSmpDD(task_id_t t) :
        SMPDD((work_fct)SimSmpThread::work),
        _task_id(t),
        _is_task(true)
{
}

SimSmpDD::SimSmpDD(work_fct w) :
        SMPDD(w),
        _task_id(0),
        _is_task(false)
{
}

SimSmpDD::SimSmpDD() :
        SMPDD(),
        _task_id(0),
        _is_task(false)
{
}

SimSmpDD::SimSmpDD(const SimSmpDD &dd) :
        SMPDD(dd),
        _task_id(dd._task_id)
{
}

SimSmpDD::~SimSmpDD()
{
}

SimSmpDD::task_id_t SimSmpDD::getTaskId() const
{
    return _task_id;
}

void SimSmpDD::lazyInit(WD &wd, bool isUserLevelThread, WD *previous)
{
    if (_is_task) {
        wd.setData(reinterpret_cast<void*>(_task_id));
    }
    SMPDD::lazyInit(wd, isUserLevelThread, previous);
}

size_t SimSmpDD::size()
{
    return sizeof(SimSmpDD);
}

SimSmpDD * SimSmpDD::copyTo(void *toAddr)
{
    SimSmpDD *dd = new (toAddr) SimSmpDD(*this);
    return dd;
}

}  // namespace ext
}  // namespace nanos
