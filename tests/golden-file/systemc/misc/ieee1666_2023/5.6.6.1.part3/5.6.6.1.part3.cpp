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

  5.6.6.1.part3.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>


struct M3 : sc_core::sc_module {
    M3(sc_core::sc_module_name name) {
        SC_THREAD(ticker);
        SC_THREAD(calling);
        SC_THREAD(target);
        t = sc_core::sc_get_current_process_handle();
    }

    sc_core::sc_process_handle t;
    sc_core::sc_event ev;
    int               count;

    void ticker()
    {
        for (;;) {
            wait( 10.0, sc_core::SC_NS );
            ev.notify();
        }
    }

    void calling()
    {
        using namespace sc_core;
        wait(15.0, SC_NS);
	sc_assert(count == 1);

        wait(10.0, SC_NS);
	sc_assert(count == 2);

        t.reset();
	sc_assert(count == 0);

        wait(10.0, SC_NS);
	sc_assert(count == 1);

	t.kill();
	sc_assert(t.terminated());

        sc_stop();
    }

    void target() {
	std::cout << "Target called reset at " << sc_core::sc_time_stamp() << std::endl;
	count = 0;
        for(;;) {
            wait(ev);
            std::cout << "Target awoke at " << sc_core::sc_time_stamp() << std::endl;
	    ++count;
        }
    }
};

        
int sc_main( int argc, char* argv[] ) { 

    M3 m3("m3");
    sc_core::sc_start();
    std::cout << "program completed" << std::endl;
    return 0;
}
