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

/* Main routine for biquad simulation */

#include "testbench.h"
#include "biquad.h"
#include "delay_line.h"
#include "op_queue.h"
#include "getres.h"

int
sc_main(int ac, char *av[])
{
  sc_signal<float> sample;
  sc_signal<float> result;
  sc_signal<float> delayed_out;
  sc_signal<float> final_out;
  sc_signal<bool> reset;
  sc_signal<bool> pop;

  sample = 0.0;
  result = 0.0;
  delayed_out = 0.0;
  final_out = 0.0;
  pop = false;

  sc_clock clk("Clock", CLOCK_PERIOD, SC_NS);

  testbench TB("TB", clk, result, reset, sample);
  biquad filter("BFILTER", clk, sample, reset, result);
  delay_line D("Delay", clk, result, delayed_out);
  op_queue OPQ("OPQueue", clk, delayed_out, pop, final_out, 1000);
  getres OP("OPStage", clk, final_out, pop);

  int n;
  if (ac == 2) 
    n = atoi(av[1]);
  else
    n = 10000;
  sc_start(n, SC_NS);
  return 0;
}
