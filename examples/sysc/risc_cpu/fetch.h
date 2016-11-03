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
 
  fetch.h -- Instruction Fetch Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


struct fetch : sc_module { 
  sc_in<unsigned > 		ramdata;	// instruction from RAM    
  sc_in<unsigned > 		branch_address; // branch target address   
  sc_in<bool>  			next_pc;   	// pc ++
  sc_in<bool>  			branch_valid;  	// branch_valid
  sc_in<bool>  			stall_fetch;   	// STALL_FETCH
  sc_in<bool>  			interrupt;   	// interrrupt 
  sc_in<unsigned>  		int_vectno;   	// interrupt vector number
  sc_in<bool>  			bios_valid;   	// BIOS input valid
  sc_in<bool>  			icache_valid;  	// Icache input valid
  sc_in<bool>  			pred_fetch;  	// branch prediction fetch
  sc_in<unsigned >  		pred_branch_address; 	// branch target address   
  sc_in<bool>  			pred_branch_valid; 	// branch prediction fetch
  sc_out<bool>  		ram_cs;   	// RAM chip select
  sc_out<bool>  		ram_we; 	// RAM write enable for SMC
  sc_out<unsigned >  	address;    	// address send to RAM
  sc_out<unsigned >  	smc_instruction; // for self-modifying code 
  sc_out<unsigned>  		instruction; 	 // instruction send to ID
  sc_out<bool>  		instruction_valid; // inst valid
  sc_out<unsigned >  	program_counter; // program counter	
  sc_out<bool>  		interrupt_ack;   // interrupt acknowledge
  sc_out<bool>  		branch_clear;    // clear outstanding branch
  sc_out<bool>  		pred_fetch_valid;  	// branch prediction fetch
  sc_out<bool>  		reset;  	// reset
  sc_in_clk                     CLK;

  // Parameter
  int memory_latency;   	// just a dummy for syntax

  void init_param(int given_delay_cycles) {
    memory_latency = given_delay_cycles; 
  }
 
  //Constructor 
  SC_CTOR(fetch) {
        SC_CTHREAD(entry, CLK.pos());
  }

  // Process functionality in member function below
  void entry();
};


