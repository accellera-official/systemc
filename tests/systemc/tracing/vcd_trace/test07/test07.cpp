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

SC_MODULE( proc1 )
{
  SC_HAS_PROCESS( proc1 );

  sc_in<bool> clk;
  sc_signal<sc_bv<4> >& bv;
  sc_signal<sc_lv<4> >& sv;

  sc_signed obj1;
  sc_unsigned obj2;

  proc1( sc_module_name NAME,
	 sc_signal<bool>& CLK,
	 sc_signal<sc_bv<4> >& BV,
	 sc_signal<sc_lv<4> >& SV )
    : bv(BV), sv(SV), obj1(4), obj2(4)
  {
    clk(CLK);
	SC_THREAD( entry );
    sensitive << clk;
    obj1 = 0;
    obj2 = 0;
    bv = "0000";
    sv = "0000";
  }

  void entry();
};
  
void proc1::entry() 
{
  wait();
  while(true) {
    obj1 = 3;
    obj2 = 7;
    bv = "0011";
    sv = "1100";
    wait();
    obj1 = -3;
    obj2 = 5;
    bv = "1111";
    sv = "1110";
    wait();
  }
}
  

int sc_main(int ac, char *av[])
{
  sc_trace_file *tf;
  sc_signal<bool> clock;
  sc_signal<sc_bv<4> > bv;
  sc_signal<sc_lv<4> > sv;

  proc1 P1("P1", clock, bv, sv);

  tf = sc_create_vcd_trace_file("test07");
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
  sc_close_vcd_trace_file( tf );
  return 0;
}
