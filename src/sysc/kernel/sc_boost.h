/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_boost.h -- Thread Semantics Provided By The Boost Library

  Original Author: Stuart Swan, Cadence Design Systems, Inc

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
// $Log: sc_boost.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_BOOST_H
#define SC_BOOST_H

// namespace sc_dp { This is off because of bugs with gcc 2.9x

// SET THE NAME OF OBJECTS THAT THE BOOST LIBRARY WILL PRODUCE AND INCLUDE IT:

#if defined(_MSC_VER) && !defined(__ICL) && !defined(__COMO__)
#   pragma warning(disable: 4786)  // identifier truncated in debug info
#   pragma warning(disable: 4710)  // function not inlined  
#   pragma warning(disable: 4711)  // funct. selected for auto-inline expansion
#   pragma warning(disable: 4514)  // unreferenced inline removed
#endif

#include "sysc/packages/boost/bind.hpp"
#include "sysc/packages/boost/ref.hpp"

#if defined(BOOST_MSVC) && (BOOST_MSVC < 1300)
#   pragma warning(push, 3)
#endif

#if defined(BOOST_MSVC) && (BOOST_MSVC < 1300)
#   pragma warning(pop)
#endif

// } // namespace sc_dp This is off because of bugs with gcc 2.9x

// macros to help avoid direct user code dependencies on boost lib

#define sc_bind    boost::bind
#define sc_ref(r)  boost::ref(r)
#define sc_cref(r) boost::cref(r)

#endif // SC_BOOST_H
