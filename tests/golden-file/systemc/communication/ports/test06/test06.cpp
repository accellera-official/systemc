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

  test06.cpp -- test get_interface_type 

  Original Author: Martin Barnasconi, NXP, 2023-08-08

 *****************************************************************************/

#include <systemc>
#include <typeindex>

SC_MODULE(mod)
{
  sc_core::sc_in<int>                                  in;
  sc_core::sc_port<sc_core::sc_signal_inout_if<bool> > out1;
  sc_core::sc_out<int>                                 out2;
  sc_core::sc_export<sc_core::sc_signal_in_if<int> >   exp_in;

  SC_CTOR(mod)
  {
    sc_assert(in.get_interface_type() == std::type_index(typeid(sc_core::sc_signal_in_if<int>)));
    sc_assert(out1.get_interface_type() == std::type_index(typeid(sc_core::sc_signal_inout_if<bool>)));
    sc_assert(out2.get_interface_type() == std::type_index(typeid(sc_core::sc_signal_inout_if<int>)));
    sc_assert(exp_in.get_interface_type() == std::type_index(typeid(sc_core::sc_signal_in_if<int>)));
  }

};

int sc_main(int, char**) 
{
  sc_core::sc_signal<int> s1, s3;
  sc_core::sc_signal<bool> s2;
  sc_core::sc_signal<int> e1;

  mod mod1("mod1");
    mod1.in(s1);
    mod1.out1(s2);
    mod1.out2(s3);
    mod1.exp_in(e1);

  sc_core::sc_start();

  std::cout << "Success" << std::endl;
  return 0;
} 
