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
	FILE *fp = fopen("icache","r");
	int size=0;
	int mem_word;
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


