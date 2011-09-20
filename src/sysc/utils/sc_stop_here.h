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

  sc_stop_here.h -- Function provided for debugging purposes.
                    This file is always compiled in debug mode, such that
                    setting a breakpoint at this function can help locate
                    the cause of a SystemC error or warning.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-11-14

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

// $Log: sc_stop_here.h,v $
// Revision 1.3  2011/08/26 20:46:19  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
#ifndef SC_STOP_HERE_H
#define SC_STOP_HERE_H


#include "sysc/utils/sc_report.h"


namespace sc_core {

// ----------------------------------------------------------------------------
//  FUNCTION : sc_interrupt_here
//
//  Debugging aid for interrupt warning, error, and fatal reports.
// ----------------------------------------------------------------------------

extern
void
sc_interrupt_here( const char* id, sc_severity severity );


// ----------------------------------------------------------------------------
//  FUNCTION : sc_stop_here
//
//  Debugging aid for warning, error, and fatal reports.
// ----------------------------------------------------------------------------

extern
void
sc_stop_here( const char* id, sc_severity severity );

} // namespace sc_core

#endif

// Revision 1.2  2011/02/18 20:38:44  acg
//  Andy Goodrich: Updated Copyright notice.
//
// Revision 1.1.1.1  2006/12/15 20:20:06  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:11  acg
// Andy Goodrich: Added $Log command so that CVS comments are reproduced in
// the source.
//

// Taf!
