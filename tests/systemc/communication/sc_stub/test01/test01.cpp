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

// test sc_tie::high, sc_tie::low and sc_open

#include <systemc.h>

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

  SC_HAS_PROCESS(mod);

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
  mod1.inp(sc_core::sc_tie::high);
  mod1.outp(sc_core::sc_open);

  mod<bool> mod2("mod2");
  mod2.inp(sc_core::sc_tie::high);
  mod2.outp(sc_core::sc_open);

  mod<int> mod3("mod3");
  mod3.inp(sc_core::sc_tie::high);
  mod3.outp(sc_core::sc_open);

  mod<double> mod4("mod4");
  mod4.inp(sc_core::sc_tie::high);
  mod4.outp(sc_core::sc_open);

  mod<sc_dt::sc_logic> mod5("mod5");
  mod5.inp(sc_core::sc_tie::high);
  mod5.outp(sc_core::sc_tie::high);

  mod<bool> mod6("mod6");
  mod6.inp(sc_core::sc_tie::high);
  mod6.outp(sc_core::sc_tie::high);

  mod<int> mod7("mod7");
  mod7.inp(sc_core::sc_tie::high);
  mod7.outp(sc_core::sc_tie::high);

  mod<double> mod8("mod8");
  mod8.inp(sc_core::sc_tie::high);
  mod8.outp(sc_core::sc_tie::high);

  mod<sc_dt::sc_logic> mod9("mod9");
  mod9.inp(sc_core::sc_tie::low);
  mod9.outp(sc_core::sc_open);

  mod<bool> mod10("mod10");
  mod10.inp(sc_core::sc_tie::low);
  mod10.outp(sc_core::sc_open);

  mod<int> mod11("mod11");
  mod11.inp(sc_core::sc_tie::low);
  mod11.outp(sc_core::sc_open);

  mod<double> mod12("mod12");
  mod12.inp(sc_core::sc_tie::low);
  mod12.outp(sc_core::sc_open);

  mod<sc_dt::sc_logic> mod13("mod13");
  mod13.inp(sc_core::sc_tie::low);
  mod13.outp(sc_core::sc_tie::low);

  mod<bool> mod14("mod14");
  mod14.inp(sc_core::sc_tie::low);
  mod14.outp(sc_core::sc_tie::low);

  mod<int> mod15("mod15");
  mod15.inp(sc_core::sc_tie::low);
  mod15.outp(sc_core::sc_tie::low);

  mod<double> mod16("mod16");
  mod16.inp(sc_core::sc_tie::low);
  mod16.outp(sc_core::sc_tie::low);

  mod<sc_dt::sc_logic> mod17("mod17");
  mod17.inp(sc_core::sc_tie::value(SC_LOGIC_1));
  mod17.outp(sc_core::sc_open);

  mod<bool> mod18("mod18");
  mod18.inp(sc_core::sc_tie::value(true));
  mod18.outp(sc_core::sc_open);

  mod<int> mod19("mod19");
  mod19.inp(sc_core::sc_tie::value(2));
  mod19.outp(sc_core::sc_open);

  mod<double> mod20("mod20");
  mod20.inp(sc_core::sc_tie::value(3.14));
  mod20.outp(sc_core::sc_open);

  mod<sc_dt::sc_lv<4> > mod21("mod21");
  sc_dt::sc_lv<4> val21 = "X0X0";
  mod21.inp(sc_core::sc_tie::value(val21));
  mod21.outp(sc_core::sc_open);

  mod<sc_dt::sc_bv<4> > mod22("mod22");
  sc_dt::sc_bv<4> val22 = "1010";
  mod22.inp(sc_core::sc_tie::value(val22));
  mod22.outp(sc_core::sc_open);

  show_hierarchy();

  sc_core::sc_start();

  return 0;
}
