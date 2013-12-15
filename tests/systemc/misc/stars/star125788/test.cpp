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

/*
Please add support in form of constructors and assigment operators
such that assigments between types sc_int, sc_uint, sc_bigint, sc_biguint
work in all cases. Currently, assigning a sc_int or sc_uint does not
work in most cases.

The following test case shows were things don't work. I tried it with
g++ 2.92.5 and Sun SC6.1 on Solaris.
*/

#include <systemc.h>

int sc_main( int, char*[] )
{
  sc_int<8>      i8   =  8;
  sc_uint<9>     u9   =  9;
  sc_bigint<10>  bi10 = 10;
  sc_biguint<11> bu11 = 11;

  sc_int<8>      i8_2;
  sc_uint<9>     u9_2;
  sc_bigint<10>  bi10_2;
  sc_biguint<11> bu11_2;

  i8_2 = sc_int<8>(i8);
  cout << i8_2 << endl;
  i8_2 = sc_int<8>(u9);     // g++ 2.95.2: ambiguous
  cout << i8_2 << endl;
  i8_2 = sc_int<8>(bi10);
  cout << i8_2 << endl;
  i8_2 = sc_int<8>(bu11);
  cout << i8_2 << endl;

  cout << endl;

  u9_2 = sc_uint<9>(i8);     // g++ 2.95.2: ambiguous
  cout << u9_2 << endl;
  u9_2 = sc_uint<9>(u9);
  cout << u9_2 << endl;
  u9_2 = sc_uint<9>(bi10);
  cout << u9_2 << endl;
  u9_2 = sc_uint<9>(bu11);
  cout << u9_2 << endl;

  cout << endl;

  bi10_2 = sc_bigint<10>(i8);
  cout << bi10_2 << endl;
  bi10_2 = sc_bigint<10>(u9);
  cout << bi10_2 << endl;
  bi10_2 = sc_bigint<10>(bi10);
  cout << bi10_2 << endl;
  bi10_2 = sc_bigint<10>(bu11);
  cout << bi10_2 << endl;

  cout << endl;

  bu11_2 = sc_biguint<11>(i8);  // g++ 2.95.2: ambiguous, SC6.1: error
  cout << bu11_2 << endl;
  bu11_2 = sc_biguint<11>(u9);  // g++ 2.95.2: ambiguous, SC6.1: error
  cout << bu11_2 << endl;
  bu11_2 = sc_biguint<11>(bi10);
  cout << bu11_2 << endl;
  bu11_2 = sc_biguint<11>(bu11);
  cout << bu11_2 << endl;

  cout << endl;

  i8_2 = i8;
  cout << i8_2 << endl;
  i8_2 = u9;
  cout << i8_2 << endl;
  i8_2 = bi10;
  cout << i8_2 << endl;
  i8_2 = bu11;
  cout << i8_2 << endl;

  cout << endl;

  u9_2 = i8;
  cout << u9_2 << endl;
  u9_2 = u9;
  cout << u9_2 << endl;
  u9_2 = bi10;
  cout << u9_2 << endl;
  u9_2 = bu11;
  cout << u9_2 << endl;

  cout << endl;

  bi10_2 = i8;
  cout << bi10_2 << endl;
  bi10_2 = u9;
  cout << bi10_2 << endl;
  bi10_2 = bi10;
  cout << bi10_2 << endl;
  bi10_2 = bu11;
  cout << bi10_2 << endl;

  cout << endl;

  bu11_2 = i8;
  cout << bu11_2 << endl;
  bu11_2 = u9;
  cout << bu11_2 << endl;
  bu11_2 = bi10;
  cout << bu11_2 << endl;
  bu11_2 = bu11;
  cout << bu11_2 << endl;

  return 0;
}
