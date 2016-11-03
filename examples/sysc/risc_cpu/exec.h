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
 
  exec.h -- Integer Execution Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


struct exec : sc_module {
  sc_in<bool>   	       	reset;			// reset not used.
  sc_in<bool>   	       	in_valid;		// input valid
  sc_in<int>  	      		opcode;			// opcode from ID
  sc_in<bool>  	      		negate;			// not implemented
  sc_in<int>        		add1;			// not implemented
  sc_in<bool>   	     	shift_sel;		// not implemented
  sc_in<signed int>        	dina;			// operand A
  sc_in<signed int>        	dinb;			// operand B
  sc_in<bool>        		forward_A;		// data forwarding A valid
  sc_in<bool>        		forward_B;		// data forwarding B valid
  sc_in<unsigned>        	dest;			// destination register number
  sc_out<bool>              	C;			// Carry bit 
  sc_out<bool>              	V;			// Overflow bit
  sc_out<bool>              	Z;			// Zero bit
  sc_out<signed int>        	dout;			// output data
  sc_out<bool>   	       	out_valid;		// output valid
  sc_out<unsigned>        	destout;		// write to which registers?
  sc_in_clk 			CLK;


  SC_CTOR(exec) {
      SC_CTHREAD(entry, CLK.pos());
  }
  
  void entry();
};

// EOF
