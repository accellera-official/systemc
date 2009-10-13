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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_fifo<T> - general test

#include "systemc.h"

SC_MODULE( writer )
{
    // port(s)
    sc_fifo_out<int> out;

    // process(es)
    void main_action()
    {
        int val = 0;
        while( true ) {
            wait( 10, SC_NS ); // wait for 10 ns
            for( int i = 0; i < 20; i ++ )
                out->write( val ++ ); // blocking write
        }
    }

    SC_CTOR( writer )
    {
        SC_THREAD( main_action );
    }
};

SC_MODULE( reader )
{
    // port(s)
    sc_fifo_in<int> in;

    // process(es)
    void main_action()
    {
        int val;
        while( true ) {
            wait( 10, SC_NS ); // wait for 10 ns
            for( int i = 0; i < 15; i ++ ) {
                in->read( val ); // blocking read
                cout << val << endl;
            }
            cout << "Available: " << in->num_available() << endl;
        }
    }

    SC_CTOR( reader )
    {
        SC_THREAD( main_action );
    }
};

int sc_main( int, char*[] )
{
    sc_clock c;

    // declare channel(s)
    sc_fifo<int> fifo( 10 );

    // instantiate block(s) and connect to channel(s)
    writer w( "writer" );
    reader r( "reader" );

    w.out( fifo );
    r.in( fifo );

    // run the simulation
    sc_start( 100, SC_NS );

    return 0;
}
