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

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*
Dec/6/01 ulrich

Comparison of bit-slices of sc_int and sc_uint vectors
does not work due to ambiguity. See example below.
It does work with sc_bigint, however.
*/

#include <systemc.h>

int sc_main(int, char**)
{
  {
    // sc_biguint<32> a,b;
    sc_uint<32> a,b;
    a=15;
    b=45;

    cout << (a.range(5,2) <  b.range(5,2)) << "\n";
    cout << (a.range(5,2) <= b.range(5,2)) << "\n";
    cout << (a.range(5,2) >  b.range(5,2)) << "\n";
    cout << (a.range(5,2) >= b.range(5,2)) << "\n";
  }

  {
    // sc_bigint<32> a,b;
    sc_int<32> a,b;
    a=15;
    b=45;

    cout << (a.range(5,2) <  b.range(5,2)) << "\n";
    cout << (a.range(5,2) <= b.range(5,2)) << "\n";
    cout << (a.range(5,2) >  b.range(5,2)) << "\n";
    cout << (a.range(5,2) >= b.range(5,2)) << "\n";
  }

  return 0;
}
