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

  verbosity.cpp -- Validate behavior or SC_REPORT_INFO_VERB

  Original Author: David Black, Doulos, Inc., 2023-05-26

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test SC_REPORT_INFO_VERB -- API

#include <systemc>
#include <string>
#include <vector>
using namespace sc_core;
using namespace std::literals;
using s = std::string;

const char* msg_type = "/Accellera/SystemC/Verbosity-test";

//------------------------------------------------------------------------------
// Helper functions
std::string offset( int level, int base )
{
  auto result = std::string{};
  int difference = level - base;
  if ( difference != 0 ) {
    result += "+";
    result += std::to_string( difference );
  }
  return result;
}

std::string v2s( int level )
{
  auto result = std::string{};
  if      ( level >= SC_DEBUG  ) { result = s{"SC_DEBUG"}  + offset( level, SC_DEBUG  ); }
  else if ( level >= SC_FULL   ) { result = s{"SC_FULL"}   + offset( level, SC_FULL   ); }
  else if ( level >= SC_HIGH   ) { result = s{"SC_HIGH"}   + offset( level, SC_HIGH   ); }
  else if ( level >= SC_MEDIUM ) { result = s{"SC_MEDIUM"} + offset( level, SC_MEDIUM ); }
  else if ( level >= SC_LOW    ) { result = s{"SC_LOW"}    + offset( level, SC_LOW    ); }
  else                           { result = s{"SC_NONE"}   + offset( level, SC_NONE   ); }
  return result;
}

//------------------------------------------------------------------------------
// A function to compute and return the text for this message.  We add the
// message verbosity level to determine under what conditions this function is
// invoked. A real world example might compute some complex hash.
std::string compute_str(const std::string& message, sc_verbosity message_level)
{
  auto result = message;

  // Optionally do some work to illustrate performance impact
#ifdef REALWORLD_IMPACT
  {
  }
#endif

  auto reporting_level = sc_report_handler::get_verbosity_level();
  if ( reporting_level < message_level ) {
    // This function was called at a time when the resultant contents would not
    // have been revealed; therefore, we have done unnecessary work and
    // impacted the performance of the simulation.
    SC_REPORT_ERROR( msg_type, 
                     ( std::string{ "Verbosity failed: message_level="} + v2s( message_level )
                     + std::string{ " reporting_level=" } + v2s( reporting_level )
                     ).c_str()
                   );
    result += " << You should never see this text!";
  }
  return result;
}

//------------------------------------------------------------------------------
[[maybe_unused]] int
sc_main( int, char*[] )
{
  // Allow errors to be simply displayed and counted
  sc_report_handler::set_actions( SC_ERROR, SC_DISPLAY );

  const auto levels = std::vector<sc_verbosity>{ SC_NONE, SC_LOW, SC_MEDIUM, SC_HIGH, SC_FULL, SC_DEBUG };
  const auto line = s{"\n--------------------------------------------------------------------------------\n"};

  for ( auto report_level: levels ) {
    sc_report_handler::set_verbosity_level( report_level );
    // The following will always be displayed
    SC_REPORT_INFO_VERB( msg_type, (line + s{"Reporting level is "} + v2s(report_level)).c_str(), SC_NONE );

    // SC_REPORT_INFO(msg_type,message) is supposed to be equivalent to SC_REPORT_INFO_VERB(msg_type,message,SC_MEDIUM)
    SC_REPORT_INFO( msg_type, compute_str( "SC_REPORT_INFO", SC_MEDIUM ).c_str());
    for ( auto message_level: levels ) {
      SC_REPORT_INFO_VERB( msg_type
                         , compute_str( s{"SC_REPORT_INFO_VERB message at level "}
                                        + v2s( message_level )
                                      , message_level
                                      ).c_str()
                         , message_level
      );
    }
    SC_REPORT_WARNING( msg_type, "Verbosity doesn't affect warning, error or fatal messages" );
  }

  auto error_count = sc_report_handler::get_count( SC_ERROR );
  SC_REPORT_INFO_VERB( msg_type, (std::to_string(error_count) + " errors").c_str(), SC_NONE );
  if ( error_count > 0 ) SC_REPORT_INFO_VERB( msg_type, "Test FAILED", SC_NONE );
  else                   SC_REPORT_INFO_VERB( msg_type, "Test PASSED", SC_NONE );

  return error_count > 0 ? 1 : 0;
}
