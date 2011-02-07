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

  method_self_reset.cpp -- Test of method self reset.

  Original Author: Andy Goodrich

 *****************************************************************************/
// $Log: method_self_reset.cpp,v $
// Revision 1.1  2011/01/28 19:48:36  acg
//  Andy Goodrich: first check in.
//

#include "systemc.h"

SC_MODULE(DUT)
{
    SC_CTOR(DUT)
    {
        SC_METHOD(method);
	sensitive << m_clk.pos();
    }
    void method()
    {
        static int first_time = true;
        cout << "Entry " << endl;
	if ( first_time )
	{
	    first_time = !first_time;
	    cout << "Issuing self reset " << endl;
	    sc_get_current_process_handle().reset();
	}
	first_time = !first_time;
        cout << "Exit " << endl;
    }
    sc_in<bool> m_clk;
    sc_in<bool> m_reset;
};

SC_MODULE(TB)
{
    SC_CTOR(TB)
    {
        SC_CTHREAD(tb_thread,m_clk.pos());
        reset_signal_is(m_reset, false);
    }
    void tb_thread()
    {
        for (;;)
        {
            wait();
        }
    }
    sc_in<bool> m_clk;
    sc_in<bool> m_reset;
};

int sc_main(int argc, char* argv[])
{
    sc_clock        clock;
    DUT             dut("dut");
    sc_signal<bool> reset;
    TB              tb("tb");

    dut.m_clk(clock);
    dut.m_reset(reset);

    tb.m_clk(clock);
    tb.m_reset(reset);

    reset = false;
    sc_start(1, SC_NS);
    reset = true;
    sc_start(1, SC_NS);

    cout << "Program completed" << endl;
    return 0;
}
