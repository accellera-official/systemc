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

  sc_cor_fiber.cpp -- Coroutine implementation with fibers.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-12-18

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_cor_fiber.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#ifdef WIN32


#include "sysc/kernel/sc_cor_fiber.h"
#include "sysc/kernel/sc_simcontext.h"

#if( defined(_MSC_VER) && _MSC_VER >= 1300 )

using std::size_t;

#endif


namespace sc_core {

// ----------------------------------------------------------------------------
//  File static variables.
// ----------------------------------------------------------------------------

// main coroutine

static sc_cor_fiber main_cor;


// ----------------------------------------------------------------------------
//  CLASS : sc_cor_fiber
//
//  Coroutine class implemented with QuickThreads.
// ----------------------------------------------------------------------------

// destructor

sc_cor_fiber::~sc_cor_fiber()
{
    if( m_fiber != 0 ) {
#     ifdef WIN32
      PVOID cur_fiber = GetCurrentFiber();
      if (m_fiber != cur_fiber)
#     endif
	DeleteFiber( m_fiber );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_cor_pkg_fiber
//
//  Coroutine package class implemented with QuickThreads.
// ----------------------------------------------------------------------------

int sc_cor_pkg_fiber::instance_count = 0;


// constructor

sc_cor_pkg_fiber::sc_cor_pkg_fiber( sc_simcontext* simc )
: sc_cor_pkg( simc )
{
    if( ++ instance_count == 1 ) {
        // initialize the main coroutine
	assert( main_cor.m_fiber == 0 );
	main_cor.m_fiber = ConvertThreadToFiber( 0 );
    }
}


// destructor

sc_cor_pkg_fiber::~sc_cor_pkg_fiber()
{
    if( -- instance_count == 0 ) {
	// cleanup the main coroutine
	main_cor.m_fiber = 0;
    }
}


// create a new coroutine

sc_cor*
sc_cor_pkg_fiber::create( std::size_t stack_size, sc_cor_fn* fn, void* arg )
{
    sc_cor_fiber* cor = new sc_cor_fiber;
    cor->m_pkg = this;
    cor->m_stack_size = stack_size;
    cor->m_fiber = CreateFiber( cor->m_stack_size,
			        (LPFIBER_START_ROUTINE) fn, arg );
    return cor;
}


// yield to the next coroutine

void
sc_cor_pkg_fiber::yield( sc_cor* next_cor )
{
    sc_cor_fiber* new_cor = SCAST<sc_cor_fiber*>( next_cor );
    SwitchToFiber( new_cor->m_fiber );
}


// abort the current coroutine (and resume the next coroutine)

void
sc_cor_pkg_fiber::abort( sc_cor* next_cor )
{
    sc_cor_fiber* new_cor = SCAST<sc_cor_fiber*>( next_cor );
    SwitchToFiber( new_cor->m_fiber );
}


// get the main coroutine

sc_cor*
sc_cor_pkg_fiber::get_main()
{
    return &main_cor;
}

} // namespace sc_core

#endif

// Taf!
