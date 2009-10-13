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

  test09.cpp -- 

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
  sc_signal<int>& Isig;
  sc_signal<char>& Csig;
  sc_signal<float>& Fsig;
  sc_signal<sc_logic>& Lsig;

  proc1( sc_module_name NAME,
	 sc_signal<bool>& CLK,
	 sc_signal<int>& ISIG,
	 sc_signal<char>& CSIG,
	 sc_signal<float>& FSIG,
	 sc_signal<sc_logic>& LSIG )
    : Isig(ISIG), Csig(CSIG), Fsig(FSIG), Lsig(LSIG)
  {
    clk(CLK);
	SC_THREAD( entry );
    sensitive << clk;
    Isig = 0;
    Csig = 0;
    Fsig = 0.0;
    Lsig = SC_LOGIC_0;//'0';
  }

  void entry();
};
  
void proc1::entry() 
{
  wait();
  while(true) {
    Isig = 1023;
    Csig = 15;
    Fsig = -4;
    Lsig = SC_LOGIC_X;//'x';
    wait();
    Isig = 10;
    Csig = 8;
    Fsig = 1000.23456;
    Lsig = SC_LOGIC_Z;//'z';
    wait();
  }
}
  

int sc_main(int ac, char *av[])
{
  sc_trace_file *tf;
  sc_signal<bool> clock;
  sc_signal<int> I;
  sc_signal<char> C;
  sc_signal<float> F;
  sc_signal<sc_logic> L;

  proc1 P1("P1", clock, I, C, F, L);

  tf = sc_create_wif_trace_file("test09");
  sc_trace(tf, clock, "Clock");
  sc_trace(tf, I, "Int");
  sc_trace(tf, C, "Char");
  sc_trace(tf, F, "Float");
  sc_trace(tf, L, "Logic");

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
