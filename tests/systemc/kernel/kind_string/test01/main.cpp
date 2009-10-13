/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
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

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test the kind string of objects

#include "systemc.h"

#define WRITE(a) \
{ \
    cout << (a).kind() << endl; \
    cout << (a).kind_string << endl; \
    sc_object* obj = &(a); \
    cout << obj->kind() << endl; \
    cout << obj->kind_string << endl; \
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
        sc_process_b* p;
        SC_METHOD( method_action );
        p = sc_get_curr_process_handle();
        WRITE( *p );
        SC_THREAD( thread_action );
        p = sc_get_curr_process_handle();
        WRITE( *p );
        SC_CTHREAD( cthread_action, clk.pos() );
        p = sc_get_curr_process_handle();
        WRITE( *p );
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
