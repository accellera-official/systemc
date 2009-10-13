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

// test of sc_int's concat operators -- basic functionality, operator , ()

#include "systemc.h"

void
test_concat( const sc_int<8>& a )
{
    sc_int<8> b;
    sc_int<4> c;
    sc_int<4> d;

    cout << a << endl;

    cout << "*** sc_int_base" << endl;

    ( c, d ) = a;
    cout << c << endl;
    cout << d << endl;

    b = ( d, c );
    cout << b << endl;

    cout << "---" << endl;

    ( c, d[1] ) = a.range( 4, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 4, 0 ) = ( c, d[1] );
    cout << b << endl;

    cout << "---" << endl;

    ( c, d.range( 1, 0 ) ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = ( c, d.range( 1, 0 ) );
    cout << b << endl;

    cout << "---" << endl;

    ( c, ( d[0], d[1] ) ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = ( c, ( d[0], d[1] ) );
    cout << b << endl;

    cout << "*** sc_int_bitref" << endl;

    ( d[1], c ) = a.range( 4, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 4, 0 ) = ( d[1], c );
    cout << b << endl;

    cout << "---" << endl;

    ( d[0], d[1] ) = a.range( 1,0 );
    cout << d << endl;

    b.range( 1, 0 ) = ( d[1], d[0] );
    cout << b << endl;

    cout << "---" << endl;

    ( d[0], c.range( 3, 0 ) ) = a.range( 4, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 4, 0 ) = ( d[0], c.range( 3, 0 ) );
    cout << b << endl;

    cout << "---" << endl;

    ( d[0], ( d[1], d[2], d[3] ) ) = a.range( 3, 0 );
    cout << d << endl;

    b.range( 3, 0 ) = ( d[3], ( d[2], d[1], d[0] ) );
    cout << b << endl;

    cout << "*** sc_int_subref" << endl;

    ( c.range( 3, 0 ), d ) = a;
    cout << c << endl;
    cout << d << endl;

    b = ( c.range( 3, 0 ), d );
    cout << b << endl;

    cout << "---" << endl;

    ( c.range( 3, 0 ), d[1] ) = a.range( 4, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 4, 0 ) = ( c.range( 3, 0 ), d[1] );
    cout << b << endl;

    cout << "---" << endl;

    ( c.range( 3, 0 ), d.range( 3, 0 ) ) = a;
    cout << c << endl;
    cout << d << endl;

    b = ( d.range( 3, 0 ), c.range( 3, 0 ) );
    cout << b << endl;

    cout << "---" << endl;

    ( c.range( 3, 0 ), ( d[0], d[1] ) ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = ( c.range( 3, 0 ), ( d[1], d[0] ) );
    cout << b << endl;

    cout << "*** sc_int_concat" << endl;

    ( ( c[1], c[0] ), d ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = ( ( c[0], c[1] ), d );
    cout << b << endl;

    cout << "---" << endl;

    ( ( c[1], c[0] ), d[0] ) = a.range( 2, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 2, 0 ) = ( ( c[0], c[1] ), d[0] );
    cout << b << endl;

    cout << "---" << endl;

    ( ( c[1], c[0] ), d.range( 3, 0 ) ) = a.range( 5, 0 );
    cout << c << endl;
    cout << d << endl;

    b.range( 5, 0 ) = ( ( c[0], c[1] ), d.range( 3, 0 ) );
    cout << b << endl;

    cout << "---" << endl;

    ( ( c[0], c[1] ), ( c[2], c[3] ) ) = a.range( 3, 0 );
    cout << c << endl;

    b.range( 3, 0 ) = ( ( c[3], c[2] ), ( c[1], c[0] ) );
    cout << b << endl;
}

int
sc_main( int, char*[] )
{
    sc_int<8> a( 33 );

    test_concat( a );

    return 0;
}
