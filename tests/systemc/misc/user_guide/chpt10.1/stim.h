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

  stim.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename stim.h */
/* This is the interface file for synchronous process `stim' */

#include "common.h"

SC_MODULE( stim )
{
  SC_HAS_PROCESS( stim );

  sc_in_clk clk;

  // Inputs
  const sc_signal<bool>& done;
  // Outputs
  sc_signal<bool>& reset;
  signal_bool_vector& a;
  signal_bool_vector& b;
  sc_signal<bool>& cin;
  sc_signal<bool>& ready;
  // Parameters
  const int data_width;
 
  // Constructor
  stim (sc_module_name NAME,
	sc_clock& TICK,
	const sc_signal<bool>& DONE,
	sc_signal<bool>& RESET,
	signal_bool_vector& A,
	signal_bool_vector& B,
	sc_signal<bool>& CIN,
	sc_signal<bool>& READY,
        const int DATA_WIDTH = 8)
    : clk(TICK), done(DONE), reset(RESET), 
      a(A),b(B), cin(CIN),
      ready(READY),
      data_width(DATA_WIDTH)
  { 
    SC_CTHREAD( entry, clk.neg() );
  }

  // Process functionality in member function below
  void entry();
};
