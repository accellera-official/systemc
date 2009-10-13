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

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*
Hi,

Im using SystemC 2.0b2 with gcc 2.95.3 (Linux). If I create an sc_lv 
assignment with range methods on every side like

lv1.range(15,8) = lv2.range(7,0);

and the lv2 vector contains "z", I get the warning message

WARNING: (E2004) sc_bv cannot contain values X and Z :
 - ../../../../../src/systemc/datatypes/bit/sc_bv.h: 340

and the assignment is not done. Further investigation revealed that the 
vector seems to get mistakenly converted to a sc_bv in between. Removing the 
range method on one side or adding a cast

lv1.range(15,8) = (sc_lv<8>)lv2.range(7,0);

solves that problem.

Regards, S. Heithecker
*/

#include "systemc.h"

int
sc_main( int, char*[] )
{
    sc_lv<16> a( "01ZX0000111101ZX" );
    sc_lv<16> b;

    b.range( 15, 8 ) = a.range( 7, 0 );
    cout << a << endl;
    cout << b << endl;

    return 0;
}
