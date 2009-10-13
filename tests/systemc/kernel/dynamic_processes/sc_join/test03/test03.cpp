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

  test03.cpp -- Test sc_join as gating mechanism for a process awaiting the
                demise of its child processes.

  Original Author: Andy Goodrich, Forte Design Systems, 18 April 2005
    
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:    
    
 *****************************************************************************/

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"

SC_MODULE(TB)
{
	SC_CTOR(TB)
	{
		SC_THREAD(abc);
		sensitive << m_clk.pos();
	}
	void abc()
	{
		for ( int i = 0; i < 3; i++ )
		{
			cout << "Time  Spawn Start Stop " << endl;
			cout << "----- ----- ----- ----" << endl;
			int              ii = 2;
			int              spawn_i;
			int              spawn_n = 8;
			sc_spawn_options options;
			sc_join          join;
			options.set_sensitivity(&m_clk.pos());
			for ( spawn_i = 0; spawn_i < spawn_n; spawn_i++ )
			{
				int process_i = spawn_i + i * spawn_n;
				cout << sc_time_stamp() << " " << process_i << endl;
				join.add_process(sc_spawn(
				    sc_bind(&TB::process, this, sc_ref(process_i)),
					sc_gen_unique_name("pipe"), &options ) );
				sc_core::wait(ii);
			}
			cout << sc_time_stamp() << " waiting for termination of " 
			     << join.process_count() << " processes" << endl;
			join.wait();
			cout << sc_time_stamp() << " back from termination wait " << endl;
		}
	}
	void process( int& instance )
	{
	    int i = instance;
		cout << sc_time_stamp() << "        " << i << endl;
		wait(6);
		cout << sc_time_stamp() << "              " << i << endl;
	}
	sc_in<bool> m_clk;
};


int sc_main(int argc, char* argv[])
{
	sc_clock clock;
	TB		 tb("tb");

	tb.m_clk(clock);
	sc_start(100);
	cout << "Program completed." << endl;
	return 0;
}
