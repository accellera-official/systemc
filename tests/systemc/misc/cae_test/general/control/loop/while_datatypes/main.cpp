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

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-27

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "while_datatypes.h"
#include "stimulus.h"
#include "display.h"

int sc_main (int argc , char *argv[]) {
  sc_clock        	clock;
  sc_signal<bool>       reset;
  sc_signal<bool>       out_valid;
  sc_signal<bool>       in_valid;
  sc_signal<int>        result;
  sc_signal<int>        in_value;


  while_datatypes while_datatypes1   ( 
		"process_body",
		clock, 
		reset,
		in_valid, 
		in_value, 
		out_valid,
		result
		);

  stimulus  stimulus1  (
			"stimulus",
			clock,
			reset,
			in_value, 
			in_valid
			);

  display  display1  (
		      "display",
		      clock,
		      result,
		      out_valid
		      );


  sc_start(-1);
  return 0;
}

// EOF
