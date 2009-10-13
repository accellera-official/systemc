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

  datatypes.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-30

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "datatypes.h"


void datatypes::entry() {

  sc_biguint<4>   tmp1;
  sc_bigint<4>    tmp2;
  sc_lv<4>        tmp3;
  sc_bv<4>        tmp4;

  // reset_loop
    if (reset.read() == true) {
      out_value1.write(0);
      out_value2.write(0);
      out_value3.write(0);
      out_value4.write(0);
      out_valid.write(false);
      wait();
    } else wait();

  //
  // main loop
  //
  while(1) {
    do { wait(); } while  (in_valid == false);

    // reading inputs
    tmp1 = in_value1.read();
    tmp2 = in_value2.read();
    tmp3 = in_value3.read();
    tmp4 = in_value4.read();

    //checking if condition on a range of bits
    if (tmp1.range(1,3) == 4) {
	out_value1.write(3);
    } else if (tmp1.range(3,1) == 4) {
	out_value1.write(2);
    } else {
	out_value1.write(tmp1);
    };
    wait();

    if (tmp2[2]) {
      out_value2.write(3);
    } else if ((bool)tmp1[1]==true) {
	out_value2.write(2);
    } else {
      out_value2.write(tmp2);
    };
    wait();

    if (tmp3.range(1,3)=="000" || ((tmp3.range(3,1).to_uint()!=4) && 
	tmp3.range(3,1).to_uint()!=5 && tmp3.range(3,1).to_uint()!=6 && 
	tmp3.range(3,1).to_uint()!=7)) {
      out_value3.write(1);
    } else {
      out_value3.write(tmp3);
    };

    if (tmp4.range(1,3)=="000" || (tmp4.range(3,1).to_uint()!=4 && 
	tmp4.range(3,1).to_uint()!=5 && tmp4.range(3,1).to_uint()!=6 && 
	tmp4.range(3,1).to_uint()!=7)) {
      out_value4.write(1);
    } else {
      out_value4.write(tmp4);
    };

    out_valid.write(true);
    wait();
    out_valid.write(false);

  }
}

// EOF

