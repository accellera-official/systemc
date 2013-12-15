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

  stage1.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename stage1.cc */
/* This is the implementation file for synchronous process `stage1' */

#include "stage1.h"
#include "f_stage1.h"

void stage1::entry()
{
  double a, b;

  a = 20.0;
  b = 5.0;
  while (true) {
    sum.write(a+b);
    diff.write(a-b);
    wait();
    a = in1.read();
    b = in2.read();
  }
} // end of entry function

void f_stage1(const char *NAME,
	      sc_clock& CLK,
	      const sc_signal<double>& IN1,
	      const sc_signal<double>& IN2,
	      sc_signal<double>& SUM,
	      sc_signal<double>& DIFF)
{
  SC_NEW(stage1(NAME, CLK, IN1, IN2, SUM, DIFF));
}
