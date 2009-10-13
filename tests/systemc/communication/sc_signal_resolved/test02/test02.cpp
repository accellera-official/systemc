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

// test of sc_signal_resolved port classes.

#include "systemc.h"

SC_MODULE( mod_a )
{
    // ports
    sc_out_resolved out1;
    sc_out_resolved out2;
    sc_in_resolved  in;

    // variables
    sc_logic l1;
    sc_logic l2;

    // events
    sc_event ready1;
    sc_event ready2;

    void out_action1()
    {
        for( int i = 0; i < 4; ++ i ) {
            l1 = sc_dt::sc_logic_value_t( i );
            for( int j = 0; j < 4; ++j ) {
                out1.write( l1 );
                wait( 1, SC_NS );
                ready1.notify();
                wait( SC_ZERO_TIME );
            }
        }
    }

    void out_action2()
    {
        for( int i = 0; i < 4; ++ i ) {
            for( int j = 0; j < 4; ++ j ) {
                l2 = sc_dt::sc_logic_value_t( j );
                out2.write( l2 );
                wait( 1, SC_NS );
                ready2.notify();
                wait( SC_ZERO_TIME );
            }
        }
    }

    void in_action()
    {
        for( int i = 0; i < 16; ++ i ) {
            wait( ready1 & ready2 );
            cout << l1 << " " << l2 << " -> " << in.read() << endl;
        }
    }

    SC_CTOR( mod_a )
    {
        SC_THREAD( out_action1 );
        SC_THREAD( out_action2 );
        SC_THREAD( in_action );
    }
};

int
sc_main( int, char*[] )
{
    // sc_signal_resolved sig;
    sc_signal<sc_logic> sig;

    mod_a a( "a" );

    a.out1( sig );
    a.out2( sig );
    a.in( sig );

    sc_start();

    return 0;
}
