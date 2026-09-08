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

  4.6.8.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>
using namespace sc_core;

SC_MODULE(TB)
{
    SC_CTOR(TB)
    {
        SC_THREAD(tb_thread);
    }
    void tb_thread()
    {
        for (size_t i = 0; i < 10; ++i )
        {
	    std::cout << " tb   " << i << "   " << sc_time_stamp() << std::endl;
            wait(1.0, SC_NS);
        }
    }
};

int sc_main( int argc, char* argv[] ) {

    sc_signal<bool>    reset;
    TB                 tb("tb");

    sc_start( SC_ZERO_TIME ); // Run the initialization phase to create pending activity

    while ( sc_pending_activity() ) {
        sc_start( sc_time_to_pending_activity() ); // Run up to the next activity
    }
    return 0;
}

