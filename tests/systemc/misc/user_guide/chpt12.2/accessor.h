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

  accessor.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename accessor.h */
/* This is the interface file for synchronous process 'accessor' */

#include "common.h"

SC_MODULE( accessor )
{
  SC_HAS_PROCESS( accessor );

  sc_in_clk clk;

  const signal_bool_vector32& datain;     //input
  sc_signal<bool>& chip_select;   //output
  sc_signal<bool>& write_enable;  //output
  signal_bool_vector10& address;    //output
  signal_bool_vector32& dataout;    //output

  // Parameter
  const int memory_latency;   

  //Constructor 
  accessor(sc_module_name NAME,
	   sc_clock& CLK,
	   const signal_bool_vector32& DATAIN,
	   sc_signal<bool>& CHIP_SELECT,
	   sc_signal<bool>& WRITE_ENABLE,
	   signal_bool_vector10& ADDRESS,
	   signal_bool_vector32& DATAOUT,
	   const int MEMORY_LATENCY = 1)
    : datain(DATAIN), chip_select(CHIP_SELECT), 
      write_enable(WRITE_ENABLE), address(ADDRESS), dataout(DATAOUT),
      memory_latency(MEMORY_LATENCY)
  {
    clk(CLK);
	SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


