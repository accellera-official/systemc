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

  stim.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

/******************************************************************************/
/***************************    stim Function            **********************/
/******************************************************************************/

SC_MODULE( STIM )
{
  SC_HAS_PROCESS( STIM );

  sc_in_clk clk;

  /*** Input and Output Ports ***/
  const sc_signal<bool>& data;
 
  /*** Constructor ***/
  STIM (   sc_module_name        	NAME,
                 sc_clock&		TICK_P,
           const sc_signal<bool>&  	DATA )
 
    : 
		data (DATA)

    {
      clk (TICK_P);
	  SC_CTHREAD( entry, clk.pos() );
    }
 
  /*** Call to Process Functionality ***/
  void entry();
 
};

void
STIM::entry()
{
  while(true) {
    at_posedge(data);			
    cout << sc_time_stamp() << " : "	
         << "POSEDGE READY DETECTED" 
	 << endl;
  }
}
