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

  mult.h -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-05-13

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "common.h"

SC_MODULE( mult )
{
    SC_HAS_PROCESS( mult );

    sc_in_clk clk;

    //====================================================================
    // [C] Always Needed Member Function
    //        --  constructor
    //        --  entry
    //====================================================================

    const sc_signal<bool>&             reset ;
    const sc_signal<int>&              in_value1;     // Input  port
    const sc_signal_bool_vector4&      in_value2;     // Input  port
    const sc_signal_bool_vector4&      in_value3;     // Input  port
    const sc_signal_bool_vector8&      in_value4;     // Input  port
    const sc_signal_bool_vector8&      in_value5;     // Input  port
    const sc_signal<bool>&             in_valid;      // Input  port
    sc_signal<int>&                    out_value1;    // Output port
    sc_signal_bool_vector4&            out_value2;    // Output port
    sc_signal_bool_vector4&            out_value3;    // Output port
    sc_signal_bool_vector8&            out_value4;    // Output port
    sc_signal_bool_vector8&            out_value5;    // Output port
    sc_signal<bool>&                   out_valid;     // Output port

    //
    // Constructor
    //

    mult(
               sc_module_name   NAME,          // referense name
               sc_clock&        CLK,           // clock
	const  sc_signal<bool>& RESET,
        const  sc_signal<int>&                    IN_VALUE1,
        const  sc_signal_bool_vector4&            IN_VALUE2,      
        const  sc_signal_bool_vector4&            IN_VALUE3,     
        const  sc_signal_bool_vector8&            IN_VALUE4,      
        const  sc_signal_bool_vector8&            IN_VALUE5,     
        const  sc_signal<bool>&                   IN_VALID,     // Input  port
               sc_signal<int>&                    OUT_VALUE1, 
               sc_signal_bool_vector4&            OUT_VALUE2,
               sc_signal_bool_vector4&            OUT_VALUE3,   
               sc_signal_bool_vector8&            OUT_VALUE4,
               sc_signal_bool_vector8&            OUT_VALUE5,   
               sc_signal<bool>&                   OUT_VALID    // Output port
        )
        : 
          reset        (RESET),
	  in_value1    (IN_VALUE1),
	  in_value2    (IN_VALUE2),
	  in_value3    (IN_VALUE3),
	  in_value4    (IN_VALUE4),
	  in_value5    (IN_VALUE5),
	  in_valid     (IN_VALID),
          out_value1   (OUT_VALUE1),
          out_value2   (OUT_VALUE2),
          out_value3   (OUT_VALUE3),
          out_value4   (OUT_VALUE4),
          out_value5   (OUT_VALUE5),
          out_valid    (OUT_VALID)

    {
      clk          (CLK);
	  SC_CTHREAD( entry, clk.pos() );
      reset_signal_is(reset,true);
    };

    //

    void entry ();

};

// EOF
