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

  test03.cpp -- test sign extension in part select assignments.

  Original Author: Andy Goodrich, Forte Design Systems, 29 April 2008

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#define TERM(EXPR) cout << #EXPR << " " << hex << (EXPR) << endl


int sc_main(int argc, char* argv[])
{
    sc_biguint<31>  a;
    sc_bigint<4>   b = 0xa;
    sc_bigint<5>   c = 0x0a;
    sc_biguint<4>  d = 0x0a;
    a = 0x12345678;
    a.range(15,8) = b;
    cout << hex << a << endl;
    a = 0x12345678;
    a.range(15,8) = c;
    cout << hex << a << endl;
    a = 0x12345678;
    a.range(15,8) = d;
    cout << hex << a << endl;

    cout << "Program completed" << endl;
    return 0;
}
