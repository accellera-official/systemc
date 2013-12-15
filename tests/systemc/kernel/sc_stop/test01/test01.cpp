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

  test01.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems, Inc. 

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: 
  Description of Modification: 

 *****************************************************************************/

#include "systemc.h"

SC_MODULE(X)
{
	SC_CTOR(X)
	{
		cout << "Creating able..." << endl;
		SC_THREAD(able);
		sensitive << clk.pos();
		cout << "Creating baker..." << endl;
		SC_THREAD(baker);
		sensitive << clk.pos();
		cout << "Creating charlie..." << endl;
		SC_THREAD(charlie);
		sensitive << clk.pos();
	}
		
	void able()
	{
		for (;;)
		{
			wait();
			cout << "able: " << sc_time_stamp() << endl;
			sc_stop();
		}
	}
	void baker()
	{
		for (;;)
		{
			wait();
			cout << "baker: " << sc_time_stamp() << endl;
			sc_stop();
		}
	}
	void charlie()
	{
		for (;;)
		{
			wait();
			cout << "charlie: " << sc_time_stamp() << endl;
			sc_stop();
		}
	}
	sc_in_clk clk;
};

int sc_main(int argc, char* argv[] )
{
	sc_clock clock;
	X        x("x");
	x.clk(clock);

	sc_set_stop_mode(SC_STOP_IMMEDIATE);
	sc_start(100, SC_NS);
	return 0;
}
