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

  sc_stop_here.cpp -- Function provided for debugging purposes.
                      This file is always compiled in debug mode, such that
                      setting a breakpoint at this function can help locate
                      the cause of a SystemC error or warning.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-11-14

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "sysc/utils/sc_stop_here.h"


namespace sc_core {

// ----------------------------------------------------------------------------
//  FUNCTION : sc_interrupt_here
//
//  Debugging aid for warning, error, and fatal reports.
//  This function *cannot* be inlined.
// ----------------------------------------------------------------------------

void
sc_interrupt_here( int id, sc_severity severity )
{
    // you can set a breakpoint at some of the lines below, either to
    // interrupt with any severity, or to interrupt with a specific severity

    switch( severity ) {
      case SC_INFO: 
	static int info_id;
	info_id = id;
	break;
      case SC_WARNING: 
	static int warning_id;
	warning_id = id;
	break;
      case SC_ERROR: 
	static int error_id;
	error_id = id;
	break;
      default:
      case SC_FATAL: 
	static int fatal_id;
	fatal_id = id;
	break;
    }
}


// ----------------------------------------------------------------------------
//  FUNCTION : sc_stop_here
//
//  Debugging aid for warning, error, and fatal reports.
//  This function *cannot* be inlined.
// ----------------------------------------------------------------------------

void
sc_stop_here( int id, sc_severity severity )
{
    // you can set a breakpoint at some of the lines below, either to
    // stop with any severity, or to stop with a specific severity

    switch( severity ) {
      case SC_INFO: 
	static int info_id;
	info_id = id;
	break;
      case SC_WARNING: 
	static int warning_id;
	warning_id = id;
	break;
      case SC_ERROR: 
	static int error_id;
	error_id = id;
	break;
      default:
      case SC_FATAL: 
	static int fatal_id;
	fatal_id = id;
	break;
    }
}

} // namespace sc_core

// Taf!
