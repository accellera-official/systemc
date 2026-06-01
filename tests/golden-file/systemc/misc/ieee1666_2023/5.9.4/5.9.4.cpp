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

  5.9.4.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

using namespace sc_core;

SC_MODULE(M)
{
    sc_event event1, event2;
    SC_CTOR(M)
    {
	SC_THREAD( firer );
        SC_THREAD( thread );
    }
    void firer()
    {
        wait(10, SC_NS);
	event1.notify();
        wait(10, SC_NS);
	event2.notify();
        wait(10, SC_NS);
	event1.notify();
	event2.notify();
        wait(10, SC_NS);
    }

    void thread()
    {
	for (;;) {
	    wait(event1 | event2);
	    if ( event1.triggered()) std::cout << sc_time_stamp() << " event1" << std::endl;
	    if ( event2.triggered()) std::cout << sc_time_stamp() << " event2" << std::endl;
        }
    }
};


int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start(50, SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
