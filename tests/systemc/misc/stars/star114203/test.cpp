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
Dec/5/00 ulrich

The constructor of an sc_biguint with an sc_bv as an argument does not
compile. That happens with SystemC 1.0.1 on Solaris with both SC5.0
and gcc.
*/

#include <systemc.h>

int sc_main(int argc, char* arg[]) 
{
    sc_bv<10>      bv10 = "0000111100";
    sc_bigint<10>  bi10;
    sc_biguint<10> bu10;

    // works fine
    bi10 = sc_bigint<10> (bv10);

    // causes errors on g++, SC5.0 :
    // g++ : 
    //   .../include/numeric_bit/sc_biguint.h: 
    //   In method `sc_biguint<10>::sc_biguint(const sc_bv_ns::sc_bv<10> &)':
    //   str.cc:10:   instantiated from here
    //   .../include/numeric_bit/sc_biguint.h:186: type `sc_signed' 
    //   is not a base type for type `sc_biguint<10>'
    //   .../include/numeric_bit/sc_unsigned.h:1365: 
    //   `sc_unsigned::sc_unsigned()' is private
    //   .../include/numeric_bit/sc_biguint.h:186: within this context
    // SC5.0:
    //   ".../include/numeric_bit/sc_biguint.h", line 186: 
    //   Error: sc_signed is not a direct base class of sc_biguint<10>.
    //   ".../include/numeric_bit/sc_biguint.h", line 187: 
    //    Error: sc_unsigned::sc_unsigned() is not accessible from .

    bu10 = sc_biguint<10>(bv10);


    cout << bv10.to_string() << endl;
    cout << bi10.to_string(SC_BIN) << endl;
    cout << bu10.to_string(SC_BIN) << endl;

    return 0;
}
