/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2003 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test10.cpp -- Test sc_module::set_stack_size

  Original Author: Andy Goodrich, Forte Design Systemc, Inc. 2003-10-13

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"

SC_MODULE(A)
{
	SC_CTOR(A)
	{
		SC_THREAD(thread);
		sensitive << m_clk;
		set_stack_size(0x600000);
	}
	void thread()
	{
		int  x[0x100000];    // Grab a lot of stack...
		x[0x100000-1] = 42;  // ... and then modify the last location`

	    for (;;) 
		{
			cout << sc_simulation_time() << endl;
			wait();
		}
	}
	sc_in_clk m_clk;
};

int sc_main(int argc, char* argv[])
{
	sc_clock clock;
	A        a("a");
	a.m_clk(clock);

	sc_start(2);

	return 0;
}
