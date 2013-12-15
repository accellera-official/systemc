/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  stimulus.h -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-22

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "common.h"

SC_MODULE( stimulus )
{
  SC_HAS_PROCESS( stimulus );

  sc_in_clk clk;

  sc_signal<bool>&             reset;
  sc_signal_bool_vector&       stim1;
  sc_signal_bool_vector&       stim2;
  sc_signal_bool_vector&       stim3;
  sc_signal<bool>&             input_valid;

  stimulus(sc_module_name           NAME,
	   sc_clock&                CLK,
	   sc_signal<bool>&         RESET,
	   sc_signal_bool_vector&   STIM1,
	   sc_signal_bool_vector&   STIM2,
	   sc_signal_bool_vector&   STIM3,
	   sc_signal<bool>&         INPUT_VALID
          )
          : 
            reset(RESET),
            stim1(STIM1),
            stim2(STIM2),
            stim3(STIM3),
            input_valid(INPUT_VALID)
  {
    clk(CLK);
	SC_CTHREAD( entry, clk.pos() );
  }
  
  void entry();
};

// EOF
