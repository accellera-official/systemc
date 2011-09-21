/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  star109678.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE(test) {
  sc_in_clk clk;
  sc_in<bool> reset;
  sc_in<sc_uint<10> > inp;
  sc_out<sc_uint<10> > outp;
  sc_out<sc_uint<10> > outp2;

  SC_CTOR(test) {
    SC_CTHREAD(entry, clk.pos());
    reset_signal_is(reset,true);
    SC_CTHREAD(entry2, clk.pos());
    end_module();
  }

  void entry();
  void entry2();
};


void test::entry() {
  sc_uint<10> tmp, tmp2;

  outp.write(0);
  wait();
  for(;;) {
    if(inp.read() == 0) {
      wait();
      while(inp.read() != 0) {
        wait();
        tmp = inp.read();
        break;
      }
      break;
    } else {
      tmp = 5;
      wait();
    }
  }
  wait();
  outp = tmp;
  wait();
}


void test::entry2() {
  sc_uint<10> tmp, tmp2;

  outp2.write(0);
  wait();
  for(;;) {
    if(inp.read() == 0) {
      wait();
      while(inp.read() != 0) {
        wait();
        tmp = inp.read();
        continue;
      }
      break;
    } else {
      tmp = 5;
      wait();
    }
  }
  wait();
  outp2 = tmp;
  wait();
}
