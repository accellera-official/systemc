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

  concat_port.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /**************************************/
                /* Interface Filename:	concat_port.h */
                /**************************************/

#include "common.h"

SC_MODULE( concat_port )
{
  SC_HAS_PROCESS( concat_port );

  sc_in_clk clk;

  // Inputs
	const signal_bool_vector8&	a;
	const signal_bool_vector8&	b;
	const sc_signal<int>&		mode;
	const sc_signal<bool>&		ready;
  // Outputs
	signal_bool_vector8&		c;
	signal_bool_vector16&		d;
	sc_signal<bool>&		done;

  // Constructor
  concat_port (sc_module_name 		NAME,
	sc_clock&			TICK,
	const signal_bool_vector8&	A,
	const signal_bool_vector8&	B,
	const sc_signal<int>&		MODE,
	const sc_signal<bool>&		READY,
	signal_bool_vector8&		C,
	signal_bool_vector16&		D,
	sc_signal<bool>&		DONE )
	
      : clk	(TICK),
	a	(A),
	b	(B),
	mode	(MODE),
	ready	(READY),
	c	(C),
	d	(D),
 	done	(DONE)

  	{
          SC_CTHREAD( entry, clk.pos() );
        }

  void entry();
};
