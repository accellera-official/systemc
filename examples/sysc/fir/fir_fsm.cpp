/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

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
