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

  test4.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"


SC_MODULE( proc1 )
{
  SC_HAS_PROCESS( proc1 );

  sc_signal_resolved& out;
  sc_in<bool> in;

  proc1( sc_module_name n,
	 sc_signal_resolved& OUT_,
	 sc_signal<bool>& IN_ )
    : out(OUT_), in(IN_)
  {
    SC_THREAD( entry );
    sensitive(in);
  }

  void entry();
};

void
proc1::entry()
{
  wait();
  while (true) {
    if ((bool) in == true) {
      cout << "P1: Set to 1" << endl;
      out = SC_LOGIC_1;//'1';
    }
    else {
      cout << "P1: Set to Z" << endl;
      out = SC_LOGIC_Z;//'Z';
    }
    wait();
  }
}


SC_MODULE( proc2 )
{
  SC_HAS_PROCESS( proc2 );

  sc_signal_resolved& out;
  sc_in<bool> in;

  proc2( sc_module_name n,
	 sc_signal_resolved& OUT_,
	 sc_signal<bool>& IN_ )
    : out(OUT_), in(IN_)
  {
    SC_THREAD( entry );
    sensitive(in);
  }

  void entry();
};

void
proc2::entry()
{
  wait();
  while (true) {
    if ((bool) in == false) {
      cout << "P2: Set to 0" << endl;
      out = SC_LOGIC_0;//'0';
    }
    else {
      cout << "P2: Set to Z" << endl;
      out = SC_LOGIC_Z;//'Z';
    }
    wait();
  }
}

SC_MODULE( proc3 )
{
  SC_HAS_PROCESS( proc3 );

  const sc_signal_resolved& in;

  proc3( sc_module_name n,
	 const sc_signal_resolved& IN_ )
    : in(IN_)
  {
    SC_METHOD( entry );
    sensitive(in);
  }

  void entry()
  {
    sc_logic v;
    v = in;
    cout << "Value on Bus = " << v.to_char() << endl;
  }
};

int sc_main(int ac, char *av[])
{
  sc_signal_resolved Bus;
  sc_signal<bool> clock;

  proc1 P1("P1", Bus, clock);
  proc2 P2("P2", Bus, clock);
  proc3 P3("P3", Bus);

  sc_initialize();
  clock = 1;
  sc_cycle(10);
  for (int i = 0; i < 3; i++) {
    clock = 0;
    sc_cycle(10);
    clock = 1;
    sc_cycle(10);
  }
  return 0;
}

