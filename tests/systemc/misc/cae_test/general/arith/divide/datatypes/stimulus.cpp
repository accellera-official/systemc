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

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-10-01

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

    sc_unsigned tmp1(2);
    sc_signed tmp2(2);
    sc_unsigned tmp3(3);
    sc_signed tmp4(3);
    sc_unsigned zero_2(2);
    sc_unsigned zero_3(3);

    zero_3 = "000";
    zero_2 = "00";
    tmp1 = "01";
    tmp2 = "10";
    tmp3 = "010";
    tmp4 = "011";


    while(true){
       // handshake
       out_valid.write(true); 
       // write stimuli    
       out_value1.write(tmp1);
       out_value2.write(tmp2);
       out_value3.write(tmp3);
       out_value4.write(tmp4);
       cout << "Stimuli: "<< tmp1 << " " << tmp2 << " " << tmp3 << " " << tmp4 << endl;
       // update stimuli
       tmp1 = tmp1 + 1;
       if (tmp1 == zero_2) tmp1 = "01";
       tmp2 = tmp2 + 1;
       if (tmp2 == zero_2) tmp2 = "01";
       tmp3 = tmp3 + 1;
       if (tmp3 == zero_3) tmp3 = "001";
       tmp4 = tmp4 + 1;
       if (tmp4 == zero_3) tmp4 = "001";
       // handshake
       wait_until(in_ack.delayed()==true);
       out_valid.write(false);
       wait();
    }
}
// EOF
