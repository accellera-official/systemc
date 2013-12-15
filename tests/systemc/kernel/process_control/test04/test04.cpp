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

  test04.cpp -- Test of interaction of suspend-resume, disable-enable, and
                resets on processes

  Original Author: Andy Goodrich

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  Revision log at end of the file to let __LINE__ give the same results 
  after a check-in.
 *****************************************************************************/
// $Log: test04.cpp,v $
// Revision 1.5  2011/04/02 00:08:29  acg
//  Andy Goodrich: turn off corner case error checking.
//
// Revision 1.4  2011/03/07 19:32:14  acg
//  Andy Goodrich: addition to set sc_core::sc_allow_process_control_corners
//  to true so that this test avoids corner case error messages.
//
// Revision 1.3  2011/02/14 17:00:00  acg
//  Andy Goodrich: updated copyright and added cvs logging information inline.
//

#include "systemc.h"

SC_MODULE(DUT)
{
    SC_CTOR(DUT)
    {
        SC_CTHREAD(master,m_clk.pos());
        SC_CTHREAD(slave,m_clk.pos());
    }
    void slave()
    {
        m_handle0 = sc_get_current_process_handle();
        cout << sc_time_stamp() << ":slave - in reset" << endl;
        for (;;)
        {
            wait();
            cout << sc_time_stamp() << ":slave - self-suspend..." << endl;
            m_handle0.suspend();
            cout << sc_time_stamp() << ":slave - ... resumed" << endl;
            wait();
            cout << sc_time_stamp() << ":slave - self-disable ..." << endl;
            m_handle0.disable();
            cout << sc_time_stamp() << ":slave - ... executing ..." << endl;
	    wait();
            cout << sc_time_stamp() << ":slave - ... enabled" << endl;
	    wait();
	    wait();
	    wait();
        }
    }
    void master()
    {
        m_handle1 = sc_get_current_process_handle();
        for (;;)
        {
            wait();
            wait();
            wait();
            cout << sc_time_stamp() 
	         << ":master -                    resuming slave" << endl;
            m_handle0.resume();
            wait();
            wait();
            wait();
            cout << sc_time_stamp() 
	         << ":master -                    enabling slave" << endl;
            m_handle0.enable();
            wait();
            cout << sc_time_stamp() 
	         << ":master -                    sync reset on slave" << endl;
            m_handle0.sync_reset_on();
            wait();
            wait();
            wait();
            cout << sc_time_stamp() 
	         << ":master -                    sync reset off slave" << endl;
            m_handle0.sync_reset_off();
            wait();
            wait();
            wait();
            cout << sc_time_stamp() 
	         << ":master -                    async reset on slave" << endl;
            m_handle0.reset();
            wait(20);
            sc_stop();
        }
    }
    sc_in<bool>       m_clk;
    sc_process_handle m_handle0;
    sc_process_handle m_handle1;
};

int sc_main(int argc, char* argv[])
{
    sc_core::sc_allow_process_control_corners = true;
    sc_clock        clock;
    DUT             dut("dut");

    dut.m_clk(clock);

    sc_core::sc_allow_process_control_corners = true;
    sc_start();

    cout << "Program completed" << endl;
    return 0;
}
