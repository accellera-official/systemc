/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

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
    : in(IN_)
    , out(OUT_)
    , input_available(INPUT_AVAILABLE)
    , send_data(SEND_DATA)
    , output_ready(OUTPUT_READY)
    , receiver_ready(RECEIVER_READY)
  {
    clk(CLK);
	SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};
