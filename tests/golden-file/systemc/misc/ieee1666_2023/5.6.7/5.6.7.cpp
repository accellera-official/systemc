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

  5.6.7.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>



SC_MODULE(Mod) {
    sc_core::sc_in<bool> in;
    SC_CTOR(Mod) {
        SC_METHOD(run);
	sensitive << in;
	sc_core::sc_process_handle h1 = sc_core::sc_get_current_process_handle();
    }

    void run()
    {
        using namespace sc_core;

	sc_process_handle h2 = sc_core::sc_get_current_process_handle();

	if ( h2.proc_kind() == SC_METHOD_PROC_ ) {
	    std::cout << "running method process" << std::endl;
	}
	sc_object* parent = h2.get_parent_object();

	if ( parent ) {
	    sc_process_handle handle = sc_process_handle(parent);
	    if ( handle.valid() ) {
	        std::cout << "valid handle" << std::endl;
	    }
	    else {
	        std::cout << "invalid handle" << std::endl;
	    }
	}
    }
};
	        

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_clock clock;
    Mod               mod("mod");
    mod.in(clock);

    sc_core::sc_start(1, sc_core::SC_NS );

    std::cout << "program completed" << std::endl;
    return 0;
}
