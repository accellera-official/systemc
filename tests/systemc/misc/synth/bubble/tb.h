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

  tb.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/******************************************************************************/
/*************************** testbench Class Definition    ********************/
/******************************************************************************/

#include "common.h"

SC_MODULE( TESTBENCH )
{
    sc_signal<bool> 	reset;
    sc_signal<bool>    	in_ok;
    sc_signal<bool> 	out_ok;
    sc_signal<bool> 	instrb;
    sc_signal<bool> 	outstrb;
    signal_bool_vector  a1,a2,a3,a4,a5,a6,a7,a8; 	// -128 to 127
    signal_bool_vector  d1,d2,d3,d4,d5,d6,d7,d8; 	// -128 to 127
    STIM		st1;
    BUBBLE		bubble;
    DISPLAY		disp1;
 
    TESTBENCH( 	sc_module_name 	NAME,
		      sc_clock&	TICK  ) 
        
       : st1	("ST1", TICK, reset, in_ok, out_ok, instrb, outstrb,
		 a1, a2, a3, a4, a5, a6, a7, a8,
		 d1, d2, d3, d4, d5, d6, d7, d8) ,

	 bubble	("B1", TICK, reset, in_ok, out_ok, instrb, outstrb,
		 a1, a2, a3, a4, a5, a6, a7, a8, 
		 d1, d2, d3, d4, d5, d6, d7, d8) ,

	 disp1 	("D1", reset, in_ok, out_ok, instrb, outstrb,
		 a1, a2, a3, a4, a5, a6, a7, a8, 
		 d1, d2, d3, d4, d5, d6, d7, d8)
    {}
};
