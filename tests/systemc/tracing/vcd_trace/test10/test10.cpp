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

  test10.cpp -- 

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

  enum Ttype {
    OK,
    NOTOK,
    SOSO
  };

  unsigned obj1;

  proc1( sc_module_name NAME,
	 sc_signal<bool>& CLK )
  {
    clk(CLK);
    SC_THREAD( entry );
    sensitive << clk;
    obj1 = OK;
  }

  void entry();
};
  
void proc1::entry() 
{
  wait();
  while(true) {
    obj1 = OK;
    wait();
    obj1 = NOTOK;
    wait();
    obj1 = SOSO;
    wait();
  }
}
  

int sc_main(int ac, char *av[])
{
  sc_trace_file *tf;
  sc_signal<bool> clock;

  char *enum_literals[4];
  enum_literals[0] = "OK";
  enum_literals[1] = "NOTOK";
  enum_literals[2] = "SOSO";
  enum_literals[3] = 0;

  proc1 P1("P1", clock);

  tf = sc_create_vcd_trace_file("test10");
  sc_trace(tf, P1.obj1, "Enum", (const char **) enum_literals);
  sc_trace(tf, clock, "Clock");

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
