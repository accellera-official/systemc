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

  main.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Main routine for tri-state simulation */

#include "tx.h"
#include "monitor.h"

int sc_main(int ac, char *av[])
{
  sc_signal<bool> tx;
  sc_signal<bool> wr;
  sc_signal<int>  dout;
  sc_signal<bool> two_stop_bits;
  sc_clock clock("CLK", 10.0, SC_NS, 0.5, 0.0, SC_NS);
  sc_trace_file *tf = sc_create_wif_trace_file("pct1");

  tx = true;
  wr = true;
  dout = 0;

  two_stop_bits = true;
  sio_tx TX("Transmitter", clock, two_stop_bits, tx);
  monitor MON("Monitor", clock, tx, dout, wr);

  //  sc_trace(tf, clock.signal(), "Clock");
  sc_trace(tf, tx, "Tx");
  sc_trace(tf, dout, "dout");
  sc_trace(tf, wr, "wr");

  sc_start(500, SC_NS);
  sc_close_wif_trace_file( tf );
  return 0;
}

