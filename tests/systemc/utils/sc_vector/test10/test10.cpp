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

#include <systemc>

#define PRINT_VECTOR(vector) \
  for(unsigned int i = 0; i < vector.size(); i++) \
    std::cout << #vector << ": " << vector[i].name() << std::endl;

#define BIND_VECTOR(vector, signal) \
  for(unsigned int i = 0; i < vector.size(); i++) \
    vector[i].bind(signal);

SC_MODULE( module )
{
  typedef sc_core::sc_in<bool> port_type;
  
  sc_core::sc_vector< port_type > vector_port1 { "vector_port1", 1 };
  sc_core::sc_vector< port_type > vector_port2 { "vector_port2", 2, sc_core::SC_VECTOR_LOCK_AFTER_INIT };
  sc_core::sc_vector< port_type > vector_port3 { "vector_port3", 3, sc_core::SC_VECTOR_LOCK_AFTER_ELABORATION };
  sc_core::sc_vector< port_type > vector_port4 { "vector_port4" };

  sc_core::sc_vector< sc_core::sc_event > vector_ev { "vector_event", 1, sc_core::SC_VECTOR_LOCK_AFTER_ELABORATION };

  SC_CTOR(module)
  {
    // specify size (if not done via constructor)
    vector_port4.init(4);

    // check sizes
    sc_assert( vector_port1.size() == 1 );
    sc_assert( vector_port2.size() == 2 );
    sc_assert( vector_port3.size() == 3 );
    sc_assert( vector_port4.size() == 4 );
    sc_assert( vector_ev.size() == 1);

    // after init, only port3 can add elements
    vector_port3.emplace_back();                      // add unnamed port
    sc_assert( vector_port3.size() == 4 );

    vector_port3.emplace_back_with_name("late_port"); // add port by name
    sc_assert( vector_port3.size() == 5 );
  }

  void end_of_elaboration()
  {
    vector_ev.emplace_back_with_name("my_event"); // note, this event is not printed, 
                                                  // since the call in top is before this one
    sc_assert( vector_ev.size() == 2 );
  }

  void start_of_simulation()
  {
    vector_ev.emplace_back_with_name("my_late_event"); // not allowed after elab, will throw an error
  }

};

SC_MODULE( top )
{
  sc_core::sc_signal<bool> sig{"sig"};
  module mod{"mod"};

  SC_CTOR(top) {}

  void before_end_of_elaboration()
  {
    // print all vector ports
    PRINT_VECTOR(mod.vector_port1);
    PRINT_VECTOR(mod.vector_port2);
    PRINT_VECTOR(mod.vector_port3);
    PRINT_VECTOR(mod.vector_port4);
    PRINT_VECTOR(mod.vector_ev);

    // bind all ports to a (dummy) signal 
    BIND_VECTOR(mod.vector_port1, sig);
    BIND_VECTOR(mod.vector_port2, sig);
    BIND_VECTOR(mod.vector_port3, sig);
    BIND_VECTOR(mod.vector_port4, sig);
  }

};

int sc_main( int, char*[] )
{
  top top1("top1");

  sc_core::sc_start();

  std::cout << "Success" << std::endl;

  return 0;
}
