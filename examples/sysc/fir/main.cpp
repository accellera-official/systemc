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
 
  main.cpp -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include <systemc.h>
#include "stimulus.h"
#include "display.h"
#include "fir.h"

int sc_main (int, char *[]) {
  sc_clock         clock;
  sc_signal<bool>  reset;
  sc_signal<bool>  input_valid;        
  sc_signal<int>   sample;  	    
  sc_signal<bool>  output_data_ready;
  sc_signal<int>   result;

  stimulus stimulus1("stimulus_block");
  stimulus1.reset(reset); 
  stimulus1.input_valid(input_valid); 
  stimulus1.sample(sample); 
  stimulus1.CLK(clock);

  fir fir1( "process_body");
  fir1.reset(reset); 
  fir1.input_valid(input_valid); 
  fir1.sample(sample); 
  fir1.output_data_ready(output_data_ready); 
  fir1.result(result); 
  fir1.CLK(clock);

  display  display1 ( "display");
  display1.output_data_ready(output_data_ready);
  display1.result(result); 

  sc_start();
  return 0;
}

