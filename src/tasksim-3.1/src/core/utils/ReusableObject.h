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


#ifndef UTILS_REUSABLEOBJECT_H_
#define UTILS_REUSABLEOBJECT_H_

#include <cstdio>

namespace sim { namespace utils {

template <typename T>
class Pool {
private:
    union Object {
        char dummy[sizeof(T)];
        Object * next;
    };

    Object *freeList_;
public:

    Pool() : freeList_(NULL)
    {}

    ~Pool()
    {
        Object * next, * tmp;
        for (next = freeList_; next; next = tmp) {
            tmp = next->next;
            delete next;
        }
    }

    T *get()
    {
        Object *ret = freeList_;
        if (ret) freeList_ = ret->next;
        return (T *) ret;
    }

    void put(T *ptr)
    {
        ((Object *) ptr)->next = freeList_;
        freeList_ = (Object *) ptr;
    }



};

template <typename T>
class ReusableObject {
public:
    void *operator new(size_t bytes)
    {
        T *res = pool.get();
        return res ? res : (T *) new typename Pool<T>::Object;
    }

    void operator delete(void *ptr)
    {
        pool.put((T *) ptr);
    }

protected:
    static Pool<T> pool;
};

// Static initialization

template<class T> Pool<T>
ReusableObject<T>::pool;

}}

#endif
