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

  sc_iostream.h - Portable iostream header file wrapper.

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_IOSTREAM_H
#define SC_IOSTREAM_H

#if !defined( _MSC_VER ) && /*!defined( __HP_aCC ) &&*/ !defined( __BORLANDC__ )

#   include <iostream>

#   if defined(__GNUC__) && (__GNUC__ == 2) // Maintain compatibility for 2.95.x
#   	include <strstream>
#   else
#   	include <sstream>
#   endif

#   include <fstream>
#   include <cstddef>
#   include <cstring>

#else
#if defined( _MSC_VER )
    // VC++6 with standard library

#  include <iostream>
#  include <strstream>
    
#  include <fstream>

#  include <cstddef>

#else
/*
#if defined( __HP_aCC )
    // HP aCC

#   include <iostream.h>
#   include <strstream.h>
#   include <fstream.h>

#endif // HP
*/
#endif // MSC
#endif // Everyone else


// shortcuts that save some typing

#ifdef CCAST
#    undef CCAST
#endif
#define CCAST       const_cast

#ifdef DCAST
#    undef DCAST
#endif
#define DCAST     dynamic_cast

#ifdef RCAST
#    undef RCAST
#endif
#define RCAST reinterpret_cast

#ifdef SCAST
#    undef SCAST
#endif
#define SCAST      static_cast

#endif // !defined(SC_IOSTREAM_H)
