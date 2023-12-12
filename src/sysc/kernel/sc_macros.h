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

  sc_macros.h -- Miscellaneous definitions that are needed by the headers.

  Original Author: Stan Y. Liao, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_MACROS_H
#define SC_MACROS_H

#include "sysc/kernel/sc_cmnhdr.h" 

namespace sc_dt {

template <class T>
inline
const T
sc_min( const T& a, const T& b )
{
    return ( ( a <= b ) ? a : b );
}

template <class T>
inline
const T
sc_max( const T& a, const T& b )
{
    return ( ( a >= b ) ? a : b );
}

template <class T>
inline
const T
sc_abs( const T& a )
{
    // return ( a >= 0 ? a : -a );
    // the code below is functionaly the same as the code above; the
    // difference is that the code below works for all arithmetic
    // SystemC datatypes.
    T z( a );
    z = 0;
    if( a >= z ) {
	return a;
    } else {
	T c( a );
	c = -a;
	return c;
    }
}

} // namespace sc_dt 

// token stringification

#define SC_STRINGIFY_HELPER_( Arg ) \
  SC_STRINGIFY_HELPER_DEFERRED_( Arg )
#define SC_STRINGIFY_HELPER_DEFERRED_( Arg ) \
  SC_STRINGIFY_HELPER_MORE_DEFERRED_( Arg )
#define SC_STRINGIFY_HELPER_MORE_DEFERRED_( Arg ) \
  #Arg 


// token concatenation
#define SC_CONCAT_HELPER_( a, b ) \
  SC_CONCAT_HELPER_DEFERRED_( a, b )
#define SC_CONCAT_HELPER_DEFERRED_( a, b ) \
  SC_CONCAT_HELPER_MORE_DEFERRED_( a,b )
#define SC_CONCAT_HELPER_MORE_DEFERRED_( a, b ) \
  a ## b
#define SC_CONCAT_UNDERSCORE_( a, b ) \
  SC_CONCAT_HELPER_( a, SC_CONCAT_HELPER_( _, b ) )


// forced token expansion
#define SC_EXPAND_HELPER_(x) x

/*
 *  These help debugging --
 *  -- the user can find out at which location in their source file a process calls wait.
 */

#define SC_WAIT()                                       \
    ::sc_core::sc_set_location( __FILE__, __LINE__ );   \
    ::sc_core::wait();                                  \
    ::sc_core::sc_set_location( NULL, 0 )

#define SC_WAITN(n)                                     \
    ::sc_core::sc_set_location( __FILE__, __LINE__ );   \
    ::sc_core::wait(n);                                 \
    ::sc_core::sc_set_location( NULL, 0 )

#define SC_WAIT_UNTIL(expr)                             \
  do { SC_WAIT(); } while( !(expr) )

// ----------------------------------------------------------------------------
// SC_NAMED - helper macro to create named objects

#define SC_NAMED(...) \
  SC_NAMED_IMPL_(__VA_ARGS__)(__VA_ARGS__)

# define SC_NAMED_IMPL_ONE_(inst) \
    inst { SC_STRINGIFY_HELPER_(inst) }
# define SC_NAMED_IMPL_MORE_(inst, ...) \
    inst { SC_STRINGIFY_HELPER_(inst), __VA_ARGS__ }

#define SC_NAMED_IMPL_(...) \
  SC_CONCAT_HELPER_(SC_NAMED_IMPL_, SC_VARARG_HELPER_EXPAND_(__VA_ARGS__))

#define SC_VARARG_HELPER_EXPAND_(...) \
  SC_EXPAND_HELPER_( SC_VARARG_HELPER_EXPAND_SEQ_( \
    __VA_ARGS__,                                        \
    MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,    \
    MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,    \
    MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,    \
    MORE_,MORE_,MORE_,MORE_,MORE_,MORE_,ONE_, FAIL_ ) )
#define SC_VARARG_HELPER_EXPAND_SEQ_( \
   _1, _2, _3, _4, _5, _6, _7, _8, \
   _9,_10,_11,_12,_13,_14,_15,_16, \
  _17,_18,_19,_20,_21,_22,_23,_24, \
  _25,_26,_27,_28,_29,_30,_31, N, ...) N

// $Log: sc_macros.h,v $
// Revision 1.5  2011/08/26 20:46:09  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.4  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.3  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.2  2008/05/22 17:06:25  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.

#endif
