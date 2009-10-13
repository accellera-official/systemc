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

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-05-13

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "stimulus.h"
#include "display.h"
#include "mult.h"

int sc_main (int argc , char *argv[]) {
  sc_clock        clock;
  sc_signal<bool> reset;
  sc_signal<int>              stimulus_line1;
  sc_signal_bool_vector4      stimulus_line2;
  sc_signal_bool_vector4      stimulus_line3;
  sc_signal_bool_vector8      stimulus_line4;
  sc_signal_bool_vector8      stimulus_line5;
  sc_signal<bool>             input_valid;
  sc_signal<bool>             output_valid;
  sc_signal<int>              result_line1;
  sc_signal_bool_vector4      result_line2;
  sc_signal_bool_vector4      result_line3;
  sc_signal_bool_vector8      result_line4;
  sc_signal_bool_vector8      result_line5;

  stimulus stimulus1("stimulus_block",
                      clock,
		      reset,
                      stimulus_line1,
                      stimulus_line2,
                      stimulus_line3,
                      stimulus_line4,
                      stimulus_line5,
		      input_valid);

  mult   mult1    ( "process_body",
                       clock, 
		       reset,
                       stimulus_line1,
                       stimulus_line2,
                       stimulus_line3,
                       stimulus_line4,
                       stimulus_line5,
		       input_valid,
                       result_line1, 
                       result_line2, 
                       result_line3, 
                       result_line4, 
                       result_line5, 
		       output_valid);

  display  display1 ( "display",
                       clock,
		       result_line1,
		       result_line2,
		       result_line3,
		       result_line4,
		       result_line5,
		       output_valid);

  sc_start(-1);
  return 0;
}

// EOF
