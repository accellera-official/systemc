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

  Original Author: Philipp A Hartmann, Intel

  Intended to follow C++ Library Fundamentals TS
    http://open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4480.html#string.view

  NOTE: The types sc_string_view and sc_zstring_view are implementation-
        defined and should not be used within user models.  After C++17 is
        fully adopted, all uses shall be replaced by std::string_view.

 *****************************************************************************/

#ifndef SYSC_UTILS_STRING_VIEW_H_INCLUDED_
#define SYSC_UTILS_STRING_VIEW_H_INCLUDED_

#include <sysc/kernel/sc_cmnhdr.h>

#if SC_CPLUSPLUS >= 201402L && defined(__has_include)
#  if SC_CPLUSPLUS > 201402L && __has_include(<string_view>) /* since C++17 */
#    define SC_STRING_VIEW_NS_ std
#    include <string_view>
   /*  available in Library Fundamentals, ISO/IEC TS 19568:2015 */
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

/// non-owning, constant reference to a string (implementation-defined)
typedef SC_STRING_VIEW_NS_::string_view sc_string_view;
// TODO: add ABI guard against inconsistent configurations

/// non-owning, const-ref to null-terminated string (implementation-defined)
class sc_zstring_view : public sc_string_view
{
public:
  /* constexpr */ sc_zstring_view() /* noexcept */
    : sc_string_view() {}

  /* constexpr */ sc_zstring_view(const char* s) /* noexcept */
    : sc_string_view(s) {}

  sc_zstring_view(const std::string& s) /* noexcept */
    : sc_string_view(s) {}

  /* constexpr */ void swap(sc_zstring_view& s) /* noexcept */
    { sc_string_view::swap(s); }

  /* constexpr */ const char* c_str() const /* noexcept */
    { return data(); }

private:
  // Hide invariant-breaking function
  using sc_string_view::remove_suffix;
}; // class sc_zstring_view

} // namespace sc_core

#undef SC_STRING_VIEW_NS_
#endif // SYSC_UTILS_STRING_VIEW_H_INCLUDED_
// Taf!
