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

  rdy_gen.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/******************************************************************************/
/***************************    RDY_GEN 		 **********************/
/******************************************************************************/

SC_MODULE( RDY_GEN )
{
    SC_HAS_PROCESS( RDY_GEN );

    sc_in_clk clk;

  sc_signal<bool>& ready;  // Output
 
  RDY_GEN (   sc_module_name        	NAME,
                sc_clock&		TICK_P,
                sc_signal<bool>&  	READY )
 
    : clk (TICK_P),
		ready (READY)

  {
      SC_CTHREAD( entry, clk.pos() );
  }
 
  void entry();
 
};

void
RDY_GEN::entry()
{
  ready.write(1);
  wait();
  cout << sc_simulation_time() << " : "
       << "WRITING ready = 1" << endl;

  ready.write(0);
  wait(); 
  cout << sc_simulation_time() << " : "
       << "WRITING ready = 0" << endl;

  ready.write(1);
  wait();
  cout << sc_simulation_time() << " : "
       << "WRITING ready = 1" << endl;
}
