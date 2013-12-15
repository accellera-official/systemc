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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-24

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_[u]int_(conc|sub)ref assignment from sc_[un]signed and
// sc_(bv|lv)_base

#include "systemc.h"

int
sc_main( int, char*[] )
{
    sc_bigint<4> a = 1;
    sc_biguint<4> b = 2;
    sc_bv<4> c = 3;
    sc_lv<4> d = 4;

    {
        cout << "\n*** sc_int ***" << endl;

        sc_int<2> e = 0;
        sc_int<2> f = 0;
        sc_int<8> g = 0;

        cout << "sc_int_concref = sc_signed" << endl;
        (e, f) = a;
        cout << e << endl;
        cout << f << endl;

        cout << "sc_int_concref = sc_unsigned" << endl;
        (e, f) = b;
        cout << e << endl;
        cout << f << endl;

        cout << "sc_int_concref = sc_bv_base" << endl;
        (e, f) = c;
        cout << e << endl;
        cout << f << endl;

        cout << "sc_int_concref = sc_lv_base" << endl;
        (e, f) = d;
        cout << e << endl;
        cout << f << endl;

        cout << "sc_int_subref = sc_signed" << endl;
        g( 7, 4 ) = a;
        cout << g << endl;

        cout << "sc_int_subref = sc_unsigned" << endl;
        g( 7, 4 ) = b;
        cout << g << endl;

        cout << "sc_int_subref = sc_bv_base" << endl;
        g( 7, 4 ) = c;
        cout << g << endl;

        cout << "sc_int_subref = sc_lv_base" << endl;
        g( 7, 4 ) = d;
        cout << g << endl;
    }

    {
        cout << "\n*** sc_uint ***" << endl;

        sc_uint<2> e = 0;
        sc_uint<2> f = 0;
        sc_uint<8> g = 0;

        cout << "sc_uint_concref = sc_signed" << endl;
        (e, f) = a;
        cout << e << endl;
        cout << f << endl;

        cout << "sc_uint_concref = sc_unsigned" << endl;
        (e, f) = b;
        cout << e << endl;
        cout << f << endl;

        cout << "sc_uint_concref = sc_bv_base" << endl;
        (e, f) = c;
        cout << e << endl;
        cout << f << endl;

        cout << "sc_uint_concref = sc_lv_base" << endl;
        (e, f) = d;
        cout << e << endl;
        cout << f << endl;

        cout << "sc_uint_subref = sc_signed" << endl;
        g( 7, 4 ) = a;
        cout << g << endl;

        cout << "sc_uint_subref = sc_unsigned" << endl;
        g( 7, 4 ) = b;
        cout << g << endl;

        cout << "sc_uint_subref = sc_bv_base" << endl;
        g( 7, 4 ) = c;
        cout << g << endl;

        cout << "sc_uint_subref = sc_lv_base" << endl;
        g( 7, 4 ) = d;
        cout << g << endl;
    }

    return 0;
}
