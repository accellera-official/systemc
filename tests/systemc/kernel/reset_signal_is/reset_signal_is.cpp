/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
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

  reset_signal_is.cpp -- Test for reset_signal_is support.

  Original Author: Andy Goodrich

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
		SC_CTHREAD(test,m_clk.pos());
		reset_signal_is( m_reset, false );
	}
	void test()
	{
		{
			cout << "A: reset" << endl;
			wait();
		}
		for (;;)
		{
			wait();
		}
	}
	sc_in_clk   m_clk;
	sc_in<bool> m_reset;
};

SC_MODULE(B)
{
    B(sc_module_name name, sc_signal<bool>* reset_p ):
		sc_module(name), m_reset_p(reset_p)
	{
		SC_HAS_PROCESS(B);
		SC_CTHREAD(test,m_clk.pos());
		reset_signal_is( *m_reset_p, false );
	}
	void test()
	{
		{
			cout << "B: reset" << endl;
			wait();
		}
		for (;;)
		{
			wait();
		}
	}
	sc_in_clk        m_clk;
	sc_signal<bool>* m_reset_p;
};

int sc_main(int argc, char* argv[])
{
	sc_clock        clk;
	sc_signal<bool> reset;
	A a("a");
    B b("b",&reset);

	a.m_clk(clk);
	a.m_reset(reset);
	b.m_clk(clk);

	cout << "Before start" << endl;
	sc_start(2);
	reset = true;
	cout << "After reset true" << endl;
	sc_start(3);
	cout << "Ending" << endl;

	return 0;
}
