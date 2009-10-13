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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of r-value and l-value concatenation -- use operator , ()

#include "systemc.h"

int
sc_main( int, char*[] )
{
    sc_bv<2> bv2a;
    sc_bv<2> bv2b;
    const sc_bv<2> bv2c( "10" );
    const sc_bv<2> bv2d( "01" );
    sc_bv<4> bv4a;
    sc_bv<4> bv4b( "1111" );

    ( bv2a, bv2b ) = "0110";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    bv4a = ( bv2a, bv2b );
    cout << "bv4a = " << bv4a << endl;

    bv4a = ( bv2c, bv2d );
    cout << "bv4a = " << bv4a << endl;

    ( bv4a.range( 2, 1 ), bv2a ) = "0110";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;

    ( bv2a, bv4a.range( 1, 2 ) ) = "0101";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;

    ( bv4a.range( 2, 1 ), bv4b.range( 2, 1 ) ) = "1100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv4b = " << bv4b << endl;

    ( ( bv2a, bv2b ), bv4a ) = "00110011";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;

    ( bv4a, ( bv2a, bv2b ) ) = "11001100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    ( ( bv2a, bv2b ), ( bv4a, bv4b ) ) = "011000001111";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;
    cout << "bv4b = " << bv4b << endl;

    ( ( bv2a, bv2b ), bv4a.range( 2, 1 ) ) = "001111";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;

    ( bv4a.range( 2, 1 ), ( bv2a, bv2b ) ) = "001100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    const char* s = "1010";
    sc_logic l = SC_LOGIC_1;
    bool b = true;

    cout << ( bv4b, s ) << endl;
    cout << ( s, bv4b ) << endl;
    cout << ( bv4a, l ) << endl;
    cout << ( l, bv4a ) << endl;
    cout << ( bv4a, b ) << endl;
    cout << ( b, bv4a ) << endl;

    return 0;
}
