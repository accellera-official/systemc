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
        
  test05.cpp -- Test reset_signal_is() and async_reset_signal_is() usage.
        
  Original Author: Andy Goodrich, Forte Design Systems, 14 December 2006
        
 *****************************************************************************/
    
/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:    
    
 *****************************************************************************/

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"

SC_MODULE(DUT)
{
    SC_CTOR(DUT)
    {
        SC_CTHREAD(creator,m_clk.pos());
        SC_CTHREAD(resetter,m_clk.neg());

        // SET UP STATICALLY DEFINED PROCESSES:

        SC_CTHREAD(static_cthread,m_clk.pos());
        async_reset_signal_is(m_areset,true);
        reset_signal_is(m_sreset,true);
        SC_METHOD(static_method);
        reset_signal_is(m_areset,true);
        SC_THREAD(static_thread_clocked);
        sensitive << m_clk.pos();
        dont_initialize();
        async_reset_signal_is(m_areset,true);
        reset_signal_is(m_sreset,true);
        SC_THREAD(static_thread_event);
        async_reset_signal_is(m_areset,true);
        reset_signal_is(m_sreset,true);
        SC_THREAD(static_thread_timed);
        async_reset_signal_is(m_areset,true);
        reset_signal_is(m_sreset,true);
    }

    // creator - create the dynamic processes after the start of simulation:
    void creator()
    {
        sc_spawn_options options_method;
        sc_spawn_options options_thread_clocked;
        sc_spawn_options options_thread_event;
        sc_spawn_options options_thread_timed;

        wait(1);

        options_method.reset_signal_is( m_areset, true );
        options_method.spawn_method();
        sc_spawn( sc_bind(&DUT::dynamic_method, this), "dynamic_method", 
            &options_method);

        options_thread_clocked.async_reset_signal_is( m_areset, true );
        options_thread_clocked.reset_signal_is( m_sreset, true );
        options_thread_clocked.set_sensitivity( &m_clk.posedge_event() );
        sc_spawn( sc_bind(&DUT::dynamic_thread_clocked, this), 
            "dynamic_thread_clocked", &options_thread_clocked);

        options_thread_event.async_reset_signal_is( m_areset, true );
        options_thread_event.reset_signal_is( m_sreset, true );
        sc_spawn( sc_bind(&DUT::dynamic_thread_event, this), 
            "dynamic_thread_event", &options_thread_event);

        options_thread_timed.async_reset_signal_is( m_areset, true );
        options_thread_timed.reset_signal_is( m_sreset, true );
        sc_spawn( sc_bind(&DUT::dynamic_thread_timed, this), 
            "dynamic_thread_timed", &options_thread_timed);

    }

    void dynamic_method()
    {
        cout << sc_time_stamp() << " ... dynamic method" << endl;
        next_trigger(m_non_event);
    }

    void dynamic_thread_clocked()
    {
        cout << sc_time_stamp() << " ... dynamic thread clocked" << endl;
        for (;;)
        {
            wait();
        }
    }

    void dynamic_thread_event()
    {
        cout << sc_time_stamp() << " ... dynamic thread event wait" << endl;
        for (;;)
        {
            wait(m_non_event);
        }
    }

    void dynamic_thread_timed()
    {
        cout << sc_time_stamp() << " ... dynamic thread timed wait" << endl;
        for (;;)
        {
            wait(1000, SC_NS);
        }
    }


    void resetter()
    {
        for ( int wait_i = 1; wait_i < 3; wait_i++ )
        {
            wait(2);
            cout << endl << sc_time_stamp() << " asserting asynchronous reset" 
                 << endl;
            m_areset = true;
            wait(wait_i);
            cout << endl << sc_time_stamp() << " clearing asynchronous reset" 
                 << endl;
            m_areset = false;
            wait(2);
            cout << endl << sc_time_stamp() << " asserting synchronous reset" 
                 << endl;
            m_sreset = true;
            wait(wait_i);
            cout << endl << sc_time_stamp() << " clearing synchronous reset" 
                 << endl;
            m_sreset = false;
            wait(5);
        }
        cout << endl << sc_time_stamp() << " terminating simulation" << endl;
        sc_stop();
    }

    void static_cthread()
    {
        cout << sc_time_stamp() << " ... static cthread" << endl;
        for (;;)
        {
            wait();
        }
    }

    void static_method()
    {
        cout << sc_time_stamp() << " ... static method" << endl;
        next_trigger(m_non_event);
    }

    void static_thread_clocked()
    {
        cout << sc_time_stamp() << " ... static thread clocked" << endl;
        for (;;)
        {
            wait();
        }
    }

    void static_thread_event()
    {
        cout << sc_time_stamp() << " ... static thread event wait " << endl;
        for (;;)
        {
            wait(m_non_event);
        }
    }

    void static_thread_timed()
    {
        cout << sc_time_stamp() << " ... static thread timed wait " << endl;
        for (;;)
        {
            wait(1000, SC_NS);
        }
    }

    sc_signal<bool> m_areset;
    sc_in<bool>     m_clk;
    sc_event        m_non_event;
    sc_signal<bool> m_sreset;
};

int sc_main(int argc, char* argv[])
{
    sc_clock        clock;
    DUT             dut("dut");

    dut.m_clk(clock);

    sc_start();

    cout << "Program completed" << endl;
    return 0;
}
