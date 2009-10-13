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

  test07.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in<bool> clk;

    sc_signal<bool> reset;
    sc_inout<bool> reset2;
    sc_clock clk2;

    sc_in<bool> clk3;

    void main_action()
    {
        reset.write( false );
        reset2.write( false );
        cout << "main_action" << endl;
        int a = 0;
        while( true ) {
            wait();
            cout << ( ++ a ) << endl;
            if( a > 20 ) {
                reset.write( true );
                reset2.write( true );
            }
        }
    }

    SC_CTOR( mod_a )
    {
        SC_CTHREAD( main_action, clk.neg() );
        watching( clk3.delayed() == false ); // ok
    }
};

int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    sc_clock clk( "clk", 0.1 );
    sc_clock clk3;
    sc_signal<bool> reset2;
    a.clk( clk );
    a.clk3( clk3 );
    a.reset2( reset2 );

    sc_start( 10 );

    return 0;
}
