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

// $Log: test01.cpp,v $
// Revision 1.2  2006/01/19 00:46:58  acg
// Andy Goodrich: Added CVS logging.
//

// test of sc_sensitive::operator()(sc_cthread_process*, sc_in(inout)<bool>)

#include "systemc.h"


SC_MODULE( mod_a )
{
    sc_in<bool>    clk;
    sc_in<bool>    in1;
 
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
 
    SC_CTOR(mod_a)
    {
	SC_CTHREAD( main_action1, clk );
	SC_CTHREAD( main_action2, in1 );
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
    sc_clock clk( "clk", 10, SC_NS );
    sc_clock clk1( "clk1", 5, SC_NS );
    sc_signal<bool> channel;
    mod_a a( "a" );
    mod_b b( "b" );

    b.clk( clk1 );
    b.in1( channel );   
    a.clk( clk );
    a.in1( channel );

    sc_start( 100, SC_NS );   

    return 0;
}
