/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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
 
  fir_fsm.cpp -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include <systemc.h>
#include "fir_fsm.h"

void fir_fsm::entry() {

  sc_uint<3> state_tmp;

  // reset behavior
  if(reset.read()==true) {
    state = reset_s;
  } else {
#ifdef DEBUG
    cout << "debug: " << state << " " << state_tmp << " " << reset.read() << endl;
#endif
    // main state machine 
    switch(state) {
    case reset_s:
      state = first_s;
      state_tmp = 0;
      state_out.write((unsigned)state_tmp);
      break;
    case first_s:
      if(in_valid.read()==true) {
	state = second_s;
      };
      state_tmp = 1;
      state_out.write((unsigned)state_tmp);
      break;
    case second_s:
      state = third_s;
      state_tmp = 2;
      state_out.write((unsigned)state_tmp);
      break;
    case third_s:
      state = output_s;
      state_tmp = 3;
      state_out.write((unsigned)state_tmp);
      break;
    default:
      state = first_s;
      state_tmp = 4;
      state_out.write((unsigned)state_tmp);
      break;
    }
  }
}
