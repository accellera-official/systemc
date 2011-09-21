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

  main.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               September 5, 2003
  Description of Modification: change sc_get_curr_proc_handle() to
                               sc_get_last_created_process_handle()

 *****************************************************************************/

// test the kind string of objects

#include "systemc.h"

#define WRITE(a) \
{ \
    cout << (a).kind() << endl; \
    const sc_object* obj = &(a); \
    cout << obj->kind() << endl; \
}

SC_MODULE( mod_a )
{
    sc_in_clk clk;

    void method_action()
    {}

    void thread_action()
    {}

    void cthread_action()
    {}

    SC_CTOR( mod_a )
    {
        SC_METHOD( method_action );
        WRITE( *sc_get_current_process_handle().get_process_object() );
        SC_THREAD( thread_action );
        WRITE( *sc_get_current_process_handle().get_process_object() );
        SC_CTHREAD( cthread_action, clk.pos() );
        WRITE( *sc_get_current_process_handle().get_process_object() );
    }
};

extern void foo( const sc_signal<int>& );

int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    WRITE( a );

    sc_clock clk;
    WRITE( clk );

    sc_fifo<int> fifo;
    WRITE( fifo );

    sc_mutex mutex;
    WRITE( mutex );

    sc_signal<int> signal;
    WRITE( signal );

    sc_signal<bool> signal_bool;
    WRITE( signal_bool );

    sc_signal<sc_logic> signal_logic;
    WRITE( signal_logic );

    sc_signal_resolved signal_resolved;
    WRITE( signal_resolved );

    sc_signal_rv<8> signal_rv;
    WRITE( signal_rv );

    foo( signal );

    return 0;
}
