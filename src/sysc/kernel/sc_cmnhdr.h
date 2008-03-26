/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  sc_cmnhdr.h - Common header file containing handy pragmas, macros and
                definitions common to all SystemC source files.

  Original Author: Amit Rao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_CMNHDR_H
#define SC_CMNHDR_H


// all windows 32-bit compilers should define WIN32

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
#ifndef WIN32
#define WIN32
#endif
#endif

// WIN64 is reserved for the future 64-bit OS

#ifdef WIN64
#error WIN64 keyword is reserved for 64-bit Windows
#endif


// ----------------------------------------------------------------------------

#ifdef _MSC_VER

// Disable VC++ warnings that are harmless

// this : used in base member initializer list
#pragma warning(disable: 4355)

// new and delete warning when exception handling is turned on
#pragma warning(disable: 4291)

// in many places implicit conversion to bool
// from other integral types is performed
#pragma warning(disable: 4800)

// unary minus operator applied to unsigned
#pragma warning(disable: 4146)

// multiple copy constructors
#pragma warning(disable: 4521)

// identifier was truncated to '255' characters in the browser information
#pragma warning(disable: 4786)


// Windows Version Build Option

#define _WIN32_WINNT 0x0400
#include <Windows.h>


// MSVC6.0 for() scope bug

#define for if( false ); else for

#endif


// ----------------------------------------------------------------------------

#ifdef __BORLANDC__

// Windows Version Build Option

#define _WIN32_WINNT 0x0400
#include <Windows.h>

#endif


// ----------------------------------------------------------------------------

#include <cassert>
#include <cstdio>
#include <cstdlib>


#endif

// Taf!
