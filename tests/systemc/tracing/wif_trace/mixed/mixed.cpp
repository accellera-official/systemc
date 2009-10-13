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

  mixed.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( adder )
{
    SC_HAS_PROCESS( adder );

    sc_in<bool> clk;
    sc_in<int>  a;
    sc_in<int>  b;
    sc_out<int> sum;

    adder( sc_module_name NAME,
           sc_clock& CLK,
           sc_signal<int>& A,
           sc_signal<int>& B,
           sc_signal<int>& SUM )
        : clk(CLK), a(A), b(B), sum(SUM)
    {
        SC_METHOD( entry );
        sensitive(clk);
        sensitive(a);
        sensitive(b);
    }
    void entry();
};

void
adder::entry()
{
    if (clk.posedge()) {
        sum = a + b;
    }
}

SC_MODULE( stim )
{
    SC_HAS_PROCESS( stim );

    sc_in_clk   clk;
    sc_out<int> a;
    sc_out<int> b;

    stim( sc_module_name NAME,
          sc_clock& CLK,
          sc_signal<int>& A,
          sc_signal<int>& B )
        : clk(CLK), a(A), b(B)
    {
        SC_CTHREAD( entry, clk.pos() );
    }
    void entry();
};

void
stim::entry()
{
    while (true) {
        a = rand() % 32768;
        b = rand() % 32768;
        wait();
    }
}

int
sc_main( int argc, char* argv[] )
{
    sc_signal<int> a("a");
    sc_signal<int> b("b");
    sc_signal<int> sum("sum");
    sc_clock clk("clk", 20);

    a = 0;
    b = 0;
    sum = 0;

    adder add("add", clk, a, b, sum);
    stim  sti("sti", clk, a, b);

    sc_trace_file* tf = sc_create_wif_trace_file("mixed");
    sc_trace(tf, a, "a");
    sc_trace(tf, b, "b");
    sc_trace(tf, sum, "sum");
    sc_trace(tf, clk, "clk");
    sc_start(1000);
    sc_close_wif_trace_file( tf );
    return 0;
}
