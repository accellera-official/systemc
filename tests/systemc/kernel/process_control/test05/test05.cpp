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
        
  test05.cpp -- Test user exception throws.
        
  Original Author: Andy Goodrich, Forte Design Systems, 15 December 2006
        
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:    
    
 *****************************************************************************/

// $Log: test05.cpp,v $
// Revision 1.3  2011/02/14 17:00:00  acg
//  Andy Goodrich: updated copyright and added cvs logging information inline.
//

#include "systemc.h"

class my_exception {};
class your_exception {};

SC_MODULE(DUT)
{
	SC_CTOR(DUT)
	{
		SC_CTHREAD(thread,m_clk.pos());
		SC_THREAD(monitor);
	}
	void monitor()
	{
		m_monitor_handle = sc_get_current_process_handle();
		for (;;)
		{
			try
			{
				wait(m_never_event);
			}
			catch (my_exception& except)
			{
				cout << sc_time_stamp() << " caught my exception " << endl;
			}
			catch (your_exception& except)
			{
				cout << sc_time_stamp() << " caught your exception " << endl;
			}
		}
	}
	void thread()
	{
		my_exception   exception;
		your_exception other_exception;
		for (;;)
		{
			wait(3);
			cout << sc_time_stamp() << " throwing my exception " << endl;
			m_monitor_handle.throw_it(exception);
			wait();

			// test that both exceptions appear.

			cout << sc_time_stamp() << " throwing my exception " << endl;
			m_monitor_handle.throw_it(exception);
			cout << sc_time_stamp() << " throwing your exception " << endl;
			m_monitor_handle.throw_it(other_exception);
			wait();
			wait();
			sc_stop();
		}
	}
	sc_in<bool>       m_clk;
	sc_process_handle m_monitor_handle;
	sc_event          m_never_event;
};

int sc_main(int argc, char* argv[])
{
	sc_clock        clock;
	DUT             dut("dut");

	dut.m_clk(clock);

	sc_start();

	cout << "Program completed" << endl;
	return 0;
}
