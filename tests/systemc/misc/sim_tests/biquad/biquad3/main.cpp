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

float signal_freq;

int
sc_main(int ac, char *av[])
{
  sc_signal<float> sample;
  sc_signal<float> result;
  sc_signal<bool> reset;
  sc_signal<float> delayed_out;

  sample = 0.0;
  result = 0.0;
  reset = false;
  delayed_out = 0.0;

  sc_clock clk("Clock", CLOCK_PERIOD, SC_NS);

  biquad filter("BFILTER", sample, reset, result);
  testbench TB("TB", clk, delayed_out, reset, sample);
  delay_line D("Delay", result, delayed_out, 300);

  int n = 10000;
  signal_freq = 100000;
  for (int i = 1; i < ac; i++) {
    if (av[i][0] == '-') {
      signal_freq = atof(av[i+1]); i++;
      continue;
    }
    n = atoi(av[i]);
  }

  sc_start(n, SC_NS);
  return 0;
}
