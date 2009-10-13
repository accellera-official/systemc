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

  test02.cpp -- Test reset_signal_is() usage with SC_CTHREAD processes.

  Original Author: Andy Goodrich, Forte Design Systems, 12 August 2005
    
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
		SC_CTHREAD(cthread,m_clk.pos());
		reset_signal_is(m_reset,true);
		SC_CTHREAD(resetter,m_clk.pos());
	}
	void cthread()
	{
		cout << sc_time_stamp() << ": initializing" << endl;
		for (;;)
		{
		    wait(3);
			cout << sc_time_stamp() << ": waited 3" << endl;
		}
	}
	void resetter()
	{
		m_reset = false;
		wait(3);
		m_reset = true;
		wait(2);
		m_reset = false;
		wait(6);
		m_reset = true;
		wait(5);
		sc_stop();
	}
	sc_in<bool>    m_clk;
	sc_inout<bool> m_reset;
};

int sc_main( int argc, char* argv[] )
{
	sc_clock        clock;
	DUT             dut("dut");
	sc_signal<bool> reset;

	dut.m_clk(clock);
	dut.m_reset(reset);

	sc_start();
	return 0;
}

