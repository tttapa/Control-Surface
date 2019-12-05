// Function-Based Exception Support -*- C++ -*-

// Copyright (C) 2001-2017 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/functexcept.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{exception}
 *
 *  This header provides support for -fno-exceptions.
 */

//
// ISO C++ 14882: 19.1  Exception classes
//

#ifndef _FUNCTEXCEPT_H
#define _FUNCTEXCEPT_H 1

#include "../bits/c++config.h"
#include "../bits/exception_defines.h"

#ifndef __AVR__

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Helper for exception objects in <except>
  void
  __throw_bad_exception(void) __attribute__((__noreturn__));

  // Helper for exception objects in <new>
  void
  __throw_bad_alloc(void) __attribute__((__noreturn__));

  // Helper for exception objects in <typeinfo>
  void
  __throw_bad_cast(void) __attribute__((__noreturn__));

  void
  __throw_bad_typeid(void) __attribute__((__noreturn__));

  // Helpers for exception objects in <stdexcept>
  void
  __throw_logic_error(const char*) __attribute__((__noreturn__));

  void
  __throw_domain_error(const char*) __attribute__((__noreturn__));

  void
  __throw_invalid_argument(const char*) __attribute__((__noreturn__));

  void
  __throw_length_error(const char*) __attribute__((__noreturn__));

  void
  __throw_out_of_range(const char*) __attribute__((__noreturn__));

  void
  __throw_out_of_range_fmt(const char*, ...) __attribute__((__noreturn__))
    __attribute__((__format__(__gnu_printf__, 1, 2)));

  void
  __throw_runtime_error(const char*) __attribute__((__noreturn__));

  void
  __throw_range_error(const char*) __attribute__((__noreturn__));

  void
  __throw_overflow_error(const char*) __attribute__((__noreturn__));

  void
  __throw_underflow_error(const char*) __attribute__((__noreturn__));

  // Helpers for exception objects in <ios>
  void
  __throw_ios_failure(const char*) __attribute__((__noreturn__));

  void
  __throw_system_error(int) __attribute__((__noreturn__));

  void
  __throw_future_error(int) __attribute__((__noreturn__));

  // Helpers for exception objects in <functional>
  void
  __throw_bad_function_call() __attribute__((__noreturn__));

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#else // __AVR__

#include <AH/Error/Error.hpp>

// Helper for exception objects in <except>
  #define __throw_bad_exception() FATAL_ERROR(F("std::bad_exception"), 0x1111)

  // Helper for exception objects in <new>
  #define __throw_bad_alloc() FATAL_ERROR(F("std::bad_alloc"), 0x1112)

  // Helper for exception objects in <typeinfo>
  #define __throw_bad_cast() FATAL_ERROR(F("std::bad_cast"), 0x1113)

  #define __throw_bad_typeid() FATAL_ERROR(F("std::bad_typeid"), 0x1114)

  // Helpers for exception objects in <stdexcept>
  #define __throw_logic_error(msg) FATAL_ERROR(F("std::logic_error: ") << msg, 0x1115)

  #define __throw_domain_error(msg) FATAL_ERROR(F("std::domain_error: ") << msg, 0x1116)

  #define __throw_invalid_argument(msg) FATAL_ERROR(F("std::invalid_argument: ") << msg, 0x1117)

  #define __throw_length_error(msg) FATAL_ERROR(F("std::length_error: ") << msg, 0x1118)

  #define __throw_out_of_range(msg) FATAL_ERROR(F("std::out_of_range: ") << msg, 0x1119)

  // void
  // __throw_out_of_range_fmt(const char*, ...) __attribute__((__noreturn__))
  //   __attribute__((__format__(__gnu_printf__, 1, 2)))
  #define __throw_out_of_range_index_length(msg, index, size) \
    FATAL_ERROR(F("std::out_of_range: ") << msg << F("; index = ") << index << F(", length = ") << size, 0x1119)

  #define __throw_runtime_error(msg) FATAL_ERROR(F("std::runtime_error: ") << msg, 0x111A)

  #define __throw_range_error(msg) FATAL_ERROR(F("std::range_error: ") << msg, 0x111B)

  #define __throw_overflow_error(msg) FATAL_ERROR(F("std::overflow_error: ") << msg, 0x111C)

  #define __throw_underflow_error(msg) FATAL_ERROR(F("std::underflow_error: ") << msg, 0x111D)

  // Helpers for exception objects in <ios>
  #define __throw_ios_failure(msg) FATAL_ERROR(F("std::ios_failure: ") << msg, 0x111E)

  #define __throw_system_error(code) FATAL_ERROR(F("std::system_error: ") << code, 0x1120)

  #define __throw_future_error(code) FATAL_ERROR(F("std::future_error: ") << code, 0x1121)


  // Helpers for exception objects in <functional>
  #define __throw_bad_function_call() FATAL_ERROR(F("std::bad_function_call"), 0x111F)

#endif

#endif
