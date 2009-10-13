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
/* This is the interface file for synchronous process 'testbench' */

#include "systemc.h"

SC_MODULE( testbench )
{
  SC_HAS_PROCESS( testbench );

  sc_in_clk clk;

  sc_in<bool>  q;
  sc_in<bool>  qp;
  sc_out<bool> s;
  sc_out<bool> r;

  // Constructor 
  testbench( sc_module_name NAME,
	     sc_clock& CLK,
	     sc_signal<bool>& Q,
	     sc_signal<bool>& QP,
	     sc_signal<bool>& S,
	     sc_signal<bool>& R )
    : clk(CLK), q(Q), qp(QP), s(S), r(R)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};

