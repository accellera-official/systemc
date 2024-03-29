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

  5.16.7.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

void scan_hierarchy( sc_core::sc_object* obj ) {
    std::vector<sc_core::sc_object*> children = obj->get_child_objects();
    std::cout << "object " << obj->name() << std::endl;
    for ( size_t i = 0; i < children.size(); ++i ) {
        if ( children[i] ) {
	    scan_hierarchy(children[i]);
	}
    }
}

void snoop()
{
    std::vector<sc_core::sc_object*> tops = sc_core::sc_get_top_level_objects();
    for ( size_t i = 0; i < tops.size(); ++i ) {
        if ( tops[i] ) {
	    scan_hierarchy(tops[i]);
	}
    }

    sc_core::sc_object* obj = sc_core::sc_find_object("foo.foobar");
    if ( obj ) {
        std::cout << obj->name() << std::endl;
    }

    sc_core::sc_module* m;
    if ( (m = dynamic_cast<sc_core::sc_module*>(obj)) ) {
        std::cout << obj->name() << " is an sc_module" << std::endl;
    }

    sc_core::sc_object* parent = obj->get_parent_object();
    if ( parent ) {
        std::cout << "parent of " << obj->name() << " is " << parent->name() 
	          << " " << parent->kind() << std::endl;
    }
}


SC_MODULE(Bottom) {
    sc_core::sc_signal<int> SC_NAMED(sig1), SC_NAMED(sig2);

    SC_CTOR(Bottom) {}
};

SC_MODULE(Top) {
    Bottom fb;
    SC_CTOR(Top) : fb("foobar") {}
};

int sc_main( int argc, char* argv[] ) { 

    Top top("foo");

    snoop();

    std::cout << "program completed" << std::endl;
    return 0;
}
