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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of the reduce methods in the sc_[u]int datatypes -- method notation

#include "systemc.h"

int
sc_main( int, char*[] )
{
    // 1) check the existence of the reduce methods

    sc_int<42> a = 42;

    // sc_int_base
    cout << endl;
    cout << a.and_reduce() << endl;
    cout << a.or_reduce() << endl;
    cout << a.xor_reduce() << endl;
    cout << a.nand_reduce() << endl;
    cout << a.nor_reduce() << endl;
    cout << a.xnor_reduce() << endl;

    // sc_int_subref
    cout << endl;
    cout << a( 7, 0 ).and_reduce() << endl;
    cout << a( 7, 0 ).or_reduce() << endl;
    cout << a( 7, 0 ).xor_reduce() << endl;
    cout << a( 7, 0 ).nand_reduce() << endl;
    cout << a( 7, 0 ).nor_reduce() << endl;
    cout << a( 7, 0 ).xnor_reduce() << endl;

    // sc_int_concref<T1,T2>
    cout << endl;
    cout << ( a( 7, 0 ), a( 15, 8 ) ).and_reduce() << endl;
    cout << ( a( 7, 0 ), a( 15, 8 ) ).or_reduce() << endl;
    cout << ( a( 7, 0 ), a( 15, 8 ) ).xor_reduce() << endl;
    cout << ( a( 7, 0 ), a( 15, 8 ) ).nand_reduce() << endl;
    cout << ( a( 7, 0 ), a( 15, 8 ) ).nor_reduce() << endl;
    cout << ( a( 7, 0 ), a( 15, 8 ) ).xnor_reduce() << endl;

    sc_uint<42> b = 42;

    // sc_uint_base
    cout << endl;
    cout << b.and_reduce() << endl;
    cout << b.or_reduce() << endl;
    cout << b.xor_reduce() << endl;
    cout << b.nand_reduce() << endl;
    cout << b.nor_reduce() << endl;
    cout << b.xnor_reduce() << endl;

    // sc_uint_subref
    cout << endl;
    cout << b( 7, 0 ).and_reduce() << endl;
    cout << b( 7, 0 ).or_reduce() << endl;
    cout << b( 7, 0 ).xor_reduce() << endl;
    cout << b( 7, 0 ).nand_reduce() << endl;
    cout << b( 7, 0 ).nor_reduce() << endl;
    cout << b( 7, 0 ).xnor_reduce() << endl;

    // sc_uint_concref<T1,T2>
    cout << endl;
    cout << ( b( 7, 0 ), b( 15, 8 ) ).and_reduce() << endl;
    cout << ( b( 7, 0 ), b( 15, 8 ) ).or_reduce() << endl;
    cout << ( b( 7, 0 ), b( 15, 8 ) ).xor_reduce() << endl;
    cout << ( b( 7, 0 ), b( 15, 8 ) ).nand_reduce() << endl;
    cout << ( b( 7, 0 ), b( 15, 8 ) ).nor_reduce() << endl;
    cout << ( b( 7, 0 ), b( 15, 8 ) ).xnor_reduce() << endl;

    // 2) check the functionality of the reduce methods

    sc_int<2> c2 = -1;
    cout << endl;
    cout << c2.and_reduce() << endl;
    cout << c2.xor_reduce() << endl;

    sc_int<3> c3 = -1;
    cout << endl;
    cout << c3.and_reduce() << endl;
    cout << c3.xor_reduce() << endl;

    sc_uint<2> d2 = sc_dt::uint_type( -1 );
    cout << endl;
    cout << d2.and_reduce() << endl;
    cout << d2.xor_reduce() << endl;

    sc_uint<3> d3 = sc_dt::uint_type( -1 );
    cout << endl;
    cout << d3.and_reduce() << endl;
    cout << d3.xor_reduce() << endl;

    sc_int<6> e;
    sc_uint<6> f;
    cout << endl;
    for( int i = 0; i >= -10; -- i ) {
        e = i;
        f = i;
        cout << e.xor_reduce() << endl;
        cout << f.xor_reduce() << endl;
    }
    
    return 0;
}
