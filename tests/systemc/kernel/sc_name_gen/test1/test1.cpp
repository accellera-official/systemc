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

  test1.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems 16 July 2004

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// TEST THAT sc_gen_unique_user_name() GENERATES THE CORRECT NAMING.

#include "systemc.h"

SC_MODULE(A) 
{
	SC_CTOR(A)
	{
		SC_CTHREAD(abc, m_clk.pos());
		SC_CTHREAD(abc, m_clk.pos());
		SC_METHOD(def);
		SC_METHOD(def);
		SC_METHOD(def);
		SC_THREAD(ghi);
		SC_THREAD(ghi);
	}
	void abc()
	{
		sc_curr_proc_handle cpi = sc_get_curr_simcontext()->get_curr_proc_info();
		cout << cpi->process_handle->name() << endl;
	}
	void def()
	{
		sc_curr_proc_handle cpi = sc_get_curr_simcontext()->get_curr_proc_info();
		cout << cpi->process_handle->name() << endl;
	}
	void ghi()
	{
		sc_curr_proc_handle cpi = sc_get_curr_simcontext()->get_curr_proc_info();
		cout << cpi->process_handle->name() << endl;
	}
	sc_in_clk m_clk;
};

int sc_main(int argc, char* argv[])
{
	A        a("a");
	sc_clock clock;
	a.m_clk(clock);

	sc_start(10, SC_NS);
    return 0;
}
