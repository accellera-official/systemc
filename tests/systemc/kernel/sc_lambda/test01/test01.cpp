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

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-22
                   Ucar Aziz, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_lambda

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in_clk   clk1;
    sc_in<bool> clk2;

    void main_action()
    {
	int i = 0;

	while( true ) {
	    wait_until( clk2.delayed() == true );
	    cout << "i = " << i << endl;
	    i ++;
	    wait();
	}
    }

    SC_CTOR( mod_a )
    {
	SC_CTHREAD( main_action, clk1.pos() );
    }
};

int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    sc_clock clk1( "clk1", 0.1 );
    sc_clock clk2( "clk2", 0.5 );
    a.clk1( clk1 );
    a.clk2( clk2 );

    sc_start( 3 );

    return 0;
}
