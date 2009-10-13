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

  while_fsm.h -- 

  Original Author: Preeti Panda, Synopsys, Inc., 2000-08-09

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

SC_MODULE( while_fsm )
{
    SC_HAS_PROCESS( while_fsm );

    sc_in_clk clk;

    const sc_signal<bool>&      reset;
    const sc_signal<bool>&      in_valid;
    const sc_signal<int>&       in_value;
          sc_signal<bool>&      out_valid;
          sc_signal<int>&       result;
 
    while_fsm(
              sc_module_name            NAME,        // referense name
              sc_clock&                 CLK,          // clock
	const sc_signal<bool>&          RESET,
	const sc_signal<bool>&          IN_VALID,
	const sc_signal<int>&           IN_VALUE,
	      sc_signal<bool>&          OUT_VALID,
              sc_signal<int>&           RESULT 
        )
        : clk            (CLK),            // connection definition
          reset          (RESET),
          in_valid       (IN_VALID),
          in_value       (IN_VALUE),
          out_valid      (OUT_VALID),
          result         (RESULT)
    {
      SC_CTHREAD( entry, clk.pos() );
      watching(reset.delayed() == true);
    };
    void entry ();
};
