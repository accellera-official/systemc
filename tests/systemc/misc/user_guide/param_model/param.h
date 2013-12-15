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

  param.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /***************************************/
                /* Interface Filename:	param.h        */
                /***************************************/

#include "common.h"

SC_MODULE( param )
{
    SC_HAS_PROCESS( param );

    sc_in_clk clk;

  // Inputs
  	const sc_signal<bool>&		reset;
	const signal_bool_vector&	a;		
	const signal_bool_vector&	b;	
	const sc_signal<bool>&		cin;
	const sc_signal<bool>&		ready;
  // Outputs
	signal_bool_vector&		sum;
	sc_signal<bool>&		co;
	sc_signal<bool>&		done;
  // Parameters
	const int			data_width;

  // Constructor
  param (sc_module_name 		NAME,
	sc_clock&			TICK,
	const sc_signal<bool>&          RESET,
	const signal_bool_vector& 	A,
	const signal_bool_vector& 	B,
	const sc_signal<bool>&          CIN,
	const sc_signal<bool>&          READY,
	signal_bool_vector&		SUM,
	sc_signal<bool>&          	CO,
	sc_signal<bool>&          	DONE,
	const int			DATA_WIDTH = 4)
	
      : reset		(RESET),
    	a		(A), 
    	b		(B), 
	cin		(CIN),
	ready		(READY),
    	sum		(SUM),
	co		(CO),
	done		(DONE),
        data_width  	(DATA_WIDTH)

  	{ 
	  clk(TICK);
          SC_CTHREAD( entry, clk.pos() );
	  reset_signal_is(reset,false);
	}

  void entry();
};
