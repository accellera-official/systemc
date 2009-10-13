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

  stimulus.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-30

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "stimulus.h"

void stimulus::entry() {

    reset.write(true);
    wait();
    reset.write(false);

    sc_signed tmp1(8);
    sc_signed tmp2(8);
    long           tmp3;
    int            tmp4;
    short          tmp5;
    char           tmp6;

    int counter = 0;

    tmp1 = "0b11011011";
    tmp2 = "0b00000001";
    tmp3 = 1;
    tmp4 = -1;
    tmp5 = 20000;
    tmp6 = 'R';

    while(counter<100){
       out_valid.write(true);    
       out_value1.write(tmp1);
       out_value2.write(tmp2);
       out_value3.write(tmp3);
       out_value4.write(tmp4);
       out_value5.write(tmp5);
       out_value6.write(tmp6);
       cout << "Stimuli: " << tmp1 << " " << tmp2 << " " << tmp3 << " " << tmp4 << " " << tmp5 << " " << endl;
       tmp1 = tmp1 + 1;
       tmp2 = tmp2 + 1;
       tmp3 = tmp3 + 1;
       tmp4 = tmp4 + 1;
       tmp5 = tmp5 + 1;
       tmp6 = tmp6 + 1;
       do { wait(); } while (in_ack==false);
       out_valid.write(false);
       counter++;
       wait();
    }
    sc_stop();
}
// EOF
