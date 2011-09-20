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
 
  bios.h -- System Bios Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

 

#define BOOT_LENGTH  5

struct bios : sc_module { 
  	sc_in<unsigned > 		datain; 	// modified instruction
  	sc_in<bool> 			cs;    		// chip select
  	sc_in<bool>  			we;    		// write enable for SMC
  	sc_in<unsigned >  		addr;  		// physical address

  	sc_out<unsigned > 		dataout;  	// ram data out
  	sc_out<bool> 			bios_valid; 	// out valid
  	sc_out<bool>	 		stall_fetch; 	// stall fetch if output not valid
	sc_in_clk 			CLK;

  // Parameter
  unsigned *imemory;	 			// BIOS program data memory
  unsigned *itagmemory;	 			// program tag memory (NOT USED)
  int wait_cycles; 				// Cycle # it takes to access memory

  void init_param(int given_delay_cycles) {
    wait_cycles = given_delay_cycles; 
  }

  //Constructor 
  SC_CTOR(bios) {
        SC_CTHREAD(entry, CLK.pos());

	// initialize instruction imemory from external file
	FILE *fp = fopen("bios","r");
	int size=0;
	int mem_word;
  	imemory 	= new unsigned[4000];
  	itagmemory 	= new unsigned[4000];
	printf("** ALERT ** BIOS: initialize BIOS\n");
	for (size = 0; size < 4000; size++) {	// initialize bad data
		imemory[size] = 0xffffffff;
		itagmemory[size] = 0xffffffff;
	}
	size = 0;
	while (fscanf(fp,"%x\n", &mem_word) != EOF) {
		imemory[size] = mem_word;
		itagmemory[size] = size;
		size++;
	}
  }

  // Process functionality in member function below
  void entry();
};


