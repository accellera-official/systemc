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

  star109218-2.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>
#include "map.h"


void my_mult(bool CLK,
            sc_uint<8> A,
            sc_uint<8> B,
            sc_uint<16> &Z) {
  // map_to_operator MULT_TC_OP
  bool SignA, SignB;
  sc_uint<8> MantA, MantB;
  sc_uint<1> SignC;
  sc_uint<16> MantC;
  sc_lv<8> A_tmp, B_tmp;
  sc_lv<16> Z_tmp;

  if(A == 0 || B == 0) {
    Z = 0;
  } else {
    SignA = A[7];
    SignB = B[7];
    MantA = SignA? (sc_uint<8>) (0-A) : A;
    MantB = SignB? (sc_uint<8>) (0-B) : B;
    MantC = MantA*MantB;
    SignC[0] = SignA ^ SignB;
    MantC = SignC[0]? (sc_uint<16>) (0-MantC) : MantC;
    Z = (SignC, MantC.range(14,0));
  }
}


void test::reset_loop() {
  sc_uint<16> a, b, c, d, result;

  dato.write(0);
  done.write((sc_logic)'0');
  wait();
  while(1) {
    a = dati;
    my_mult(clk.read(), a.range(7,0), a.range(15,8), result);
    dato.write(result);
    done.write((sc_logic)'1');
    wait();
  }
}
