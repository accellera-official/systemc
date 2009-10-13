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

  test1.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( p1 )
{
  SC_HAS_PROCESS( p1 );

  sc_fifo<int>& a;
  sc_fifo<int>& b;
  sc_signal<bool>& clock;

  int init;

  p1(sc_module_name name,
     sc_fifo<int>& A,
     sc_fifo<int>& B, 
     sc_signal<bool>& CLOCK,
     int INIT)
    : a(A), b(B), clock(CLOCK)
  {
    init = INIT;
    SC_THREAD( entry );
    sensitive(clock);
    // sensitive(b);
  }

  void entry() {
    wait();
    int i = init;
    wait();
    while (true) {
      a.write(i);
      int j = b.read();
      cout << "Value sent = " << i << " Value read = " << j << endl;
      wait(); i++;
    }
  }
};

int sc_main(int ac, char *av[])
{
  sc_fifo<int> a(2), b(2);
  sc_signal<bool> clock;

  p1 Proc1("Proc1", a, b, clock, 10);
  p1 Proc2("Proc2", b, a, clock, 129);

  sc_initialize();
  clock = 1;
  sc_cycle(1);
  clock = 0;
  sc_cycle(1);

  clock = 1;
  sc_cycle(1);
  clock = 0;
  sc_cycle(1);

  clock = 1;
  sc_cycle(1);
  clock = 0;
  sc_cycle(1);

  clock = 1;
  sc_cycle(1);
  clock = 0;
  sc_cycle(1);

  clock = 1;
  sc_cycle(1);
  clock = 0;
  sc_cycle(1);

  return 0;
}
