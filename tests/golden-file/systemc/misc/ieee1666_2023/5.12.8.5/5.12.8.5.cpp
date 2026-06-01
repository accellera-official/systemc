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

  5.12.8.5.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

// If the clock period is printed to the log file the before_end_of_elaboration() callback
// was executed successfully.

#include <systemc>

SC_MODULE(Top) {
    sc_core::sc_in<bool> clock;
    SC_CTOR(Top) {}
    void before_end_of_elaboration() {
        using namespace sc_core;
	sc_interface* i_f = clock.get_interface();
	sc_clock* clk = dynamic_cast<sc_clock*>(i_f);
	std::cout << "clock period is " << clk->period() << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_clock clock;
    Top top("top");
    top.clock(clock);

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;

    return 0;
}
