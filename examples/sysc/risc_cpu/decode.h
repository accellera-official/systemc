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
 
  decode.h -- Instruction Decode Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/
 

struct decode : sc_module { 
  	sc_in<bool>  			resetin;      		// input reset
  	sc_in<unsigned>  		instruction;		// fetched instruction
  	sc_in<unsigned>  		pred_instruction;	// fetched instruction
  	sc_in<bool>  			instruction_valid;      // input valid
  	sc_in<bool>  			pred_inst_valid;	// input valid
  	sc_in<bool>  			destreg_write;      	// register write enable
  	sc_in<unsigned>  		destreg_write_src;	// which register to write?      
  	sc_in<signed>  		alu_dataout; 		// data from ALU     
  	sc_in<signed>  		dram_dataout;           // data from Dcache
  	sc_in<bool>  			dram_rd_valid;      	// Dcache read data valid
  	sc_in<unsigned>  		dram_write_src;         // Dcache data write to which reg
  	sc_in<signed>  		fpu_dout;      		// data from FPU
  	sc_in<bool>  			fpu_valid;      	// FPU data valid
  	sc_in<unsigned>  		fpu_destout;      	// write to which register
  	sc_in<bool>  			clear_branch;      	// clear outstanding branch
  	sc_in<bool>  			display_done;      	// display to monitor done
  	sc_in<unsigned >  		pc;      		// program counter from IFU
  	sc_in<bool>  			pred_on;      		// branch prediction is on
  	sc_out<unsigned > 		br_instruction_address; // branch invoke instruction
  	sc_out<bool>  			next_pc;      		// next pc ++ ?
  	sc_out<bool>  			branch_valid;      	// branch valid signal
  	sc_out<unsigned >  		branch_target_address;	// branch target address
  	sc_out<bool>  			mem_access;      	// memory access valid 
  	sc_out<unsigned >  		mem_address;      	// memory physical address
  	sc_out<int>  			alu_op;      		// ALU/FPU/MMU Opcode
  	sc_out<bool>  			mem_write;      	// memory write enable
  	sc_out<unsigned>  			alu_src;      		// destination register number
  	sc_out<bool>  			reg_write;      	// not implemented
  	sc_out<signed int>           	src_A;			// operand A
  	sc_out<signed int>           	src_B;			// operand B
  	sc_out<bool>  			forward_A;      	// data forwarding to operand A
  	sc_out<bool>  			forward_B;      	// data forwarding to operand B
  	sc_out<bool>  			stall_fetch;      	// stall fetch due to branch
  	sc_out<bool>  			decode_valid;      	// decoder output valid
  	sc_out<bool>  			float_valid;      	// enable FPU
  	sc_out<bool>  			mmx_valid;      	// enable MMU
  	sc_out<bool>  			pid_valid;      	// load process ID
  	sc_out<signed>  		pid_data;      		// process ID value
        sc_in_clk 			CLK;


        signed int cpu_reg[32];			//CPU register
        signed int vcpu_reg[32];		//virtual CPU register
        bool 	   cpu_reg_lock[32];		//lock architectural state register
	unsigned int pc_reg;			//pc register
	unsigned int jalpc_reg;			//jump back register

  //Constructor 
  SC_CTOR(decode) {
      SC_CTHREAD(entry, CLK.pos());
        FILE *fp = fopen("register.img","r");
        int size=0;
        unsigned mem_word;
	printf("** ALERT ** ID: initialize Architectural Registers\n");
        while (fscanf(fp,"%x", &mem_word) != EOF) {
                cpu_reg[size] = mem_word;
		size++;
	}
	pc_reg = 0;
	jalpc_reg = 0;
	for (int j =0; j<32; j++) vcpu_reg[j] 	= 0;
	for (int k =0; k<32; k++) cpu_reg_lock[k] = 0;
  }

  // Process functionality in member function below
  	void entry();
};


