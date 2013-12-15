/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
+  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  empty_msg_id.cpp -- Test for empty message IDs

  Original Author: Philipp A. Hartmann, OFFIS, 2013-09-17

 *****************************************************************************/

#include <systemc>

using std::cout;
using std::endl;
using sc_core::sc_report_handler;

static const char * null_msg  = NULL;
static const char * empty_msg = "";

int sc_main(int,char*[])
{
  SC_REPORT_INFO( empty_msg, "empty msg id" );
    cout << sc_report_handler::get_count(empty_msg) << endl;
    sc_assert( sc_report_handler::get_count(empty_msg) == 1 );

  SC_REPORT_INFO( 1, "empty msg id" ); // integer ID
    cout << sc_report_handler::get_count("") << endl;
    sc_assert( sc_report_handler::get_count(empty_msg) == 2 );


  SC_REPORT_INFO( null_msg,  "null msg id" );
    cout << sc_report_handler::get_count(null_msg) << endl;
    sc_assert( sc_report_handler::get_count(null_msg) == 1 );

  SC_REPORT_INFO( 0,  "another (integer) null msg id" );
    cout << sc_report_handler::get_count(null_msg) << endl;
    sc_assert( sc_report_handler::get_count(null_msg) == 2 );

  return 0;
}
