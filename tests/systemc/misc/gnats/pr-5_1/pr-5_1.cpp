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

  pr-5_1.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( matrix )
{
    SC_HAS_PROCESS( matrix );

    sc_in_clk clk;

    // Constructor
    matrix (
        sc_module_name NAME, 
        sc_clock& CLK,

        const sc_signal<bool>& RESET
	)
          : reset(RESET)
    {
        clk (CLK);
		SC_CTHREAD( entry, clk.pos() );
		reset_signal_is(reset,true);
    }

    void entry();

    // INPUTS
    const sc_signal<bool>& reset;
};

int sc_main(int, char**) { return 0; }
