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

  8.5.4.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

// Note this test differs from the example in the LRM, but still tests the same
// statements, the difference being that the signal vector has been moved to
// sc_main() from my_module in order to check the operation of sc_vector.

#include <systemc.h>

typedef sc_core::sc_signal_out_if<int> i_f;

SC_MODULE(my_module) {
    sc_core::sc_vector<sc_core::sc_port<i_f> >    ports;

    SC_CTOR(my_module) 
    : ports("ports",4) {
        SC_METHOD(display);
    }

    void display() {
	for ( size_t i = 0; i < 4; ++i ) {
	    std::cout << i << " " << ports[i]->read() << endl;
	}
	std::cout << std::endl;
    }
};


int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_vector<sc_core::sc_signal<int> > signals;
    signals.init(4);

    my_module mm("mm");
    for ( size_t i = 0; i < 4; ++i ) {
	mm.ports[i](signals[i]);
	signals[i] = i;
    }
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
