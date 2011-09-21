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

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-27

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "stimulus.h"

void stimulus::entry() {

  int i, j;

  // sending some reset values
  reset.write(true);
  in_valid.write(false);
  in_value.write(0);
  wait();
  reset.write(false);
  wait(5);
  for(i=0; i<3; i++){
    in_valid.write(true);
    for(j=0; j<=10; j++) {
      in_value.write(j);
      cout << "Stimuli1 : in_valid = true in_value " << j << " at "
	   << sc_time_stamp() << endl;
      wait();
    };
    in_valid.write(false);
    wait(4);
    for(j=0; j<=10; j++) {
      in_value.write(j);
      cout << "Stimuli2 : in_valid = true in_value " << j << " at "
	   << sc_time_stamp() << endl;
      wait();
    };
    wait(10);
  };

  wait(15);
  sc_stop();
}

// EOF
