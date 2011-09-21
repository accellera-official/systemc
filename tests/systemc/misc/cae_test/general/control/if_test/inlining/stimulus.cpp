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

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-22

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "stimulus.h"

void stimulus::entry() {

int i;

  // sending some reset values
  reset.write(true);
  stim1.write(0);
  stim2.write(0);
  stim3.write(0);
  stim4.write(0);
  wait();
  reset.write(false);
  wait();
  for  (i=0; i<= 15; i++) {
    stim1.write(i);
    stim2.write(i);
    stim3.write(i);
    stim4.write(i);
    input_valid.write(true);
    cout << "Stimuli: stim1= " << i << " stim2= " << i << " stim3= " 
	 << i << " stim4= " << i << "  " 
	 << sc_time_stamp() << endl; 
    wait();
    input_valid.write(false);
    wait(10);
  }
 
  sc_stop();
}

// EOF
