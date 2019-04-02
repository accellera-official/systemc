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

# define SC_STATIC_CONSTANT_(Type,Value) \
    static const Type Value

#else // use Boost/local implementation for type traits
  template<bool Cond, typename T = void>
  struct enable_if : sc_boost::enable_if_c<Cond, T> {};

# define SC_STATIC_CONSTANT_(Type,Value) \
    SC_BOOST_STATIC_CONSTANT(Type,Value)

  // simplistic version to reduce Boost usage
  template< typename T > struct remove_const          { typedef T type; };
  template< typename T > struct remove_const<const T> { typedef T type; };

  template< typename T, typename U >
  struct is_same      { SC_BOOST_STATIC_CONSTANT( bool, value = false ); };
  template< typename T >
  struct is_same<T,T> { SC_BOOST_STATIC_CONSTANT( bool, value = true );  };

  template< typename T >
  struct is_const           { SC_BOOST_STATIC_CONSTANT( bool, value = false ); };
  template< typename T >
  struct is_const< const T> { SC_BOOST_STATIC_CONSTANT( bool, value = true );  };

#endif // type traits

  template< typename CT, typename T >
  struct is_more_const {
    SC_STATIC_CONSTANT_( bool, value
       = ( is_same< typename remove_const<CT>::type
                 , typename remove_const<T>::type
                 >::value
          && ( is_const<CT>::value >= is_const<T>::value ) ) );
  };

  struct special_result {};
  template< typename T > struct remove_special_fptr {};
  template< typename T >
  struct remove_special_fptr< special_result& (*)( T ) >
    { typedef T type; };

} // namespace sc_meta
} // namespace sc_core

#undef SC_STATIC_CONSTANT_
#endif // SC_META_H_INCLUDED_
// Taf!
