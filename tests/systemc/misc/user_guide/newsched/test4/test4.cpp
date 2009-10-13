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

  test4.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*
  Corner case testing for new scheduler.
  Case 4: Checking gated clocks and triggering of processes
  */

#include "systemc.h"

SC_MODULE( cgater )
{
  SC_HAS_PROCESS( cgater );

  const sc_signal<bool>& gate;
  sc_in<bool>  clock_in;
  sc_out<bool> clock_out;

  cgater(sc_module_name NAME,
	 const sc_signal<bool>& GATE,
	 sc_signal_in_if<bool>& CLOCK_IN,
	 sc_signal_out_if<bool>& CLOCK_OUT)
    : gate(GATE)
  {
    clock_in(CLOCK_IN); 
    clock_out(CLOCK_OUT);
    SC_METHOD( entry );
    sensitive << gate;
    sensitive << clock_in;
  }

  void entry()
  {
    clock_out = clock_in & gate;
  }
};

SC_MODULE( watcher )
{
  SC_HAS_PROCESS( watcher );

  const sc_signal<bool>& gate;
  sc_in<bool> clock;
  sc_in<bool> dclock;
  const sc_signal<int>& a;

  watcher(sc_module_name NAME,
	  const sc_signal<bool>& GATE,
	  sc_signal_in_if<bool>& CLOCK,
	  sc_signal_in_if<bool>& DCLOCK,
	  const sc_signal<int>& A)
    : gate(GATE), a(A)
  {
    clock(CLOCK);
    dclock(DCLOCK);
    SC_METHOD( entry );
    sensitive << clock;
    sensitive << a;
    sensitive << gate;
    sensitive << dclock;
  }

  void entry()
  {
    cout << "[ "; 
    if (clock.posedge()) cout << "Posedge - ";
    if (clock.negedge()) cout << "Negedge - ";
    if (dclock.posedge()) cout << "Posedge(D) - ";
    if (dclock.negedge()) cout << "Negedge(D) - ";
    if (a.event()) cout << "A = " << a.read() << " - ";
    if (gate.event()) cout << "Gate = " << gate.read() << " - ";
    cout << "]" << endl;
  }
};


SC_MODULE( gategen )
{
  SC_HAS_PROCESS( gategen );

  sc_in<bool> clk;

  sc_signal<bool>& gate;
  
  gategen(sc_module_name NAME,
	  sc_signal_in_if<bool>& CLK,
	  sc_signal<bool>& GATE)
    : gate(GATE)
  {
    clk(CLK);
    SC_CTHREAD( entry, clk.pos() );
    gate = 1;
  }

  void entry()
  {
    while (true) {
      gate = 1; wait(3);
      gate = 0; wait (3);
    }
  }
};

SC_MODULE( trigp )
{
  SC_HAS_PROCESS( trigp );

  sc_in<bool> clk;

  sc_signal<int>& out;

  trigp(sc_module_name NAME,
	sc_signal_in_if<bool>& CLK,
	sc_signal<int>& OUT_)
    : out(OUT_)
  { 
    clk(CLK);
    SC_CTHREAD( entry, clk.pos() );
    out = 0;
  }

  void entry()
  {
    int i = 11;
    while (true) {
      out = i++;
      wait();
    }
  }
};

int
sc_main(int ac, char *av[])
{
  // sc_clock clock1("Clock1", 20, SC_NS, 0.5);
  // sc_clock dclock("Derived");
  sc_signal<bool> clock1( "Clock1" );
  sc_signal<bool> dclock( "Derived" );

  sc_signal<bool> Gate;
  sc_signal<int> Output;

  cgater CG("CG", Gate, clock1, dclock);
  watcher W("W", Gate, clock1, dclock, Output);
  gategen G("G", clock1, Gate);
  trigp T("T", dclock, Output);

  sc_trace_file *tf = sc_create_vcd_trace_file("systemc");
  sc_trace(tf, clock1, "Clock");
  sc_trace(tf, dclock, "Dclock");
  sc_trace(tf, Gate, "Gate");
  sc_trace(tf, Output, "Out");

  sc_start(0, SC_NS);
  clock1.write(0);
  sc_start(5, SC_NS);
  for (int i=0; i < 30; i++) {
    clock1.write(1);
    sc_start(5, SC_NS);
    clock1.write(0);
    sc_start(5, SC_NS);
  }

  return 0;
}
