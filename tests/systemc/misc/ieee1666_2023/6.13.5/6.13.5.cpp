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

  6.13.5.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(M) 
{
    sc_core::sc_signal_resolved sig;

    SC_CTOR(M) {
        SC_METHOD(snooper);
	sensitive << sig;
        SC_THREAD(T1);
	SC_THREAD(T2);
	SC_THREAD(T3);
    }

    void T1() {
        wait(10, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_0;
        wait(20, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_Z;
    }

    void T2() {
        wait(20, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_Z;
        wait(30, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_0;
    }

    void T3() {
        wait(40, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_1;
    }

    void snooper() {
        std::cout << sc_core::sc_time_stamp() << " " << sig << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_core::sc_start();
    std::cout << "program completed" << std::endl;
    return 0;
}
