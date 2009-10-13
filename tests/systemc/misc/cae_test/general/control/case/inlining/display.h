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

  display.h -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-30

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "common.h"

SC_MODULE( display )
{
  SC_HAS_PROCESS( display );

  sc_in_clk clk;

  const sc_signal_bool_vector&  in_data1;     // Input  port
  const sc_signal_bool_vector&  in_data2;     // Input  port
  const sc_signal<bool>&        in_valid;

  display( sc_module_name   NAME,
	   sc_clock&        CLK,
	   const  sc_signal_bool_vector&   IN_DATA1,      
	   const  sc_signal_bool_vector&   IN_DATA2,     
           const  sc_signal<bool>&         IN_VALID
	   )
  : clk(CLK),
    in_data1(IN_DATA1),
    in_data2(IN_DATA2),
    in_valid(IN_VALID)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  void entry();
};

// EOF
