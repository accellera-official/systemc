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

  8.5.11.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc.h>

struct i_f : virtual sc_core::sc_interface { };

struct Init : sc_core::sc_module {
    sc_core::sc_port<i_f> port;

    Init(sc_core::sc_module_name name) : port("port") { }
};

struct Targ: public sc_core::sc_module, private i_f {
    sc_export<i_f> xp;

    Targ(sc_core::sc_module_name name) : xp("xp") {}
};

struct Top : public sc_core::sc_module {
    sc_core::sc_vector<Init> init_vec;
    sc_core::sc_vector<Targ> targ_vec;

    Top(sc_core::sc_module_name name, int N) 
    : init_vec("init_vec",N), targ_vec("targ_vec", N) {
        // Vector-to_vector bind from vector-of-ports to vector-of-exports
	sc_core::sc_assemble_vector(init_vec, &Init::port).bind(
	    sc_core::sc_assemble_vector(targ_vec, &Targ::xp));
    }
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top",4);

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
