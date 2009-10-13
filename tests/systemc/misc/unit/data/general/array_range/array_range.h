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

  array_range.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /**************************************/
                /* Interface Filename:	array_range.h   */
                /**************************************/

#include "common.h"

SC_MODULE( array_range )
{
  SC_HAS_PROCESS( array_range );

  sc_in_clk clk;

  // Inputs
	const signal_bool_vector8&	in1;
  // Outputs
	signal_bool_vector4&		o1;
	signal_bool_vector4&		o2;
	signal_bool_vector8&		o3;
	signal_bool_vector8&		o4;
	signal_bool_vector8&		o5;

  // Constructor
  array_range (sc_module_name 		NAME,
	sc_clock&			TICK,
	const signal_bool_vector8&	IN1,
	signal_bool_vector4&		O1,
	signal_bool_vector4&		O2,
	signal_bool_vector8&		O3,
	signal_bool_vector8&		O4,
	signal_bool_vector8&		O5 )
	
      : 
	in1	(IN1),
	o1	(O1),
	o2	(O2),
	o3	(O3),
	o4	(O4),
	o5	(O5)

  	{
          clk	(TICK);
	  SC_CTHREAD( entry, clk.pos() );
        }

  void entry();
};
