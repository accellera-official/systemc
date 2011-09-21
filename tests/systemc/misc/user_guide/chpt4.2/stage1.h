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

  stage1.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename stage1.h */
/* This is the interface file for synchronous process `stage1' */

#include "systemc.h"

SC_MODULE( stage1 )
{
  SC_HAS_PROCESS( stage1 );

  sc_in_clk clk;

  const sc_signal<double>& in1; //input
  const sc_signal<double>& in2; //input
  sc_signal<double>& sum; //output
  sc_signal<double>& diff; //output

  //Constructor 
  stage1(sc_module_name NAME,
	 sc_clock& CLK,
	 const sc_signal<double>& IN1,
	 const sc_signal<double>& IN2,
	 sc_signal<double>& SUM,
	 sc_signal<double>& DIFF)
    : in1(IN1), in2(IN2), sum(SUM), diff(DIFF)
  {
    clk(CLK);
	SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


