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

  Intended to follow std::string_view as defined by ISO/IEC 14882:2017.

  NOTE: Once C++20 is adopted, all uses can be replaced by std::string_view.

 *****************************************************************************/

#ifndef SYSC_UTILS_STRING_VIEW_H_INCLUDED_
#define SYSC_UTILS_STRING_VIEW_H_INCLUDED_

#include "sysc/kernel/sc_cmnhdr.h"
#include <string>
#include <type_traits> // std::enable_if, std::is_convertible
#include <string_view>

namespace sc_core {

/// non-owning, constant reference to a string
// TODO: add ABI guard against inconsistent configurations
class SC_API sc_string_view
  : public std::string_view
{
  typedef std::string_view base_type;
  template<typename T> struct enable_if_convertible
    : std::enable_if<std::is_convertible<T,base_type>::value > {};
public:
    // inherit constructors from base class
    using base_type::base_type;

    // allow same conversions as base class, needed for C++17
    template<typename T>
    /* constexpr */ sc_string_view( const T& s
                                  , typename enable_if_convertible<T>::type* = 0)
      : base_type(s) {}

  // helper function to create an explicit string copy
  inline std::string str() const
    { return std::string( data(), size() ); }

#if SC_CPLUSPLUS <= 201703L // before C++20
  /* constexpr */ bool starts_with(base_type sv) const /* noexcept */
    { return size() >= sv.size() && compare(0, sv.size(), sv) == 0; }
  /* constexpr */ bool starts_with(char c) const /* noexcept */
    { return !empty() && front() == c; }
  /* constexpr */ bool starts_with(const char* s) const /* noexcept */
    { return starts_with( sc_string_view(s) ); }

  /* constexpr */ bool ends_with(base_type sv) const /* noexcept */
    { return size() >= sv.size() && compare(size() - sv.size(), npos, sv) == 0; }
  /* constexpr */ bool ends_with(char c) const /* noexcept */
    { return !empty() && back() == c; }
  /* constexpr */ bool ends_with(const char* s) const /* noexcept */
    { return ends_with( sc_string_view(s) ); }
#endif // before C++20
};     // class sc_string_view

/// non-owning, constant reference to null-terminated string
class SC_API sc_zstring_view : public sc_string_view
{
  typedef sc_string_view base_type;

public:
  /* constexpr */ sc_zstring_view() /* noexcept */
    : base_type("") {}

  /* constexpr */ sc_zstring_view(const char* s) /* noexcept */
    : base_type(SC_LIKELY_(s) ? s : "") {}

  sc_zstring_view(const std::string& s) /* noexcept */
    : base_type(s.c_str()) {} // enforce ending string at first \0 character

  /* constexpr */ void swap(sc_zstring_view& s) /* noexcept */
    { base_type::swap(s); }

  /* constexpr */ const char* c_str() const /* noexcept */
    { return data(); }

private:
  // Hide invariant-breaking function
  using sc_string_view::remove_suffix;
}; // class sc_zstring_view

} // namespace sc_core

#endif // SYSC_UTILS_STRING_VIEW_H_INCLUDED_
// Taf!
