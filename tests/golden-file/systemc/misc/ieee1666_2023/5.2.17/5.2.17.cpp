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

  5.2.17.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(M) {
    SC_CTOR(M) {
        SC_METHOD(entry);
	sensitive << sig;
    }

    void entry() {
        std::cout << sc_core::sc_time_stamp() << " " << sig << std::endl;
        if ( sig == 0 ) next_trigger(e1 | e2);
        else if ( sig == 1 ) next_trigger(1, sc_core::SC_NS);
        else next_trigger();
    }
    sc_core::sc_signal<int> SC_NAMED(sig);
    sc_core::sc_event       SC_NAMED(e1), SC_NAMED(e2);
    // .  .  .
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");
    m.sig = 1;
    sc_core::sc_start(2, sc_core::SC_NS);
    m.sig = 0;
    sc_core::sc_start(5, sc_core::SC_NS);
    m.e1.notify();
    sc_core::sc_start(5, sc_core::SC_NS);
    m.e2.notify();
    sc_core::sc_start(5, sc_core::SC_NS);
    m.e1.notify();
    m.e2.notify();
    sc_core::sc_start(5, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
