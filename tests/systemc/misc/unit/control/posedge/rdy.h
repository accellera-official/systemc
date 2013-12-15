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

  rdy.h -- 

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
/***************************    rdy Function             **********************/
/******************************************************************************/

SC_MODULE( RDY )
{
  SC_HAS_PROCESS( RDY );

  sc_in_clk clk;

  /*** Input and Output Ports ***/
  sc_signal<bool>& data;
 
  /*** Constructor ***/
  RDY (   sc_module_name        	NAME,
                sc_clock&		TICK_N,
                sc_signal<bool>&  	DATA )
 
    : 
		data (DATA)

    {
      clk (TICK_N);
	  SC_CTHREAD( entry, clk.neg() );
    }
 
  /*** Call to Process Functionality ***/
  void entry();
 
};

void
RDY::entry()
{
  // IMPLICIT wait(); AT FIRST NEGEDGE
  cout << sc_time_stamp() << " : "	// Time 10
       << "ready = " << data 
       << "\t\t (RDY) "
       << endl;
  data.write(0); 

  wait();						// Time 30
  cout << sc_time_stamp() << " : "	
       << "ready = " << data 
       << "\t\t (RDY) "
       << endl;
  data.write(1); 

  wait(); 						// Time 50
  cout << sc_time_stamp() << " : "
       << "ready = " << data 
       << "\t\t (RDY) "
       << endl;
  data.write(0); 

  wait(); 						// Time 70
  cout << sc_time_stamp() << " : "
       << "ready = " << data 
       << "\t\t (RDY) "
       << endl;
  data.write(1); 

  wait(); 						// Time 90
  cout << sc_time_stamp() << " : "
       << "ready = " << data 
       << "\t\t (RDY) "
       << endl;

  halt();
}
