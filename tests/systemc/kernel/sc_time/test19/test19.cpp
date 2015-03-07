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

  test19.cpp -- Test sc_time string/tuple conversions

  Original Author: Philipp A. Hartmann, OFFIS

 *****************************************************************************/


#include "systemc.h"

void check_time( const sc_time& t, sc_time_unit tu, const std::string & str )
{
    sc_time_tuple tp = t;

    std::cout << t.to_string() << ", value=" << t.value() << std::endl;
    std::cout << "  ";
    if( tp.has_value() )
        std::cout << "t.value=" << tp.value();
    else
        std::cout << "t.double=" << tp.to_double();
    std::cout << ", t.unit="  << tp.unit_symbol()
              << std::endl;

    sc_assert( tp.has_value() );
    sc_assert( t.to_string() == str );
    sc_assert( tp.to_string() == t.to_string() );
    sc_assert( tp.unit() == tu );

    sc_time u = sc_time::from_string( str.c_str() );
    sc_assert( t == u );
    sc_assert( u == tp );
    sc_assert( tp.unit() == sc_time_tuple(u).unit() );

    u = sc_time( tp.to_double(), tp.unit_symbol() );
    sc_assert( t == u );
    sc_assert( u == tp );
    sc_assert( tp.unit() == sc_time_tuple(u).unit() );
}

int sc_main( int, char*[] )
{
    sc_report_handler::set_actions( SC_ID_SET_TIME_RESOLUTION_, SC_DO_NOTHING );
    sc_report_handler::set_actions( SC_ID_TIME_CONVERSION_FAILED_, SC_DISPLAY );

    unsigned resolutions[] = { 100, 10, 1 };
    sc_time_unit resunit   = SC_FS;
    unsigned* res = resolutions;

    do
    {
        std::cout << "\nResolution = " << sc_get_time_resolution() << std::endl;

        check_time( sc_time(  10,   SC_NS),  SC_NS,  "10 ns" );
        check_time( sc_time( 100,   SC_NS),  SC_NS, "100 ns" );
        check_time( sc_time(1000,   SC_NS),  SC_US,   "1 us" );
        check_time( sc_time(   0.1, SC_US),  SC_NS, "100 ns" );
        check_time( sc_time(   1,   SC_US),  SC_US,   "1 us" );
        check_time( sc_time(  10,   SC_US),  SC_US,  "10 us" );
        check_time( sc_time( 100,   SC_US),  SC_US, "100 us" );
        check_time( sc_time(1000,   SC_US),  SC_MS,   "1 ms" );
        check_time( sc_time( 100,  SC_SEC), SC_SEC, "100 s" );

        sc_set_time_resolution( *res, resunit );
    }
    while( res++ < resolutions + (sizeof(resolutions)/sizeof*resolutions) );

    cout << "\nProgram completed" << endl;
    return 0;
}
