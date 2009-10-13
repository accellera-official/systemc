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

  stimgen.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /**************************************/
                /* Interface Filename:	stimgen.h   */
                /**************************************/

#include "common.h"

SC_MODULE( stimgen )
{
  SC_HAS_PROCESS( stimgen );

  sc_in_clk clk;

  // Inputs
	const signal_bool_vector8&	c;
	const signal_bool_vector16&	d;
	const sc_signal<bool>&		done;
  // Outputs
	signal_bool_vector8&		a;
	signal_bool_vector8&		b;
	sc_signal<int>&			mode;
	sc_signal<bool>&		ready;

  // Constructor
  stimgen (sc_module_name		NAME,
	sc_clock&			TICK,
	const signal_bool_vector8&	C,
	const signal_bool_vector16&	D,
	const sc_signal<bool>&		DONE,
	signal_bool_vector8&		A,
	signal_bool_vector8&		B,
	sc_signal<int>&			MODE,
	sc_signal<bool>&		READY )
	
      : clk	(TICK),
	c	(C),
	d	(D),
	done	(DONE),
	a	(A),
	b	(B),
	mode	(MODE),
	ready	(READY)

  	{
          SC_CTHREAD( entry, clk.pos() );
        }

  void entry();
};
