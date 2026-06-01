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

  5.2.8.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(M) {
    sc_core::sc_in<bool> clk;
    SC_CTOR(M) : clk("clk") {
        SC_METHOD(a_method);
        SC_THREAD(a_thread);
        SC_CTHREAD(a_cthread, clk.pos() );
    }
    void a_method() { std::cout << "a_method at " << sc_core::sc_time_stamp() << std::endl; }
    void a_thread() { std::cout << "a_thread at " << sc_core::sc_time_stamp() << std::endl; }
    void a_cthread() { std::cout << "a_cthread at " << sc_core::sc_time_stamp() << std::endl; }
};

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_clock clock;
    M m("m");
    m.clk(clock);

    sc_core::sc_start(1, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
