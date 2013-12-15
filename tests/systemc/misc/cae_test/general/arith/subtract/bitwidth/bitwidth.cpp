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

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-12-14

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "bitwidth.h"

void bitwidth::entry(){

  sc_biguint<2>     tmp1;
  sc_biguint<2>     tmp1a;
  sc_bigint<4>      tmp2;
  sc_bigint<4>      tmp2a;
  sc_biguint<6>     tmp3;
  sc_biguint<6>     tmp3a;
  sc_bigint<8>      tmp4;
  sc_bigint<8>      tmp4a;

  // reset_loop
    out_valid.write(false);
    out_ack.write(false);
    wait();

  //
  // main loop
  //
  //
  while(1) {
    while(in_valid.read()==false) wait();
    //reading the inputs
    tmp1 = in_value1.read();
    tmp2 = in_value2.read();
    tmp3 = in_value3.read();
    tmp4 = in_value4.read();

    //execute simple operations
    // expected bitwidth 2 2 2
    tmp1a = tmp1 - tmp1;
    // expected bitwidth 2 4 4
    tmp2a = tmp2 - tmp1;
    // expected bitwidth 6 2 6
    tmp3a = tmp3 - tmp1;
    // expected bitwidth 4 8 8
    tmp4a = tmp4 - tmp2;
   
    out_ack.write(true);
    wait();
    out_ack.write(false);

    // write outputs
    out_value1.write(tmp1a);
    out_value2.write(tmp2a);
    out_value3.write(tmp3a);
    out_value4.write(tmp4a);
    out_valid.write(true);
    wait();
    out_valid.write(false);
    wait();
  }
}

// EOF


