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

  inlining.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-30

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "inlining.h"

// list of defines
#define clock(a) wait(a)
#define intu4(a) sc_biguint<4>  a;
#define vec4(a) sc_lv<4>  a;
#define my_case(a, b, c)   switch (a) {     \
                             case 0:       \
                             case 1:       \
                             case 2:       \
                             case 3:       \
                               b = 0;      \
                               break;      \
                             default :     \
                               b = c;      \
                               break;      \
                           };
#define my_wait_case(a, b, c)  switch (a) { \
                                 case 0:   \
                                 case 1:   \
                                 case 2:   \
                                 case 3:   \
                                   b = 0;  \
                                   wait(); \
                                   break;  \
                                 default : \
                                   b = c;  \
                                   wait(); \
                                   break;  \
                               };  

void inlining::entry(){

  int tmp1;
  int tmp2;
  vec4(tmp3);
  sc_bv<4>  tmp4;

  // reset_loop
    if (reset.read() == true) {
      out_value1.write(0);
      out_value2.write(0);
      out_valid.write(false);
      clock(1);
    } else clock(1);

  //
  // main loop
  //
  while(1) {
    do { wait(); } while  (in_valid == false);

    //reading inputs
    tmp1 = in_value1.read().to_int();
    tmp2 = in_value2.read().to_int();
    tmp3 = in_value3.read();
    tmp4 = in_value4.read();

    //execution
    my_wait_case(tmp1, tmp3, tmp4); 
    out_value1.write(tmp3);
    clock(1);

    my_case(tmp2, tmp3, tmp4); 
    out_value2.write(tmp4);
    out_valid.write(true);
    clock(1);
    out_valid.write(false);

  }
}

// EOF

