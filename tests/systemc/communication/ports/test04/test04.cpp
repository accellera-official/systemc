/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test04.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

//test of port connections

#include "systemc.h"


#define WRITE(a) \
    cout << a.name() << " (" << a.kind() << ")" << endl


SC_MODULE( mod_a)
{
  sc_port<sc_signal_in_if<bool>,2> in_1;
  sc_port<sc_signal_in_if<sc_logic>,2> in_2;
  sc_port<sc_signal_inout_if<bool>,2> inout_1;
  sc_port<sc_signal_inout_if<sc_logic>,2> inout_2;

  SC_CTOR( mod_a )
    {
     WRITE(in_1);
     WRITE(in_2);
     WRITE(inout_1);
     WRITE(inout_2);
    }
};

SC_MODULE( mod_b)
{
  sc_port<sc_signal_in_if<bool>,2> in_1;
  sc_port<sc_signal_in_if<sc_logic>,2> in_2;
  sc_port<sc_signal_inout_if<bool>,2> inout_1;
  sc_port<sc_signal_inout_if<sc_logic>,2> inout_2;

  SC_CTOR( mod_b )
    {
     WRITE(in_1);
     WRITE(in_2);
     WRITE(inout_1);
     WRITE(inout_2);   
    }
};

SC_MODULE( mod_c )
{
  sc_port<sc_signal_in_if<bool>,0> input_1;
  sc_port<sc_signal_in_if<bool>,3> input_2;
  sc_port<sc_signal_in_if<sc_logic>,0> input_3;
  sc_port<sc_signal_in_if<sc_logic>,3> input_4;
  sc_port<sc_signal_inout_if<bool>,0> inout_1;
  sc_port<sc_signal_inout_if<bool>,3> inout_2;  
  sc_port<sc_signal_inout_if<sc_logic>,0> inout_3;
  sc_port<sc_signal_inout_if<sc_logic>,3> inout_4;
  sc_signal<bool> sig_1;
  sc_signal<bool> sig_2;
  sc_signal<sc_logic> sig_3;
  sc_signal<sc_logic> sig_4;

  mod_a a;
  mod_b b;

  SC_CTOR( mod_c )
    :a("a"), b("b")
  {
    a.in_1(input_2);
    a.in_1(sig_1);
    a.in_2(input_4);
    a.in_2(sig_3);
    a.inout_1(inout_2);
    a.inout_1(sig_2);
    a.inout_2(inout_4);
    a.inout_2(sig_4);

    b.in_1(input_1);
    b.in_1(input_2);
    b.in_2(input_3);
    b.in_2(input_4);
    b.inout_1(inout_1);
    b.inout_1(inout_2);
    b.inout_2(inout_3);
    b.inout_2(inout_4);

     WRITE(input_1);
     WRITE(input_2);
     WRITE(inout_1);
     WRITE(inout_2); 
 }
};

SC_MODULE( mod_d )
{
  sc_port<sc_signal_in_if<bool>,1> input_1;
  sc_port<sc_signal_in_if<sc_logic>,1> input_2;
  sc_port<sc_signal_inout_if<bool>,1> inout_1;
  sc_port<sc_signal_inout_if<sc_logic>,1> inout_2;

  mod_c c;

  SC_CTOR( mod_d )
    : input_1("input_1"), input_2("input_2"),
    inout_1("inout_1"), inout_2("inout_2"), c("c")
  {
    c.input_1(input_1);
    c.input_3(input_2);
    c.inout_1(inout_1);
    c.inout_3(inout_2);
  }
};


int sc_main(int, char* []){

  mod_d d("d");
 
  return 0;
}
