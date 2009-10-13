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

// Main routine
 
#include "test.h"
#include "tb.h"
#include "define.h"
 
int sc_main(int ac, char *av[])
{
  sc_clock clock("Clock", CLOCK_PERIOD, DUTY_CYCLE, 0);
 
  sc_signal<bool> reset_sig;

  sc_signal<int> i1;
  sc_signal<int> i2;
  sc_signal<int> i3;
  sc_signal<int> i4;
  sc_signal<int> i5;
  
  sc_signal<bool> cont1;
  sc_signal<bool> cont2;
  sc_signal<bool> cont3;
  
  sc_signal<int> o1;
  sc_signal<int> o2;
  sc_signal<int> o3;
  sc_signal<int> o4;
  sc_signal<int> o5;

  test TEST ("TEST", clock, reset_sig, i1, i2, i3, i4, i5,
	 cont1, cont2, cont3, o1, o2, o3, o4, o5);
  tb TB ("TB", clock, reset_sig, i1, i2, i3, i4, i5,
	 cont1, cont2, cont3, o1, o2, o3, o4, o5);

  // Simulation Run Control
  sc_start (-1);
  return 0;
}
