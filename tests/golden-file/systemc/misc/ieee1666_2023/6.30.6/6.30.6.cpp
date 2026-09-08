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

  6.30.6.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>


SC_MODULE(Mod) {
    sc_core::sc_event_queue EQ;

    SC_CTOR(Mod) {
        SC_THREAD(T);
	SC_METHOD(M);
	sensitive << EQ;
	dont_initialize();
    }

    void T() {
        EQ.notify(2.0, sc_core::SC_NS);
        EQ.notify(1.0, sc_core::SC_NS);
	EQ.notify(sc_core::SC_ZERO_TIME);
        EQ.notify(1.0, sc_core::SC_NS);
    }

    void M() {
        std::cout << sc_core::sc_time_stamp() << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    Mod m("m");

    sc_core::sc_start();
    std::cout << "program completed" << std::endl;
    return 0;
}
