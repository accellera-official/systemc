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

  5.3.5.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>


struct A : sc_core::sc_module {
    A(sc_core::sc_module_name) {}
};

struct B : sc_core::sc_module {
    B( sc_core::sc_module_name n ) : sc_core::sc_module(n) {}
};

struct C : B {
    C( sc_core::sc_module_name n ) : B(n) {}
};

struct Top : sc_core::sc_module {
    A a;
    C c;

    Top(sc_core::sc_module_name n) : sc_core::sc_module(n) ,
                                  a("a"),
				  c("c")
    {}
};

void scan_hierarchy( sc_core::sc_object* obj ) {
    std::vector<sc_core::sc_object*> children = obj->get_child_objects();
    std::cout << "object " << obj->name() << std::endl;
    for ( size_t i = 0; i < children.size(); ++i ) {
        if ( children[i] ) {
            scan_hierarchy(children[i]);
        }
    }
}

int sc_main( int argc, char* argv[] ) { 

    Top top("top");

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::vector<sc_core::sc_object*> tops = sc_core::sc_get_top_level_objects();
    for ( size_t i = 0; i < tops.size(); ++i ) {
        if ( tops[i] ) {
            scan_hierarchy(tops[i]);
        }
    }

    std::cout << "program completed" << std::endl;
    return 0;
}
