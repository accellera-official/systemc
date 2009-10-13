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

  display.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /***************************************/
                /* Interface Filename:	display.h      */
                /***************************************/

#include "common.h"

SC_MODULE( displayp )
{
    SC_HAS_PROCESS( displayp );

    sc_in_clk clk;

  // Inputs
	const sc_signal<bool>&		prime_ready;
	const signal_bool_vector&      	prime;

  // Constructor
  displayp (sc_module_name 		NAME,
	sc_clock&			TICK,
	const sc_signal<bool>&		PRIME_READY,
	const signal_bool_vector&     	PRIME )
	
      : clk	(TICK),
	prime_ready	(PRIME_READY),
	prime		(PRIME)

  	{
            SC_CTHREAD( entry, clk.pos() );
        }

  void entry();
};
