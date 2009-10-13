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

  test06.cpp -- 

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

  float obj1;
  double obj2;

  proc1( sc_module_name NAME,
	 sc_signal<bool>& CLK )
    : clk(CLK)
  {
    SC_THREAD( entry );
    sensitive(clk);
    obj1 = 0.0;
    obj2 = 0.0;
  }

  void entry();
};
  
void proc1::entry() 
{
  wait();
  while(true) {
    obj1 = 12.345;
    obj2 = -13.5678923;
    wait();
    obj1 = -182634876.5659374;
    obj2 = 1672357.298346;
    wait();
  }
}
  

int sc_main(int ac, char *av[])
{
  sc_trace_file *tf;
  sc_signal<bool> clock;

  proc1 P1("P1", clock);

  tf = sc_create_wif_trace_file("test06");
  sc_trace(tf, P1.obj1, "Float");
  sc_trace(tf, P1.obj2, "Double");

  clock.write(0);
  sc_start(0);
  for (int i = 0; i< 10; i++) {
    clock.write(1);
    sc_start(10);
    clock.write(0);
    sc_start(10);
  }
  sc_close_wif_trace_file( tf );
  return 0;
}
