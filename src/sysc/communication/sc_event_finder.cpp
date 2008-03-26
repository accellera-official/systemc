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

  sc_event_finder.cpp --

  Original Author: Martin Janssen, Synopsys, Inc.
                   Stan Y. Liao, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "sysc/communication/sc_event_finder.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_event_finder
//
//  Event finder base class.
// ----------------------------------------------------------------------------

// error reporting

void
sc_event_finder::report_error( const char* id, const char* add_msg ) const
{
    char msg[BUFSIZ];
    if( add_msg != 0 ) {
	sprintf( msg, "%s: port '%s' (%s)",
		 add_msg, m_port.name(), m_port.kind() );
    } else {
	sprintf( msg, "port '%s' (%s)", m_port.name(), m_port.kind() );
    }
    SC_REPORT_ERROR( id, msg );
}


// constructor

sc_event_finder::sc_event_finder( const sc_port_base& port_ )
: m_port( port_ )
{}


// destructor (does nothing)

sc_event_finder::~sc_event_finder()
{}

} // namespace sc_core

// Taf!
