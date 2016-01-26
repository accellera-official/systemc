/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_string_view.h -- Non-owning, constant reference to a character string

  Intended to follow C++ Library Fundamentals TS
    http://open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4480.html#string.view

  Original Author: Philipp A Hartmann, Intel

 *****************************************************************************/

#ifndef SYSC_UTILS_STRING_VIEW_H_INCLUDED_
#define SYSC_UTILS_STRING_VIEW_H_INCLUDED_

#include <sysc/kernel/sc_cmnhdr.h>

#if defined(__has_include) && SC_STD_CPLUSPLUS >= 201402L
#  if __has_include(<string_view>) // available in C++17
#    define SC_STRING_VIEW_NS_ std
#    include <string_view>
#  elif __has_include(<experimental/string_view>)
#    define SC_STRING_VIEW_NS_ std::experimental
#    include <experimental/string_view>
#  endif
#else
// TODO: other ways to detect availability of std::(experimental::)string_view?
#endif

#ifndef SC_STRING_VIEW_NS_
// fallback to (mostly compatible) implementation from Boost
#  include <sysc/packages/boost/utility/string_view.hpp>
#  define SC_STRING_VIEW_NS_ sc_boost
#endif // Boost fallback

namespace sc_core {

typedef SC_STRING_VIEW_NS_::string_view sc_string_view;
// TODO: add ABI guard against inconsistent configurations

} // namespace sc_core

#undef SC_STRING_VIEW_NS_
#endif // SYSC_UTILS_STRING_VIEW_H_INCLUDED_
// Taf!
