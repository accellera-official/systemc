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

  data_gen.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "define.h"

/******************************************************************************/
/***************************    data_gen Function        **********************/
/******************************************************************************/

SC_MODULE( DATA_GEN )
{
    SC_HAS_PROCESS( DATA_GEN );

    sc_in_clk clk;

  /*** Input and Output Ports ***/
  const sc_signal<bool>&       	ready;
        signal_bool_vector8&     	data;
        sc_signal<int>&		addr;
 
  /*** Constructor ***/
  DATA_GEN (   sc_module_name                	NAME,
                     sc_clock&            	TICK_N,
               const sc_signal<bool>&    	READY,
                     signal_bool_vector8& 	DATA,
  		     sc_signal<int>&		ADDR   )
 
    : clk (TICK_N),
		ready (READY),
              	data  (DATA),	// 8 bits
		addr  (ADDR)

    {
        SC_CTHREAD( entry, clk.neg() );
    }
 
  /*** Call to Process Functionality ***/
  void entry();
 
};
 
void
DATA_GEN::entry()
{
  while(true) {

//  WAIT FOR POSEDGE OF ready 

    at_posedge(ready);

//  CHECK TO SEE IF THE END OF MEMORY HAS BEEN REACHED

    if(addr.read() > LIMIT) { 		// if(addr > LIMIT)
	break; 
    }

//  WRITE VALUE OF MEMORY AT CURRENT ADDRESS TO data 
//  INCREMENT addr BY 1  

    data.write(mem[addr.read()]);	// data = mem[addr]
    addr.write(addr.read() + 1);   	// addr = addr + 1 
  }

}
