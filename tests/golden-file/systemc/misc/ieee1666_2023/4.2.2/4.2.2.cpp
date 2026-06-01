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

  4.2.2.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

struct Mod : sc_core::sc_module {
  SC_CTOR(Mod) {}
};

struct S {
  Mod m;                         // Not recommended coding style - module instance within struct
  S(const char* name_) : m(name_) {}
};

struct Top : sc_core::sc_module { // Five instances of module Mod exist within module Top.
  Mod m1;                        // Recommended coding style
  Mod *m2;                       // Recommended coding style
  S s1;

  SC_CTOR(Top)
  : m1("m1"),                    // m1.name() returns "top.m1"
    s1("s1")                     // s1.m.name() returns "top.s1"
  {
    m2 = new Mod("m2");          // m2->name() returns "top.m2"
    std::cout << m2->name() << std::endl;
    f();
    S *s2 = new S("s2");         // s2->m.name() returns "top.s2"
    std::cout << s2->m.name() << std::endl;
  }

  void f() {
    Mod *m3 = new Mod("m3");     // Not recommended coding style
    std::cout << m3->name() << std::endl;
  }                              // m3->name() returns "top.m3"
};

int sc_main(int argc, char* argv[]) {
  Top top("top");
  sc_core::sc_start();
  return 0;
}

