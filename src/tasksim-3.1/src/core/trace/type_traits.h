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


#ifndef CORE_TRACE_TYPE_TRAITS_H_
#define CORE_TRACE_TYPE_TRAITS_H_

#include <type_traits>

#include <iostream>

namespace sim {


template<typename LocalEvent>
class check_event {
private:
    template<typename Target, Target> struct type_check;
    template<typename Target, typename T> struct prototype {
        typedef void (Target::*fptr)(T &);
    };
    template<typename Target, typename T> struct const_prototype {
        typedef void (Target::*fptr)(T &) const;
    };

    template<typename Target>
    static inline
    char (&test_read(type_check<typename prototype<Target, std::istream>::fptr , &Target::read> *))[1];

    template<typename>
    static inline
    char (&test_read(...))[2];

    template<typename Target>
    static inline
    char (&test_write(type_check<typename const_prototype<Target, std::ostream>::fptr, &Target::write> *))[1];

    template<typename>
    static inline
    char (&test_write(...))[2];

public:
    enum {has_read = sizeof(test_read<LocalEvent>(0)) == 1};
    enum {has_write = sizeof(test_write<LocalEvent>(0)) == 1};
};


template<typename Class, typename T, T (Class::*Method)(...)>
class has_method {
private:
    template<typename Target, Target> struct type_check;
    template<typename Target> struct prototype {
            typedef void (Target::*fptr)(...);
    };

    template<typename Target>
    static inline
    char (&test_method(type_check<typename prototype<Target>::fptr , Method> *))[1];


    template<typename, typename>
    static inline
    char (&test_method(...))[2];

public:
    enum {value = sizeof(test_method<Class>(0)) == 1};
};

template<typename Class, typename Target>
class is_base_of {
private:
    static inline
    char (&check(Target *))[1];

    static inline
    char (&check(...))[2];
public:
    enum {value = sizeof(check(static_cast<Class *>(0))) == 1 };
};

}  // namespace sim


#endif  // CORE_TRACE_TYPE_TRAITS_H_
