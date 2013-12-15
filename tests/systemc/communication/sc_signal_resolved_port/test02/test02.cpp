/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test02.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_inout_resolved::operator=

#include "systemc.h"


SC_MODULE( mod_a)
{
  sc_inout_resolved in_out1;
  sc_inout_resolved in_out2;
  sc_inout_resolved in_out3;
  sc_inout_resolved in_out4;
  sc_port<sc_signal_inout_if<sc_logic>,1> in_out5;
  sc_port<sc_signal_in_if<sc_logic>,1> in_1;

  sc_in<bool> clk;

  void main_action()
    {
      
      sc_logic m;
      m = 'Z';

      while(1){
      wait();
      cout<< m<<"   ";
      in_out1 = m;
      cout<<in_out1->read()<<"   ";
      in_out2 = in_out1;
      cout<<in_out2->read()<<"   ";
      in_out3 = in_1;
      cout<<in_out3->read()<<"   ";
      in_out4 = in_out5;
      cout<<in_out4->read()<<endl;
      }
    }
 
  SC_CTOR( mod_a ) 
    {
      SC_THREAD(main_action)
	sensitive << clk.pos();
     }
};

int sc_main(int, char*[])
{
  sc_clock clk("clk", 5, SC_NS);
  mod_a a("a");
  sc_signal_resolved sig1;
  sc_signal_resolved sig2;
  sc_signal_resolved sig3;
  sc_signal<sc_logic> sig4;
  sc_signal_resolved sig5;
  sc_signal<sc_logic> sig6;

  a.clk(clk);
  a.in_out1(sig1);
  a.in_out2(sig2);
  a.in_out3(sig3);
  a.in_1(sig4);
  a.in_out4(sig5);
  a.in_out5(sig6);

  sc_start(15, SC_NS);
  return 0;
}
