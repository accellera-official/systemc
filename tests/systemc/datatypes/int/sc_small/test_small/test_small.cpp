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

  test_small.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"


int sc_main(int argc, char* argv[])
{
  sc_int<4> a(8);
  sc_uint<4> b(8);
  
  cout << "a = " << a << " " << "b = " << b << endl;
  a += 1;
  b += 1;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a -= 1;
  b -= 1;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a *= 2;
  b *= 2;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a = a * 2;
  b = b * 2;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a /= 2;
  b /= 2;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a = a/2;
  b = b/2;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a &= 0x04;
  b &= 0x04;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a |= 0x04;
  b |= 0x04;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a ^= 0x04;
  b ^= 0x04;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  int c = -8;
  c %= 6;
  a %= 6;
  b %= 6;
  cout << "a = " << a << " " << "b = " << b << " " << "c = " << c << endl;
  a = 8;
  b = 8;
  a <<= 1;
  b <<= 1;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  a >>= 1;
  b >>= 1;
  cout << "a = " << a << " " << "b = " << b << endl;
  a = 8;
  b = 8;
  cout << "a = " << a++ << " " << "b = " << b++ << endl;
  a = 8;
  b = 8;
  cout << "a = " << ++a << " " << "b = " << ++b << endl;
  a = 8;
  b = 8;
  cout << "a = " << a-- << " " << "b = " << b-- << endl;
  a = 8;
  b = 8;
  cout << "a = " << --a << " " << "b = " << --b << endl;
  return 0;
}
