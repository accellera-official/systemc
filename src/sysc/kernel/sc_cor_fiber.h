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

  sc_cor_fiber.h -- Coroutine implementation with fibers.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-12-18

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_cor_fiber.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_COR_FIBER_H
#define SC_COR_FIBER_H


#ifdef WIN32

#include "sysc/kernel/sc_cor.h"
#include "sysc/kernel/sc_cmnhdr.h"

namespace sc_core {

class sc_cor_pkg_fiber;

#if( defined(_MSC_VER) && _MSC_VER >= 1300 )
typedef std::size_t size_t;
#endif

// ----------------------------------------------------------------------------
//  CLASS : sc_cor_fiber
//
//  Coroutine class implemented with QuickThreads.
// ----------------------------------------------------------------------------

class sc_cor_fiber
: public sc_cor
{

public:

    // constructor
    sc_cor_fiber()
	: m_stack_size( 0 ), m_fiber( 0 ), m_pkg( 0 )
	{}

    // destructor
    virtual ~sc_cor_fiber();

public:

    std::size_t            m_stack_size;  // stack size
    PVOID             m_fiber;       // fiber

    sc_cor_pkg_fiber* m_pkg;         // the creating coroutine package

private:

    // disabled
    sc_cor_fiber( const sc_cor_fiber& );
    sc_cor_fiber& operator = ( const sc_cor_fiber& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_cor_pkg_fiber
//
//  Coroutine package class implemented with QuickThreads.
// ----------------------------------------------------------------------------

class sc_cor_pkg_fiber
: public sc_cor_pkg
{
  public:

    // constructor
    sc_cor_pkg_fiber( sc_simcontext* simc );

    // destructor
    virtual ~sc_cor_pkg_fiber();

    // create a new coroutine
    virtual sc_cor* create( std::size_t stack_size, sc_cor_fn* fn, void* arg );

    // yield to the next coroutine
    virtual void yield( sc_cor* next_cor );

    // abort the current coroutine (and resume the next coroutine)
    virtual void abort( sc_cor* next_cor );

    // get the main coroutine
    virtual sc_cor* get_main();

private:

    static int instance_count;

private:

    // disabled
    sc_cor_pkg_fiber();
    sc_cor_pkg_fiber( const sc_cor_pkg_fiber& );
    sc_cor_pkg_fiber& operator = ( const sc_cor_pkg_fiber& );
};

} // namespace sc_core

#endif // WIN32


#endif

// Taf!
