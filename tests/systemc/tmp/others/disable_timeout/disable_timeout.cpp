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

  disable_timeout.cpp -- Test of wait timeout when process disabled.

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
	m_method = sc_get_current_process_handle();
	SC_THREAD(stimulus);
        SC_THREAD(thread);
	m_thread = sc_get_current_process_handle();
    }
    void method()
    {
	cout << sc_time_stamp() << " method: entered";
	if ( sc_core::timed_out( sc_get_curr_simcontext() ) )
	    cout << " - timed out";
	cout << endl;
        next_trigger( 10, SC_NS, m_event );
    }

    void stimulus()
    {
	wait( 1, SC_NS );

	m_method.disable();
        m_thread.disable();
	cout << sc_time_stamp() << " stimulus: disabling method and thread" 
	     << endl;
	wait( 20, SC_NS );

	cout << endl;
	cout << sc_time_stamp() << " stimulus: enabling method and thread" 
	     << endl;
	m_method.enable();
	m_thread.enable();
	wait( 1, SC_NS );

	cout << sc_time_stamp() << " stimulus: disabling method and thread" 
	     << endl;
	m_method.disable();
        m_thread.disable();
	wait( 1, SC_NS );

	cout << endl;
	cout << sc_time_stamp() << " stimulus: firing event" << endl;
	wait( 20, SC_NS );

	cout << endl;
	cout << sc_time_stamp() << " stimulus: enabling method and thread" 
	     << endl;
	m_method.enable();
	m_thread.enable();
	wait( 1, SC_NS );

	cout << endl;
	cout << sc_time_stamp() << " stimulus: calling sc_stop" << endl;
	sc_stop();
    }

    void thread()
    {
	cout << sc_time_stamp() << " thread: first entry" << endl;
        for (;;)
	{
	    wait( 10, SC_NS, m_event );
	    cout << sc_time_stamp() << " thread: back from wait";
	    if ( sc_core::timed_out( sc_get_curr_simcontext() ) )
		cout << " - timed out";
	    cout << endl;
	}
    }

    sc_event          m_event;  // event method and process wait on.
    sc_process_handle m_method; // process hand for method process.
    sc_process_handle m_thread; // process hand for thread process.
};

int sc_main(int argc, char* argv[])
{
    DUT             dut("dut");

    sc_core::sc_allow_process_control_corners = true;
    sc_start();

    cout << "Program completed" << endl;
    return 0;
}
