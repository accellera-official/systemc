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
