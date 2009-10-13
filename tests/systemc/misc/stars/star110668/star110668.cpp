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

  star110668.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
typedef struct tstruct {
  sc_int<16> val1;
  sc_int<16> val2;
} dummy_struct;
SC_MODULE(design) {
  sc_in_clk    clock;
  sc_in <sc_int<8> > input;
  sc_out<sc_int<8> > output;
  void write_in_fifo();
  SC_CTOR(design) {
    SC_CTHREAD(write_in_fifo, clock.pos() );
  }  
};
void design :: write_in_fifo() {
  sc_int<8> data_in;
  dummy_struct foofoo;
 write_loop: while (1) {
   data_in = input.read();
   foofoo.val1.range(0,7) = data_in;
   output.write(foofoo.val2.range(8,15));
   wait();
 }
}
