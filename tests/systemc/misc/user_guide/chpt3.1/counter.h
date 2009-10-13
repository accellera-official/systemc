/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  counter.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Header file for the counter that counts number of strings found */

#include "systemc.h"

SC_MODULE( counter )
{
  SC_HAS_PROCESS( counter );

  sc_in_clk clk;

  // The inputs
  const sc_signal<bool>& found;

  // The constructor
  counter(sc_module_name NAME,
	  sc_clock& POS_CLK,
	  const sc_signal<bool>& FOUND)
    : clk(POS_CLK), found(FOUND) {
    SC_CTHREAD( entry, clk.pos() );
  }

  // The main functionality of the process
  void entry();
};
