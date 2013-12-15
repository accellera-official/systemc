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
#include "test.h"

void test::entry() 
{
  int i,j,h, temp;
 
  wait ();
  j = i1.read();
  for (i = 0; i < 4; i = i + 1)
  {  // Default: no unrolling
    j = j + 1;
    wait();
  }
  wait ();
  temp = 4+j;
  modify (o1, temp); 
  h = (i1 > i2) ? i3 : i4;
  o2 = h;
  wait ();
  i= 9;
  noModify (i);
  wait();
}
 

void test::modify (sc_signal<int>& i, int& j)
{
  i = i1 + j;
}
 
void test::noModify (int i)
{
  o3 = i;
}
