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

  mean.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename mean.h */
/* This is the interface file for synchronous process `mean' */

#include "systemc.h"

typedef sc_signal<sc_bv<24> > sc_signal_bool_vector;

SC_MODULE( mean )
{
  SC_HAS_PROCESS( mean );

  sc_in_clk clk;

  const sc_signal_bool_vector& in; //input
  sc_signal_bool_vector& out; //output
  const sc_signal<bool>& input_available; //input
  sc_signal<bool>& send_data; //output
  sc_signal<bool>& output_ready; //output
  const sc_signal<bool>& receiver_ready; //input

  //Constructor 
  mean(sc_module_name NAME,
       sc_clock& CLK,
       const sc_signal_bool_vector& IN_,
       sc_signal_bool_vector& OUT_,
       const sc_signal<bool>& INPUT_AVAILABLE,
       sc_signal<bool>& SEND_DATA,
       sc_signal<bool>& OUTPUT_READY,
       const sc_signal<bool>& RECEIVER_READY)
    : clk(CLK), in(IN_), out(OUT_), receiver_ready(RECEIVER_READY), send_data(SEND_DATA), input_available(INPUT_AVAILABLE), output_ready(OUTPUT_READY)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};
