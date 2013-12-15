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

  test12.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( proc1 )
{
    SC_HAS_PROCESS( proc1 );

    sc_in<bool> clk;
    sc_out<sc_int<10> > bv;
    sc_out<sc_uint<10> > sv;

    sc_int<10> obj1;
    sc_uint<10> obj2;

    proc1( sc_module_name NAME,
	   sc_signal<bool>& CLK,
	   sc_signal<sc_int<10> >& BV,
	   sc_signal<sc_uint<10> >& SV )
	: bv(BV), sv(SV)
    {
        clk(CLK);
		SC_THREAD( entry );
	sensitive << clk;
	obj1 = 0;
	obj2 = 0;
    }

    void entry();
};
  
void proc1::entry() 
{
    bv = obj1;
    sv = obj2;
    wait();
    while(true) {
	obj1 = 3;
	obj2 = 7;
	bv = obj1;
	sv = obj2;
	wait();
	obj1 = -3;
	obj2 = 5;
	bv = obj1;
	sv = obj2;
	wait();
    }
}
  

int sc_main(int ac, char *av[])
{
  sc_trace_file *tf;
  sc_signal<bool> clock;
  sc_signal<sc_int<10> > bv;
  sc_signal<sc_uint<10> > sv;

  proc1 P1("P1", clock, bv, sv);

  tf = sc_create_wif_trace_file("test12");
  sc_trace(tf, P1.obj1, "Signed");
  sc_trace(tf, P1.obj2, "Unsigned");
  sc_trace(tf, bv, "BV");
  sc_trace(tf, sv, "SV");

  clock.write(0);
  sc_start(0, SC_NS);
  for (int i = 0; i< 10; i++) {
    clock.write(1);
    sc_start(10, SC_NS);
    clock.write(0);
    sc_start(10, SC_NS);
  }
  sc_close_wif_trace_file( tf );
  return 0;
}
