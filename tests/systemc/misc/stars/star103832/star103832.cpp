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

  star103832.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>

struct test : sc_module {
  
  sc_in<bool> reset;
  sc_in_clk clk;
  sc_in<sc_uint<8> >  dati;
  sc_out<sc_uint<8> >  dato;
  sc_out<bool> done;

  SC_HAS_PROCESS( test );
  
  test (const char *NAME) : sc_module(NAME) {
    SC_CTHREAD( reset_loop, clk.pos() );
    watching(reset.delayed()==true);
    end_module();
  }

  void tc_mult_8x8(sc_uint<8> A, sc_uint<8> B, sc_uint<16>& Y);
  void reset_loop();
};

void test::tc_mult_8x8(sc_uint<8> A, sc_uint<8> B, sc_uint<16>& Z) {
  sc_uint<8> MantA;
  sc_uint<16> MantC;
  sc_uint<1> SignC, SignA, SignB;
  sc_uint<8> MantB;
  if (A == 0 || B == 0) {
    Z = 0;
  } else {
    SignA = A[7];
    SignB = B[7];
    MantA = SignA ? sc_uint<8>(0 - A) : A;
    MantB = SignB ? sc_uint<8>(0 - B) : B;
    MantC = MantA *  MantB;
    SignC = SignA ^ SignB;
    MantC = SignC ? sc_uint<16>(0 - MantC) : MantC;
    Z = (SignC, MantC.range(14,0));
  }
}

void test::reset_loop() {
  sc_uint<16> tmp;
  sc_uint<8> inp;
  done = 0;
  dato = 0;
  tmp = 0;
  wait();
  operational_loop : while (1 != 0) {
    inp = dati.read();
    wait();
    tc_mult_8x8( inp, -2, tmp);
    wait();
    done_loop: while (1) {
      dato = tmp;
      done = 1;
      wait();
    }
  }
}
 
