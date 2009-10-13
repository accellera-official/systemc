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

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-30

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "balancing.h"
#include "stimulus.h"
#include "display.h"

int sc_main (int argc , char *argv[]) {
  sc_clock        	clock;
  sc_signal<bool> 	reset;
  sc_signal_bool_vector stim1;
  sc_signal_bool_vector stim2;
  sc_signal_bool_vector stim3;
  sc_signal<bool>       input_valid;
  sc_signal_bool_vector result1;
  sc_signal_bool_vector result2;
  sc_signal_bool_vector result3;
  sc_signal<bool>       output_valid1;
  sc_signal<bool>       output_valid2;
  sc_signal<bool>       output_valid3;


  balancing  balancing1   ( "process_body",
			    clock, 
			    reset,
			    stim1,
			    stim2,
			    stim3,
			    input_valid,
			    result1,
			    result2,
			    result3,
			    output_valid1,
			    output_valid2,
			    output_valid3
			    ); 

  stimulus stimulus1   ("stimulus",
			clock,
			reset,
			stim1,
			stim2,
			stim3,
			input_valid);

  display display1   ("display",
		      clock,
		      result1,
		      result2,
		      result3,
		      output_valid1,
		      output_valid2,
		      output_valid3
		      );

  sc_start(-1);
  return 0;
}

// EOF
