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

  irq.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

#define MAX_NUM 5

SC_MODULE( proc1 )
{
  SC_HAS_PROCESS( proc1 );

  sc_in<bool> clock;
  sc_in<bool> irq[MAX_NUM];

  proc1( sc_module_name NAME,
	 sc_signal<bool>& CLOCK,
         sc_signal<bool>* IRQ )
  {
    clock(CLOCK);
    for( int i = 0; i < MAX_NUM; ++ i ) {
        irq[i]( IRQ[i] );
    }

    SC_THREAD( entry );
    sensitive << irq[0];
    sensitive << irq[1];
  }

  void entry()
  {
    while (true) {
      for (int i = 0; i<MAX_NUM; i++) {
	if (irq[i].posedge())
	  cout << "Posedge on IRQ " << i << endl;
	if (irq[i].negedge())
          cout << "Negedge on IRQ " << i << endl;
      }
      wait();
    }
  }
};

int sc_main(int ac, char *av[])
{
  sc_signal<bool> clock;
  sc_signal<bool> irq[MAX_NUM];

  proc1 P1("P1", clock, irq);

  for( int i = 0; i < MAX_NUM; ++ i ) {
    irq[i] = 0;
  }
  sc_start(0, SC_NS);
  for (int i=0; i < MAX_NUM; i++) {
    clock = 0;
    sc_start(1, SC_NS);
    irq[i] = 1;
    clock = 1;
    sc_start(1, SC_NS);
    clock = 0;
    irq[i] = 0;
    sc_start(1, SC_NS);
  }
  return 0;
}
