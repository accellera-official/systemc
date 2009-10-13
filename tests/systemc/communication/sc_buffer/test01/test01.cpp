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

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_buffer<T> -- general test

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in<bool> clk;
    sc_out<int> out;

    void main_action()
    {
        while( true ) {
            wait();
            out = 3;
        }
    }

    SC_CTOR( mod_a )
    {
        SC_THREAD( main_action );
        sensitive_pos << clk;
    }
};

SC_MODULE( mod_b )
{
    sc_in<int> in;

    void main_action()
    {
        cout << sc_simulation_time() << " " << in.read() << endl;
    }

    SC_CTOR( mod_b )
    {
        SC_METHOD( main_action );
        sensitive << in;
    }
};

int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    mod_b b( "b" );

    sc_clock clk;
    sc_buffer<int> buf;

    a.clk( clk );
    a.out( buf );
    b.in( buf );

    sc_start( 100 );

    return 0;
}
