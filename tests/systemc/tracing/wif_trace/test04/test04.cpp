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

  test04.cpp -- 

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

  char obj1;
  short obj2;
  int obj3;
  long obj4;
  int64 obj5;

  proc1( sc_module_name NAME,
	 sc_signal<bool>& CLK )
  {
    clk(CLK);
    SC_THREAD( entry );
    sensitive << clk;
    obj1 = 0;
    obj2 = 0;
    obj3 = 0;
    obj4 = 0;
    obj5 = 0;
  }

  void entry();
};
  
void proc1::entry() 
{
  wait();
  while(true) {
    obj1 = 7;
    obj2 = 31;
    obj3 = -1023;
    obj4 = 2047;
	obj5 = -1;
	obj5 = ~(obj5 << 40);
    wait();
    obj1 = 1;
    obj2 = -2;
    obj3 = 1024;
    obj4 = -2048;
	obj5 = -(obj5+1);
    wait();
  }
}
  

int sc_main(int ac, char *av[])
{
  sc_trace_file *tf;
  sc_signal<bool> clock;

  proc1 P1("P1", clock);

  tf = sc_create_wif_trace_file("test04");
  sc_trace(tf, P1.obj1, "Char");
  sc_trace(tf, P1.obj2, "Short");
  sc_trace(tf, P1.obj3, "Int");
  sc_trace(tf, P1.obj4, "Long");
  sc_trace(tf, P1.obj5, "Int64");
  sc_trace(tf, clock, "Clock");

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
