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

  test18.cpp -- Test that time cannot go backwards in simulator

  Original Author: Andy Goodrich, Forte Design Systems, 2006-05-03

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: test18.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:26:06  acg
// systemc_tests-2.3
//
// Revision 1.1  2006/05/03 19:37:10  acg
//  Andy Goodrich: new test to check that time will not run backward if a
//  negative value is given to sc_event::notify()
//

#include "systemc.h"

SC_MODULE(DUT)
{
	SC_CTOR(DUT) 
	{
		SC_METHOD(catcher);
		sensitive << m_event;
		dont_initialize();
		SC_CTHREAD(thrower,m_clk.pos());
	}
	void catcher()
	{
		cout << sc_time_stamp() << " caught" << endl;
	}
	void thrower()
	{
		sc_time minus_four(-4.0, SC_NS);
		wait(7);
		for (;;)
		{
			wait();
			cout << sc_time_stamp() << " throwing" << endl;
			m_event.notify(minus_four);
		}
	}
	sc_in<bool> m_clk;
	sc_event    m_event;
};

int sc_main(int argc, char* argv[])
{
	sc_clock        clock;
	DUT             dut("dut");

	dut.m_clk(clock);

	sc_start(11, SC_NS);

	cout << "Program completed" << endl;
	return 0;
}
