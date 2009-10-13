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

  accumulator.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename accumulator.h */
/* This is the interface file for synchronous process `accumulator' */

#include "systemc.h"

SC_MODULE( accumulator )
{
  SC_HAS_PROCESS( accumulator );

  sc_in_clk clk;

  const sc_signal<int>& number; //input
  const sc_signal<bool>& reset_add; //input
  const sc_signal<bool>& reset_mult; //input
  sc_signal<int>& sum; //output
  sc_signal<int>& prod; //output
  int sum_acc; //internal variable
  int mult_acc; //internal variable

  //Constructor 
  accumulator(sc_module_name NAME,
	      sc_clock& CLK,
	      const sc_signal<int>& NUMBER,
	      const sc_signal<bool>& RESET_ADD,
	      const sc_signal<bool>& RESET_MULT,
	      sc_signal<int>& SUM,
	      sc_signal<int>& PROD)
    : clk(CLK), number(NUMBER), reset_add(RESET_ADD), reset_mult(RESET_MULT), sum(SUM), prod(PROD)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


