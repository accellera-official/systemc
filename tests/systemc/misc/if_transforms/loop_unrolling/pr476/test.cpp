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
//	Verifies loop unrolling for various data types
//
//      Test Plan: 5.2
//      From PR-476
//
//	Author: PRP
//	Date Created: 05 APR 99
//


#include "test.h"
 
void test::entry() 
{
  int         i;
  short int   j;
  long int    k;
  sc_int<4>   l;
  sc_uint<4>  m;
  sc_bigint<4>   p;
  sc_biguint<4>  q;
  sc_signed   n(8);
  sc_unsigned o(8);
  int tmp = 0;

  for (i = 0; i < 3; i++)
    tmp++;

  for (j = 0; j < 3; j++)
    tmp++;

  for (k = 0; k < 3; k++)
    tmp++;

  for (l = 0; l < 3; l++)
    tmp++;

  for (m = 0; m < 3; m++)
    tmp++;

  for (n = 0; n < 3; n++)
    tmp++;

  for (o = 0; o < 3; o++)
    tmp++;

  for (p = 0; p < 3; p++)
    tmp++;

  for (q = 0; q < 3; q++)
    tmp++;

  o1 = tmp;

  wait();
}
 
