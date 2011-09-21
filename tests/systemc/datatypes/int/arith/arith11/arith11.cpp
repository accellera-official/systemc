/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

  arith11.cpp -- test auto increment on sc_bigint<N> and sc_biguint<N>

  Original Author: Andy Goodrich, Forte Design Systems, 24 March 2008

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>
 
int sc_main(int argc, char**argv)
{
  sc_biguint<3> a = 7;
  sc_bigint<3>  b = 7;

  ++a;
  cout << "a         is " << a << endl;
  cout << "a == 0    is " << (a==0) << endl;
  cout << "raw digit: " << *a.get_raw() << endl;
  cout << endl;

  ++b;
  cout << "b         is " << b << endl;
  cout << "b == 0    is " << (b==0) << endl;
  cout << "raw digit: " << *b.get_raw() << endl;

  return 0;
}
