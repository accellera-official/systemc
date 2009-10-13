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

    const sc_signal_bool_vector4&      in_value1;     // Output  port
    const sc_signal_bool_vector5&      in_value2;     // Output  port
    const sc_signal_bool_vector6&      in_value3;     // Output  port
    const sc_signal_bool_vector7&      in_value4;     // Output  port
    const sc_signal_bool_vector8&      in_value5;     // Output  port
    const sc_signal<bool>&             in_valid;                         // Output  port

   // 
   // Constructor
   //

   display(
               sc_module_name    NAME,      // reference name
               sc_clock&      CLK,          // clock
               const sc_signal_bool_vector4&     IN_VALUE1,
               const sc_signal_bool_vector5&     IN_VALUE2,
               const sc_signal_bool_vector6&     IN_VALUE3,
               const sc_signal_bool_vector7&     IN_VALUE4,
               const sc_signal_bool_vector8&     IN_VALUE5,
               const sc_signal<bool>&            IN_VALID
           ) 
           : clk          (CLK),
             in_value1    (IN_VALUE1),
             in_value2    (IN_VALUE2),
             in_value3    (IN_VALUE3),
             in_value4    (IN_VALUE4),
             in_value5    (IN_VALUE5),
             in_valid     (IN_VALID)
     {
       SC_CTHREAD( entry, clk.pos() );
     };

 
 void entry();
};

// EOF
