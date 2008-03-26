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

  sc_interface.cpp -- Abstract base class of all interface classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


// $Log: sc_interface.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:35  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:47:42  acg
// Added $Log command so that CVS comments are reproduced in the source.
//

#include "sysc/communication/sc_interface.h"
#include "sysc/communication/sc_communication_ids.h"
#include "sysc/kernel/sc_event.h"


namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_interface
//
//  Abstract base class of all interface classes.
//  BEWARE: Direct inheritance from this class must be done virtual.
// ----------------------------------------------------------------------------

// register a port with this interface (does nothing by default)

void
sc_interface::register_port( sc_port_base&, const char* )
{}


// get the default event

const sc_event&
sc_interface::default_event() const
{
    SC_REPORT_WARNING( SC_ID_NO_DEFAULT_EVENT_, 0 );
    return m_never_notified;
}


// destructor (does nothing)

sc_interface::~sc_interface()
{}


// constructor (does nothing)

sc_interface::sc_interface()
{}


sc_event sc_interface::m_never_notified;

} // namespace sc_core

// Taf!
