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

  test03.cpp -- Test of disabling of SC_FATAL

  Original Author: Andy Goodrich, Forte Design Systems, Inc., 2005-12-12

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE(DUT)
{
	SC_CTOR(DUT)
	{
		SC_CTHREAD(thread,m_clk.pos());
		reset_signal_is(m_reset, true);
	}
	void thread()
	{
		sc_report_handler::set_actions(SC_FATAL,SC_DISPLAY);
		sc_report_handler::stop_after(SC_FATAL,-1);
		for (;;)
		{
			wait();
			SC_REPORT_FATAL("Oh no!","A bad thing has happened");
		}
	}
	sc_in<bool> m_clk;
	sc_in<bool> m_reset;
};
int sc_main(int argc, char* argv[])
{
	sc_clock        clock;
	DUT             dut("dut");
	sc_signal<bool> reset;

	dut.m_clk(clock);
	dut.m_reset(reset);

	reset = true;
	sc_start(1, SC_NS);
	reset = false;
	sc_start(2, SC_NS);

	cout << "Program completed" << endl;
	return 0;
}
