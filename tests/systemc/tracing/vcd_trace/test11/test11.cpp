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

  test11.cpp -- 

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

  bool obj1;
  sc_logic obj2;
  sc_bv<4> obj3;
  sc_lv<4> obj4;

  proc1( sc_module_name NAME,
	 sc_signal<bool>& CLK )
    : clk(CLK)
  {
    SC_THREAD( entry );
    sensitive(clk);
    obj1 = 0;
    obj2 = 0;
    obj3 = "0000";
    obj4 = "0000";
  }

  void entry();
};
  
void proc1::entry() 
{
  sc_bv<4> bv;
  sc_lv<4> sv;
  int i = 0;
  bv = 0;
  sv = 0;
  wait();
  while(true) {
    bv[i] = bv[i] ^ 1;
    sv[i] ^= SC_LOGIC_1;
    i = (i + 1) % 4;
    obj3 = bv;
    obj4 = sv;
    wait();
  }
}
  

int sc_main(int ac, char *av[])
{
  sc_trace_file *tf;
  sc_signal<bool> clock;

  proc1 P1("P1", clock);

  tf = sc_create_vcd_trace_file("test11");
  ((vcd_trace_file *) tf)->sc_set_vcd_time_unit(-12);
  sc_trace(tf, P1.obj1, "Bool");
  sc_trace(tf, P1.obj2, "SC_Logic");
  sc_trace(tf, P1.obj3, "SC_BV");
  sc_trace(tf, P1.obj4, "SC_LV");
  sc_trace(tf, clock, "Clock");

  clock.write(0);
  sc_start(0);
  for (int i = 0; i< 10; i++) {
    clock.write(1);
    sc_start(10);
    clock.write(0);
    sc_start(10);
  }
  sc_close_vcd_trace_file( tf );
  return 0;
}
