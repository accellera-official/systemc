/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  main.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /***************************************/
                /* Main Filename:       main.cc        */
                /***************************************/
 
#include "array_range.h" 	
#include "stimgen.h" 	

int sc_main(int ac, char *av[])
{

// Signal Instantiation
  signal_bool_vector8  	  in1 		("in1");
  signal_bool_vector4  	  o1		("o1");
  signal_bool_vector4  	  o2		("o2");
  signal_bool_vector8  	  o3		("o3");
  signal_bool_vector8  	  o4		("o4");
  signal_bool_vector8  	  o5		("o5");

// Clock Instantiation
  sc_clock clk( "clock", 10, SC_NS, 0.5, 0, SC_NS); 

// Process Instantiation
  array_range	D1 ("D1", clk, in1, o1, o2, o3, o4, o5); 

  stimgen	T1 ("T1", clk, o1, o2, o3, o4, o5, in1); 

// Simulation Run Control
  sc_start(); 
  return 0;
}
