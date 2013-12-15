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

  test.cpp -- 

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
    sc_in_clk clk;

    void main_action()
    {
        while( true ) {
            cout << sc_time_stamp() << endl;
            // wait( 1 ); // fine
            wait( 0 );
        }
    }

    SC_CTOR( mod_a )
    {
        SC_CTHREAD( main_action, clk.pos() );
    }
};

int
sc_main( int, char*[] )
{
    sc_clock clk;

    mod_a a( "a" );

    a.clk( clk );

    sc_start( 20, SC_NS );

    return 0;
}
