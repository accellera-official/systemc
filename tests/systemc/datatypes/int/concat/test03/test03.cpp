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

  test03.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_[u]int concatenation with type bool, operator , ()

#include "systemc.h"

#define WRITE(a) \
    cout << a << endl

int
sc_main( int, char*[] )
{
    bool b;

    // )))  sc_uint  (((

    sc_uint<3> ui3 = 3;
    sc_uint<4> ui4;
    sc_uint<2> ui2;

    // sc_uint_base

    b = false;
    ui4 = ( ui3, b );
    WRITE( ui4 );
    ui4 = ( b, ui3 );
    WRITE( ui4 );

    b = true;
    ui4 = ( ui3, b );
    WRITE( ui4 );
    ui4 = ( b, ui3 );
    WRITE( ui4 );

    // sc_uint_bitref

    cout << endl;

    b = false;
    ui2 = ( ui3[0], b );
    WRITE( ui2 );
    ui2 = ( b, ui3[0] );
    WRITE( ui2 );

    b = true;
    ui2 = ( ui3[0], b );
    WRITE( ui2 );
    ui2 = ( b, ui3[0] );
    WRITE( ui2 );

    // sc_uint_subref

    cout << endl;

    b = false;
    ui2 = ( ui3.range( 0, 0 ), b );
    WRITE( ui2 );
    ui2 = ( b, ui3.range( 0, 0 ) );
    WRITE( ui2 );

    b = true;
    ui2 = ( ui3.range( 0, 0 ), b );
    WRITE( ui2 );
    ui2 = ( b, ui3.range( 0, 0 ) );
    WRITE( ui2 );

    // sc_uint_concat

    cout << endl;

    b = false;
    ui4 = ( ( ui3.range( 2, 1 ), ui3[0] ), b );
    WRITE( ui4 );
    ui4 = ( b, ( ui3.range( 2, 1 ), ui3[0] ) );
    WRITE( ui4 );

    b = true;
    ui4 = ( ( ui3.range( 2, 1 ), ui3[0] ), b );
    WRITE( ui4 );
    ui4 = ( b, ( ui3.range( 2, 1 ), ui3[0] ) );
    WRITE( ui4 );


    // )))  sc_int  (((

    sc_int<3> i3 = 3;
    sc_int<4> i4;
    sc_int<2> i2;

    // sc_int_base

    cout << endl;

    b = false;
    i4 = ( i3, b );
    WRITE( i4 );
    i4 = ( b, i3 );
    WRITE( i4 );

    b = true;
    i4 = ( i3, b );
    WRITE( i4 );
    i4 = ( b, i3 );
    WRITE( i4 );

    // sc_int_bitref

    cout << endl;

    b = false;
    i2 = ( i3[0], b );
    WRITE( i2 );
    i2 = ( b, i3[0] );
    WRITE( i2 );

    b = true;
    i2 = ( i3[0], b );
    WRITE( i2 );
    i2 = ( b, i3[0] );
    WRITE( i2 );

    // sc_int_subref

    cout << endl;

    b = false;
    i2 = ( i3.range( 0, 0 ), b );
    WRITE( i2 );
    i2 = ( b, i3.range( 0, 0 ) );
    WRITE( i2 );

    b = true;
    i2 = ( i3.range( 0, 0 ), b );
    WRITE( i2 );
    i2 = ( b, i3.range( 0, 0 ) );
    WRITE( i2 );

    // sc_int_concat

    cout << endl;

    b = false;
    i4 = ( ( i3.range( 2, 1 ), i3[0] ), b );
    WRITE( i4 );
    i4 = ( b, ( i3.range( 2, 1 ), i3[0] ) );
    WRITE( i4 );

    b = true;
    i4 = ( ( i3.range( 2, 1 ), i3[0] ), b );
    WRITE( i4 );
    i4 = ( b, ( i3.range( 2, 1 ), i3[0] ) );
    WRITE( i4 );

    return 0;
}
