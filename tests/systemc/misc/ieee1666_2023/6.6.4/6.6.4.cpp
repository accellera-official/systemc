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

  6.6.4.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>


SC_MODULE(M) 
{
    sc_core::sc_buffer<int> buf;

    SC_CTOR(M) {
        SC_THREAD(writer);
	SC_METHOD(reader);
	sensitive << buf;
    }

    void writer() {
        buf.write(1);
	wait( sc_core::SC_ZERO_TIME);
	buf.write(1);
    }

    void reader() {
        std::cout << sc_core::sc_time_stamp() << " " << buf << std::endl;
    }
}; 

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_core::sc_start();

    std::cout << "program completed" << std::endl;
    return 0;
}
