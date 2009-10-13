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

  accumulator.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename accumulator.cc */
/* This is the implementation file for synchronous process `accumulator' */

#include "accumulator.h"

void accumulator::entry()
{
  int a = 1;
  sum_acc = 1234; // some random number
  mult_acc = 3;   // some random number

  while (true) {
    W_BEGIN 
      {
	watching (reset_add.delayed() == true);
      }
    W_DO
      {
	W_BEGIN
	  {
	    watching (reset_mult.delayed() == true);
	  }
	W_DO
	  {
	    while (true) {
	      sum_acc += a;
	      mult_acc *= a;
	      sum.write(sum_acc);
	      prod.write(mult_acc);
	      wait();
	      a = number.read();
	    }
	  }
	W_ESCAPE
	  {
            cout << "Reset mult seen" << endl;
	    mult_acc = 1;
	    wait(5);
	  }
	W_END
      }
    W_ESCAPE
      {
        cout << "Reset add seen" << endl;
	sum_acc = 0;
	wait(3);
      }
    W_END
  }
} // end of entry function

