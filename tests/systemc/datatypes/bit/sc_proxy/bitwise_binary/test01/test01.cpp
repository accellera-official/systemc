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

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of the bitwise binary operators of sc_proxy<X>

#include "systemc.h"

int
sc_main( int, char*[] )
{
    sc_bv<32> bv = 10;
    sc_bv<32> lv = 11;

    int I = 12;
    unsigned U = 12;

    sc_bv<32> bv1;
    sc_bv<32> bv2;
    sc_lv<32> lv1;
    sc_lv<32> lv2;

    // &

    bv1 = bv;
    bv2 = bv;
    bv1 &= I;
    bv2 = bv2 & I;
    assert( bv1 == bv2 );

    bv1 = bv;
    bv2 = bv;
    bv1 &= U;
    bv2 = bv2 & U;
    assert( bv1 == bv2 );

    lv1 = lv;
    lv2 = lv;
    lv1 &= I;
    lv2 = lv2 & I;
    assert( lv1 == lv2 );

    lv1 = lv;
    lv2 = lv;
    lv1 &= U;
    lv2 = lv2 & U;
    assert( lv1 == lv2 );

    // |

    bv1 = bv;
    bv2 = bv;
    bv1 |= I;
    bv2 = bv2 | I;
    assert( bv1 == bv2 );

    bv1 = bv;
    bv2 = bv;
    bv1 |= U;
    bv2 = bv2 | U;
    assert( bv1 == bv2 );

    lv1 = lv;
    lv2 = lv;
    lv1 |= I;
    lv2 = lv2 | I;
    assert( lv1 == lv2 );

    lv1 = lv;
    lv2 = lv;
    lv1 |= U;
    lv2 = lv2 | U;
    assert( lv1 == lv2 );

    // ^

    bv1 = bv;
    bv2 = bv;
    bv1 ^= I;
    bv2 = bv2 ^ I;
    assert( bv1 == bv2 );

    bv1 = bv;
    bv2 = bv;
    bv1 ^= U;
    bv2 = bv2 ^ U;
    assert( bv1 == bv2 );

    lv1 = lv;
    lv2 = lv;
    lv1 ^= I;
    lv2 = lv2 ^ I;
    assert( lv1 == lv2 );

    lv1 = lv;
    lv2 = lv;
    lv1 ^= U;
    lv2 = lv2 ^ U;
    assert( lv1 == lv2 );

    cout << "OK" << endl;

    return 0;
}
