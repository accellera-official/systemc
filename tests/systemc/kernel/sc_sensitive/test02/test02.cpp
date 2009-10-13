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

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-22
                   Ucar Aziz, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: test02.cpp,v $
// Revision 1.2  2006/01/19 00:47:01  acg
// Andy Goodrich: Added CVS logging.
//

// test of sc_sensitive_neg::operator(<<)()(sc_inout<bool>)

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in<bool> in1;
    sc_in<bool> in2;
 
    void main_action1()
    { 
	int i = 0;
	while( true ) {
	    wait();
	    cout << "i = " << i << endl;
	    i ++;
	}
    }

    void main_action2()
    { 
	int j = 0;
	while( true ) {
	    wait();
	    cout << "j = " << j << endl;
	    j ++;
	}
    }
 
    SC_CTOR( mod_a )
    {
	SC_THREAD( main_action1 );
	sensitive_neg( in1 );
	SC_THREAD( main_action2 );
	sensitive_neg << in2;
    }
};

SC_MODULE( mod_b )
{
    sc_in<bool>    clk;
    sc_inout<bool> in1;
 
    void main_action()
    { 
	bool j = true;
	while( true ) {
	    wait();
	    in1->write( j );
	    j = !j;
	}
    }
 
    SC_CTOR( mod_b )
    {
	SC_CTHREAD( main_action, clk );
    }
};

int
sc_main( int, char*[] )
{
    sc_clock clk1( "clk", 5, SC_NS );
    sc_clock clk2( "clk1", 5, SC_NS );
    sc_signal<bool> sig_1;
    sc_signal<bool> sig_2;
    mod_a a( "a" );
    mod_b b1( "b1" );
    mod_b b2( "b2" );

    b1.clk( clk1 );
    b1.in1( sig_1 );   
    b2.clk( clk2 );
    b2.in1( sig_2 );

    a.in1( sig_1 );
    a.in2( sig_2 );

    sc_start( 100, SC_NS );   

    return 0;
}
