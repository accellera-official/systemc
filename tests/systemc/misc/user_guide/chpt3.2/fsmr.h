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

  fsmr.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Header file for FSM recognizer */

#include "systemc.h"

SC_MODULE( fsm_recognizer )
{
  SC_HAS_PROCESS( fsm_recognizer );

  sc_in_clk clk;

  // The input signals
  const sc_signal<char>& input_char; // The input character
  const sc_signal<bool>& data_ready; // The data ready signal
  // The output signals
  sc_signal<bool>& found;            // The indicator that sequence found

  // The internal variables
  char pattern[4];   // This string will hold the pattern to match against
  
  // The constructor
  fsm_recognizer(sc_module_name NAME,
		 sc_clock& TICK,
		 const sc_signal<char>& INPUT_CHAR,
		 const sc_signal<bool>& DATA_READY,
		 sc_signal<bool>& FOUND)
    : input_char(INPUT_CHAR), data_ready(DATA_READY), 
      found(FOUND) {
        clk(TICK);
		SC_CTHREAD( entry, clk.pos() );
	pattern[0] = 'S';
	pattern[1] = 'c';
	pattern[2] = 'e';
	pattern[3] = 'n';
  }

  // The functionality of the process
  void entry();
};
