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

  datawidth.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /**************************************/
                /* Interface Filename:	datawidth.h   */
                /**************************************/

#include "common.h"

SC_MODULE( datawidth )
{
  SC_HAS_PROCESS( datawidth );

  sc_in_clk clk;

  // Inputs
	const sc_signal<int>&	in1;
	const sc_signal<int>&	in2;
	const sc_signal<bool>&	ready;
  // Outputs
	sc_signal_bool_vector&	result;

  // Constructor
  datawidth (sc_module_name 	NAME,
	sc_clock&		TICK,
	const sc_signal<int>&	IN1,
	const sc_signal<int>&	IN2,
	const sc_signal<bool>&	READY,
	sc_signal_bool_vector&	RESULT )
	
      : clk	(TICK),
	in1	(IN1),
	in2	(IN2),
	ready   (READY),
	result	(RESULT)

  	{
          SC_CTHREAD( entry, clk.pos() );
        }

  void entry();
};
