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

  testbench.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename testbench.h */
/* This is the interface file for synchronous process `testbench' */

#include "systemc.h"

SC_MODULE( testbench )
{
  SC_HAS_PROCESS( testbench );

  sc_in_clk clk;

  const sc_signal<int>& resulta; //input
  const sc_signal<int>& resultm; //input
  sc_signal<int>& number; //output
  sc_signal<bool>& add_reset; //output
  sc_signal<bool>& mult_reset; //output

  //Constructor 
  testbench(sc_module_name NAME,
	    sc_clock& CLK,
	    const sc_signal<int>& RESULTA,
	    const sc_signal<int>& RESULTM,
	    sc_signal<int>& NUMBER,
	    sc_signal<bool>& ADD_RESET,
	    sc_signal<bool>& MULT_RESET)
    : clk(CLK), resulta(RESULTA), resultm(RESULTM), number(NUMBER), add_reset(ADD_RESET), mult_reset(MULT_RESET)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


