/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_mempool.h - Memory pools for small objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_MEMPOOL_H
#define SC_MEMPOOL_H


#include "sysc/utils/sc_iostream.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_mempool
//
//  ...
// ----------------------------------------------------------------------------

class sc_mempool
{
public:

    static void* allocate( std::size_t sz );
    static void release( void* p, std::size_t sz );
    static void display_statistics();
};


// ----------------------------------------------------------------------------
//  CLASS : sc_mpobject
//
//  ...
// ----------------------------------------------------------------------------

class sc_mpobject
{
public:

    static void* operator new( std::size_t sz )
	{ return sc_mempool::allocate( sz ); }

    static void operator delete( void* p, std::size_t sz )
	{ sc_mempool::release( p, sz ); }

    static void* operator new[]( std::size_t sz )
	{ return sc_mempool::allocate( sz ); }

    static void operator delete[]( void* p, std::size_t sz )
	{ sc_mempool::release( p, sz ); }
};

} // namespace sc_core

// $Log: sc_mempool.h,v $
// Revision 1.3  2011/08/26 20:46:18  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.2  2011/02/18 20:38:44  acg
//  Andy Goodrich: Updated Copyright notice.
//
// Revision 1.1.1.1  2006/12/15 20:20:06  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:11  acg
// Andy Goodrich: Added $Log command so that CVS comments are reproduced in
// the source.

#endif
