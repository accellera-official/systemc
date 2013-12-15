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

  Original Author: Bishnupriya Bhattacharya, Cadence Design Systems, Inc., 2004-03-10

 *****************************************************************************/

// tests parent spawning process dying before child spawned process.

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
// $Log: test06.cpp,v $
// Revision 1.2  2011/02/01 17:17:40  acg
//  Andy Goodrich: update of copyright notice, added visible CVS logging.
//

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

void p3() {
  cerr << sc_time_stamp() << ":entering p3" << endl;
  wait(30, SC_NS);
  cerr << sc_time_stamp() << ":exiting p3" << endl;
}

void p2() {
  cerr << sc_time_stamp() << ":entering p2, spawning p3" << endl;
  sc_spawn(sc_bind(&p3));
  wait(20, SC_NS);
  cerr << sc_time_stamp() << ":exiting p2" << endl;
}

void p1() {
  cerr << sc_time_stamp() << ":entering p1, spawning p2" << endl;
  sc_spawn(sc_bind(&p2));
  wait(10, SC_NS);
  cerr << sc_time_stamp() << ":exiting p1" << endl;
}

void p0() {
  cerr << sc_time_stamp() << ":entering p0, spawning p1" << endl;
  sc_spawn(sc_bind(&p1));
  cerr << sc_time_stamp() << ":exiting p0" << endl;
}

int sc_main(int, char**) {

  sc_start(10, SC_NS);
  p0();
  sc_start(50, SC_NS);

  return 0;
}
