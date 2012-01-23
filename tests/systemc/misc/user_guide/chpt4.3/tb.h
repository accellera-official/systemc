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

  tb.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename tb.h */
/* This is the interface file for the testbench module */

#include "systemc.h"

struct testbench : public sc_module {
  sc_signal<bool> out_clk_pos;
  sc_signal<bool> out_clk_neg;
  sc_signal<bool> out_clk2_pos;
  sc_signal<bool> out_clk2_neg;
  clk_pos clkp;
  clk_neg clkn;
  clk2_pos clkp2;
  clk2_neg clkn2;

  // Constructor
  testbench(sc_module_name NAME,
	    sc_clock&    TICK,
	    sc_clock&    TICK2)
    : sc_module (NAME),
      out_clk_pos	("out_clk_pos"),
      out_clk_neg	("out_clk_neg"),
      out_clk2_pos	("out_clk2_pos"),
      out_clk2_neg	("out_clk2_neg"),
      clkp 		("CLKP", TICK, out_clk_pos),
      clkn 		("CLKN", TICK, out_clk_neg),
      clkp2 		("CLKP2", TICK2, out_clk2_pos),
      clkn2 		("CLKN2", TICK2, out_clk2_neg)
    {
    }
};
