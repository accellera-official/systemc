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

  5.2.15.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

#define CLEAR(X) { std::cout  << "clearing " << #X << std::endl; mod.X = false; }
#define ENABLE(X) { std::cout  << "setting " << #X << std::endl; mod.X = true; }
#define STATUS \
    std::cout << sc_core::sc_time_stamp() << "| " << mod.A << " " << mod.B << " " << mod.C \
              << " " << mod.D << " " << mod.E << " ";


SC_MODULE(Mod) {
    sc_core::sc_signal<bool> SC_NAMED(A), SC_NAMED(B), SC_NAMED(C),
                             SC_NAMED(D), SC_NAMED(E);
    
    SC_CTOR(Mod) {
        sensitive << A;

	SC_THREAD(T);
	sensitive << B << C;

	SC_METHOD(M);
	f();
	sensitive << E;
    }

    void f() { sensitive << D; }
    void T() { 
        for (;;) {
	    wait();
	    std::cout << sc_core::sc_time_stamp() << "| " << A << " " << B << " " << C << " " 
	              << D << " " << E << " invoking thread T " << std::endl; 
	}
    }
    void M() { 
	std::cout << sc_core::sc_time_stamp() << "| " << A << " " << B << " " << C << " " 
	          << D << " " << E << " invoking method M " << std::endl; 
    }
};

int sc_main( int argc, char* argv[] ) { 

    Mod mod("mod");

    std::cout << "Time  A B C D E" << std::endl;
    std::cout << "      - - - - -" << std::endl;

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    STATUS ENABLE(A)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(A)

    STATUS ENABLE(B)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(B)

    STATUS ENABLE(C)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(C)

    STATUS ENABLE(D)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(D)

    STATUS ENABLE(E)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(E)

    std::cout << "program completed" << std::endl;
    return 0;
}
