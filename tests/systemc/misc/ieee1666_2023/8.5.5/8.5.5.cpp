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

  8.5.5.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc.h>

struct my_module : public sc_core::sc_module {
    my_module( sc_core::sc_module_name n, std::string extra_arg ) {}
};

struct Top : sc_core::sc_module {
    sc_core::sc_vector<my_module> vector1;
    sc_core::sc_vector<my_module> vector2;

    // Case 1: creator is a function object
    struct my_module_creator {
        my_module_creator( std::string arg ) : extra_arg(arg) {}

	my_module* operator()(const char* name, size_t) {
	    return new my_module(name, extra_arg);
	}
	std::string extra_arg;
    };

    // Case 2: creator is a member function
    my_module* my_module_creator_func( const char* name, size_t ) {
        return new my_module(name, "value_of_extra_arg" );
    }

    Top(sc_core::sc_module_name name, int N) {
        // Initialize vector passing through constructor arguments to my_module
	// Case 1: constructor and pass in function object
	vector1.init(N, my_module_creator("value of extra arg"));

	// Case 2: pass in a member function using Boost bind
	vector2.init(N,
	    sc_core::sc_bind(&Top::my_module_creator_func, this, sc_unnamed::_1, sc_unnamed::_2));
    }
};


int sc_main( int argc, char* argv[] ) { 

    Top top("top", 3);
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
