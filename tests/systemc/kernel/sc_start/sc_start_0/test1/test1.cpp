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

  test1.cpp -- Test alternations of sc_start(0, SC_NS) and sc_start(1, SC_NS).

  Original Author: Andy Goodrich, Forte Design Systems, 18 August 2006

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: 
  Description of Modification: - 

 *****************************************************************************/


#include "systemc.h"

SC_MODULE(X)
{
	SC_CTOR(X)
	{
		SC_METHOD(comb);
		sensitive << tweak;
		SC_CTHREAD(sync, clk.pos());
	}
	void comb()
	{
		cout << "  X::comb() - " << 
			sc_time_stamp() << "  " <<
			tweak.read() << endl;
	}
	void sync()
	{
		for (;;)
		{
			wait();
			cout << "  X::sync() - " << 
				sc_time_stamp() << "  " <<
				tweak.read() << endl;
		}
	}
	sc_in_clk   clk;
	sc_in<bool> tweak;
};

#define ACTION(action,descr) \
{ \
	cout << descr << " - " << \
		sc_time_stamp() << "   " << \
		tweak.read() << endl; \
	action ; \
	cout << "                                        " << \
                sc_time_stamp() << "   " << \
		tweak.read() << endl; \
}


int sc_main(int argc, char* argv[])
{
	sc_clock clock;
	sc_signal<bool> tweak;
	X        x("x");
	x.clk(clock);
	x.tweak(tweak);

	cout << "Event         Start Parameters         End Parameters" << endl;
	cout << "------------- ----------------         --------------\n" << endl;

	ACTION(sc_start(1, SC_NS),"sc_start(1, SC_NS)")
	ACTION(tweak = !tweak,"~tweak     ")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(1, SC_NS),"sc_start(1, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")

	ACTION(tweak = !tweak,"~tweak     ")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")

	ACTION(tweak = !tweak,"~tweak     ")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")

	ACTION(sc_start(1, SC_NS),"sc_start(1, SC_NS)")
	ACTION(sc_start(1, SC_NS),"sc_start(1, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(tweak = !tweak,"~tweak     ")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")

    cerr << "Program completed" << endl;
    return 0;
}

