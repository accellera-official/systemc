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

  test01.cpp --

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-13

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of former asserts (should not be reachable from public APIs) now errors

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in_clk clk;

    SC_CTOR( mod_a )
    {
        clk.pos().find_event();
    }
};

SC_MODULE( mod_b )
{
    sc_in_clk clk;

    SC_CTOR( mod_b )
    {
        clk->read();
    }
};

SC_MODULE( mod_c )
{
    const sc_in_clk clk;

    SC_CTOR( mod_c )
    {
        clk->read();
    }
};

int
sc_main( int, char*[] )
{
    // sc_clock error(s)

    try {
        sc_clock clk1( "clk1", 0, SC_PS );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }

    try {
        sc_clock clk2( "clk2", 1, SC_PS, 0.1 );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }

    try {
        sc_clock clk3( "clk3", 1, SC_PS, 0.9 );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }


    // sc_event_finder error(s)

    try {
        mod_a a( "a" );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }


    // sc_port error(s)

    try {
        mod_b b( "b" );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }

    try {
        mod_c c( "c" );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }


    // sc_semaphore error(s)

    try {
        sc_semaphore sem1( -1 );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }

    try {
        sc_semaphore sem2( "sem2", -1 );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }


    // sc_event error(s)

    try {
        sc_event e1;
        e1.notify( 10, SC_MS );
        e1.notify_delayed();
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }

    try {
        sc_event e2;
        e2.notify( 10, SC_MS );
        e2.notify_delayed( SC_ZERO_TIME );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }


    // sc_name_gen error(s)

    try {
        sc_gen_unique_name( 0 );
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }

    return 0;
}
