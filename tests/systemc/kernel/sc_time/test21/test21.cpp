/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test21.cpp -- Test sc_time constructor using enum and its constants

  Original Author: Martin Barnasconi, NXP Semiconductors

 *****************************************************************************/

/* 
  Test backwards compatibility for implementations using enum sc_time_unit 
  with predefined constants.
  Time units SC_AS, SC_ZS and SC_YS not tested, since these are not part of
  IEEE 1666-2011. 
*/

#include <systemc>


void check_time( const sc_core::sc_time& t1, const sc_core::sc_time& t2 )
{
    sc_assert( t1 == t2 );
}

int sc_main( int, char*[] )
{
    using namespace sc_core;

    sc_set_time_resolution( 1.0, SC_PS );

    check_time( sc_time(1.0, SC_FS), sc_time(1.0, sc_time_unit(0)) );
    check_time( sc_time(1.0, SC_PS), sc_time(1.0, sc_time_unit(1)) );
    check_time( sc_time(1.0, SC_NS), sc_time(1.0, sc_time_unit(2)) );
    check_time( sc_time(1.0, SC_US), sc_time(1.0, sc_time_unit(3)) );
    check_time( sc_time(1.0, SC_MS), sc_time(1.0, sc_time_unit(4)) );
    check_time( sc_time(1.0, SC_SEC), sc_time(1.0, sc_time_unit(5)) );

    return 0;
}
