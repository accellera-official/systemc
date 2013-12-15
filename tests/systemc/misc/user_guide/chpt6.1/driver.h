/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  driver.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename driver.h */
/* This is the interface file for synchronous process `driver' */

#include "common.h"

SC_MODULE( driver )
{
  SC_HAS_PROCESS( driver );

  sc_in_clk clk;

  const signal_std_logic& in; //input
  sc_signal<bool>& control; //output
  sc_signal<bool>& out; //output

  //Constructor 
  driver(sc_module_name NAME,
	 sc_clock& CLK,
	 const signal_std_logic& IN_,
	 sc_signal<bool>& CONTROL,
	 sc_signal<bool>& OUT_)
    : in(IN_), control(CONTROL), out(OUT_)
  {
    clk(CLK);
	SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


