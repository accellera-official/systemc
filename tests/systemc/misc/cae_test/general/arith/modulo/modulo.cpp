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

  modulo.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-05-13

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "modulo.h"

void modulo::entry(){

  int             tmp1;
  sc_bigint<4>    tmp2;
  sc_biguint<4>   tmp3;
  sc_bigint<8>    tmp4;
  sc_biguint<8>   tmp5;

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

    //execute operations
    tmp1 = tmp1%5;
    tmp2 = tmp2%5;
    tmp3 = tmp3%5;
    tmp4 = tmp4%5;
    tmp5 = tmp5%5;
    wait();

    // write outputs
    out_value1.write(tmp1);
    out_value2.write(tmp2);
    out_value3.write(tmp3);
    out_value4.write(tmp4);
    out_value5.write(tmp5);
    out_valid.write(true);
    wait();
    out_valid.write(false);
    wait();
    //execute self assignment operations
    tmp1 %= 3;
    tmp2 %= 3;
    tmp3 %= 3;
    tmp4 %= 3;
    tmp5 %= 3;
    wait();

    // write outputs
    out_value1.write(tmp1);
    out_value2.write(tmp2);
    out_value3.write(tmp3);
    out_value4.write(tmp4);
    out_value5.write(tmp5);
    out_valid.write(true);
    wait();
    out_valid.write(false);
    wait();
    //execute different type operations
    if (tmp3!=0) 
       tmp1 = (tmp2%tmp3).to_int();
    if (tmp4!=0) 
       tmp2 = tmp2%tmp4;
    if (tmp5!=0) 
       tmp3 = tmp3%tmp5;
    if (tmp5!=0) 
       tmp4 = tmp4%tmp5;
    wait();

    // write outputs
    out_value1.write(tmp1);
    out_value2.write(tmp2);
    out_value3.write(tmp3);
    out_value4.write(tmp4);
    out_value5.write(tmp5);
    out_valid.write(true);
    wait();
    out_valid.write(false);
    wait();
  }
}

// EOF

