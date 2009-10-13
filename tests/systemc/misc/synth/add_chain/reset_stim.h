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

  reset_stim.h -- 

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
/***************************   reset_stim Function       **********************/
/******************************************************************************/
bool_vector8 mem [LIMIT + 1]; // Stimulus input memory

SC_MODULE( RESET_STIM )
{
    SC_HAS_PROCESS( RESET_STIM );

    sc_in_clk clk;

  /*** Input and Output Ports ***/
  sc_signal<bool>& 	ready;
  sc_signal<bool>& 	reset;
  sc_signal<int>&	addr;

  /*** Constructor ***/
  RESET_STIM (   sc_module_name    	NAME,
                      sc_clock&    TICK_N,
                      sc_signal<bool>&  READY,
                      sc_signal<bool>&  RESET,
  		      sc_signal<int>&	ADDR   )
 
    : 
		ready (READY),
		reset (RESET),
		addr  (ADDR)

    {
		clk (TICK_N);
        SC_CTHREAD( entry, clk.neg() );
    }
 
  /*** Call to Process Functionality ***/
  void entry();
 
};
 
void
RESET_STIM::entry()
{

//  LOAD MEMORY WITH DATA AT TIME ZERO

  ifstream 		stimulus ("add_chain/add_chain.dat");
  char			buffer[WIDTH+1];

  for(int i=1; i < LIMIT+1; i++) {
      stimulus >> buffer;
      mem[i] = buffer;  
  }
  
  stimulus.close();
 
//  INITIALIZE reset AND addr, THEN REMOVE RESET AFTER 2 CLOCK CYCLES 

  reset.write(0);	// reset = 0
  addr.write(1);	// addr = 1
  wait(2);

  reset.write(1);	// reset = 1
  wait();

// WAIT FOR LAST MEMORY ADDRESS, THEN 3 CLOCKS, THEN STOP SIMULATION 

  // do { wait(); } while (addr == LIMIT);	
  do { wait(); } while (!(addr == LIMIT));
  wait(LATENCY);
  do { wait(); } while (ready != 1);			
  sc_clock::stop();
  halt();
}
