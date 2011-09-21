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

  stage2.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename stage2.h */
/* This is the interface file for synchronous process `stage2' */

#include "systemc.h"

SC_MODULE( stage2 )
{
  SC_HAS_PROCESS( stage2 );

  sc_in_clk clk;

  const sc_signal<double>& sum; //input
  const sc_signal<double>& diff; //input
  sc_signal<double>& prod; //output
  sc_signal<double>& quot; //output

  //Constructor 
  stage2(sc_module_name NAME,
	 sc_clock& CLK,
	 const sc_signal<double>& SUM,
	 const sc_signal<double>& DIFF,
	 sc_signal<double>& PROD,
	 sc_signal<double>& QUOT)
    : sum(SUM), diff(DIFF), prod(PROD), quot(QUOT)
  {
    clk(CLK);
	SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


