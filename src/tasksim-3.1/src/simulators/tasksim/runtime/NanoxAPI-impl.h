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


#ifndef SIMULATORS_TASKSIM_RUNTIME_NANOXAPI_IMPL_H_
#define SIMULATORS_TASKSIM_RUNTIME_NANOXAPI_IMPL_H_

#include <vector>
#include <utility>
#include "nanos.h"
#include "simulators/tasksim/runtime/smp/SimSmpDD.h"

// These macros are to enable backwards compatibility with older versions of NANOS++
// Set NANOS_LEGACY_ENABLED to 0 to use the last version
#define NANOS_LEGACY_ENABLED 0
// #define NANOS_0_6_SUPPORT_ENABLED 0

namespace sim {
namespace runtime {

#if not NANOS_LEGACY_ENABLED

void * nanos_tasksim_factory(void *args)
{
    nanos_tasksim_args_t *tasksim = reinterpret_cast<nanos_tasksim_args_t*>(args);
    return ( void * )new nanos::ext::SimSmpDD( tasksim->task_id );
}

// Helper functions
inline
unsigned compute_total_dimensions(const std::vector<sim::trace::ts::dep_t> &deps)
{
    unsigned num_dimensions = 0;
    for (std::vector<sim::trace::ts::dep_t>::const_iterator it = deps.cbegin(); it != deps.cend(); it++) {
        num_dimensions += it->dimensions.size();
    }
    return num_dimensions;
}

inline
void dep_t2nanos_data_access_t(const std::vector<sim::trace::ts::dep_t> &deps,
                               nanos_data_access_t *data_accesses,
                               unsigned int num_deps, nanos_region_dimension_t  *my_dimensions)
{
    // The dimensions for all dependencies or regions. Each region can
    // have multiple dimensions. We store pairs of a pointer to the first
    // element of a dimension array and the number of dimensions in this
    // array.
    std::vector<std::pair<nanos_region_dimension_t*, unsigned int> > dimensions;
    // For each dependency copy all dimensions
    // Iterate over all dependencies
    int c = 0;
    for (unsigned int i = 0; i < num_deps; ++i) {
        for (unsigned int j = 0; j < deps[i].dimensions.size(); ++j) {
            my_dimensions[c++] = deps[i].dimensions[j];
        }
        dimensions.push_back(std::make_pair(&(my_dimensions[0]), deps[i].dimensions.size()));
    }

    // The data accesses for all dependencies or regions.
    for (unsigned int i = 0; i < num_deps; ++i) {
        data_accesses[i].address = deps[i].address;
        data_accesses[i].dimension_count = dimensions[i].second;
        data_accesses[i].dimensions = dimensions[i].first;
        data_accesses[i].flags = deps[i].flags;
        data_accesses[i].offset = deps[i].offset;
    }
}

inline
void set_copies(const std::vector<sim::trace::ts::dep_t> &deps, nanos_copy_data_t *copies,
                nanos_region_dimension_internal_t * copy_dims, unsigned int num_copies)
{
    assert(num_copies == deps.size());
    unsigned dims_offset = 0;
    for (unsigned i = 0; i < num_copies; i++) {
        copies[i].address = deps[i].address;
        copies[i].sharing = NANOS_SHARED;  // NANOS_PRIVATE
        copies[i].flags.input = deps[i].flags.input;
        copies[i].flags.output = deps[i].flags.output;
        copies[i].dimension_count = deps[i].dimensions.size();
        for (short j = 0; j < copies[i].dimension_count; j++) {
            copy_dims[dims_offset+j].size = deps[i].dimensions[j].size;
            copy_dims[dims_offset+j].lower_bound = deps[i].dimensions[j].lower_bound;
            copy_dims[dims_offset+j].accessed_length = deps[i].dimensions[j].accessed_length;
        }
        copies[i].dimensions = &copy_dims[dims_offset];
        copies[i].offset = deps[i].offset;
        dims_offset += copies[i].dimension_count;
    }
}

inline
void NanoxAPI::createTask(unsigned int task_id, const std::vector<sim::trace::ts::dep_t> &deps, bool copy_deps)
{
    // CAA removed
    const unsigned int num_deps = deps.size();
    // this is to work with software prefetch (maybe enable it only if set in config file?):
    const unsigned int num_copies = copy_deps ? num_deps : 0;
    const unsigned int total_dimensions = copy_deps ? compute_total_dimensions(deps) : 0;
    nanos_tasksim_args_t task_args = {task_id};
    // Setting the parameters to call nanos_create_wd_compact()
    // This is copied from the intermediate output of Mercurium,
    // with slight modifications.
    struct nanos_const_wd_definition_local_t {
        nanos_const_wd_definition_t base;
        nanos_device_t devices[1];
    };
    struct nanos_const_wd_definition_local_t _const_def = {
        {
            {
                0,  // mandatory creation
                0,  // tied
                0,  // reserved0
                0,  // reserved1
                0,  // reserved2
                0,  // reserved3
                0,  // reserved4
                0   // reserved5
            },
            1,                 // data alignment, must not be zero (segmentation fault if zero)
            num_copies,        // num_copies
            1,                 // num devices
            total_dimensions,  // the total num of dims for all deps
            NULL
        },
        {{
            nanos_tasksim_factory,
            &task_args
        }}
    };

    nanos_wd_t wd = (nanos_wd_t) 0;
    nanos_wd_dyn_props_t dyn_props = { {0}, 0, 0};
    // nanos_wd_dyn_props_t dyn_props = {0,0};
    nanos_err_t err;
    // these two arrays are allocated inside nanos_create_wd_compact (if the WD is created)
    nanos_copy_data_t *copy_data = NULL;
    nanos_region_dimension_internal_t *copy_dims_buffer = NULL;

    // The next if-else is done because Nanos++ expects a null-pointer if there
    // are no copies, instead of a pointer to null. (will fail an assert).
    if (num_copies > 0) {
        err = nanos_create_wd_compact(
            &wd,                             // Pointer to work descriptor
            &_const_def.base,                // Work descriptor definition
            &dyn_props,                      // Dynamic work descriptor properties
            0,  // sizeof(_nx_data_env_0_t), // Data size
            NULL,  // &ol_args,              // Pointer to data
            nanos_current_wd(),              // Work group
            &copy_data,                      // Copies
            &copy_dims_buffer);              // Copy dimensions
    } else {
        err = nanos_create_wd_compact(
            &wd,                             // Pointer to work descriptor
            &_const_def.base,                // Work descriptor definition
            &dyn_props,                      // Dynamic work descriptor properties
            0,  // sizeof(_nx_data_env_0_t), // Data size
            NULL,  // &ol_args,              // Pointer to data
            nanos_current_wd(),              // Work group
            NULL,                            // Copies
            NULL);                           // Copy dimensions
    }
    if (err != NANOS_OK)
        nanos_handle_error(err);

    nanos_data_access_t data_accesses[num_deps];
    size_t total_size = 0;
    for (size_t i = 0; i < deps.size(); i++) {
      total_size += deps[i].dimensions.size();
    }
    nanos_region_dimension_t my_dimensions[total_size];

    dep_t2nanos_data_access_t(deps, data_accesses, num_deps, my_dimensions);

    if (wd != (nanos_wd_t) 0) {
        if (copy_data != NULL) {
            set_copies(deps, copy_data, copy_dims_buffer, num_copies);
        }
        err = nanos_submit(
            wd,
            num_deps,
            reinterpret_cast<nanos_data_access_t*>(&data_accesses),
            (nanos_team_t) 0);

    } else {
        if (num_copies > 0) {
            nanos_copy_data_t copy_data[num_copies];
            nanos_region_dimension_internal_t copy_dims_buffer[total_dimensions];
            set_copies(deps, copy_data, copy_dims_buffer, num_copies);

            err = nanos_create_wd_and_run_compact(
                &_const_def.base,                        // Work descriptor definition
                &dyn_props,                              // Dynamic work descriptor properties
                0,  // sizeof(_nx_data_env_0_t),         // Data size
                NULL,  // &imm_args,                     // Pointer to data
                num_deps,                                // Number of data accesses
                reinterpret_cast<nanos_data_access_t*>(&data_accesses),  // Data accesses
                copy_data,                               // Copies
                copy_dims_buffer,                        // Copy dimensions
                0);
        } else {
            err = nanos_create_wd_and_run_compact(
                &_const_def.base,                        // Work descriptor definition
                &dyn_props,                              // Dynamic work descriptor properties
                0,  // sizeof(_nx_data_env_0_t),         // Data size
                NULL,  // &imm_args,                     // Pointer to data
                num_deps,                                // Number of data accesses
                reinterpret_cast<nanos_data_access_t*>(&data_accesses),  // Data accesses
                0,                                       // Copies
                0,                                       // Copy dimensions
                0);
        }
    }
    if (err != NANOS_OK)
        nanos_handle_error(err);
}

inline
void NanoxAPI::waitForTaskGroup(unsigned int tg)
{
    nanos_wg_wait_completion(nanos_current_wd(), 0 /*avoid_flush*/);
}

inline
void NanoxAPI::waitOn(const std::vector<sim::trace::ts::dep_t> &deps)
{
    const unsigned int num_deps = deps.size();

    nanos_data_access_t data_accesses[num_deps];
    size_t total_size = 0;
    for (size_t i = 0; i < deps.size(); i++) {
      total_size += deps[i].dimensions.size();
    }
    nanos_region_dimension_t my_dimensions[total_size];

    dep_t2nanos_data_access_t(deps, data_accesses, num_deps, my_dimensions);

    nanos_wait_on(num_deps, data_accesses);
}

#elif not NANOS_0_6_SUPPORT_ENABLED
// NANOS++ 0.7

inline
void* nanos_tasksim_factory(void *args)
{
    nanos_tasksim_args_t *tasksim = reinterpret_cast<nanos_tasksim_args_t*>(args);
    return reinterpret_cast<void*>(new ext::SimSmpDD(tasksim->task_id));
}

inline
void NanoxAPI::createTask(unsigned int task_id, const std::vector<nanos_dependence_t> &deps)
{
    unsigned int num_deps = deps.size();
    nanos_dependence_t* task_deps = const_cast<nanos_dependence_t*>(deps.data());

    // Setting the parameters to call nanos_create_wd_compact
    nanos_tasksim_args_t task_args = {task_id};
    nanos_wd_t wd = (nanos_wd_t) 0;
    struct nanos_const_wd_definition_local_t {
            nanos_const_wd_definition_t base;
            nanos_device_t devices[1];
    };
    static struct nanos_const_wd_definition_local_t const_def = {
        {
            {
                0,  // mandatory_creation
                1,  // tied
                0,  // reserved0
                0,  // reserved1
                0,  // reserved2
                0,  // reserved3
                0,  // reserved4
                0   // reserved5
            },
            1,         // data alignment (this cannot be 0)
       //     0,
            num_deps,  // number of copies (same as number of deps), used for SW prefetch (as of 20120510)
                       // TODO(???): do not specify copies if SW prefetch is not enabled
            1          // number of devices
        },
        {{
            nanos_tasksim_factory,
            &task_args
        }}
    };
    nanos_wd_dyn_props_t dyn_props = {0, 0};
    nanos_err_t err;
    dyn_props.priority = 0;

    nanos_copy_data_t *copies = NULL;  // this array is allocated inside nanos_create_wd_compact
    if (num_deps > 0) {
        err = nanos_create_wd_compact(
                &wd,
                &const_def.base,
                &dyn_props,
                0,            // size of data
                NULL,         // pointer to data
                nanos_current_wd(),
                &copies);     // the actual pointer to the copies (allocated inside NANOS++)
    } else {
        err = nanos_create_wd_compact(
                &wd,
                &const_def.base,
                &dyn_props,
                0,            // size of data
                NULL,         // pointer to data
                nanos_current_wd(),
                NULL);        // the actual pointer to the copies (allocated inside NANOS++)
    }

    if (err != NANOS_OK)
        nanos_handle_error(err);

    if (wd != (nanos_wd_t) 0) {
        // Setting copies info, after allocated inside nanos_create_wd_compact
        if (copies != NULL) {
            for (int i = 0; i < num_deps; i++) {
                copies[i].address = (size_t)*(task_deps[i].address);
                copies[i].sharing = NANOS_SHARED;  // NANOS_PRIVATE
                copies[i].flags.input = task_deps[i].flags.input;
                copies[i].flags.output = task_deps[i].flags.output;
                copies[i].size = task_deps[i].size;
            }
        }

        err = nanos_submit(wd,
                           num_deps,
                           reinterpret_cast<nanos_dependence_t*>(task_deps),
                           (nanos_team_t) 0);
        if (err != NANOS_OK)
            nanos_handle_error(err);
    } else {
        copies = new nanos_copy_data_t[num_deps];

        for (int i = 0; i < num_deps; i++) {
            copies[i].address = (size_t)*(task_deps[i].address);
            copies[i].sharing = NANOS_SHARED;  // NANOS_PRIVATE
            copies[i].flags.input = task_deps[i].flags.input;
            copies[i].flags.output = task_deps[i].flags.output;
            copies[i].size = task_deps[i].size;
        }

        err = nanos_create_wd_and_run_compact(
                &const_def.base,
                &dyn_props,
                0,            // size of data
                NULL,         // pointer to data
                num_deps,     // number of depedencies
                task_deps,    // task dependencies
                copies,       // the actual pointer to the copies
                0);
        if (err != NANOS_OK)
            nanos_handle_error(err);

        delete [] copies;
    }
}

inline
void NanoxAPI::waitForTaskGroup(unsigned int tg)
{
    nanos_wg_wait_completion(nanos_current_wd(), 0 /*avoid_flush*/);
}

inline
void NanoxAPI::waitOn(const std::vector<nanos_dependence_t> &deps)
{
    unsigned int num_deps = deps.size();
    nanos_dependence_t* task_deps = const_cast<nanos_dependence_t*>(deps.data());

    /*arico DEBUG:
    std::cout << "----\nWaiting for num_deps = " << num_deps << ", at array " << task_deps << std::endl;
    for(int i=0; i<num_deps; i++)
    {
        std::cout << "address " << *(task_deps[i].address) << " <- " << task_deps[i].address << std::endl;
        std::cout << "offset " << task_deps[i].offset << std::endl;
        std::cout << "input " << task_deps[i].flags.input << std::endl;
        std::cout << "output " << task_deps[i].flags.output << std::endl;
        std::cout << "can_rename " << task_deps[i].flags.can_rename << std::endl;
        std::cout << "size " << task_deps[i].size << std::endl;
    }*/

    nanos_wait_on(num_deps, task_deps);
}

#else
// NANOS++ 0.6a

const size_t nanos_tasksim_dd_size = sizeof(ext::SimSmpDD);

inline
void * nanos_tasksim_factory(void *prealloc, void *args)
{
    nanos_tasksim_args_t *tasksim = reinterpret_cast<nanos_tasksim_args_t*>(args);
    if ( prealloc != NULL ) {
        return ( void * )new (prealloc) ext::SimSmpDD( tasksim->task_id );
    } else {
        return ( void * )new ext::SimSmpDD( tasksim->task_id );
    }
}

inline
void NanoxAPI::createTask(unsigned int task_id, const std::vector<nanos_dependence_t> &deps)
{
    // Setting the parameters to call nanos_create_wd
    nanos_tasksim_args_t task_args = {task_id};
    nanos_device_t task_devices[] = {{
            nanos_tasksim_factory,
            nanos_tasksim_dd_size,
            &task_args
    }};  // NANOS_TASKSIM_DESC(task_args);
    nanos_wd_t wd = (nanos_wd_t) 0;
    nanos_wd_props_t props;
    __builtin_memset(&props, 0, sizeof (props));
    // props.mandatory_creation = 1;
    props.tied = 1;
    // props.tie_to = NULL;

    // Setting task dependences
    unsigned int num_deps = deps.size();
    nanos_dependence_t* task_deps = const_cast<nanos_dependence_t*>(deps.data());

    nanos_copy_data_t *copies = NULL;  // this array is allocated inside nanos_create_wd

    nanos_err_t err;

    if (num_deps > 0) {
        err = nanos_create_wd(
                &wd,
                1,             // number of devices
                task_devices,  // pointer to devices
                0,             // size of data
                1,             // alignment of data (this cannot be 0)
                NULL,          // pointer to data
                nanos_current_wd(),
                &props,        // creation properties
                num_deps,      // number of copies (same as number of deps), it is used for prefetching (as of 20110421)
                &copies);      // the actual pointer to the copies (allocated inside NANOS++)
    } else {
        err = nanos_create_wd(
                &wd,
                1,             // number of devices
                task_devices,  // pointer to devices
                0,             // size of data
                1,             // alignment of data (this cannot be 0)
                NULL,          // pointer to data
                nanos_current_wd(),
                &props,        // creation properties
                num_deps,      // number of copies (same as number of deps), it is used for prefetching (as of 20110421)
                NULL);         // the actual pointer to the copies (allocated inside NANOS++)
    }

    if (err != NANOS_OK)
        nanos_handle_error(err);

    if (wd != (nanos_wd_t) 0) {
        // Setting copies info, after allocated inside NANOS++ call
        for (int i = 0; i < num_deps; i++) {
            copies[i].address = (size_t)*(task_deps[i].address);
            copies[i].sharing = NANOS_SHARED;  // NANOS_PRIVATE
            copies[i].flags.input = task_deps[i].flags.input;
            copies[i].flags.output = task_deps[i].flags.output;
            copies[i].size = task_deps[i].size;
        }

        nanos_submit(wd,
                     num_deps,
                     reinterpret_cast<nanos_dependence_t*>(task_deps),
                     (nanos_team_t) 0);
    } else {
        copies = new nanos_copy_data_t[num_deps];

        // Setting copies info, after allocated inside NANOS++ call
        for (int i = 0; i < num_deps; i++) {
            copies[i].address = (size_t)*(task_deps[i].address);
            copies[i].sharing = NANOS_SHARED;  // NANOS_PRIVATE
            copies[i].flags.input = task_deps[i].flags.input;
            copies[i].flags.output = task_deps[i].flags.output;
            copies[i].size = task_deps[i].size;
        }

        nanos_create_wd_and_run(1,             // number of devices
                                task_devices,  // pointer to devices
                                0,             // size of data
                                1,             // alignment of data (this cannot be 0)
                                NULL,          // pointer to data
                                num_deps,      // number of depedencies
                                task_deps,     // task dependencies
                                &props,        // creation properties
                                num_deps,      // number of copies (same as number of deps),
                                               // it is used for prefetching (as of 20110421)
                                copies,        // the actual pointer to the copies
                                0);

        delete [] copies;
    }
}

inline
void NanoxAPI::waitForTaskGroup(unsigned int tg)
{
    nanos_wg_wait_completion(nanos_current_wd(), 0 /*avoid_flush*/);
}

inline
void NanoxAPI::waitOn(const std::vector<nanos_dependence_t> &deps)
{
    unsigned int num_deps = deps.size();
    nanos_dependence_t* task_deps = const_cast<nanos_dependence_t*>(deps.data());

    /*arico DEBUG:
    std::cout << "----\nWaiting for num_deps = " << num_deps << ", at array " << task_deps << std::endl;
    for(int i=0; i<num_deps; i++)
    {
        std::cout << "address " << *(task_deps[i].address) << " <- " << task_deps[i].address << std::endl;
        std::cout << "offset " << task_deps[i].offset << std::endl;
        std::cout << "input " << task_deps[i].flags.input << std::endl;
        std::cout << "output " << task_deps[i].flags.output << std::endl;
        std::cout << "can_rename " << task_deps[i].flags.can_rename << std::endl;
        std::cout << "size " << task_deps[i].size << std::endl;
    }*/

    nanos_wait_on(num_deps, task_deps);
}

#endif

}  // namespace runtime
}  // namespace sim

#endif  // SIMULATORS_TASKSIM_RUNTIME_NANOXAPI_IMPL_H_
