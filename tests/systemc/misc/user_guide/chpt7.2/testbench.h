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

  const sc_signal<int>& Ssum; //input
  const sc_signal<int>& Sdiff; //input
  const sc_signal<bool>& adder_sub_done; //input
  sc_signal<int>& Sa; //output
  sc_signal<int>& Sb; //output
  sc_signal<int>& Sc; //output
  sc_signal<bool>& adder_sub_ready; //output

  //Constructor 
  testbench(sc_module_name NAME,
	    sc_clock& CLK,
	    const sc_signal<int>& SSUM,
	    const sc_signal<int>& SDIFF,
	    const sc_signal<bool>& ADDER_SUB_DONE,
	    sc_signal<int>& SA,
	    sc_signal<int>& SB,
	    sc_signal<int>& SC,
	    sc_signal<bool>& ADDER_SUB_READY)
    : clk(CLK), Ssum(SSUM), Sdiff(SDIFF), 
      adder_sub_done(ADDER_SUB_DONE),
      Sa(SA), Sb(SB), Sc(SC), adder_sub_ready(ADDER_SUB_READY)
      
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


