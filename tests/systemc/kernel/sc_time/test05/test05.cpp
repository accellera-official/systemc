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

  test05.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of function sc_set_time_resolution

#include "systemc.h"

SC_MODULE( source )
{
    sc_in_clk   clk;
    sc_out<int> out;

    void main_action()
    {
        sc_set_time_resolution( 10, SC_PS );
        int a = 0;
        while( true ) {
            wait();
            out = ++ a;
        }
    }

    SC_CTOR( source )
    {
        SC_CTHREAD( main_action, clk.pos() );
    }
};

int
sc_main( int, char*[] )
{
    sc_clock clk( "clk" );
    sc_signal<int> sig( "sig" );

    source src( "src" );
    src.clk( clk );
    src.out( sig );

    sc_start( -1 );

    return 0;
}
