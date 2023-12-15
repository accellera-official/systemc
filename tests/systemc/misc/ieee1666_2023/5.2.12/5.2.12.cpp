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

  5.2.12.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(M) {
    sc_core::sc_in<bool> SC_NAMED(clock);
    sc_core::sc_in<bool> SC_NAMED(reset);
    sc_core::sc_in<bool> SC_NAMED(async_reset);

    SC_CTOR(M) {
        SC_CTHREAD(CT1, clock.pos());
	reset_signal_is(reset, true);
	SC_CTHREAD(CT2, clock.pos());
	async_reset_signal_is(async_reset, true);
    }
    void CT1() {
        if (reset.read()) {
	    std::cout << sc_core::sc_time_stamp() << " " << "CT1: reset true" << std::endl;
	    while(true) {
	        wait(true); // Wait for 1 clock cycle
		std::cout << sc_core::sc_time_stamp() << " " << "CT1: looping" << std::endl;
	    }
	}
	else {
	    std::cout << sc_core::sc_time_stamp() << " " << "CT1: reset false" << std::endl;
	}
    }

    void CT2() {
	std::cout << sc_core::sc_time_stamp() << " " << "CT2: reset true" << std::endl;
	while(true) {
	        try {
		while (true) {
		    std::cout << sc_core::sc_time_stamp() << " " << "CT2: looping" << std::endl;
		    wait(); // Wait for 1 clock cycle
		}
	    } catch(sc_core::sc_unwind_exception& ex) {
		std::cout << sc_core::sc_time_stamp() << " " << "CT2: unwind " << std::endl;
	        throw ex;
	    }
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_signal<bool> async_reset;
    sc_core::sc_clock        clock;
    sc_core::sc_signal<bool> reset;
    M m("m");
    m.clock(clock);
    m.async_reset(async_reset);
    m.reset(reset);

    sc_core::sc_start(sc_core::SC_ZERO_TIME);
    async_reset = true;
    reset = true;
    sc_core::sc_start(1, sc_core::SC_NS);
    reset = false;
    sc_core::sc_start(1, sc_core::SC_NS);
    async_reset = false;
    sc_core::sc_start(1, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
