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
 
  fir_data.cpp -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/
 
#include <systemc.h>
#include "fir_data.h"

void fir_data::entry()
{
  int state;
  sc_int<8> sample_tmp;

  // reset functionality
  if(reset.read()==true) {
    sample_tmp   = 0;
    acc = 0;
    for (int i=0; i<=15; i++) 
      shift[i] = 0;
  }

  // default settings
  result.write(0); 
  output_data_ready.write(false);
  state = state_out.read();
#ifdef DEBUG
  // cout << "Data debug : " << " " << state << " " << acc << " " << " at time " << sc_time_stamp() << endl;
  cout << "Data debug : " << " " << state << " " << acc << " " << " at time " << sc_time_stamp().to_double() << endl;
  for(int i=15; i>=0; i--) {
    cout << "Data debug : shift(" << i << ") " << shift[i] << endl;
  }; 
#endif

  // cycle behavior could be as well a case statement
  switch (state) {
  case 1 :
    sample_tmp = sample.read();
    acc = sample_tmp*coefs[0];
    acc += shift[14]* coefs[15];
    acc += shift[13]*coefs[14];
    acc += shift[12]*coefs[13];
    acc += shift[11]*coefs[12];
    break; 
  case 2 : 
    acc += shift[10]*coefs[11];
    acc += shift[9]*coefs[10];
    acc += shift[8]*coefs[9];
    acc += shift[7]*coefs[8];
    break; 
  case 3 : 
    acc += shift[6]*coefs[7];
    acc += shift[5]*coefs[6];
    acc += shift[4]*coefs[5];
    acc += shift[3]*coefs[4];
    break; 
  case 4 : 
    acc += shift[2]*coefs[3];
    acc += shift[1]*coefs[2];
    acc += shift[0]*coefs[1];
    for(int i=14; i>=0; i--) {
      shift[i+1] = shift[i];
    }; 
    shift[0] = sample.read();
    result.write((int)acc); 
    output_data_ready.write(true);
    break;
  default :
    cout << "Information : Reset state" << endl;
  }
} 



