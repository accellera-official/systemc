/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  main.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /****************************************/
                /* Main Filename:       main.cc         */
                /****************************************/
                /*                                      */
                /* 4-bit bool = 4-bit bool + 6-bit bool */
                /*                                      */
		/*	Max addition is 63 + 63	        */
                /*                                      */
                /****************************************/

#include "datawidth.h" 	
#include "stimgen.h" 	

int sc_main(int ac, char *av[])
{

// Parameter Settings
  int result_size = 4;
  int in1_size = 4;
  int in2_size = 6;
  
// Signal Instantiation
  signal_bool_vector4  	  in1 		("in1");
  signal_bool_vector6  	  in2		("in2");
  signal_bool_vector4  	  result 	("result");   
  sc_signal<bool> 	  ready 	("ready");     

// Clock Instantiation
  sc_clock clk( "clock", 10, 0.5, 0); 

// Process Instantiation
  datawidth	D1 ("D1", clk, in1, in2, ready, result, 
			  in1_size, in2_size, result_size);

  stimgen	T1 ("T1", clk, result, in1, in2, ready);

// Simulation Run Control
  sc_clock::start( -1 ); 
  return 0;

}
