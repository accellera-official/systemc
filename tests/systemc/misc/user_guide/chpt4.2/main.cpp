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

/* Main file for pipeline simulation */

#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "display.h"
#include "numgen.h"
#include "stage1_2.h"
#include "pipeline.h"
#include "testbench.h"

int sc_main(int ac, char *av[])
{
  sc_signal<double> in1;
  sc_signal<double> in2;
  sc_signal<double> powr;

  in1 = 0.0;
  in2 = 0.0;
  powr = 0.0;
  sc_clock clk("CLOCK", 20.0, 0.5, 0.0);

  testbench T("Testbench", clk, powr, in1, in2);
  pipeline P("PIPE", clk, in1, in2, powr);

  sc_clock::start(1000);
  return 0;
}

