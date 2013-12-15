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

  Original Author: Andy Goodrich Forte Design Systems - 2005-10-10

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: test01.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:26:18  acg
// systemc_tests-2.3
//
// Revision 1.2  2006/01/24 21:05:38  acg
//  Andy Goodrich: replacement of deprecated features with their non-deprecated
//  counterparts.
//


// Test inclusion of systemc rather than systemc.h

#include "systemc"


SC_MODULE(DUT)
{
	SC_CTOR(DUT)
	{
		SC_METHOD(x);
		sensitive << m_clk.pos();
		SC_CTHREAD(z, m_clk.pos());
		SC_THREAD(y);
		sensitive << m_clk.pos();
	}
	void x()
	{
		std::cout << "x: Hello World" << std::endl;
	}
	void y()
	{
		for (;;)
		{
			wait();
		    std::cout << "y: Hello World" << std::endl;
		}
	}
	void z()
	{
		for (;;)
		{
			wait();
		    std::cout << "z: Hello World" << std::endl;
		}
	}
	::sc_core::sc_in<bool> m_clk;
};

int sc_main(int argc, char* argv[])
{
	sc_core::sc_clock clock;
	DUT               dut("dut");

    dut.m_clk(clock);
	sc_core::sc_start(10, sc_core::SC_NS);
	return 0;
}
