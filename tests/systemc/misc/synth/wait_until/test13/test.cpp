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

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "test.h"

/* From Test Case 57.sc */
void test::entry() 
{
  while (true) {

  do { wait(); } while  (cont1 != 1);
  wait ();
  o1 = 9;
 
  switch (i5) {
  case 1: 
        if (i2 > 4) {
                o2 = 80;
                do { wait(); } while  (cont2 != 1);
        } else {
                o2 = 81;
                wait();
        }
        break;
  case 2: 
        o2 = 9; 
        if (i4 == 5) {
                wait(); wait(); break;
        }
        else 
                wait(); 
        break;
  case 3: 
        o2 = 10;
        wait();
        while (i3 < 5) {
                wait();
                o3 = i4 + 1;
                if (i4 > 7) break;
                wait();
        }
        wait ();
        break;
   default: o2 = 11; wait(); break;
  }
  if (i3 == 3) {
        o1 = 4;
        wait();
  } else {
        o1 = 5;
        wait();
  }

  }
}

