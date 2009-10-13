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
//      From 39.sc
//
//	Author: PRP
//	Date Created: 05 APR 99
//


#include "test.h"
 
void test::entry() 
{
  int i,j,k,m;
  int a[10],b[10];
   

  wait_until (cont1.delayed() == 1);
  wait ();

 
  for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++)
                for (k = 0; k < 4; k++)
                        a[i] = b[i] + 53;
 
  for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++)
                for (k = 0; k < 4; k++)
                        for (m = 0; m < 2; m++)
                                a[i] = b[i] + 17;

  wait();

}
 
