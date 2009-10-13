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

  test13.cpp -- Test detection of write from process and sc_main.

  Original Author: Andy Goodrich, Forte Design Systems, 02 Apr 2007

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
	}
	void thread()
	{
		m_data = false;
		for (;;)
		{
			wait();
		}
	}
	sc_in<bool>  m_clk;
	sc_out<bool> m_data;
};

int sc_main(int argc, char* argv[])
{
	sc_clock        clock;
	sc_signal<bool> data;
	DUT             dut("dut");

	dut.m_clk(clock);
	dut.m_data(data);

	sc_start(1, SC_NS);
	data = true;
	sc_start(1, SC_NS);

	cout << "Program completed" << endl;
	return 0;
}
