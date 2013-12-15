/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test06.cpp -- test multiple interfaces

  Original Author: Andy Goodrich, Forte Design Systems, 03 April 2007

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of multiple interfaces for an sc_fifo 

#include "systemc.h"

#define W_INFO(msg,iface) \
    cout << sc_time_stamp() << "," << sc_delta_count() \
         << ": writer" << iface << ": " << msg << endl;

#define R_INFO(msg,iface) \
    cout << sc_time_stamp() << "," << sc_delta_count() \
         << ": reader" << iface << ": " << msg << endl;

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
            for ( int iface=0; iface < 3; iface++ )
            {
                W_INFO( "blocking write", iface );
                for( int i = 0; i < 20; i ++ ) {
                    out[iface]->write( val ++ ); // blocking write
                }
            }
        }
    }

    SC_CTOR( writer )
    {
        SC_THREAD( main_action );
        sensitive << out.data_read();
    }
};

SC_MODULE( reader )
{
    // port(s)
    sc_fifo_in<int> in;

    // process(es)
    void main_action()
    {
        int iface;
        int val;
        while( true ) {
            wait( 10, SC_NS ); // wait for 10 ns
            for ( iface=0; iface < 3; iface++ )
            {
                R_INFO( "blocking read 1", iface );
                for( int i = 0; i < 15; i ++ ) {
                    in[iface]->read( val ); // blocking read
                    R_INFO( val, iface );
                }
            }
            wait( 10, SC_NS );
            R_INFO( in.num_available() << " available samples", iface );
            R_INFO( "blocking read 2", iface );
            for ( iface=0; iface < 3; iface++ )
            {
                for( int i = 0; i < 15; i ++ ) {
                    val = in[iface]->read(); // blocking read
                    R_INFO( val, iface );
                }
            }
        }
    }

    SC_CTOR( reader )
    {
        SC_THREAD( main_action );
        sensitive << in.data_written();
    }
};

int sc_main( int, char*[] )
{
    // sc_clock c;

    // declare channel(s)
    sc_fifo<int> fifo( 10 );
    sc_fifo<int> fifo1( 10 );
    sc_fifo<int> fifo2( 10 );

    // instantiate block(s) and connect to channel(s)
    writer w( "writer" );
    reader r( "reader" );

    w.out( fifo );
    w.out( fifo1 );
    w.out( fifo2 );
    r.in( fifo );
    r.in( fifo1 );
    r.in( fifo2 );

    // run the simulation
    sc_start( 100, SC_NS );

    return 0;
}
