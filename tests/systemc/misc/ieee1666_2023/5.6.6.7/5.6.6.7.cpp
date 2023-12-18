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

  5.6.6.7.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(M) {
    SC_CTOR(M) {
        SC_THREAD(run);
        SC_THREAD(controller);
    }

    void run()
    {
        run_process = sc_core::sc_get_current_process_handle();
        for (;; ) {
	    try {
		sc_core::wait(1, sc_core::SC_NS);
	    } catch( const sc_core::sc_unwind_exception& ex ) {
		// perform clean up
		if ( ex.is_reset() ) {
		    std::cout << "Performing unwind reset" << std::endl;
		}
		else {
		    std::cout << "Not performing unwind reset" << std::endl;
		}
		throw ex;
	    }
	}
    }
    void controller()
    {
        sc_core::wait(10, sc_core::SC_NS);
	run_process.reset();
        sc_core::wait(10, sc_core::SC_NS);
	run_process.kill();
    }
    sc_core::sc_process_handle run_process;
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");
    sc_core::sc_start();

    std::cout << "program completed" << std::endl;
    return 0;
}
