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

  5.6.8.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

struct wait_on_exit {
    ~wait_on_exit() noexcept(false) {
        if ( !sc_core::sc_is_unwinding() ) {
	    std::cout << sc_core::sc_time_stamp() << " waiting in ~wait_on_exit()" << std::endl;
	    sc_core::wait(10.0, sc_core::SC_NS);
	}
	else {
	    std::cout << sc_core::sc_time_stamp() << " unwinding in ~wait_on_exit()" << std::endl;
	}
    }
};

SC_MODULE(M) 
{
    SC_CTOR(M) {
	SC_THREAD(controller);
	SC_THREAD(some_process);
    }

    void controller()
    {
        wait(25.0, sc_core::SC_NS);
	some_process_handle.kill();
    }

    void some_process()
    {
        some_process_handle = sc_core::sc_get_current_process_handle();
	while(true) {
	    try {
		wait_on_exit w;
	    }
	    catch( const sc_core::sc_unwind_exception& ) {
	        std::cout << sc_core::sc_time_stamp() << " caught exception" << std::endl;
		if ( !sc_core::sc_is_unwinding() ) {
		    std::cout << "unwinding" << std::endl;
		}
		throw;
	    }

	}
    }
    sc_core::sc_process_handle some_process_handle;
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_core::sc_start();

    std::cout << "program completed" << std::endl;
    return 0;
}
