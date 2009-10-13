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

// test of reporting -- implementation

#include "systemc.h"

int
sc_main( int, char*[] )
{
    const int MY_ID = 9999;

    try {
        sc_report::register_id( -1, "first try" );
    }
    catch( sc_exception x ) {
        cout << "\ncaught exception" << endl;
        cout << x.what() << endl;
    }

    try {
        sc_report::register_id( MY_ID, 0 );
    }
    catch( sc_exception x ) {
        cout << "\ncaught exception" << endl;
        cout << x.what() << endl;
    }

    try {
        sc_report::register_id( MY_ID, "bogus message" );
        sc_report::register_id( MY_ID, "another bogus message" );
    }
    catch( sc_exception x ) {
        cout << "\ncaught exception" << endl;
        cout << x.what() << endl;
    }

    cout << endl;
    cout << sc_report::get_message( 1234 ) << endl;

    sc_report::suppress_id( 4321, true );
    cout << endl;
    cout << sc_report::is_suppressed( 4321 ) << endl;

    return 0;
}
