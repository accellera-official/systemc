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

/* Main routine - top level */

#include "counter.h"
#include "sg.h"
#include "fsmr.h"

extern "C" int atoi(const char *);

int sc_main(int ac, char *av[])
{
  sc_signal<bool> handshake ("HS");
  sc_signal<bool> found;
  sc_signal<char> stream ("ST");

  sc_clock clk("Clock", 20, SC_NS, 0.5, 0.0, SC_NS);

  counter cnt("COUNTER", clk, found);
  fsm_recognizer fsm("Recog", clk, stream, handshake, found);
  stimgen chargen("TESTB", clk, stream, handshake);

  int n;
  if (ac == 2) n = atoi(av[1]);
  else n = 6340;
  sc_start(n, SC_NS);
  cout << endl;
  return 0;
}

