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

#ifndef SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPDD_H_
#define SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPDD_H_

#include <cstdint>

#include "memtracker.hpp"
#include "workdescriptor.hpp"
#include "smpdd.hpp"

#include "simulators/tasksim/runtime/smp/SimSmpThread.h"

namespace nanos {
namespace ext {

class SimSmpDD : public SMPDD
{
public:
     typedef int task_id_t;

private:
     task_id_t _task_id;
     bool _is_task;

public:
     // constructors
     explicit SimSmpDD(task_id_t t);
     explicit SimSmpDD(work_fct w);
     SimSmpDD();

     // copy constructors
     SimSmpDD(const SimSmpDD &dd);

     // assignment operator
     const SimSmpDD & operator=(const SimSmpDD &wd);
     // destructor

     virtual ~SimSmpDD();

     task_id_t getTaskId() const;

     virtual void lazyInit(WD &wd, bool isUserLevelThread, WD *previous);
     virtual size_t size();
     virtual SimSmpDD *copyTo(void *addr);
};

inline const SimSmpDD & SimSmpDD::operator=(const SimSmpDD &dd)
{
    // self-assignment: ok
    if ( &dd == this ) {
        return *this;
    }

    DD::operator=(dd);

    _task_id = dd._task_id;

    return *this;
}

}  // namespace ext
}  // namespace nanos

#endif  // SIMULATORS_TASKSIM_RUNTIME_SMP_SIMSMPDD_H_
