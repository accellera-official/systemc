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

  sc_stop_here.cpp -- Function provided for debugging purposes.
                      This file is always compiled in debug mode, such that
                      setting a breakpoint at this function can help locate
                      the cause of a SystemC error or warning.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-11-14

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#include "sysc/utils/sc_stop_here.h"


namespace sc_core {

static const char* info_id    = 0;
static const char* warning_id = 0;
static const char* error_id   = 0;
static const char* fatal_id   = 0;

// ----------------------------------------------------------------------------
//  FUNCTION : sc_interrupt_here
//
//  Debugging aid for warning, error, and fatal reports.
//  This function *cannot* be inlined.
// ----------------------------------------------------------------------------

void
sc_interrupt_here( const char* id, sc_severity severity )
{
    // you can set a breakpoint at some of the lines below, either to
    // interrupt with any severity, or to interrupt with a specific severity

    switch( severity ) {
      case SC_INFO: 
	info_id = id;
	break;
      case SC_WARNING: 
	warning_id = id;
	break;
      case SC_ERROR: 
	error_id = id;
	break;
      default:
      case SC_FATAL: 
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
sc_stop_here( const char* id, sc_severity severity )
{
    // you can set a breakpoint at some of the lines below, either to
    // stop with any severity, or to stop with a specific severity

    switch( severity ) {
      case SC_INFO: 
	info_id = id;
	break;
      case SC_WARNING: 
	warning_id = id;
	break;
      case SC_ERROR: 
	error_id = id;
	break;
      default:
      case SC_FATAL: 
	fatal_id = id;
	break;
    }
}

} // namespace sc_core

// $Log: sc_stop_here.cpp,v $
// Revision 1.4  2011/08/26 21:49:08  acg
//  Philipp A. Hartmann: eliminate compiler warning by moving static variables
//  out of functions.
//
// Revision 1.3  2011/08/26 20:46:19  acg
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

// Taf!
