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

  Original Author: Andy Goodrich, Forte Design Systems, 7 Apr 2005

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of l-value concatenation

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

    cout << "( bv2a, bv2b ) = \"0110\"" << endl;
    ( bv2a, bv2b ) = "0110";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    cout << "bv4a = ( bv2a, bv2b )" << endl;
    bv4a = ( bv2a, bv2b );
    cout << "bv4a = " << bv4a << endl;

    cout << "bv4a = ( bv2c, bv2d )" << endl;
    bv4a = ( bv2c, bv2d );
    cout << "bv4a = " << bv4a << endl;

    cout << "( bv4a.range( 2, 1 ), bv2a ) = \"0110\"" << endl;
    ( bv4a.range( 2, 1 ), bv2a ) = "0110";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;

#if 0
    cout << "( bv2a, bv4a.range( 1, 2 ) ) = \"0101\"" << endl;
    ( bv2a, bv4a.range( 1, 2 ) ) = "0101";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
#endif // 0

    cout << "( bv4a.range( 2, 1 ), bv4b.range( 2, 1 ) ) = \"1100\"" << endl;
    ( bv4a.range( 2, 1 ), bv4b.range( 2, 1 ) ) = "1100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv4b = " << bv4b << endl;

    cout << "( ( bv2a, bv2b ), bv4a ) = \"00110011\"" << endl;
    ( ( bv2a, bv2b ), bv4a ) = "00110011";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;

    cout << "( bv4a, ( bv2a, bv2b ) ) = \"11001100\"" << endl;
    ( bv4a, ( bv2a, bv2b ) ) = "11001100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    cout << "( ( bv2a, bv2b ), ( bv4a, bv4b ) ) = \"011000001111\"" << endl;
    ( ( bv2a, bv2b ), ( bv4a, bv4b ) ) = "011000001111";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;
    cout << "bv4b = " << bv4b << endl;

    cout << "( ( bv2a, bv2b ), bv4a.range( 2, 1 ) ) = \"001111\"" << endl;
    ( ( bv2a, bv2b ), bv4a.range( 2, 1 ) ) = "001111";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;

    cout << "( bv4a.range( 2, 1 ), ( bv2a, bv2b ) ) = \"001100\"" << endl;
    ( bv4a.range( 2, 1 ), ( bv2a, bv2b ) ) = "001100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    return 0;
}
