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
 
  floating.h -- Floating Execution Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


struct floating : sc_module {
  sc_in<bool>   	       	in_valid;	// input valid bit
  sc_in<int>  	 	     	opcode;		// opcode
  sc_in<signed int>        	floata;		// operand A
  sc_in<signed int>        	floatb;		// operand B
  sc_in<unsigned>        	dest;		// write to which register
  sc_out<signed int>        		fdout;		// FPU output
  sc_out<bool>   	       		fout_valid;	// output valid
  sc_out<unsigned>        		fdestout;	// write to which register
  sc_in_clk 			CLK;


  SC_CTOR(floating) {
      SC_CTHREAD(entry, CLK.pos());
  }

  
  void entry();
};

// EOF
