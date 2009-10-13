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

  wait_rdy.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/******************************************************************************/
/***************************    WAIT_RDY 		 **********************/
/******************************************************************************/

SC_MODULE( WAIT_RDY )
{
    SC_HAS_PROCESS( WAIT_RDY );

    sc_in_clk clk;

  const sc_signal<bool>& ready;	// Input
 
  WAIT_RDY (   sc_module_name        	NAME,
               sc_clock&		TICK_P,
               const sc_signal<bool>&  	READY )
 
    : clk (TICK_P),
		ready (READY)

    {
        SC_CTHREAD( entry, clk.pos() );
    }
 
  void entry();
 
};

void
WAIT_RDY::entry()
{
  while (true) {
    wait_until(ready.delayed() == 1); 
  
    cout << sc_simulation_time() << " : "
         << "READY = 1 DETECTED" << endl;
  }
}
