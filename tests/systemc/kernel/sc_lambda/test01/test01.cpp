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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-22
                   Ucar Aziz, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems, 2005-11-10
  Description of Modification: Removal of Lambda exressions

 *****************************************************************************/
// $Log: test01.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:26:04  acg
// systemc_tests-2.3
//
// Revision 1.2  2006/01/24 21:04:54  acg
//  Andy Goodrich: replacement of deprecated features with their non-deprecated
//  counterparts.
//

// test of sc_lambda-style wait_until w/normal loop.

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in_clk   clk1;
    sc_in<bool> clk2;

    void main_action()
    {
	int i = 0;

	while( true ) {
	    do { wait(); } while ( !(clk2 == true) );
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
    sc_clock clk1( "clk1", 0.1, SC_NS );
    sc_clock clk2( "clk2", 0.5, SC_NS );
    a.clk1( clk1 );
    a.clk2( clk2 );

    sc_start( 3, SC_NS );

    return 0;
}
