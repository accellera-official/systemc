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

  test05.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_uint's concat operators -- basic functionality, concat()

#include "systemc.h"

void
test_concat( const sc_uint<8>& a )
{
    sc_uint<8> b;
    sc_uint<4> c;
    sc_uint<4> d;

    cout << a << endl;

    cout << "*** sc_uint_base" << endl;

    concat( c, d ) = a;
    cout << c << endl;
    cout << d << endl;

    b = concat( d, c );
    cout << b << endl;

    cout << "---" << endl;

    concat( c, d[1] ) = a.range( 4, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 4, 0 ) = concat( c, d[1] );
    cout << b << endl;

    cout << "---" << endl;

    concat( c, d.range( 1, 0 ) ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = concat( c, d.range( 1, 0 ) );
    cout << b << endl;

    cout << "---" << endl;

    concat( c, concat( d[0], d[1] ) ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = concat( c, concat( d[0], d[1] ) );
    cout << b << endl;

    cout << "*** sc_uint_bitref" << endl;

    concat( d[1], c ) = a.range( 4, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 4, 0 ) = concat( d[1], c );
    cout << b << endl;

    cout << "---" << endl;

    concat( d[0], d[1] ) = a.range( 1,0 );
    cout << d << endl;

    b.range( 1, 0 ) = concat( d[1], d[0] );
    cout << b << endl;

    cout << "---" << endl;

    concat( d[0], c.range( 3, 0 ) ) = a.range( 4, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 4, 0 ) = concat( d[0], c.range( 3, 0 ) );
    cout << b << endl;

    cout << "---" << endl;

    concat( d[0], concat( concat( d[1], d[2] ), d[3] ) ) = a.range( 3, 0 );
    cout << d << endl;

    b.range( 3, 0 ) = concat( d[3], concat( concat( d[2], d[1] ), d[0] ) );
    cout << b << endl;

    cout << "*** sc_uint_subref" << endl;

    concat( c.range( 3, 0 ), d ) = a;
    cout << c << endl;
    cout << d << endl;

    b = concat( c.range( 3, 0 ), d );
    cout << b << endl;

    cout << "---" << endl;

    concat( c.range( 3, 0 ), d[1] ) = a.range( 4, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 4, 0 ) = concat( c.range( 3, 0 ), d[1] );
    cout << b << endl;

    cout << "---" << endl;

    concat( c.range( 3, 0 ), d.range( 3, 0 ) ) = a;
    cout << c << endl;
    cout << d << endl;

    b = concat( d.range( 3, 0 ), c.range( 3, 0 ) );
    cout << b << endl;

    cout << "---" << endl;

    concat( c.range( 3, 0 ), concat( d[0], d[1] ) ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = concat( c.range( 3, 0 ), concat( d[1], d[0] ) );
    cout << b << endl;

    cout << "*** sc_uint_concat" << endl;

    concat( concat( c[1], c[0] ), d ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = concat( concat( c[0], c[1] ), d );
    cout << b << endl;

    cout << "---" << endl;

    concat( concat( c[1], c[0] ), d[0] ) = a.range( 2, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 2, 0 ) = concat( concat( c[0], c[1] ), d[0] );
    cout << b << endl;

    cout << "---" << endl;

    concat( concat( c[1], c[0] ), d.range( 3, 0 ) ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = concat( concat( c[0], c[1] ), d.range( 3, 0 ) );
    cout << b << endl;

    cout << "---" << endl;

    concat( concat( c[0], c[1] ), concat( c[2], c[3] ) ) = a.range( 3, 0 );
    cout << c << endl;

    b.range( 3, 0 ) = concat( concat( c[3], c[2] ), concat( c[1], c[0] ) );
    cout << b << endl;
}

int
sc_main( int, char*[] )
{
    sc_uint<8> a( 33 );

    test_concat( a );

    return 0;
}
