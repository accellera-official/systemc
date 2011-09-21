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


//
//      Verifies function inlining
//
//      Author: PRP
//      Date Created: 26 Feb 99
//
 
#include "systemc.h"
#include "define.h"
#include "test.h"
 
 
int incr1 (int x, int y)
{
  return x + y;
}
 
 
int incr (int x, int y)
{
  return incr1 (x, y);
}
 
 
void test::entry() 
{
  int i;
 
  wait();
 
  i = incr (i1.read(), i2.read());
  o1 = i;
}
