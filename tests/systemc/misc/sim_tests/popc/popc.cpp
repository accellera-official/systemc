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

  popc.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

/*
 * Test bench
 */

SC_MODULE( proc1 )
{
  SC_HAS_PROCESS( proc1 );

  sc_in_clk clk;

  // Inputs
  sc_in<bool> data_ack;
  sc_in<int>  popc;
  // Outputs
  sc_out<bool> reset;
  sc_out<bool> data_ready;
  sc_out<int>  in;

  // Constructor
  proc1( sc_module_name NAME,
	 sc_clock& CLK,
	 sc_signal<bool>& DATA_ACK,
         sc_signal<int>& POPC,
         sc_signal<bool>& RESET,
	 sc_signal<bool>& DATA_READY,
         sc_signal<int>& IN_ )
    : clk(CLK), data_ack(DATA_ACK), popc(POPC),
      reset(RESET), data_ready(DATA_READY), in(IN_)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality goes here
  void entry();
};

/*
 * popc - The process doing the population count
 *
 */

SC_MODULE( proc2 )
{
  SC_HAS_PROCESS( proc2 );

  sc_in_clk clk;

  // Inputs
  sc_in<bool> reset;
  sc_in<bool> data_ready;
  sc_in<int>  in;
  // Outputs
  sc_out<bool> data_ack;
  sc_out<int>  popc;

  // Internal variables
  int c;
  int t;
  int no;

  proc2( sc_module_name NAME,
	 sc_clock& CLK,
         sc_signal<bool>& RESET,
	 sc_signal<bool>& DATA_READY,
         sc_signal<int>& IN_,
	 sc_signal<bool>& DATA_ACK,
         sc_signal<int>& POPC )
    : clk(CLK), reset(RESET), data_ready(DATA_READY), in(IN_),
      data_ack(DATA_ACK), popc(POPC)
  {
    SC_CTHREAD( entry, clk.pos() );
    // Global watching for reset
    watching(reset.delayed() == true);
    c = 0;
    t = 0;
  }

  // Process functionality
  void entry();
};


/*
 * Testbench functionality
 */

void proc1::entry()
{
    int i;
    int j;

    j = 1;
    i = 0;
    data_ready.write(false);
    reset.write(false);

    wait();

    while(true){
	in.write(j);

        data_ready.write(true);
        wait_until(data_ack.delayed() == true);
        data_ready.write(false);
        wait_until(data_ack.delayed() == false);

        char buf[BUFSIZ];
        sprintf( buf, "Input: %7d   Population Count: %3d", j, popc.read() );
        cout << buf << endl;

	i++;

	if( i == 3){
		reset.write(true);
		wait();
		reset.write(false);
		wait(2);
	}

	if( i == 16)
	    sc_stop();

	j = (j<<1)|1;
    }
}


/*
 * popc - functionality
 */

void proc2::entry()
{
    // Reset behavior
    no = 0;
    data_ack.write(false);

    wait();
  
    while (true) {
        wait_until(data_ready.delayed() == true);

	t = in.read();
	c = 0;
	while( t ){
	    c++;
	    t &= (t-1);
	    wait();
	}

	no++;
        popc.write(c);

        data_ack.write(true);
        wait_until(data_ready.delayed() == false);
        data_ack.write(false);
    }
}


int
sc_main(int argc, char *argv[])
{
  sc_signal<bool>  data_ready("Ready");
  sc_signal<bool>  data_ack("Ack");
  sc_signal<int>   in;
  sc_signal<int>   popc;
  sc_signal<bool>  reset;

  sc_clock clock("CLOCK", 10, 0.5, 0.0);

  proc1 TestBench("TestBench", clock, data_ack, popc, reset, data_ready, in);
  proc2 Popc("Popc", clock, reset, data_ready, in,  data_ack, popc);


  // Create trace file
  sc_trace_file *tf = sc_create_vcd_trace_file("tracefile");
  // Trace signals
  sc_trace(tf, data_ready, "data_ready");
  sc_trace(tf, data_ack, "data_ack");
  sc_trace(tf, in, "in");
  sc_trace(tf, popc, "popc");
  sc_trace(tf, reset, "reset");
  // sc_trace(tf, clock.signal(), "Clock");
  sc_trace(tf, clock, "Clock");
  // Trace internal variables
  sc_trace(tf, Popc.t, "Popc.t");
  sc_trace(tf, Popc.c, "Popc.c");
  sc_trace(tf, Popc.no, "Popc.no");

  sc_start(-1);
  return 0;
}

