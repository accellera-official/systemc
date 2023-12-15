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

  4.4.5.3.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/
#include <systemc>
SC_MODULE(Top)
{
    Top(const char* name) : sc_module( sc_core::sc_module_name(name) ) {}
};
int sc_main( int argc, char* argv[] ) {
  using namespace sc_core;

  Top top("top");         // Instantiate module hierarchy

  sc_start(100.0, SC_NS); // Run for exactly 100 ns
  sc_start();             // Run until no more activity

  if (sc_get_status() == SC_PAUSED) {
    SC_REPORT_INFO("", "sc_stop called to terminate a paused simulation");
    sc_stop();
  }

  return 0;
}
