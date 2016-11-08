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
 
  main_rtl.cpp -- 
 
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
#include "fir_top.h"

int sc_main (int, char *[]) {
  sc_clock        clock;
  sc_signal<bool> reset;
  sc_signal<bool> input_valid;        
  sc_signal<int>  sample;  	    
  sc_signal<bool> output_data_ready;
  sc_signal<int>  result;

  stimulus stimulus1("stimulus_block");
  stimulus1.reset(reset); 
  stimulus1.input_valid(input_valid); 
  stimulus1.sample(sample); 
  stimulus1.CLK(clock);

  fir_top   fir_top1    ( "process_body");
  fir_top1.RESET(reset); 
  fir_top1.IN_VALID(input_valid); 
  fir_top1.SAMPLE(sample); 
  fir_top1.OUTPUT_DATA_READY(output_data_ready); 
  fir_top1.RESULT(result); 
  fir_top1.CLK(clock);

  display  display1 ( "display");
  display1.output_data_ready(output_data_ready);
  display1.result(result); 

  sc_start();
  return 0;
}

