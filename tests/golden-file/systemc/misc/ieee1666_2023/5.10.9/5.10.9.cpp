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

  5.10.9.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

using namespace sc_core;

#define LRM_DEFINITION 0

#define NOTIFY0() { \
    std::cout << "FIRE: event.notify1()" << std::endl; \
    event1.notify(); \
}

#define NOTIFY1(ARG) { \
    std::cout << "FIRE: event.notify1(" << #ARG << ")" << std::endl; \
    event1.notify(ARG); \
}

#define NOTIFY2(ARG1,ARG2) { \
    std::cout << "FIRE: event.notify1(" << #ARG1 << "," << #ARG2 << ")" << std::endl; \
    event1.notify(ARG1,ARG2); \
}

SC_MODULE(M)
{
    sc_event event1;
    SC_CTOR(M)
    {
        SC_THREAD( firer );
        SC_THREAD( thread );
    }

    void firer() {
#if LRM_DEFINITION
    event1.notify(SC_ZERO_TIME); // Delta notification
    event1.notify(1.0, SC_NS);   // Timed notification ignored due to pending delta notification
    event1.notify();             // Immediate notification cancels pending delta notification
                                 // e is notified
    event1.notify(2.0, SC_NS);   // Timed notification
    //event1.notify(3.0, SC_NS);   // Timed notification ignored due to earlier pending timed notification
    event1.notify(1.0, SC_NS);   // Timed notification cancels pending timed notification
    event1.notify(SC_ZERO_TIME); // Delta notification cancels pending timed notification
                                 // e is notified in the next delta cycle
#else
    NOTIFY1(SC_ZERO_TIME);
    NOTIFY2(1.0, SC_NS);
    NOTIFY0();
    NOTIFY2(2.0, SC_NS);
    NOTIFY2(1.0, SC_NS);
    NOTIFY1(SC_ZERO_TIME);
#endif
    }

    void thread()
    {
        for (;;) {
            wait(event1);
            if ( event1.triggered()) std::cout << sc_time_stamp() << " event1" << std::endl;
        }
    }
};


int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start(20, SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
