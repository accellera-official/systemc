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
	FILE *fp = fopen("bios.img","r");
	int size=0;
	unsigned mem_word;
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
