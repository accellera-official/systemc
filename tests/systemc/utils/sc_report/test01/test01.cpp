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

// test of reporting -- API

#include "systemc.h"

int
sc_main( int, char*[] )
{
    const int MY_ID = 9999;

    sc_report::register_id( MY_ID, "bogus message" );
    cout << sc_report::get_message( MY_ID ) << endl;
    cout << sc_report::is_suppressed( MY_ID ) << endl;
    sc_report::suppress_id( MY_ID, true );
    cout << sc_report::is_suppressed( MY_ID ) << endl;
    sc_report::suppress_id( MY_ID, false );
    cout << sc_report::is_suppressed( MY_ID ) << endl;

    SC_REPORT_INFO( MY_ID, "infos ..." );
    sc_report::suppress_infos( true );
    SC_REPORT_INFO( MY_ID, "infos suppressed" );
    sc_report::suppress_infos( false );
    SC_REPORT_INFO( MY_ID, "infos not suppressed" );
    sc_report::suppress_id( MY_ID, true );
    SC_REPORT_INFO( MY_ID, "suppressed" );
    sc_report::suppress_id( MY_ID, false );
    SC_REPORT_INFO( MY_ID, "not suppressed" );

    SC_REPORT_WARNING( MY_ID, "warnings ..." );
    sc_report::suppress_warnings( true );
    SC_REPORT_WARNING( MY_ID, "warnings suppressed" );
    sc_report::suppress_warnings( false );
    SC_REPORT_WARNING( MY_ID, "warnings not suppressed" );
    sc_report::suppress_id( MY_ID, true );
    SC_REPORT_WARNING( MY_ID, "suppressed" );
    sc_report::suppress_id( MY_ID, false );
    SC_REPORT_WARNING( MY_ID, "not suppressed" );

    sc_report::make_warnings_errors( true );
    try {
        SC_REPORT_WARNING( MY_ID, "do make warnings errors" );
    }
    catch( sc_exception x ) {
        cout << "\ncaught exception" << endl;
        cout << x.what() << endl;
    }
    sc_report::make_warnings_errors( false );
    try {
        SC_REPORT_WARNING( MY_ID, "do not make warnings errors" );
    }
    catch( sc_exception x ) {
        cout << "\ncaught exception" << endl;
        cout << x.what() << endl;
    }

    try {
        SC_REPORT_ERROR( MY_ID, "errors ..." );
    }
    catch( sc_exception x ) {
        cout << "\ncaught exception" << endl;
        cout << x.what() << endl;
    }
    sc_report::suppress_id( MY_ID, true );
    try {
        SC_REPORT_ERROR( MY_ID, "cannot be suppressed" );
    }
    catch( sc_exception x ) {
        cout << "\ncaught exception" << endl;
        cout << x.what() << endl;
    }

    return 0;
}
