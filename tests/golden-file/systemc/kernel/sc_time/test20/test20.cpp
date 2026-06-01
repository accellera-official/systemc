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
#include <sstream>

static bool g_error; // error flag

void local_report_handler( const sc_core::sc_report& rep,
                           const sc_core::sc_actions& actions)
{
  if (rep.get_severity() == sc_core::SC_ERROR) {
    std::cout << "expected error occurred" << std::endl;
    g_error = true;
  }
}

void check_time( const std::string& desc,  const sc_core::sc_time& t1, const sc_core::sc_time& t2 )
{
    std::cout << desc << ": ";
    if (t1 == t2) std::cout << "valid construct." << std::endl; 
    else std::cout << "invalid construct." << std::endl;
    sc_assert( t1 == t2 );
}

void check_error( const std::string& desc, const std::string& arg )
{
    g_error = false;
    std::cout << desc << ": ";
    sc_core::sc_time t = sc_core::sc_time(arg); // local_report_handler will be called in case of error
    if (!g_error) std::cout << "expected error did not occurr!" << std::endl;
}

int sc_main( int, char*[] )
{
    using namespace sc_core;
    using namespace std;

    sc_set_time_resolution( 1.0, SC_PS );
    sc_report_handler::set_handler(local_report_handler);

    // test constructors 
    check_time( "constructor SC_ZERO_TIME", sc_time(SC_ZERO_TIME), sc_time("0") ); // no unit, assumes seconds
    check_time( "constructor 1e-3", sc_time(1.0, SC_MS), sc_time("1e-3") );
    check_time( "constructor 1ms", sc_time(1.0, SC_MS), sc_time("1ms") );
    check_time( "constructor 1 ms", sc_time(1.0, SC_MS), sc_time("1 ms") );
    check_time( "constructor 1.0 ms", sc_time(1.0, SC_MS), sc_time("1.0 ms") );
    check_time( "constructor 1.00000 ms", sc_time(1.0, SC_MS), sc_time("1.00000 ms") );
    check_time( "constructor 1e-3 s", sc_time(1.0, SC_MS), sc_time("1e-3 s") );
    check_time( "constructor 1E-3 s", sc_time(1.0, SC_MS), sc_time("1E-3 s") ); // exponent in upper case is supported
    check_time( "constructor 1e-03 s", sc_time(1.0, SC_MS), sc_time("1e-03 s") );
    check_time( "constructor 1e-003 s", sc_time(1.0, SC_MS), sc_time("1e-003 s") );
    check_time( "constructor 1.0e-3 s", sc_time(1.0, SC_MS), sc_time("1.0e-3 s") );
    check_time( "constructor 1.0e+3 us",  sc_time(1.0, SC_MS), sc_time("1.0e+3 us") ); // strange scaling but valid

#if SC_CPLUSPLUS >= 201703L
    string_view sv{"1ms"};
    check_time( "constructor 1ms as stringview", sc_time(1.0, SC_MS), sc_time(sv) ); 
#endif
    // To support older (<C++17) compilers, a constructor accepting std::string or old C-string
    // has been made available, note: this is outside the scope of the SystemC standard
    check_time( "constructor 1ms as string", sc_time(1.0, SC_MS), sc_time(string("1ms")) ); 

    char cstr[] = "1ms";
    check_time( "constructor 1ms as const char*", sc_time(1.0, SC_MS), sc_time(cstr) ); 

    // use sc_time output stream or string as input for new sc_time object
    // Note that IEEE 1666 defines that the format of the string is implementation-defined, however, for the 
    // reference implementation we expect that approach should work

    std::ostringstream os;
    sc_time t = sc_time(1.0, SC_MS);
    os << t; // print sc_time
    check_time( "constructor uses sc_time output string from stream", sc_time(1.0, SC_MS), sc_time(os.str()) );
    check_time( "constructor uses sc_time output string using to_string", sc_time(1.0, SC_MS), sc_time(t.to_string()) );

    // test static member function from_string
    check_time( "member function from_string 1ms", sc_time(1.0, SC_MS), sc_time::from_string("1 ms") ); 

    // Invalid syntax, report errors
    check_error( "constructor with imcomplete exponent", "3E US" );
    check_error( "constructor with missing value", "ps" );
    check_error( "constructor with invald character '!'", "1 ms!" );
    check_error( "constructor with invald character '+'", "+1 ms" );
    check_error( "constructor with invald character '_'", "1 m_s" );
    check_error( "constructor with invalid SI unit notation (sec)", "1 sec" );
    check_error( "constructor with invalid character '2' at end of string", "1 ms2" );
    check_error( "constructor with invalid space at start of string", " 1 ms" );
    check_error( "constructor with invalid space at end of string", "1 ms " );
    check_error( "constructor with invalid space in middle of string", "1  ms" );
    check_error( "constructor with invalid spaces at multiple places in string", "1 m s" );
    check_error( "constructor with invalid upper case character in string", "1 Ms" );
    check_error( "constructor with invalid SI unit in upper case", "1 MS" );

    return 0;
}
