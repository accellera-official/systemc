/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_macros.h -- Miscellaneous definitions that are needed by the headers.

  Original Author: Stan Y. Liao, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_MACROS_H
#define SC_MACROS_H


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

namespace sc_core {

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

/*
 *  These help debugging --
 *  -- user can find out where each process is stopped at.
 */

#define WAIT()                                                                \
    ::sc_core::sc_set_location( __FILE__, __LINE__ );                         \
    ::sc_core::wait()

#define WAITN(n)                                                              \
    ::sc_core::sc_set_location( __FILE__, __LINE__ );                         \
    ::sc_core::wait(n)

#define WAIT_UNTIL(expr)                                                      \
    ::sc_core::sc_set_location( __FILE__, __LINE__ );                         \
    do { ::sc_core::wait(); } while( !(expr) )

} // namespace sc_core

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
