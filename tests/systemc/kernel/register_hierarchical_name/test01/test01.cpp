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
  test01.cpp -- test sc_register_hierachical_name

  Original Author: Martin Barnasconi, NXP, 2023-08-08
 *****************************************************************************/

#include <systemc>

SC_MODULE(moda)
{
  SC_CTOR(moda) {}
};

SC_MODULE(modc)
{
  SC_CTOR(modc) {}
};

SC_MODULE(modb)
{
  moda a1{"a1"}; // warning: object already exists: b0.a1. Latter declaration will be renamed to b0.a1_0
  moda b1{"b1"};
  modc *c;

  SC_CTOR(modb)
  {
    bool ok;
    ok = sc_core::sc_register_hierarchical_name("b0"); // warning: name already exists: b0 (sc_module)
    sc_assert(!ok);

    ok = sc_core::sc_register_hierarchical_name(this, "c1"); // valid
    sc_assert(ok);

    c = new modc("c1"); // warning: object already exists: b0.c1. Latter declaration will be renamed to b0.c1_0
    
    ok = sc_core::sc_unregister_hierarchical_name("b0");
    sc_assert(!ok);

    ok = sc_core::sc_unregister_hierarchical_name(this, "c1");
    sc_assert(ok);
  }
};

int sc_main(int, char**) 
{
  bool ok;
  ok = sc_core::sc_register_hierarchical_name("a0");
  sc_assert(ok);

  ok = sc_core::sc_register_hierarchical_name("b0.a1");
  sc_assert(ok);

  moda a0("a0"); // warning: object already exists: a0. Latter declaration will be renamed to a0_0
  modb b0("b0"); // valid

  ok = sc_core::sc_unregister_hierarchical_name("a0");
  sc_assert(ok);

  ok = sc_core::sc_unregister_hierarchical_name("b0.a1");
  sc_assert(ok);

  return 0;
}
