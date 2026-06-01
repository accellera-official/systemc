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

  test01.cpp -- 

  Original Author: Martin Barnasconi, NXP, 2021-06-01

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test sc_tie::high, sc_tie::low and sc_unbound

#include <systemc>

void scan_hierarchy(sc_core::sc_object* obj)
{
  std::vector<sc_core::sc_object*> children = obj->get_child_objects();
  for ( unsigned i = 0; i < children.size(); i++ )
    if ( children[i] )
    {
      scan_hierarchy( children[i] );

      std::cout << " name: "
                << children[i]->name()
                << " kind: " << children[i]->kind()
                << std::endl;
     }
}

void show_hierarchy() {

  std::vector<sc_core::sc_object*> tops = sc_core::sc_get_top_level_objects();

  for ( unsigned i = 0; i < tops.size(); i++ )
    if ( tops[i] )
    {
      std::cout << " name: "
              << tops[i] ->name()
              << " kind: " << tops[i]->kind()
              << std::endl;
      scan_hierarchy( tops[i] );
    }
}

template < typename T >
struct mod : public sc_core::sc_module
{
  sc_core::sc_in<T> inp;
  sc_core::sc_out<T> outp;


  mod(sc_core::sc_module_name nm): inp("inp"), outp("outp")
  {
    SC_METHOD(proc);
    sensitive << inp;
  }

  void proc()
  {
    T in_val = inp.read();
    std::cout << name() << " input value : " << in_val << std::endl;
    outp.write(in_val);
    outp.read(); // output can be read, but value is implementation defined
  }
};

int sc_main(int argc, char* argv[])
{
  mod<sc_dt::sc_logic> mod1("mod1");
  mod1.inp(sc_core::sc_tie::value(sc_dt::SC_LOGIC_1));
  mod1.outp(sc_core::sc_unbound);

  mod<bool> mod2("mod2");
  mod2.inp(sc_core::sc_tie::value(true));
  mod2.outp(sc_core::sc_unbound);

  mod<int> mod3("mod3");
  mod3.inp(sc_core::sc_tie::value(2));
  mod3.outp(sc_core::sc_unbound);

  mod<double> mod4("mod4");
  mod4.inp(sc_core::sc_tie::value(3.14));
  mod4.outp(sc_core::sc_unbound);

  mod<sc_dt::sc_lv<4> > mod5("mod5");
  sc_dt::sc_lv<4> val5 = "X0X0";
  mod5.inp(sc_core::sc_tie::value(val5));
  mod5.outp(sc_core::sc_unbound);

  mod<sc_dt::sc_bv<4> > mod6("mod6");
  sc_dt::sc_bv<4> val6 = "1010";
  mod6.inp(sc_core::sc_tie::value(val6));
  mod6.outp(sc_core::sc_unbound);

  show_hierarchy();

  sc_core::sc_start();

  return 0;
}
