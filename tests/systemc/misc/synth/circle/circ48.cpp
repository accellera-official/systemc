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

  circ48.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "tb.h"
#include "circ48.h" 	

int
sc_main( int, char *[] )
{
    sc_clock clk( "clk", 20, 0.5, 10 );

    sc_signal<bool>    I_reset;
    sc_signal<bool>    I_x_ok;
    sc_signal<bool>    I_y_ok;
    sc_signal<bool>    O_out_wr;
    sc_signal<bool>    O_out_sel;
    signal_bool_vector O_out_xy;
    signal_bool_vector O_diffs;

    testbench tb( "TB", clk, I_reset, I_x_ok, I_y_ok,
		  O_out_wr, O_out_sel, O_out_xy );

    circ48 c1( "C1", clk, I_reset, I_x_ok, I_y_ok,
	       O_out_wr, O_out_sel, O_out_xy, O_diffs );

    sc_start();

    return 0;
}
