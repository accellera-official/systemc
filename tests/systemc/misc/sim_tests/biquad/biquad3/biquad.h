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

  biquad.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename biquad.h */
/* This is the interface file for synchronous process `biquad' */

#include "systemc.h"

SC_MODULE( biquad )
{
  SC_HAS_PROCESS( biquad );

  sc_in<float>  in;
  sc_in<bool>   reset;
  sc_out<float> out;

  int num_taps; //internal variable
  float Del[4]; //internal variable
  float Cte[5]; //internal variable

  // Constructor 
  biquad( sc_module_name NAME,
	  sc_signal<float>& IN1,
	  sc_signal<bool>& RESET,
	  sc_signal<float>& OUT1 )
    : in(IN1), reset(RESET), out(OUT1)
  {
    // initialize the coefficient matrix
    Cte[0] = 1.0;
    Cte[1] = 2.0;
    Cte[2] = 1.0;
    Cte[3] = 0.75;
    Cte[4] = -0.125;
    Del[0] = Del[1] = Del[2] = Del[3] = 0.0;

    SC_METHOD( entry );
    sensitive << in << reset;
  }

  // Process functionality in member function below
  void entry();
};
