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

  5.2.13.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(M)
{
    sc_core::sc_in<bool> SC_NAMED(clock);
    sc_core::sc_in<bool> SC_NAMED(reset);
    // .  .  .

    SC_CTOR(M) {
        SC_METHOD(rtl_proc);
        sensitive << clock.pos();
        async_reset_signal_is( reset, true );
        // .  .  .
    }
    void rtl_proc() {
        if ( reset.read() ) {
	    std::cout << "asynchronous" << std::endl;
	}
	else {
	    std::cout << "synchronous" << std::endl;
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_clock        clock;
    sc_core::sc_signal<bool> reset;
    M                        m("m");

    m.clock(clock);
    m.reset(reset);

    reset = true;
    sc_start(1, sc_core::SC_NS);
    reset = false;
    sc_core::sc_start(1, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
