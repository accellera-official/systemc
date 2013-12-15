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
//	Verifies loop unrolling
//
//      Test Plan: 5.2
//      From 40.sc
//
//	Author: PRP
//	Date Created: 05 APR 99
//


#include "test.h"
 
void test::entry() 
{
  int i,j,k,m;
  int a[10],b[10];
   

  do { wait(); } while  (cont1 == 0);
  wait ();

  for (i = 0; i < 2; i++) {
        if (i == 1)
                continue;
        a[i] = 5;
 
        wait();
  }
  for (i = 0; i < 2; i++) {
        if (i == 1)
                break;
        a[i] = 10;
 
        wait();
  }
 
  for (i = 0; i < 2; i++)
        a[i] = 6;
 
  for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
                a[j] = 8;
                if (j == 1)
                        continue;
                a[j] = 7;
 
                wait();
        }
        for (j = 0; j < 2; j++)
                a[j] = 8;
  }



  wait();

}
 
