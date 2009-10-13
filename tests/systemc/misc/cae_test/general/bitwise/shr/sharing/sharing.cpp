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

  sharing.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-30

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "sharing.h"
#define true 1
#define false  0

void sharing::entry()

{

  sc_bigint<8>    tmp1;
  sc_bigint<8>    tmp1r;
  sc_biguint<8>   tmp2;
  sc_biguint<8>   tmp2r;
  long            tmp3;
  long            tmp3r;
  int             tmp4;
  int             tmp4r;
  short           tmp5;
  short           tmp5r;
  char            tmp6;
  char            tmp6r;

// define 1 dimensional array
   int  tmp7[2];
   char tmp8[2];
   int  tmp9[2];

// define sc_bool_vector
  sc_bv<4>	tmp10;
  tmp10[3] = 0;  tmp10[2] = 1;  tmp10[1] = 0;  tmp10[0] = 1;

// define 2 dimentional array
   sc_bv<1> tmp11[2];

// reset_loop
  if (reset.read() == true) {
    out_valid.write(false);
    out_ack.write(false);
    wait();
  } else wait();

//
// main loop
//

// initialization of sc_array

   tmp7[0] = 3;
   tmp7[1] = 12;
   tmp8[0] = 'S';
   tmp8[1] = 'C';
   // tmp9 = tmp7;
   tmp11[0][0] = "1";
   tmp11[1][0] = "0";


 while(1) {
    while(in_valid.read()==false) wait();

    //reading the inputs
    tmp1 = in_value1.read();
    tmp2 = in_value2.read();
    tmp3 = in_value3.read();
    tmp4 = in_value4.read();
    tmp5 = in_value5.read();
    tmp6 = in_value6.read();
    
    out_ack.write(true);

    //execute mixed data type addition operations
    tmp1r = tmp1 >> tmp7[0];
    tmp2r = tmp2 >> 2;
    tmp3r = tmp3 >> 1;
    tmp4r = tmp4 >> tmp7[1];
    tmp5r = tmp3 >> int(tmp1.to_int());
    tmp6r = tmp6 >> 1;

    //write outputs
    out_value1.write(tmp1r);
    out_value2.write(tmp2r);
    out_value3.write(tmp3r);
    out_value4.write(tmp4r);
    out_value5.write(tmp5r);
    out_value6.write(tmp6r);
  
    out_valid.write(true);
    wait();
    out_ack.write(false);
    out_valid.write(false);

    //execute mixed data type addition operations
    tmp1r = tmp1 >> tmp7[0];
    tmp2r = tmp2 >> tmp4;
    tmp3r = tmp3 >> tmp5;
    tmp4r = tmp4 >> 2;
    tmp5r = tmp3 >> int(tmp5);
    tmp6r = tmp6 >> int(tmp2.to_uint());
 
    //write outputs
    out_value1.write(tmp1r);
    out_value2.write(tmp2r);
    out_value3.write(tmp3r);
    out_value4.write(tmp4r);
    out_value5.write(tmp5r);
    out_value6.write(tmp6r);
 
    out_valid.write(true);
    wait();
    out_ack.write(false);
    out_valid.write(false);

 }

} // End 

