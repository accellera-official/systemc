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

  5.16.9.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

// TODO
//
// THE LRM EXAMPLE HAS ISSUES:
//
// (1) SomeModule::in is a declared a reference and needs to be an instance.
// (2) ~sc_prim_channel is a protected method.
//
// @@@@ ISSUES @@@@


#include <systemc>

SC_MODULE(SignalStub) {
    SC_CTOR(SignalStub) {}
    ~SignalStub() {
        for ( auto o : get_child_objects() ) {
	    std::cout << o->name() << std::endl;
	    // delete dynamic_cast<sc_core::sc_prim_channel*>(o);
	}
    }

    template<typename T>
    void stub( sc_core::sc_in<T>& port) {
        sc_core::sc_hierarchy_scope scope = get_hierarchy_scope();
	sc_core::sc_signal<T>* sig;
	sig = new sc_core::sc_signal<T>( sc_core::sc_gen_unique_name(port.basename()) );
	port.bind(*sig);
    }
};

SC_MODULE(SomeModule) {
    sc_core::sc_in<int> in;
    SC_CTOR(SomeModule) : in("in")  { }
};

int sc_main( int argc, char* argv[] ) { 

    SignalStub SC_NAMED(sigStub);
    SomeModule SC_NAMED(top);
    sigStub.stub(top.in);

    std::cout << "program completed" << std::endl;
    return 0;
}
