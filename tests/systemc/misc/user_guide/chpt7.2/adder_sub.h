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

  adder_sub.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename adder_sub.h */
/* This is the interface file for synchronous process `adder_sub' */

#include "systemc.h"

SC_MODULE( adder_sub )
{
  SC_HAS_PROCESS( adder_sub );

  sc_in_clk clk;

  const sc_signal<int>& Sa; //input
  const sc_signal<int>& Sb; //input
  const sc_signal<int>& Sc; //input
  const sc_signal<bool>& adder_sub_ready; //input
  sc_signal<int>& Sd; //output
  sc_signal<int>& Ssum; //output
  sc_signal<bool>& adder_sub_done; //output

  //Constructor 
  adder_sub(sc_module_name NAME,
	    sc_clock& CLK,
	    const sc_signal<int>& SA,
	    const sc_signal<int>& SB,
	    const sc_signal<int>& SC,
	    const sc_signal<bool>& ADDER_SUB_READY,  
	    sc_signal<int>& SD,
	    sc_signal<int>& SSUM,
	    sc_signal<bool>& ADDER_SUB_DONE)
    : clk(CLK), Sa(SA), Sb(SB), Sc(SC), 
      adder_sub_ready(ADDER_SUB_READY),
      Sd(SD), Ssum(SSUM), adder_sub_done(ADDER_SUB_DONE)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


