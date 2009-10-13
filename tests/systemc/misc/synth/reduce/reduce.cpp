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

  reduce.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

unsigned
reduce_imp(const sc_bv<8>& x)
{
    unsigned r = 0;
    r = (r << 1) | x.and_reduce();
    r = (r << 1) | x.nand_reduce();
    r = (r << 1) | x.or_reduce();
    r = (r << 1) | x.nor_reduce();
    r = (r << 1) | x.xor_reduce();
    r = (r << 1) | x.xnor_reduce();

    r = (r << 1) | and_reduce(x);
    r = (r << 1) | nand_reduce(x);
    r = (r << 1) | or_reduce(x);
    r = (r << 1) | nor_reduce(x);
    r = (r << 1) | xor_reduce(x);
    r = (r << 1) | xnor_reduce(x);

    return r;
}

int
sc_main(int argc, char* argv[])
{
    sc_bv<8> u;
    u = "10011011";
    cout << reduce_imp(u) << endl;
    u = "11101001";
    cout << reduce_imp(u) << endl;
    u = "01101001";
    cout << reduce_imp(u) << endl;
    u = "11111111";
    cout << reduce_imp(u) << endl;
    u = "00000000";
    cout << reduce_imp(u) << endl;

    return 0;
}
