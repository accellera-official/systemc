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

  Original Author: John Rose, Cadence Design Systems, Inc., 2004-02-15

 *****************************************************************************/

// tests sc_spawn() in start_of_simulation()

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
// $Log: test05.cpp,v $
// Revision 1.2  2011/02/01 17:17:40  acg
//  Andy Goodrich: update of copyright notice, added visible CVS logging.
//

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"
SC_MODULE(mod)
{
  void start_of_simulation()
  {
    cout << "spawn thetest" << endl;
    sc_spawn(sc_bind(&mod::thetest, this));
  }
  void thetest()
  {
    cout << "in thetest()" << endl;
    wait(10,SC_NS);
    sc_stop();
  }
  SC_CTOR(mod) {
  }
};

int sc_main(int, char**) {
  mod m1("m1");
  sc_start();
  return 0;
}
