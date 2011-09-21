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

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-14

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

    const sc_signal<int>&              in_value1;                        // Input  port
    const sc_signal_bool_vector&       in_value2;     // Input  port
    const sc_signal<bool>&             in_valid;                         // Input  port

   // 
   // Constructor
   //

   display(
               sc_module_name    NAME,      // reference name
               sc_clock&      CLK,          // clock
               const sc_signal<int>&             IN_VALUE1,
               const sc_signal_bool_vector&      IN_VALUE2,
               const sc_signal<bool>&            IN_VALID
           ) 
           : 
             in_value1    (IN_VALUE1),
             in_value2    (IN_VALUE2),
             in_valid     (IN_VALID)
     {
       clk          (CLK);
	   SC_CTHREAD( entry, clk.pos() );
     };

 
 void entry();
};

// EOF
