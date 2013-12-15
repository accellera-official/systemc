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

// test of sc_signal_resolved::operator

#include "systemc.h"

SC_MODULE( mod_a )
{
  sc_in<bool> clk;

  sc_signal_resolved sig1;
  sc_signal_resolved sig2;

  void main_action(){
    sc_logic data('1');
    sig1.write(data);
    int i = 1;

    while(1){
      wait();
      cout<<i<<". cycle\n";
      cout<<sig1.read()<<endl;
      sig2 = sig1;
      cout<<sig2.read()<<endl;
      i = i+1;
    }
  }

  SC_CTOR( mod_a ):sig1("res_sig1"), sig2("res_sig2")
       {
        SC_THREAD(main_action);
        sensitive << clk.pos();
       }
};


int
sc_main( int, char*[] )
{
    sc_clock clk("clk",5, SC_NS);
    mod_a a( "a" );
    a.clk( clk );
    cout<<a.sig1.name()<<endl;
    cout<<a.sig2.name()<<endl;
    sc_start(10, SC_NS);

    return 0;
}
