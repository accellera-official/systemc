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

  manual_clock.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "isaac.h"

QTIsaac<8> rng;		// Platform independent random number generator.

SC_MODULE( proc1 )
{
    SC_HAS_PROCESS( proc1 );

    sc_in_clk clk;

    sc_in<bool>    a;
    sc_in<bool>    b;
    sc_inout<bool> c;

    proc1( sc_module_name NAME,
           sc_signal_in_if<bool>& CLK,
           sc_signal<bool>& A,
           sc_signal<bool>& B,
           sc_signal<bool>& C )
    {
        clk( CLK );
        a(A); b(B); c(C);
		SC_CTHREAD( entry, clk.pos() );
    }

    void entry()
    {
        while (true) {
            wait();
            c = a.read() && b.read();
            wait();
	    cout << sc_time_stamp() << " P1(a&&b):: C = " << c.read() 
		<< endl;
            c = a.read() || b.read();
            wait();
	    cout << sc_time_stamp() << " P1(a||b):: C = " << c.read() 
		<< endl;
            c = a ^ b;
        }
    }
};

SC_MODULE( proc2 )
{
    SC_HAS_PROCESS( proc2 );

    sc_in_clk clk;

    sc_in<bool>    a;
    sc_in<bool>    b;
    sc_inout<bool> c;

    proc2( sc_module_name NAME,
           sc_signal_in_if<bool>& CLK,
           sc_signal<bool>& A,
           sc_signal<bool>& B,
           sc_signal<bool>& C )
    {
        clk( CLK );
        a(A); b(B); c(C);
		SC_CTHREAD( entry, clk.pos() );
    }

    void entry()
    {
        while (true) {
            wait();
            c = ! (a.read() && b.read());
            wait();
	    cout << sc_time_stamp() << " P2(a&&b):: C = " << c.read() 
		<< endl;
            c = ! (a.read() || b.read());
            wait();
	    cout << sc_time_stamp() << " P2(a||b):: C = " << c.read() 
		<< endl;
            c = ! (a ^ b);
        }
    }
};           

// comparator
SC_MODULE( proc3 )
{
    SC_HAS_PROCESS( proc3 );

    sc_in<bool>  a;
    sc_in<bool>  b;
    sc_out<bool> c;
    sc_out<bool> d;

    proc3( sc_module_name NAME,
           sc_signal<bool>& A,
           sc_signal<bool>& B,
           sc_signal<bool>& C,
           sc_signal<bool>& D )
    {
        a(A); b(B); c(C); d(D);
        SC_METHOD( entry );
        sensitive << a << b;
    }
           
    void entry()
    {
        c = (a == b);
        d = (a != b);
    }
};

int
sc_main( int argc, char* argv[] )
{
    sc_signal<bool> clk1("clk1");
    sc_signal<bool> clk2("clk2");

    sc_signal<bool> a("a"), b("b");
    sc_signal<bool> p("p"), q("q");
    sc_signal<bool> zero("zero"), one("one");

    proc1 p1( "p1", clk1, a, b, p );
    proc2 p2( "p2", clk2, a, b, q );
    proc3 p3( "p3", p, q, zero, one );

    sc_start(0, SC_NS);
    for (double t = 0; t < 0.00001; t += 1e-9) {
        clk1 = 1;
        clk2 = 1;
        a = rng.rand() & 16;
        b = rng.rand() & 32;
        sc_start( 1, SC_NS );
        clk1 = 0;
        clk2 = 0;
        sc_start( 1, SC_NS );
    }

    return 0;
}
