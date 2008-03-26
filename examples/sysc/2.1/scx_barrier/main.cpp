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

  scx_barrier.cpp -- This example demonstrates the proposed scx_barrier class
             that provides execution synchronization between threads. Each
             thread in the "thread pool" of an scx_barrier object instance
             waits on the scx_barrier object instance. When all the threads
             in the thread pool are waiting, they will all be dispatched. 

  Original Author: Andy Goodrich, Forte Design Systems, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "scx_barrier.h"
using sc_core::scx_barrier;

SC_MODULE(X)
{
	SC_CTOR(X)
	{
		SC_THREAD(a);
		SC_THREAD(b);
		SC_THREAD(c);

		m_barrier.initialize(3);
	}
	void a()
	{
		wait(5.0, SC_NS);
		m_barrier.wait();
		printf("%f - a\n", sc_time_stamp().to_double());
	}
	void b()
	{
		wait(11.0, SC_NS);
		m_barrier.wait();
		printf("%f - b\n", sc_time_stamp().to_double());
	}
	void c()
	{
		m_barrier.wait();
		printf("%f - c\n", sc_time_stamp().to_double());
	}
	scx_barrier   m_barrier;
};

int sc_main( int argc, char* argv[] )
{
	sc_clock clock;
	X x("x");

	sc_start(1000, SC_NS);

	cout << "Program completed" << endl;
	return 0;
}

