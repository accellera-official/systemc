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

  test11.cpp -- Test that sign extension and bit inversion works for the
                ~ operator when used in sc_XXsigned::concat_get_data()

  Original Author: Andy Goodrich, Forte Design Systems, 29 April 2008

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
    sc_bigint<20> w;
    sc_biguint<20> x;
    sc_biguint<30> y;

    w = 1;
    x = 1;
    y = (sc_bigint<21>)(sc_uint<1>(1),~w);
    cout << "sc_signed 1 " << hex << y << endl;
    y = (sc_bigint<21>)(sc_uint<1>(1),~x);
    cout << "sc_unsigned 1 " << hex << y << endl;

    w = -1;
    x = -1;
    y = (sc_bigint<21>)(sc_uint<1>(1),~w);
    cout << "sc_signed -1 " << hex << y << endl;
    y = (sc_bigint<21>)(sc_uint<1>(1),~x);
    cout << "sc_unsigned -1 " << hex << y << endl;

    w = 0x80000;
    x = 0x80000;
    y = (sc_bigint<21>)(sc_uint<1>(1),~w);
    cout << "sc_signed 0x800000 " << hex << y << endl;
    y = (sc_bigint<21>)(sc_uint<1>(1),~x);
    cout << "sc_unsigned 0x800000 " << hex << y << endl;

    w = 0;
    x = 0;
    y = (sc_bigint<21>)(sc_uint<1>(1),~w);
    cout << "sc_signed 0 " << hex << y << endl;
    y = (sc_bigint<21>)(sc_uint<1>(1),~x);
    cout << "sc_unsigned 0 " << hex << y << endl;

    cout << "Program completed" << endl;
    return 0;
}
