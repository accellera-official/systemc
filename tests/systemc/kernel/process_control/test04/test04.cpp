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

  test04.cpp -- Test of interaction of suspend-resume, disable-enable, and
                resets on processes

  Original Author: Andy Goodrich

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  Revision log at end of the file to let __LINE__ give the same results 
  after a check-in.
 *****************************************************************************/

#include "systemc.h"

SC_MODULE(DUT)
{
	SC_CTOR(DUT)
	{
		SC_CTHREAD(thread0,m_clk.pos());
		SC_CTHREAD(thread1,m_clk.pos());
	}
	void thread0()
	{
		m_handle0 = sc_get_current_process_handle();
		cout << sc_time_stamp() << " in reset" << endl;
		for (;;)
		{
			wait();
			cout << sc_time_stamp() << " before thread0 self-suspend" << endl;
			m_handle0.suspend();
			cout << sc_time_stamp() << " after thread0 self-suspend" << endl;
			wait();
			cout << sc_time_stamp() << " before thread0 self-disable" << endl;
			m_handle0.disable();
			cout << sc_time_stamp() << " after thread0 self-disable" << endl;
		}
	}
	void thread1()
	{
		m_handle1 = sc_get_current_process_handle();
		for (;;)
		{
			wait();
			wait();
			wait();
			cout << sc_time_stamp() << " resuming thread0 " << endl;
			m_handle0.resume();
			wait();
			wait();
			wait();
			cout << sc_time_stamp() << " enabling thread0 " << endl;
			m_handle0.enable();
			wait();
			cout << sc_time_stamp() << " synchronous reset on thread0 " << endl;
			m_handle0.sync_reset_on();
			wait();
			wait();
			wait();
			cout << sc_time_stamp() << " no synchronous reset on thread0 " 
			     << endl;
			m_handle0.sync_reset_off();
			wait();
			wait();
			wait();
			cout << sc_time_stamp() << " asynchronous reset on thread0 " 
			     << endl;
			m_handle0.reset();
			wait(20);
			sc_stop();
		}
	}
	sc_in<bool>       m_clk;
	sc_process_handle m_handle0;
	sc_process_handle m_handle1;
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
