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

  8.3.15.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc.h>

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_report_handler::set_log_file_name("foo");
    std::cout << "Name is'" << sc_core::sc_report_handler::get_log_file_name() << "'" << std::endl;

    if ( true == sc_core::sc_report_handler::set_log_file_name("bar") ) {
        std::cout << "ERROR: successive calls to set_log_file_name() with non-null strings ";
	std::cout << "should return false" << std::endl;
    }
    std::cout << "Name is'" << sc_core::sc_report_handler::get_log_file_name() << "'" << std::endl;

    if ( true == sc_core::sc_report_handler::set_log_file_name(0) ) {
        std::cout << "calling set_log_file_name() NULL should return false" << std::endl;
    }

    if ( sc_core::sc_report_handler::get_log_file_name() != NULL ) {
        std::cout << "ERROR: get_log_file_name() should have returned NULL" << std::endl;
    }

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
