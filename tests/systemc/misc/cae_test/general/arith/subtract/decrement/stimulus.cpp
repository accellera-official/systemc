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

  stimulus.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-14

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

    int tmp1;
    sc_signed tmp2(4);

    tmp1 = 0;
    tmp2 = "0001";


    while(true){
       out_valid.write(true);    
       out_value1.write(tmp1);
       out_value2.write(tmp2);
       cout << "Stimuli: " << tmp1 << " " << tmp2 << endl;
       tmp1 = tmp1 + 1;
       tmp2 = tmp2 + 1;
       wait_until(in_ack.delayed()==true);
       out_valid.write(false);
       wait();
    }
}
// EOF
