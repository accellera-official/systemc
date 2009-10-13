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

typedef sc_signal<sc_bv<24> > signal_bool_vector;

SC_MODULE( mean )
{
  SC_HAS_PROCESS( mean );

  sc_in_clk clk;

  const signal_bool_vector& a; //input
  const signal_bool_vector& b; //input
  const signal_bool_vector& c; //input
  const signal_bool_vector& d; //input
  signal_bool_vector& am; //output
  signal_bool_vector& gm; //output
  signal_bool_vector& hm; //output

  //Constructor 
  mean(sc_module_name NAME,
       sc_clock& CLK,
       const signal_bool_vector& A,
       const signal_bool_vector& B,
       const signal_bool_vector& C,
       const signal_bool_vector& D,
       signal_bool_vector& AM,
       signal_bool_vector& GM,
       signal_bool_vector& HM)
    : clk(CLK), a(A), b(B), c(C), d(D), am(AM), gm(GM), hm(HM)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  // Process functionality in member function below
  void entry();
};


