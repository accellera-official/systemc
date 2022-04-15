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

  test20.cpp -- Test sc_time constructor and sc_time::from_string
                accepting string_view argument

  Original Author: Original Author: Martin Barnasconi, NXP Semiconductors

 *****************************************************************************/


#include <systemc>

void local_report_handler( const sc_core::sc_report& rep,
                           const sc_core::sc_actions& actions)
{
  if (rep.get_severity() == sc_core::SC_ERROR) {
    std::cout << "expected error occurred" << std::endl;
  }
}

void check_time( const sc_core::sc_time& t1, const sc_core::sc_time& t2 )
{
    sc_assert( t1 == t2 );
}

int sc_main( int, char*[] )
{
    using namespace sc_core;
    using namespace std;

    sc_set_time_resolution( 1.0, SC_PS );
    sc_report_handler::set_handler(local_report_handler);

    // test constructors

    check_time( sc_time(SC_ZERO_TIME), sc_time("0") ); // nu unit assumes seconds
    check_time( sc_time(1.0, SC_MS), sc_time("1e-3") );
    check_time( sc_time(1.0, SC_MS), sc_time("1ms") );
    check_time( sc_time(1.0, SC_MS), sc_time("1 ms") );
    check_time( sc_time(1.0, SC_MS), sc_time("1.0 ms") );
    check_time( sc_time(1.0, SC_MS), sc_time("1.00000 ms") );
    check_time( sc_time(1.0, SC_MS), sc_time(" 1 ms") );
    check_time( sc_time(1.0, SC_MS), sc_time(" 1 ms ") );
    check_time( sc_time(1.0, SC_MS), sc_time(" 1 m s ") );
    check_time( sc_time(1.0, SC_MS), sc_time("1e-3 s") );
    check_time( sc_time(1.0, SC_MS), sc_time("1e-03 s") );
    check_time( sc_time(1.0, SC_MS), sc_time("1e-003 s") );
    check_time( sc_time(1.0, SC_MS), sc_time("1.0e-3 s") );
    check_time( sc_time(1.0, SC_MS), sc_time("1.0e+3 us") ); // strange scaling but valid

    check_time( sc_time(1.0, SC_MS), sc_time("1E-3") ); 
    check_time( sc_time(1.0, SC_MS), sc_time("1MS") );
    check_time( sc_time(1.0, SC_MS), sc_time("1 MS") );
    check_time( sc_time(1.0, SC_MS), sc_time("1.0 MS") );
    check_time( sc_time(1.0, SC_MS), sc_time("1.00000 MS") );
    check_time( sc_time(1.0, SC_MS), sc_time(" 1 MS") );
    check_time( sc_time(1.0, SC_MS), sc_time(" 1 MS ") );
    check_time( sc_time(1.0, SC_MS), sc_time(" 1 M S ") );
    check_time( sc_time(1.0, SC_MS), sc_time("1E-3 S") );
    check_time( sc_time(1.0, SC_MS), sc_time("1E-003 S") );
    check_time( sc_time(1.0, SC_MS), sc_time("1.0E-3 S") );
    check_time( sc_time(1.0, SC_MS), sc_time("1.0E+3 US") ); 

    check_time( sc_time(1.0, SC_MS), sc_time("1mS") ); // mixed upper and lower case, but valid
    check_time( sc_time(1.0, SC_MS), sc_time("1Ms") ); // mixed upper and lower case, but valid

    const char* str = "1ms";
    check_time( sc_time(1.0, SC_MS), sc_time(str) ); 

#if SC_CPLUSPLUS >= 201703L
    string_view sv{"1ms"};
    check_time( sc_time(1.0, SC_MS), sc_time(sv) ); 
#endif

    // To support older (<C++17) compilers, a constructor accepting std::string
    // has been made available, not this is outside the scope of the SystemC standard

    check_time( sc_time(1.0, SC_MS), sc_time(string("1ms")) ); 

    // test static member function from_string

    sc_time t;
    t = sc_time::from_string("1 ms");
    check_time( sc_time(1.0, SC_MS), t ); 

    // Invalid syntax, report error

    t = sc_time("3E US");  // incomplete exponent notation
    t = sc_time("ps");     // value missing
    t = sc_time("1 ms!");  // invalid character found
    t = sc_time("+1 ms");  // invalid character found
    t = sc_time("1 m_s");  // invalid character found
    t = sc_time("1 sec");  // invalid SI notation (should be s)
    t = sc_time("1 ms 2"); // invalid character found

    return 0;
}
