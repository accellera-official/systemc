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

  test02.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of assigning sc_lv_base to the integer types

#include "systemc.h"

void
test_smaller( const sc_lv<4>& lv_one, const sc_lv<4>& lv_two )
{
    cout << "*** smaller ***" << endl;
    {
        sc_int<8> a( lv_one );
        sc_int<8> b( lv_two );
        sc_int<8> c;
        sc_int<8> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_uint<8> a( lv_one );
        sc_uint<8> b( lv_two );
        sc_uint<8> c;
        sc_uint<8> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_bigint<8> a( lv_one );
        sc_bigint<8> b( lv_two );
        sc_bigint<8> c;
        sc_bigint<8> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_biguint<8> a( lv_one );
        sc_biguint<8> b( lv_two );
        sc_biguint<8> c;
        sc_biguint<8> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
}

void
test_equal( const sc_lv<4>& lv_one, const sc_lv<4>& lv_two )
{
    cout << "*** equal ***" << endl;
    {
        sc_int<4> a( lv_one );
        sc_int<4> b( lv_two );
        sc_int<4> c;
        sc_int<4> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_uint<4> a( lv_one );
        sc_uint<4> b( lv_two );
        sc_uint<4> c;
        sc_uint<4> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_bigint<4> a( lv_one );
        sc_bigint<4> b( lv_two );
        sc_bigint<4> c;
        sc_bigint<4> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_biguint<4> a( lv_one );
        sc_biguint<4> b( lv_two );
        sc_biguint<4> c;
        sc_biguint<4> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
}

void
test_larger( const sc_lv<4>& lv_one, const sc_lv<4>& lv_two )
{
    cout << "*** larger ***" << endl;
    {
        sc_int<2> a( lv_one );
        sc_int<2> b( lv_two );
        sc_int<2> c;
        sc_int<2> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_uint<2> a( lv_one );
        sc_uint<2> b( lv_two );
        sc_uint<2> c;
        sc_uint<2> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_bigint<2> a( lv_one );
        sc_bigint<2> b( lv_two );
        sc_bigint<2> c;
        sc_bigint<2> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
    cout << endl;
    {
        sc_biguint<2> a( lv_one );
        sc_biguint<2> b( lv_two );
        sc_biguint<2> c;
        sc_biguint<2> d;
        c = lv_one;
        d = lv_two;
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
    }
}

int
sc_main( int, char*[] )
{
    sc_lv<4> lv_one( "0101" );
    sc_lv<4> lv_two( "1010" );

    test_smaller( lv_one, lv_two );
    test_equal( lv_one, lv_two );
    test_larger( lv_one, lv_two );

    return 0;
}
