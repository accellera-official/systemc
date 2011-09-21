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

  test3.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*
  Corner case testing for new scheduler.
  Case 3: Checking sensitive_pos() and sensitive_neg() methods
*/

#include "systemc.h"

SC_MODULE( asyncproc )
{
  SC_HAS_PROCESS( asyncproc );

  sc_in<bool> clock;

  asyncproc(sc_module_name NAME,
	    sc_signal_in_if<bool>& CLOCK)
  {
    clock(CLOCK);
    SC_THREAD( entry );
    sensitive << clock.pos();
  }

  void entry()
  {
    wait();
    while (true) {
      if (clock.posedge()) {
	cout << "AsyncProc: Posedge\n";
      }
      else {
	cout << "AsyncProc: ERROR" << endl;
      }
      wait();
    }
  }
};

SC_MODULE( asyncblock )
{
  SC_HAS_PROCESS( asyncblock );

  sc_in<bool> clock;

  asyncblock(sc_module_name NAME,
	     sc_signal_in_if<bool>& CLOCK)
  {
    clock(CLOCK);
    SC_METHOD( entry );
    sensitive << clock.neg();
  }

  void entry()
  {
    if (clock.posedge()) {
      cout << "AsyncBlock: ERROR\n";
    }
    else {
      cout << "AsyncBlock: Negedge" << endl;
    }
  }
};
    

int
sc_main(int ac, char *av[])
{
  sc_clock clock("Clock", 20, SC_NS, 0.5);

  asyncproc P2("P2", clock);
  asyncblock P3("P3", clock);

  sc_start(160, SC_NS);
  return 0;

}
