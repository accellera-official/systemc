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
 
  mmxu.h -- MMX-Like Execution Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


struct mmxu : sc_module {
  sc_in<bool>   	       	mmx_valid;	// MMX unit enable
  sc_in<int>  	      		opcode;		// opcode
  sc_in<signed int>        	mmxa;		// operand A
  sc_in<signed int>        	mmxb;		// operand B
  sc_in<unsigned>        	dest;		// Destination register number
  sc_out<signed int>        	mmxdout;	// MMX output
  sc_out<bool>   	       	mmxout_valid;	// MMX output valid
  sc_out<unsigned>        	mmxdestout;	// destination number
  sc_in_clk 			CLK;


  // can make it asynchronous process to speed up simulation
  SC_CTOR(mmxu) {
      SC_CTHREAD(entry, CLK.pos());
  }
  
  void entry();
};

// EOF
