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

  testbench.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename testbench.cc */
/* This is the implementation file for synchronous process `testbench' */

#include "testbench.h"

void testbench::entry()
{
  char buf[BUFSIZ];
  s.write(true);
  r.write(false);
  wait();
  sprintf(buf, "SR=%x%x QQ'=%x%x", true, false, q.read(), qp.read());
  cout << buf << endl;
  s.write(false);
  r.write(true);
  wait();
  sprintf(buf, "SR=%x%x QQ'=%x%x", false, true, q.read(), qp.read());
  cout << buf << endl;
  s.write(false);
  r.write(false);
  wait();
  sprintf(buf, "SR=%x%x QQ'=%x%x", false, false, q.read(), qp.read());
  cout << buf << endl;
  s.write(true);
  r.write(true);
  wait();
  sprintf(buf, "SR=%x%x QQ'=%x%x", true, true, q.read(), qp.read());
  cout << buf << endl;
  sc_stop();
} // end of entry function

