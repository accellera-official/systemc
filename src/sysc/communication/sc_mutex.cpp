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

  sc_mutex.cpp -- The sc_mutex primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


// $Log: sc_mutex.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:35  acg
// SystemC 2.2
//
// Revision 1.4  2006/03/21 00:00:27  acg
//   Andy Goodrich: changed name of sc_get_current_process_base() to be
//   sc_get_current_process_b() since its returning an sc_process_b instance.
//
// Revision 1.3  2006/01/13 18:47:42  acg
// Added $Log command so that CVS comments are reproduced in the source.
//

#include "sysc/communication/sc_mutex.h"
#include "sysc/kernel/sc_simcontext.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_mutex
//
//  The sc_mutex primitive channel class.
// ----------------------------------------------------------------------------

// constructors

sc_mutex::sc_mutex()
: sc_prim_channel( sc_gen_unique_name( "mutex" ) ),
  m_owner( 0 )
{}

sc_mutex::sc_mutex( const char* name_ )
: sc_prim_channel( name_ ),
  m_owner( 0 )
{}


// destructor

sc_mutex::~sc_mutex()
{}

// interface methods

// blocks until mutex could be locked

int
sc_mutex::lock()
{
    if (m_owner == sc_get_current_process_b()) return 0;
    while( in_use() ) {
	wait( m_free );
    }
    m_owner = sc_get_current_process_b();
    return 0;
}


// returns -1 if mutex could not be locked

int
sc_mutex::trylock()
{
    if (m_owner == sc_get_current_process_b()) return 0;
    if( in_use() ) {
	return -1;
    }
    m_owner = sc_get_current_process_b();
    return 0;
}


// returns -1 if mutex was not locked by caller

int
sc_mutex::unlock()
{
    if( m_owner != sc_get_current_process_b() ) {
	return -1;
    }
    m_owner = 0;
    m_free.notify();
    return 0;
}

} // namespace sc_core

// Taf!
