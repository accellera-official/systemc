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
 
  icache.h -- Instruction Cache Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/
 

#define BOOT_LENGTH 5
#define MAX_CODE_LENGTH 500

struct icache : sc_module { 
  	sc_in<unsigned >  	datain; 	// modified instruction
  	sc_in<bool>  		cs;    		// chip select
  	sc_in<bool>  		we;    		// write enable for SMC
  	sc_in<unsigned >  	addr;  		// address
  	sc_in<bool>  		ld_valid;	// load valid    
  	sc_in<signed>  		ld_data;    	// load data value
  	sc_out<unsigned >  	dataout;  	// ram data out
  	sc_out<bool>  		icache_valid; 	// output valid
  	sc_out<bool>  		stall_fetch; 	// stall fetch if busy
  	sc_in_clk 		CLK;

  // Parameter
  unsigned *icmemory;	 				// icache data memory
  unsigned *ictagmemory; 				// icache tag memory
  signed int pid;	 				// process ID

  int wait_cycles; // Number of cycles it takes to access imemory

  void init_param(int given_delay_cycles) {
    wait_cycles = given_delay_cycles;
  }
 
  //Constructor
  SC_CTOR(icache) {
        SC_CTHREAD(entry, CLK.pos());

	// initialize instruction icmemory from external file
	pid = 0;
	FILE *fp = fopen("icache.img","r");
	int size=0;
	unsigned int mem_word;
  	icmemory 	= new unsigned[MAX_CODE_LENGTH];
  	ictagmemory 	= new unsigned[MAX_CODE_LENGTH];
	for (size = 0; size < MAX_CODE_LENGTH; size++) {	// initialize bad data
		icmemory[size] = 0xeeeeeeee;
		ictagmemory[size] = 0xeeeeeeee;
	}
	size = 0;
	while (fscanf(fp,"%x", &mem_word) != EOF) {
		icmemory[size] = mem_word;
		ictagmemory[size] = size;
		size++;
	}
  }

  // Process functionality in member function below
  void entry();
};


