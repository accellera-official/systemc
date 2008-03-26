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

  sc_semaphore.cpp -- The sc_semaphore primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


// $Log: sc_semaphore.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:35  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:47:42  acg
// Added $Log command so that CVS comments are reproduced in the source.
//

#include "sysc/communication/sc_communication_ids.h"
#include "sysc/communication/sc_semaphore.h"
#include "sysc/kernel/sc_simcontext.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_semaphore
//
//  The sc_semaphore primitive channel class.
// ----------------------------------------------------------------------------

// error reporting

void
sc_semaphore::report_error( const char* id, const char* add_msg ) const
{
    char msg[BUFSIZ];
    if( add_msg != 0 ) {
	std::sprintf( msg, "%s: semaphore '%s'", add_msg, name() );
    } else {
	std::sprintf( msg, "semaphore '%s'", name() );
    }
    SC_REPORT_ERROR( id, msg );
}


// constructors

sc_semaphore::sc_semaphore( int init_value_ )
: sc_prim_channel( sc_gen_unique_name( "semaphore" ) ),
  m_value( init_value_ )
{
    if( m_value < 0 ) {
	report_error( SC_ID_INVALID_SEMAPHORE_VALUE_ );
    }
}

sc_semaphore::sc_semaphore( const char* name_, int init_value_ )
: sc_prim_channel( name_ ), m_value( init_value_ )
{
    if( m_value < 0 ) {
	report_error( SC_ID_INVALID_SEMAPHORE_VALUE_ );
    }
}


// interface methods

// lock (take) the semaphore, block if not available

int
sc_semaphore::wait()
{
    while( in_use() ) {
	sc_prim_channel::wait( m_free );
    }
    -- m_value;
    return 0;
}


// lock (take) the semaphore, return -1 if not available

int
sc_semaphore::trywait()
{
    if( in_use() ) {
	return -1;
    }
    -- m_value;
    return 0;
}


// unlock (give) the semaphore

int
sc_semaphore::post()
{
    ++m_value;
    m_free.notify();
    return 0;
}

} // namespace sc_core

// Taf!
