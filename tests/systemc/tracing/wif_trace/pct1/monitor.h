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

  monitor.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( monitor )
{
  SC_HAS_PROCESS( monitor );

  sc_in_clk   clk;
  sc_in<bool> tx;
  sc_in<int>  dout;
  sc_in<bool> wr;

  // Constructor 
  monitor( sc_module_name   NAME,
	   sc_clock&        CLK,
	   sc_signal<bool>& TX,
	   sc_signal<int>&  DOUT,
	   sc_signal<bool>& WR )
    : tx(TX), dout(DOUT), wr(WR)
  {
    clk(CLK);
	SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};
