/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
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

  test08.cpp -- Test of suspend and asynchronous reset interaction

  Original Author: Andy Goodrich, 14 Februrary 2011

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
// $Log: test08.cpp,v $
// Revision 1.4  2011/04/02 00:08:36  acg
//  Andy Goodrich: turn off corner case error checking.
//
// Revision 1.3  2011/03/07 19:32:16  acg
//  Andy Goodrich: addition to set sc_core::sc_allow_process_control_corners
//  to true so that this test avoids corner case error messages.
//
// Revision 1.2  2011/02/20 13:44:06  acg
//  Andy Goodrich: updates for IEEE 1666 2011.
//
// Revision 1.1  2011/02/14 16:59:29  acg
//  Andy Goodrich: first check in.
//

#include "systemc.h"

sc_event event1;
sc_process_handle t;
SC_MODULE(DUT)
{
    SC_CTOR(DUT)
    {
        SC_THREAD(thread);
	t = sc_get_current_process_handle();
    }
    void thread()
    {
        cout << sc_time_stamp() << " target: initializing" << endl;
        for (;;)
        {
            wait(event1);
	    cout << sc_time_stamp() << " target: awoke" << endl;
        }
    }
};

SC_MODULE(TB)
{
    SC_CTOR(TB)
    {
        SC_THREAD(tb_thread);
    }
    void tb_thread()
    {
	wait( 10, SC_NS );
	cout << sc_time_stamp() << " tb: firing event " << endl;
	event1.notify();
	wait( 10, SC_NS );

	cout << sc_time_stamp() << " tb: suspending target " << endl;
        t.suspend();
	wait( 10, SC_NS );

	cout << sc_time_stamp() << " tb: firing event while target suspended" 
	     << endl;
	event1.notify();
	wait( 10, SC_NS );

	cout << sc_time_stamp() << " tb: resetting target " << endl;
	t.reset();
	wait( 10, SC_NS );

	cout << sc_time_stamp() << " tb: resuming target" << endl;
	t.resume();
	wait( 10, SC_NS );

	cout << sc_time_stamp() << " tb: 10ns after resume" << endl;
	wait( 10, SC_NS );

	cout << sc_time_stamp() << " tb: firing event again" << endl;
	event1.notify();
	wait( 10, SC_NS );

	cout << sc_time_stamp() << " tb: stopping" << endl;
	sc_stop();
    }
};

int sc_main(int argc, char* argv[])
{
    sc_core::sc_allow_process_control_corners = true;
    DUT             dut("dut");
    TB              tb("tb");

    sc_core::sc_allow_process_control_corners = true;
    sc_start();

    cout << "Program completed" << endl;
    return 0;
}
