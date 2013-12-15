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

  bitwidth.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-08-02

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "bitwidth.h"

void bitwidth::entry(){

  sc_bigint<4>    tmp1;
  sc_biguint<4>   tmp2;
  sc_bigint<6>    tmp3;
  sc_biguint<6>   tmp4;
  sc_bigint<8>    tmp5;
  sc_biguint<8>   tmp6;

  // reset_loop
  if (reset.read() == true) {
    out_valid.write(false);
    wait();
  } else wait();

  //
  // main loop
  //
  //
  while(1) {
    while(in_valid.read()==false) wait();
    wait();

    //reading the inputs
    tmp1 = in_value1.read();
    tmp2 = in_value2.read();
    tmp3 = in_value3.read();
    tmp4 = in_value4.read();
    tmp5 = in_value5.read();
    tmp6 = in_value6.read();

    //execute simple operations
    // expected bitwidth 4 4 4 signed
    tmp1 = tmp1 + tmp2;
    // expected bitwidth 4 6 6 signed
    tmp3 = tmp1 + tmp3;
    // expected bitwidth 4 4 6 signed
    tmp6 = tmp2 + tmp1;
    // expected bitwidth 8 8 6 signed
    tmp4 = tmp5 + tmp6;
    // expected bitwidth 6 8 4 unsigned
    tmp2 = tmp4 + tmp6;
    wait();

    // write outputs
    out_value1.write(tmp1);
    out_value2.write(tmp2);
    out_value3.write(tmp3);
    out_value4.write(tmp4);
    out_value5.write(tmp5);
    out_value6.write(tmp6);
    out_valid.write(true);
    wait();
    out_valid.write(false);
    wait();
  }
}

// EOF

