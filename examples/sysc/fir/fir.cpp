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
 
  fir.cpp -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include <systemc.h>
#include "fir.h"

void fir::entry() {

  sc_int<8>  sample_tmp;
  sc_int<17> pro;
  sc_int<19> acc;
  sc_int<8> shift[16];

  // reset watching
  /* this would be an unrolled loop */
  for (int i=0; i<=15; i++) 
    shift[i] = 0;
  result.write(0);
  output_data_ready.write(false);
  wait();

  // main functionality
  while(1) {
    output_data_ready.write(false);
    do { wait(); } while ( !(input_valid == true) );
    sample_tmp = sample.read();
    acc = sample_tmp*coefs[0];

    for(int i=14; i>=0; i--) {
      /* this would be an unrolled loop */
      pro = shift[i]*coefs[i+1];
      acc += pro;
    };

    for(int i=14; i>=0; i--) {
      /* this would be an unrolled loop */
      shift[i+1] = shift[i];
    };

    shift[0] = sample_tmp;
    // write output values
    result.write((int)acc); 
    output_data_ready.write(true);
    wait();
  };
}
