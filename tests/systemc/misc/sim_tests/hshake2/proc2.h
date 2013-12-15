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

  proc2.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename proc2.h */
/* This is the interface file for synchronous process `proc2' */

#include "systemc.h"

SC_MODULE( proc2 )
{
  SC_HAS_PROCESS( proc2 );

  sc_in_clk clk;

  sc_in<bool>  data_ready;
  sc_in<int>   data;
  sc_out<bool> data_ack;

  // Constructor 
  proc2( sc_module_name NAME,
	 sc_clock& CLK,
	 sc_signal<bool>& DATA_READY,
	 sc_signal<int>& DATA,
	 sc_signal<bool>& DATA_ACK )
  {
    clk(CLK);
    data_ready(DATA_READY); data(DATA); data_ack(DATA_ACK);
	SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};

