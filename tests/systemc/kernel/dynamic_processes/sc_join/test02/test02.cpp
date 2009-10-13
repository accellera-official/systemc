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

  test02.cpp -- Test SC_FORK and SC_JOIN macros.

  Original Author: Andy Goodrich, Forte Design Systems, 10 October 2004
    
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:    
    
 *****************************************************************************/

#include "systemc.h"
#include "sysc/kernel/sc_dynamic_processes.h"

SC_MODULE(X)
{
	SC_CTOR(X)
	{

		SC_THREAD(waiting);
	}
	void sync(int context)
	{
		for ( int i = 0; i < context; i++ ) 
		{
			wait(m_clk.posedge_event());
		}
		cout << sc_time_stamp() << ": sync(" << context << ") terminating"<< endl;
	}
	void waiting()
	{
		SC_FORK
		    sc_spawn( sc_bind( &X::sync, this, 3 ) ),
		    sc_spawn( sc_bind( &X::sync, this, 4 ) ),
		    sc_spawn( sc_bind( &X::sync, this, 5 ) ),
		    sc_spawn( sc_bind( &X::sync, this, 5 ) ),
		    sc_spawn( sc_bind( &X::sync, this, 7 ) ),
		    sc_spawn( sc_bind( &X::sync, this, 11) ),
		    sc_spawn( sc_bind( &X::sync, this, 21) )
		SC_JOIN
		cout << sc_time_stamp() << ": waiting waking" << endl;
	}

	sc_in_clk m_clk;
	sc_join   m_join;
};

int sc_main( int argc, char* argv[] )
{
	sc_clock clock;
	X x("x");
	x.m_clk(clock);

	sc_start(1000);

	cerr << "Program completed" << endl;
	return 0;
}

