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
//	Verifies loop unrolling
//
//      Test Plan: 5.2
//
//	Author: PRP
//	Date Created: 19 Feb 99
//


#include "test.h"
 
void test::entry() 
{
  int i,j;
  int a[10],b[10];
   

  wait_until (cont1.delayed() == 1);
  wait ();

  i = 0;
  while (i < 4) {
        a[i] = 0;
        i = i + 1;
  }
 
  i = 0;
  while (i <= 4) {
        b[i] = 10;
        i = i + 1;
  }

  i = 9;
  while (i > 4) {
        a[i] = 20;
        i = i - 1;
  }
 
  i = 9;
  while (i >= 4) {
        b[i] = 30;
        i = i - 1;
  }
 
  i = -4;
  while (i < 0) {
        a[i+4] = 40;
        i = i + 1;
  }
 
 
  i = -4;
  while (i < 0) {
        a[i+4] = 50;
        i = i + 2;
  }
 
  i = -4;
  while (i <= 0) {
        a[i+4] = 60;
        i = i + 2;
  }
 
  i = -4;
  while (i <= 0) {
        a[i+4] = 70;
        i = i + 3;
  }
  i = -4;
  while (i <= 0) {
        a[i+4] = 80;
        i = i + 4;
  }
 
  i = -6;
  while (i <= 0) {
        a[i+6] = 90;
        i = i + 5;
  }
 
  i = 8;
  if (i) {
        j = 9;
  }
 
  while (i <= 10) {
        a[i] = 80;
        i = i + 5;
  }
 
  for (i = 0; i < 2; i++)
        a[i] = 8;
 
  i = 0;
  for (; i < 3; i++)
        a[i] = 10;
 
  i = 0;
  wait();
  for (; i < 3; ++i) {
        a[i] = 11;
        //i = i + 1;
  wait();
  }

   i = 0;
  for (; i < 3; i++)
        a[i] = 12;
 
  wait();

}
 
