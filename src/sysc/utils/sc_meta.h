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

  sc_meta.h - A set of (implementation-defined) meta programming helpers

  Original Author: Philipp A. Hartmann, Intel

 *****************************************************************************/

#ifndef SC_META_H_INCLUDED_
#define SC_META_H_INCLUDED_

#include "sysc/kernel/sc_cmnhdr.h"

#if SC_CPLUSPLUS >= 201103L // use C++11 for type traits
# include <type_traits>
#else // use Boost for type traits
# include "sysc/packages/boost/config.hpp"
# include "sysc/packages/boost/utility/enable_if.hpp"
#endif // type traits

namespace sc_core {
namespace sc_meta {

#if SC_CPLUSPLUS >= 201103L // use C++11 for type traits
  using std::enable_if;
  using std::remove_const;
  using std::is_same;
  using std::is_const;

  using std::integral_constant;
#if SC_CPLUSPLUS >= 201703L
  using std::bool_constant;
#else
  template<bool B> using bool_constant = integral_constant<bool,B>;
#endif // C++17
  using std::true_type;
  using std::false_type;

#else // use Boost/local implementation for type traits

  template<typename T, T v>
  struct integral_constant
  {
    typedef integral_constant type;
    typedef T value_type;
    SC_BOOST_STATIC_CONSTANT(value_type, value = v);
    operator value_type() const { return v; }
  };
  template<bool v> struct bool_constant : integral_constant<bool,v> {};
  typedef bool_constant<true>  true_type;
  typedef bool_constant<false> false_type;

  template<bool Cond, typename T = void>
  struct enable_if : sc_boost::enable_if_c<Cond, T> {};

  // simplistic version to reduce Boost usage
  template< typename T > struct remove_const          { typedef T type; };
  template< typename T > struct remove_const<const T> { typedef T type; };

  template< typename T, typename U > struct is_same : false_type {};
  template< typename T > struct is_same<T,T>        : true_type {};

  template< typename T > struct is_const           : false_type {};
  template< typename T > struct is_const< const T> : true_type {};

#endif // type traits

  template< typename CT, typename T >
  struct is_more_const
    : bool_constant<
          is_same< typename remove_const<CT>::type
                 , typename remove_const<T>::type
                 >::value
          && ( is_const<CT>::value >= is_const<T>::value )
      > {};

//@cond SC_HIDDEN_FROM_DOXYGEN
  struct special_result {};
  template< typename T > struct remove_special_fptr {};
  template< typename T >
  struct remove_special_fptr< special_result& (*)( T ) >
    { typedef T type; };
//@endcond

} // namespace sc_meta
} // namespace sc_core

#endif // SC_META_H_INCLUDED_
// Taf!
