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

  5.21.5.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(TopModule) { SC_CTOR(TopModule) {} };

SC_MODULE(SomeModule)
{
    sc_core::sc_in<bool> SC_NAMED(in);
    SC_CTOR(SomeModule) 
    {
        {
	    sc_core::sc_hierarchy_scope scope( sc_core::sc_hierarchy_scope::get_root() );
	    TopModule* mod = new TopModule("TopModule");
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    SomeModule sm("sm");

    std::vector<sc_core::sc_object*> tops = sc_core::sc_get_top_level_objects();

    for ( size_t i = 0; i < tops.size(); ++i ) {
        std::cout << tops[i]->name() << std::endl;
    }

    std::cout << "program completed" << std::endl;
    return 0;
}
