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

  test02.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of immediate event notification

#include "systemc.h"

SC_MODULE( source )
{
    sc_in_clk clk;
    sc_out<int> out;

    void main_action()
    {
        int a = 0;
        while( true ) {
            wait();
            out = a ++;
        }
    }

    SC_CTOR( source )
    {
        SC_THREAD( main_action );
        sensitive_pos << clk;
    }
};

SC_MODULE( sink )
{
    sc_in_clk clk;
    sc_in<int> in;

    sc_event e;

    void main_action()
    {
        int a;
        while( true ) {
            wait();
            cout << simcontext()->delta_count() << " -- " << in.read() << endl;
            a = in.read();
            if( ( a % 3 ) == 0 ) {
                e.notify();
            }
        }
    }

    void other_action()
    {
        while( true ) {
            wait( e );
            cout << simcontext()->delta_count() << " AA " << in.read() << endl;
            wait( e | e );  // same as wait( e )
            cout << simcontext()->delta_count() << " BB " << in.read() << endl;
            wait( e & e );  // same as wait( e )
            cout << simcontext()->delta_count() << " CC " << in.read() << endl;
            wait( e | e | e );  // same as wait( e )
            cout << simcontext()->delta_count() << " DD " << in.read() << endl;
            wait( e & e & e );  // same as wait( e )
            cout << simcontext()->delta_count() << " EE " << in.read() << endl;
            wait( e & clk->negedge_event() );
            cout << simcontext()->delta_count() << " FF " << in.read() << endl;
            wait( e | clk->negedge_event() );
            cout << simcontext()->delta_count() << " GG " << in.read() << endl;
        }
    }

    SC_CTOR( sink )
    {
        SC_THREAD( main_action );
        sensitive_pos << clk;
        SC_THREAD( other_action );
    }
};

int sc_main( int, char** )
{
    sc_clock clk;

    sc_signal<int> sig;
    source src( "src" );
    sink snk( "snk" );

    src.clk( clk );
    src.out( sig );
    snk.clk( clk );
    snk.in( sig );

    sc_start( 100 );

    return 0;
}
