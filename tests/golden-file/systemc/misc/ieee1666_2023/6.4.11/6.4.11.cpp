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

  6.4.11.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

// As designed this code will not detect the second writer and instead will 
// loop infinitely, at least with the POC.
//
// This is because the SC_THREAD, writer, has a wait() as its first operation. 
// So it will not write a value until after that wait. The SC_METHOD, writer2, 
// does write a value immediately upon its first execution, making it the first
// one to attempt a write to the signal. And because it writes a new value each
// time it is entered we end up in an infinite update cascade, so the program 
// will loop forever.
//
// At coded below only SC_THREAD instances are used, so we get the expected
// error.
//
// @@@@ ISSUES @@@@

#include <systemc>

SC_MODULE(M) {
    sc_core::sc_signal<int> sig;
    SC_CTOR(M) {
        SC_THREAD(writer);
	SC_THREAD(reader);
	SC_THREAD(writer2); // Error
	// SC_METHOD(writer2); // Infinite loop, no error
	sensitive << sig;
    }
    void writer() {
        using namespace sc_core;
	wait(50.0,SC_NS);
	sig.write(1);
	sig.write(2);
	wait(50.0,SC_NS);
	sig = 3;
    }

    void reader() {
        wait(sig.value_changed_event());
	std::cout << sc_core::sc_time_stamp() << " " <<  sig.read() << std::endl;
	wait(sig.value_changed_event());
	std::cout << sc_core::sc_time_stamp() << " " <<  sig.read() << std::endl;
    }
    void writer2() {
        sig.write(sig+1); // SHOULD CAUSE A WRITE ERROR, BUT GENERATES AN INFINITE LOOP
    }
};



int sc_main( int argc, char* argv[] ) { 

    M m("m");
    sc_core::sc_start(200, sc_core::SC_NS);
    std::cout << "program completed" << std::endl;
    return 0;
}
