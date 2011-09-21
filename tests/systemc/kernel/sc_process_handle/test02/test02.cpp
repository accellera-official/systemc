/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC  
  source code Copyright (c) 1996-2005 by all Contributors.  All Rights reserved.
  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors  
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
  KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
 *****************************************************************************/
/*****************************************************************************

  test02.cpp -- Test proper process type for terminated processes

  Original Author: Andy Goodrich, Forte Design Systems

 *****************************************************************************/
/*****************************************************************************
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and  
  changes you are making here.

      Name, Affiliation, Date: 
  Description of Modification: 

 *****************************************************************************/

// TEST THAT THE CORRECT PROCESS TYPE IS MAINTAINED FOR TERMINATED PROCESSES

#include "systemc.h"

SC_MODULE(DUT)
{
    SC_CTOR(DUT)
    {
        SC_CTHREAD(cthread_target,m_clk.pos());
        SC_THREAD(thread_target)
        sensitive << m_clk.pos();
        SC_CTHREAD(watcher,m_clk.pos());
    }
    void cthread_target()
    {
        m_cthread_handle = sc_get_current_process_handle();
        wait();
    }
    void thread_target()
    {
        m_thread_handle = sc_get_current_process_handle();
        wait();
    }
    void watcher()
    {
        wait();
        wait();
        if ( m_cthread_handle.valid() )
        {
            if ( m_cthread_handle.proc_kind() == SC_NO_PROC_ )
                cout << "Cthread process handle kind not maintained" << endl;
            if ( m_cthread_handle.terminated() == false )
                cout<< "Cthread process handle doesn't show terminated" << endl;
        }
        if ( m_thread_handle.valid() )
        {
            if ( m_thread_handle.proc_kind() == SC_NO_PROC_ )
                cout << "Thread process handle kind not maintained" << endl;
            if ( m_thread_handle.terminated() == false )
                cout<< "Thread process handle does not show terminated" << endl;
        }
    }
    sc_in<bool>      m_clk;
    sc_process_handle m_cthread_handle;
    sc_process_handle m_thread_handle;
};
int sc_main(int argc, char* argv[])
{
    sc_clock        clock;
    DUT             dut("dut");

    dut.m_clk(clock);

    sc_start(5, SC_NS);

    cout << "Program completed" << endl;
    return 0;
}
