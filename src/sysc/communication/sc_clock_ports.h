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

  sc_clock_ports.h -- The clock ports.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG IS AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_CLOCK_PORTS_H
#define SC_CLOCK_PORTS_H


#include "sysc/communication/sc_signal_ports.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  The clock ports.
//
//  (Provided for backward compatibility reasons.)
// ----------------------------------------------------------------------------

typedef sc_in<bool>    sc_in_clk;
typedef sc_inout<bool> sc_inout_clk;
typedef sc_out<bool>   sc_out_clk;

} // namespace sc_core

//$Log: sc_clock_ports.h,v $
//Revision 1.3  2011/08/26 20:45:39  acg
// Andy Goodrich: moved the modification log to the end of the file to
// eliminate source line number skew when check-ins are done.
//
//Revision 1.2  2011/02/18 20:23:45  acg
// Andy Goodrich: Copyright update.
//
//Revision 1.1.1.1  2006/12/15 20:20:04  acg
//SystemC 2.3
//
//Revision 1.2  2006/01/03 23:18:26  acg
//Changed copyright to include 2006.
//
//Revision 1.1.1.1  2005/12/19 23:16:43  acg
//First check in of SystemC 2.1 into its own archive.
//
//Revision 1.8  2005/06/10 22:43:55  acg
//Added CVS change log annotation.
//

#endif

// Taf!
