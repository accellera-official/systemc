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

  adder_sub.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename adder_sub.cc */
/* This is the implementation file for synchronous process `adder_sub' */

#include "adder_sub.h"

int add(int a, int b)
{
  return (a + b);
}

void adder_sub::entry()
{
  int sum;
  int a, b, c, d;

  while (true) {
    // Wait until you get signal to go
    do { wait(); } while (adder_sub_ready != true);
    // Read inputs
    a = Sa.read();
    b = Sb.read();
    c = Sc.read();
    
    // Perform the computation.
    sum = add(a, b);
    sum = add(sum, c);
    d = a - b;

    // Write outputs
    adder_sub_done.write(true);
    Ssum.write(sum);
    Sd.write(d);
    wait();
    adder_sub_done.write(false);
    // Loop back to do { wait(); } while . 
  }

} // end of entry function

