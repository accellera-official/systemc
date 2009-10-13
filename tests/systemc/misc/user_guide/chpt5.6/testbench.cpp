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
  int num = 2;

  add_reset.write(false);
  mult_reset.write(false);
  number.write(num);
  wait();

  while (true) {
    // Send out some numbers
    for (int i=0; i<13; i++) {
      number.write(num++);
      wait();
      cout << "Result = " << resulta.read() << " and " << resultm.read()
           << endl;
    }

    cout << "Asserting both reset" << endl;
    num = 2;
    number.write(num);
    add_reset.write(true);
    mult_reset.write(true);
    for (int i=0; i<3; i++) { // Keep reset low for 3 cycles
      wait();
      cout << "Result = " << resulta.read() << " and " << resultm.read()
           << endl;
    }
    add_reset.write(false);
    mult_reset.write(false);
    wait();

    // Send out some more numbers
    for (int i=0; i<13; i++) { 
      number.write(num++);
      wait();
      cout << "Result = " << resulta.read() << " and " << resultm.read()
           << endl;
    }
    
    cout << "Asserting ADDER reset" << endl;
    num = 2;
    number.write(num);
    add_reset.write(true);
    for (int i=0; i<3; i++) { // Keep reset low for 3 cycles
      wait();
      cout << "Result = " << resulta.read() << " and " << resultm.read()
           << endl;
    }
    add_reset.write(false);
    wait();

    // Send out some more numbers
    for (int i=0; i<3; i++) { 
      number.write(num++);
      wait();
      cout << "Result = " << resulta.read() << " and " << resultm.read()
           << endl;
    }
    
    cout << "Asserting MULT reset" << endl;
    num = 2;
    number.write(num);
    mult_reset.write(true);
    for (int i=0; i<3; i++) { // Keep reset low for 3 cycles
      wait();
      cout << "Result = " << resulta.read() << " and " << resultm.read()
           << endl;
    }
    mult_reset.write(false);
    wait();
  }
} // end of entry function

