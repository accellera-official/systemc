/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC  
  source code Copyright (c) 1996-2014 by all Contributors.  All Rights reserved.
  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors  
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
  KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
 *****************************************************************************/
/*****************************************************************************

  test02.cpp -- 
  Original Author: Andy Goodrich, Forte Design Systems

 *****************************************************************************/
/*****************************************************************************
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and  
  changes you are making here.

      Name, Affiliation, Date: 
  Description of Modification: 

 *****************************************************************************/

// TEST THAT THE CORRECT PARENT POINTER IS SET FOR THE before_end_of_elaboration
// and end_of_elaboration CALLBACKS

#include "systemc.h"

class my_object : public sc_object 
{
  public:
	my_object() {}
	virtual ~my_object() {}
};

SC_MODULE(DUT)
{
	SC_CTOR(DUT)
	{
		SC_CTHREAD(thread,m_clk.pos());
		reset_signal_is(m_reset, true);
	}
	void before_end_of_elaboration()
	{
		m_before_p = new my_object;		
	}
	void end_of_elaboration()
	{
		m_end_p = new my_object;		
	}
	void thread()
	{
		for (;;)
		{
			wait();
			if ( m_before_p->get_parent_object() == 0 )
				cout << "before_end_of_elaboration parent is 0!" <<endl;
			if ( m_end_p->get_parent_object() == 0 )
				cout << "end_of_elaboration parent is 0!" <<endl;
		}
	}
	my_object*  m_before_p;
	sc_in<bool> m_clk;
	my_object*  m_end_p;
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
	sc_start(1, SC_NS);

	cout << "Program completed" << endl;
	return 0;
}
